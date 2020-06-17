/*
 * request_indication.h
 *
 * Possible values are - UE_REQ_PDU_SES_MOD - UE_REQ_PDU_SES_REL - PDU_SES_MOB - NW_REQ_PDU_SES_AUTH - NW_REQ_PDU_SES_MOD - NW_REQ_PDU_SES_REL - EBI_ASSIGNMENT_REQ
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

typedef struct OpenAPI_request_indication_s OpenAPI_request_indication_t;
typedef struct OpenAPI_request_indication_s {
} OpenAPI_request_indication_t;

OpenAPI_request_indication_t *OpenAPI_request_indication_create(
    );
void OpenAPI_request_indication_free(OpenAPI_request_indication_t *request_indication);
OpenAPI_request_indication_t *OpenAPI_request_indication_parseFromJSON(cJSON *request_indicationJSON);
cJSON *OpenAPI_request_indication_convertToJSON(OpenAPI_request_indication_t *request_indication);
OpenAPI_request_indication_t *OpenAPI_request_indication_copy(OpenAPI_request_indication_t *dst, OpenAPI_request_indication_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_request_indication_H_ */

