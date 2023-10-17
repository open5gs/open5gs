/*
 * authorization_info.h
 *
 * Represents NIDD authorization information.
 */

#ifndef _OpenAPI_authorization_info_H_
#define _OpenAPI_authorization_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "context_info.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_authorization_info_s OpenAPI_authorization_info_t;
typedef struct OpenAPI_authorization_info_s {
    struct OpenAPI_snssai_s *snssai;
    char *dnn;
    char *mtc_provider_information;
    char *auth_update_callback_uri;
    char *af_id;
    char *nef_id;
    char *validity_time;
    struct OpenAPI_context_info_s *context_info;
} OpenAPI_authorization_info_t;

OpenAPI_authorization_info_t *OpenAPI_authorization_info_create(
    OpenAPI_snssai_t *snssai,
    char *dnn,
    char *mtc_provider_information,
    char *auth_update_callback_uri,
    char *af_id,
    char *nef_id,
    char *validity_time,
    OpenAPI_context_info_t *context_info
);
void OpenAPI_authorization_info_free(OpenAPI_authorization_info_t *authorization_info);
OpenAPI_authorization_info_t *OpenAPI_authorization_info_parseFromJSON(cJSON *authorization_infoJSON);
cJSON *OpenAPI_authorization_info_convertToJSON(OpenAPI_authorization_info_t *authorization_info);
OpenAPI_authorization_info_t *OpenAPI_authorization_info_copy(OpenAPI_authorization_info_t *dst, OpenAPI_authorization_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_authorization_info_H_ */

