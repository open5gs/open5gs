/*
 * n4_message_type.h
 *
 * Possible values are   - PFCP_SES_EST_REQ   - PFCP_SES_EST_RSP   - PFCP_SES_MOD_REQ   - PFCP_SES_MOD_RSP   - PFCP_SES_DEL_REQ   - PFCP_SES_DEL_RSP   - PFCP_SES_REP_REQ   - PFCP_SES_REP_RSP
 */

#ifndef _OpenAPI_n4_message_type_H_
#define _OpenAPI_n4_message_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_n4_message_type_s OpenAPI_n4_message_type_t;
typedef struct OpenAPI_n4_message_type_s {
} OpenAPI_n4_message_type_t;

OpenAPI_n4_message_type_t *OpenAPI_n4_message_type_create(
    );
void OpenAPI_n4_message_type_free(OpenAPI_n4_message_type_t *n4_message_type);
OpenAPI_n4_message_type_t *OpenAPI_n4_message_type_parseFromJSON(cJSON *n4_message_typeJSON);
cJSON *OpenAPI_n4_message_type_convertToJSON(OpenAPI_n4_message_type_t *n4_message_type);
OpenAPI_n4_message_type_t *OpenAPI_n4_message_type_copy(OpenAPI_n4_message_type_t *dst, OpenAPI_n4_message_type_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_n4_message_type_H_ */

