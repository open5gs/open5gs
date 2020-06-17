/*
 * n2_sm_info_type.h
 *
 *
 */

#ifndef _OpenAPI_n2_sm_info_type_H_
#define _OpenAPI_n2_sm_info_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_n2_sm_info_type_NULL = 0, OpenAPI_n2_sm_info_type_PDU_RES_SETUP_REQ, OpenAPI_n2_sm_info_type_PDU_RES_SETUP_RSP, OpenAPI_n2_sm_info_type_PDU_RES_SETUP_FAIL, OpenAPI_n2_sm_info_type_PDU_RES_REL_CMD, OpenAPI_n2_sm_info_type_PDU_RES_REL_RSP, OpenAPI_n2_sm_info_type_PDU_RES_MOD_REQ, OpenAPI_n2_sm_info_type_PDU_RES_MOD_RSP, OpenAPI_n2_sm_info_type_PDU_RES_MOD_FAIL, OpenAPI_n2_sm_info_type_PDU_RES_NTY, OpenAPI_n2_sm_info_type_PDU_RES_NTY_REL, OpenAPI_n2_sm_info_type_PDU_RES_MOD_IND, OpenAPI_n2_sm_info_type_PDU_RES_MOD_CFM, OpenAPI_n2_sm_info_type_PATH_SWITCH_REQ, OpenAPI_n2_sm_info_type_PATH_SWITCH_SETUP_FAIL, OpenAPI_n2_sm_info_type_PATH_SWITCH_REQ_ACK, OpenAPI_n2_sm_info_type_PATH_SWITCH_REQ_FAIL, OpenAPI_n2_sm_info_type_HANDOVER_REQUIRED, OpenAPI_n2_sm_info_type_HANDOVER_CMD, OpenAPI_n2_sm_info_type_HANDOVER_PREP_FAIL, OpenAPI_n2_sm_info_type_HANDOVER_REQ_ACK, OpenAPI_n2_sm_info_type_HANDOVER_RES_ALLOC_FAIL, OpenAPI_n2_sm_info_type_SECONDARY_RAT_USAGE } OpenAPI_n2_sm_info_type_e;

char* OpenAPI_n2_sm_info_type_ToString(OpenAPI_n2_sm_info_type_e n2_sm_info_type);

OpenAPI_n2_sm_info_type_e OpenAPI_n2_sm_info_type_FromString(char* n2_sm_info_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_n2_sm_info_type_H_ */

