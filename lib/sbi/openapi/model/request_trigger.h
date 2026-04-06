/*
 * request_trigger.h
 *
 * 
 */

#ifndef _OpenAPI_request_trigger_H_
#define _OpenAPI_request_trigger_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_request_trigger_NULL = 0, OpenAPI_request_trigger_LOC_CH, OpenAPI_request_trigger_PRA_CH, OpenAPI_request_trigger_SERV_AREA_CH, OpenAPI_request_trigger_RFSP_CH, OpenAPI_request_trigger_ALLOWED_NSSAI_CH, OpenAPI_request_trigger_UE_AMBR_CH, OpenAPI_request_trigger_UE_SLICE_MBR_CH, OpenAPI_request_trigger_SMF_SELECT_CH, OpenAPI_request_trigger_ACCESS_TYPE_CH, OpenAPI_request_trigger_NWDAF_DATA_CH, OpenAPI_request_trigger_TARGET_NSSAI, OpenAPI_request_trigger_SLICE_REPLACE_MGMT, OpenAPI_request_trigger_FEAT_RENEG, OpenAPI_request_trigger_PARTIALLY_ALLOWED_NSSAI_CH, OpenAPI_request_trigger_SNSSAIS_PARTIALLY_REJECTED_CH, OpenAPI_request_trigger_REJECTED_SNSSAIS_CH, OpenAPI_request_trigger_PENDING_NSSAI_CH, OpenAPI_request_trigger_RAT_TYPE_CH, OpenAPI_request_trigger_SLICE_REPLACE_OUTCOME, OpenAPI_request_trigger_ENERGY_SAV_IND_CH } OpenAPI_request_trigger_e;

char* OpenAPI_request_trigger_ToString(OpenAPI_request_trigger_e request_trigger);

OpenAPI_request_trigger_e OpenAPI_request_trigger_FromString(char* request_trigger);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_request_trigger_H_ */

