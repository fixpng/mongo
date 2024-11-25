/**
 *    Copyright (C) 2024-present MongoDB, Inc.
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the Server Side Public License, version 1,
 *    as published by MongoDB, Inc.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    Server Side Public License for more details.
 *
 *    You should have received a copy of the Server Side Public License
 *    along with this program. If not, see
 *    <http://www.mongodb.com/licensing/server-side-public-license>.
 *
 *    As a special exception, the copyright holders give permission to link the
 *    code of portions of this program with the OpenSSL library under certain
 *    conditions as described in each individual source file and distribute
 *    linked combinations including the program with the OpenSSL library. You
 *    must comply with the Server Side Public License in all respects for
 *    all of the code used other than as permitted herein. If you modify file(s)
 *    with this exception, you may extend this exception to your version of the
 *    file(s), but you are not obligated to do so. If you do not wish to do so,
 *    delete this exception statement from your version. If you delete this
 *    exception statement from all source files in the program, then also delete
 *    it in the license file.
 */

#include "testfile.h"

#include <algorithm>
#include <ostream>
#include <vector>

#include "command_helpers.h"
#include "mongo/base/error_codes.h"
#include "mongo/db/query/util/jparse_util.h"

namespace mongo::query_tester {
namespace {
void runCommandAssertOK(DBClientConnection*,
                        const BSONObj& command,
                        const std::string& db,
                        std::vector<ErrorCodes::Error> acceptableErrorCodes = {});

void dropCollections(DBClientConnection* const conn,
                     const std::string& dbName,
                     const std::vector<std::string>& collections) {
    auto cmd = BSON("drop"
                    << "");
    for (const auto& coll : collections) {
        auto bob = BSONObjBuilder{};
        bob.append("drop", std::get<0>(getCollAndFileName(coll)));
        // Allow NamespaceNotFound.
        runCommandAssertOK(conn, bob.done(), dbName, {ErrorCodes::NamespaceNotFound});
    }
}

// Format result set so that each result is on a separate line.
std::string formatResultSet(const BSONObj& obj) {
    auto oss = std::ostringstream{};
    const auto arrayElt = obj.hasField("res") ? obj.getField("res") : obj.firstElement();
    if (arrayElt.type() == Array) {
        oss << ArrayResult<BSONElement>{arrayElt.Array()};
    } else {
        uasserted(9670433,
                  str::stream{} << "Expected result set to be of type array, but got "
                                << arrayElt.type());
    }
    return oss.str();
}

void moveResultsFile(const std::filesystem::path& actualPath,
                     const std::filesystem::path& filePath,
                     const WriteOutOptions writeOutOpts) {
    switch (writeOutOpts) {
        case WriteOutOptions::kNone: {
            // Clean up.
            std::filesystem::remove(actualPath);
            break;
        }
        case WriteOutOptions::kOnelineResult:
        case WriteOutOptions::kResult: {
            std::filesystem::rename(actualPath,
                                    std::filesystem::path{filePath}.replace_extension(".results"));
            break;
        }
    }
}

void readAndBuildIndexes(DBClientConnection* const conn,
                         const std::string& dbName,
                         const std::string& collName,
                         std::fstream& fs) {
    verifyFileStreamGood(
        fs, std::filesystem::path{collName}, std::string{"Stream not ready to read indexes"});
    auto lineFromFile = std::string{};
    auto bob = BSONObjBuilder{};
    bob.append("createIndexes", collName);
    auto indexBuilder = BSONArrayBuilder{};

    readLine(fs, lineFromFile);
    for (auto indexNum = 0; !lineFromFile.empty(); readLine(fs, lineFromFile), ++indexNum) {
        BSONObjBuilder indexBob;
        indexBob.append("key", fromFuzzerJson(lineFromFile));
        indexBob.append("name", str::stream{} << "index_" << indexNum);
        indexBuilder.append(indexBob.done());
    }

    bob.append("indexes", indexBuilder.arr());
    const auto cmd = bob.done();
    runCommandAssertOK(conn, cmd, dbName);
}

// Returns true if another batch is required.
bool readAndInsertNextBatch(DBClientConnection* const conn,
                            const std::string& dbName,
                            const std::string& collName,
                            std::fstream& fileToRead) {
    auto bob = BSONObjBuilder{};
    bob.append("insert", collName);
    auto docBuilder = BSONArrayBuilder{};
    auto currentObjSize = 0;
    auto lineFromFile = std::string{};
    for (readLine(fileToRead, lineFromFile); !fileToRead.eof();
         readLine(fileToRead, lineFromFile)) {
        currentObjSize += lineFromFile.size();
        docBuilder.append(fromFuzzerJson(lineFromFile));
        if (currentObjSize > 100000) {
            bob.append("documents", docBuilder.arr());
            auto cmd = bob.done();
            runCommandAssertOK(conn, cmd, dbName);
            return true;
        }
    }
    bob.append("documents", docBuilder.arr());
    auto cmd = bob.done();
    runCommandAssertOK(conn, cmd, dbName);
    return false;
}

bool readAndLoadCollFile(DBClientConnection* const conn,
                         const std::string& dbName,
                         const std::string& collName,
                         const std::filesystem::path& filePath) {
    auto collFile = std::fstream{filePath};
    verifyFileStreamGood(collFile, filePath, "Failed to open file");
    // Read in indexes.
    readAndBuildIndexes(conn, dbName, collName, collFile);
    for (auto needMore = readAndInsertNextBatch(conn, dbName, collName, collFile); needMore;
         needMore = readAndInsertNextBatch(conn, dbName, collName, collFile)) {
        verifyFileStreamGood(collFile, filePath, "Failed to read batch");
    }
    return true;
}

void runCommandAssertOK(DBClientConnection* const conn,
                        const BSONObj& command,
                        const std::string& db,
                        const std::vector<ErrorCodes::Error> acceptableErrorCodes) {
    auto cmdResponse = runCommand(conn, db, command);
    if (cmdResponse.getField("ok").trueValue()) {
        return;
    }
    for (const auto& error : acceptableErrorCodes) {
        if (error == cmdResponse.getField("code").safeNumberInt()) {
            return;
        }
    }
    uasserted(9670420,
              str::stream{} << "Expected OK command result from " << command << " but got "
                            << cmdResponse);
}
}  // namespace

void QueryFile::dropStaleCollections(DBClientConnection* const conn,
                                     const std::set<std::string>& prevFileCollections) const {
    std::vector<std::string> collectionsToDrop;
    for (const auto& collFileName : _collectionsNeeded) {
        if (prevFileCollections.find(collFileName) == prevFileCollections.end()) {
            collectionsToDrop.emplace_back(collFileName);
        }
    }
    if (!collectionsToDrop.empty()) {
        dropCollections(conn, _databaseNeeded, collectionsToDrop);
    }
}

std::vector<std::string>& QueryFile::getCollectionsNeeded() {
    return _collectionsNeeded;
}

size_t QueryFile::getFailedQueryCount() const {
    return _failedQueryCount;
}

size_t QueryFile::getTestsRun() const {
    return _testsRun;
}

void QueryFile::loadCollections(DBClientConnection* const conn,
                                const bool dropData,
                                const bool loadData,
                                const std::set<std::string>& prevFileCollections) const {
    if (dropData) {
        dropStaleCollections(conn, prevFileCollections);
    }

    if (loadData) {
        // Load collections.
        uassert(9670419,
                "Expected at least one collection to be required. Has the file been read?",
                !_collectionsNeeded.empty());
        // Figure out our path.
        const auto pathPrefix = std::filesystem::path{_filePath}.remove_filename();
        // Deduce collection file.
        for (const auto& collSpec : _collectionsNeeded) {
            const auto [collName, fileName] = getCollAndFileName(collSpec);
            const auto fullPath = pathPrefix / fileName;
            if (prevFileCollections.find(collName) == prevFileCollections.end()) {
                // Only load a collection if it wasn't marked as loaded by the previous file.
                readAndLoadCollFile(conn, _databaseNeeded, collName, fullPath);
            }
        }
    }
}

// Expects 'fs' to be open.
void QueryFile::parseHeader(std::fstream& fs) {
    auto lineFromFile = std::string{};
    _comments.preName = readLine(fs, lineFromFile);
    verifyFileStreamGood(fs, _filePath, "Failed to read header line");
    // The first line of a file is required to be the filename.
    const auto nameNoExtension = getTestNameFromFilePath(_filePath);
    uassert(9670402,
            str::stream{} << "Expected first test line of " << _filePath.string()
                          << " to match the test name, but got " << nameNoExtension,
            nameNoExtension == lineFromFile);
    _comments.preCollName = readLine(fs, lineFromFile);
    uassert(9670411,
            str::stream{} << "Expected single database, got multiple in " << _filePath.string()
                          << ". Databases are " << lineFromFile,
            lineFromFile.find(' ') == std::string::npos);
    // Next line is a single database.
    _databaseNeeded = lineFromFile;

    // Next lines are a set of collection specifications.
    for (_comments.preCollFiles.push_back(readLine(fs, lineFromFile));
         !lineFromFile.empty() && !fs.eof();
         _comments.preCollFiles.push_back(readLine(fs, lineFromFile))) {
        _collectionsNeeded.push_back(lineFromFile);
    }

    // Final header line should be a newline.
    uassert(9670432,
            str::stream{} << "Expected newline at end of header for file " << _filePath.string(),
            lineFromFile.empty());
}

void QueryFile::printFailedQueries(const std::vector<size_t>& failedTestNums) const {
    std::cout << applyRed() << "------------------------------------------------------------"
              << applyReset() << std::endl
              << applyCyan() << "FAIL: " << getTestNameFromFilePath(_filePath) << applyReset()
              << std::endl;
    for (const auto& testNum : failedTestNums) {
        uassert(9699600,
                str::stream() << "Test " << testNum << " does not exist.",
                _testNumToQuery.find(testNum) != _testNumToQuery.end());
        std::cout << applyBold() << "TestNum: " << applyReset() << testNum << std::endl
                  << applyBold() << "Query: " << applyReset() << _testNumToQuery.at(testNum)
                  << std::endl
                  << std::endl;
    }
}

bool QueryFile::readInEntireFile(const ModeOption mode) {
    // Open read only.
    auto fs = std::fstream{_filePath, std::fstream::in};
    verifyFileStreamGood(fs, _filePath, "Failed to open file");

    // Read the header.
    parseHeader(fs);

    // The rest of the file is tests.
    for (auto testNum = 0; !fs.eof(); ++testNum) {
        try {
            _tests.push_back(Test::parseTest(fs, mode, testNum));
            _tests.back().setDB(_databaseNeeded);

        } catch (AssertionException& ex) {
            fs.close();
            ex.addContext(str::stream{} << "Failed to read test number " << _tests.size());
            throw;
        }
    }
    // Close the file.
    fs.close();
    return true;
}

void QueryFile::runTestFile(DBClientConnection* conn, const ModeOption mode) {
    _testsRun = 0;
    for (auto& test : _tests) {
        test.runTestAndRecord(conn, mode);
        ++_testsRun;
    }
}

/**
 * Write out all the non-test information to a string for debug purposes.
 */
std::string QueryFile::serializeStateForDebug() const {
    auto ss = std::stringstream{};
    ss << "_filePath: " << _filePath.string() << " | db: " << _databaseNeeded + " | ";
    for (const auto& coll : _collectionsNeeded) {
        ss << coll << " , ";
    }
    ss << " NumTests: " << _tests.size() << " | "
       << " Tests to run: " << _testsToRun.first << _testsToRun.second << " | ";
    return ss.str();
}

bool QueryFile::textBasedCompare(const std::filesystem::path& expectedPath,
                                 const std::filesystem::path& actualPath) {
    if (const auto& diffOutput = gitDiff(expectedPath, actualPath); !diffOutput.empty()) {
        // Write out the diff output.
        std::cout << diffOutput << std::endl;

        const auto& failedTestNums = getFailedTestNums(diffOutput);
        if (!failedTestNums.empty()) {
            printFailedQueries(failedTestNums);
            _failedQueryCount += failedTestNums.size();
        }

        // No cleanup on failure.
        return false;
    } else {
        // Clean up on success.
        std::filesystem::remove(actualPath);
        // This might be clearer than `return result.empty()`.
        return true;
    }
}

bool QueryFile::writeAndValidate(const ModeOption mode, const WriteOutOptions writeOutOpts) {
    // Set up the text-based diff environment.
    std::filesystem::create_directories(_actualPath.parent_path());
    auto actualStream = std::fstream{_actualPath, std::ios::out | std::ios::trunc};
    // Default to kResult for comparisons unless another write out option is specified.
    writeOutAndNumber(actualStream,
                      writeOutOpts == WriteOutOptions::kNone ? WriteOutOptions::kResult
                                                             : writeOutOpts);
    actualStream.close();

    // One big comparison, all at once.
    if (mode == ModeOption::Compare ||
        (mode == ModeOption::Normalize && writeOutOpts == WriteOutOptions::kNone)) {
        return textBasedCompare(_expectedPath, _actualPath);
    } else {
        const bool includeResults = writeOutOpts == WriteOutOptions::kResult ||
            writeOutOpts == WriteOutOptions::kOnelineResult;
        uassert(9670450,
                "Must have run query file before writing out result file",
                !includeResults || _testsRun == _tests.size());
        moveResultsFile(_actualPath, _filePath, writeOutOpts);
        return true;
    }
}

bool QueryFile::writeOutAndNumber(std::fstream& fs, const WriteOutOptions opt) {
    // Write out the header.
    auto nameNoExtension = getTestNameFromFilePath(_filePath);
    for (const auto& comment : _comments.preName) {
        fs << comment << std::endl;
    }
    fs << nameNoExtension << std::endl;
    for (const auto& comment : _comments.preCollName) {
        fs << comment << std::endl;
    }
    fs << _databaseNeeded << std::endl;

    // Interleave comments and coll file lines.
    auto commentItr = _comments.preCollFiles.begin();
    for (const auto& coll : _collectionsNeeded) {
        if (commentItr != _comments.preCollFiles.end()) {
            for (const auto& comment : *commentItr) {
                fs << comment << std::endl;
            }
            ++commentItr;
        }
        fs << coll << std::endl;
    }

    // Drain the remaining comments. In practice, there should only be at most one more entry in
    // _comments.preCollFiles than in _collectionsNeeded.
    for (; commentItr != _comments.preCollFiles.end(); ++commentItr) {
        for (const auto& comment : *commentItr) {
            fs << comment << std::endl;
        }
    }

    // Newline after the header is included in the write-out before each test.

    // Write out each test.
    for (const auto& test : _tests) {
        // Newline before each test write-out.
        fs << std::endl;
        _testNumToQuery[test.getTestNum()] = test.getTestLine();
        test.writeToStream(fs, opt);
    }

    return true;
}
}  // namespace mongo::query_tester
