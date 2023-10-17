/*
 * failure_code.h
 *
 * 
 */

#ifndef _OpenAPI_failure_code_H_
#define _OpenAPI_failure_code_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_failure_code_NULL = 0, OpenAPI_failure_code_UNK_RULE_ID, OpenAPI_failure_code_RA_GR_ERR, OpenAPI_failure_code_SER_ID_ERR, OpenAPI_failure_code_NF_MAL, OpenAPI_failure_code_RES_LIM, OpenAPI_failure_code_MAX_NR_QoS_FLOW, OpenAPI_failure_code_MISS_FLOW_INFO, OpenAPI_failure_code_RES_ALLO_FAIL, OpenAPI_failure_code_UNSUCC_QOS_VAL, OpenAPI_failure_code_INCOR_FLOW_INFO, OpenAPI_failure_code_PS_TO_CS_HAN, OpenAPI_failure_code_APP_ID_ERR, OpenAPI_failure_code_NO_QOS_FLOW_BOUND, OpenAPI_failure_code_FILTER_RES, OpenAPI_failure_code_MISS_REDI_SER_ADDR, OpenAPI_failure_code_CM_END_USER_SER_DENIED, OpenAPI_failure_code_CM_CREDIT_CON_NOT_APP, OpenAPI_failure_code_CM_AUTH_REJ, OpenAPI_failure_code_CM_USER_UNK, OpenAPI_failure_code_CM_RAT_FAILED, OpenAPI_failure_code_UE_STA_SUSP, OpenAPI_failure_code_UNKNOWN_REF_ID, OpenAPI_failure_code_INCORRECT_COND_DATA, OpenAPI_failure_code_REF_ID_COLLISION, OpenAPI_failure_code_TRAFFIC_STEERING_ERROR, OpenAPI_failure_code_DNAI_STEERING_ERROR, OpenAPI_failure_code_AN_GW_FAILE, OpenAPI_failure_code_MAX_NR_PACKET_FILTERS_EXCEEDED, OpenAPI_failure_code_PACKET_FILTER_TFT_ALLOCATION_EXCEEDED, OpenAPI_failure_code_MUTE_CHG_NOT_ALLOWED } OpenAPI_failure_code_e;

char* OpenAPI_failure_code_ToString(OpenAPI_failure_code_e failure_code);

OpenAPI_failure_code_e OpenAPI_failure_code_FromString(char* failure_code);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_failure_code_H_ */

