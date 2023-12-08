/* This file was generated by upbc (the upb compiler) from the input
 * file:
 *
 *     xds/type/matcher/v3/range.proto
 *
 * Do not edit -- your changes will be discarded when the file is
 * regenerated. */

#include <stddef.h>
#include "upb/generated_code_support.h"
#include "xds/type/matcher/v3/range.upb.h"
#include "xds/type/v3/range.upb.h"
#include "xds/type/matcher/v3/matcher.upb.h"
#include "validate/validate.upb.h"

// Must be last.
#include "upb/port/def.inc"

static const upb_MiniTableSub xds_type_matcher_v3_Int64RangeMatcher_submsgs[1] = {
  {.submsg = &xds_type_matcher_v3_Int64RangeMatcher_RangeMatcher_msg_init},
};

static const upb_MiniTableField xds_type_matcher_v3_Int64RangeMatcher__fields[1] = {
  {1, 0, 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
};

const upb_MiniTable xds_type_matcher_v3_Int64RangeMatcher_msg_init = {
  &xds_type_matcher_v3_Int64RangeMatcher_submsgs[0],
  &xds_type_matcher_v3_Int64RangeMatcher__fields[0],
  8, 1, kUpb_ExtMode_NonExtendable, 1, UPB_FASTTABLE_MASK(8), 0,
  UPB_FASTTABLE_INIT({
    {0x0000000000000000, &_upb_FastDecoder_DecodeGeneric},
    {0x000000003f00000a, &upb_prm_1bt_max64b},
  })
};

static const upb_MiniTableSub xds_type_matcher_v3_Int64RangeMatcher_RangeMatcher_submsgs[2] = {
  {.submsg = &xds_type_v3_Int64Range_msg_init},
  {.submsg = &xds_type_matcher_v3_Matcher_OnMatch_msg_init},
};

static const upb_MiniTableField xds_type_matcher_v3_Int64RangeMatcher_RangeMatcher__fields[2] = {
  {1, UPB_SIZE(4, 8), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
  {2, UPB_SIZE(8, 16), 1, 1, 11, (int)kUpb_FieldMode_Scalar | ((int)UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
};

const upb_MiniTable xds_type_matcher_v3_Int64RangeMatcher_RangeMatcher_msg_init = {
  &xds_type_matcher_v3_Int64RangeMatcher_RangeMatcher_submsgs[0],
  &xds_type_matcher_v3_Int64RangeMatcher_RangeMatcher__fields[0],
  UPB_SIZE(16, 24), 2, kUpb_ExtMode_NonExtendable, 2, UPB_FASTTABLE_MASK(24), 0,
  UPB_FASTTABLE_INIT({
    {0x0000000000000000, &_upb_FastDecoder_DecodeGeneric},
    {0x000800003f00000a, &upb_prm_1bt_maxmaxb},
    {0x0010000001010012, &upb_psm_1bt_maxmaxb},
    {0x0000000000000000, &_upb_FastDecoder_DecodeGeneric},
  })
};

static const upb_MiniTableSub xds_type_matcher_v3_Int32RangeMatcher_submsgs[1] = {
  {.submsg = &xds_type_matcher_v3_Int32RangeMatcher_RangeMatcher_msg_init},
};

static const upb_MiniTableField xds_type_matcher_v3_Int32RangeMatcher__fields[1] = {
  {1, 0, 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
};

const upb_MiniTable xds_type_matcher_v3_Int32RangeMatcher_msg_init = {
  &xds_type_matcher_v3_Int32RangeMatcher_submsgs[0],
  &xds_type_matcher_v3_Int32RangeMatcher__fields[0],
  8, 1, kUpb_ExtMode_NonExtendable, 1, UPB_FASTTABLE_MASK(8), 0,
  UPB_FASTTABLE_INIT({
    {0x0000000000000000, &_upb_FastDecoder_DecodeGeneric},
    {0x000000003f00000a, &upb_prm_1bt_max64b},
  })
};

static const upb_MiniTableSub xds_type_matcher_v3_Int32RangeMatcher_RangeMatcher_submsgs[2] = {
  {.submsg = &xds_type_v3_Int32Range_msg_init},
  {.submsg = &xds_type_matcher_v3_Matcher_OnMatch_msg_init},
};

static const upb_MiniTableField xds_type_matcher_v3_Int32RangeMatcher_RangeMatcher__fields[2] = {
  {1, UPB_SIZE(4, 8), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
  {2, UPB_SIZE(8, 16), 1, 1, 11, (int)kUpb_FieldMode_Scalar | ((int)UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
};

const upb_MiniTable xds_type_matcher_v3_Int32RangeMatcher_RangeMatcher_msg_init = {
  &xds_type_matcher_v3_Int32RangeMatcher_RangeMatcher_submsgs[0],
  &xds_type_matcher_v3_Int32RangeMatcher_RangeMatcher__fields[0],
  UPB_SIZE(16, 24), 2, kUpb_ExtMode_NonExtendable, 2, UPB_FASTTABLE_MASK(24), 0,
  UPB_FASTTABLE_INIT({
    {0x0000000000000000, &_upb_FastDecoder_DecodeGeneric},
    {0x000800003f00000a, &upb_prm_1bt_maxmaxb},
    {0x0010000001010012, &upb_psm_1bt_maxmaxb},
    {0x0000000000000000, &_upb_FastDecoder_DecodeGeneric},
  })
};

static const upb_MiniTableSub xds_type_matcher_v3_DoubleRangeMatcher_submsgs[1] = {
  {.submsg = &xds_type_matcher_v3_DoubleRangeMatcher_RangeMatcher_msg_init},
};

static const upb_MiniTableField xds_type_matcher_v3_DoubleRangeMatcher__fields[1] = {
  {1, 0, 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
};

const upb_MiniTable xds_type_matcher_v3_DoubleRangeMatcher_msg_init = {
  &xds_type_matcher_v3_DoubleRangeMatcher_submsgs[0],
  &xds_type_matcher_v3_DoubleRangeMatcher__fields[0],
  8, 1, kUpb_ExtMode_NonExtendable, 1, UPB_FASTTABLE_MASK(8), 0,
  UPB_FASTTABLE_INIT({
    {0x0000000000000000, &_upb_FastDecoder_DecodeGeneric},
    {0x000000003f00000a, &upb_prm_1bt_max64b},
  })
};

static const upb_MiniTableSub xds_type_matcher_v3_DoubleRangeMatcher_RangeMatcher_submsgs[2] = {
  {.submsg = &xds_type_v3_DoubleRange_msg_init},
  {.submsg = &xds_type_matcher_v3_Matcher_OnMatch_msg_init},
};

static const upb_MiniTableField xds_type_matcher_v3_DoubleRangeMatcher_RangeMatcher__fields[2] = {
  {1, UPB_SIZE(4, 8), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
  {2, UPB_SIZE(8, 16), 1, 1, 11, (int)kUpb_FieldMode_Scalar | ((int)UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
};

const upb_MiniTable xds_type_matcher_v3_DoubleRangeMatcher_RangeMatcher_msg_init = {
  &xds_type_matcher_v3_DoubleRangeMatcher_RangeMatcher_submsgs[0],
  &xds_type_matcher_v3_DoubleRangeMatcher_RangeMatcher__fields[0],
  UPB_SIZE(16, 24), 2, kUpb_ExtMode_NonExtendable, 2, UPB_FASTTABLE_MASK(24), 0,
  UPB_FASTTABLE_INIT({
    {0x0000000000000000, &_upb_FastDecoder_DecodeGeneric},
    {0x000800003f00000a, &upb_prm_1bt_maxmaxb},
    {0x0010000001010012, &upb_psm_1bt_maxmaxb},
    {0x0000000000000000, &_upb_FastDecoder_DecodeGeneric},
  })
};

static const upb_MiniTable *messages_layout[6] = {
  &xds_type_matcher_v3_Int64RangeMatcher_msg_init,
  &xds_type_matcher_v3_Int64RangeMatcher_RangeMatcher_msg_init,
  &xds_type_matcher_v3_Int32RangeMatcher_msg_init,
  &xds_type_matcher_v3_Int32RangeMatcher_RangeMatcher_msg_init,
  &xds_type_matcher_v3_DoubleRangeMatcher_msg_init,
  &xds_type_matcher_v3_DoubleRangeMatcher_RangeMatcher_msg_init,
};

const upb_MiniTableFile xds_type_matcher_v3_range_proto_upb_file_layout = {
  messages_layout,
  NULL,
  NULL,
  6,
  0,
  0,
};

#include "upb/port/undef.inc"
