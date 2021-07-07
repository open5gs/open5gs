/*
 * request_indication.h
 *
 * 
 */

#ifndef _OpenAPI_request_indication_H_
#define _OpenAPI_request_indication_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_request_indication_NULL = 0, OpenAPI_request_indication_UE_REQ_PDU_SES_MOD, OpenAPI_request_indication_UE_REQ_PDU_SES_REL, OpenAPI_request_indication_PDU_SES_MOB, OpenAPI_request_indication_NW_REQ_PDU_SES_AUTH, OpenAPI_request_indication_NW_REQ_PDU_SES_MOD, OpenAPI_request_indication_NW_REQ_PDU_SES_REL, OpenAPI_request_indication_EBI_ASSIGNMENT_REQ, OpenAPI_request_indication_REL_DUE_TO_5G_AN_REQUEST } OpenAPI_request_indication_e;

char* OpenAPI_request_indication_ToString(OpenAPI_request_indication_e request_indication);

OpenAPI_request_indication_e OpenAPI_request_indication_FromString(char* request_indication);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_request_indication_H_ */

