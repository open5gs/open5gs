/*
 * app_session_context_resp_data.h
 *
 * Describes the authorization data of an Individual Application Session Context created by the PCF.
 */

#ifndef _OpenAPI_app_session_context_resp_data_H_
#define _OpenAPI_app_session_context_resp_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "serv_auth_info.h"
#include "ue_identity_info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_app_session_context_resp_data_s OpenAPI_app_session_context_resp_data_t;
typedef struct OpenAPI_app_session_context_resp_data_s {
    OpenAPI_serv_auth_info_e serv_auth_info;
    OpenAPI_list_t *ue_ids;
    char *supp_feat;
} OpenAPI_app_session_context_resp_data_t;

OpenAPI_app_session_context_resp_data_t *OpenAPI_app_session_context_resp_data_create(
    OpenAPI_serv_auth_info_e serv_auth_info,
    OpenAPI_list_t *ue_ids,
    char *supp_feat
);
void OpenAPI_app_session_context_resp_data_free(OpenAPI_app_session_context_resp_data_t *app_session_context_resp_data);
OpenAPI_app_session_context_resp_data_t *OpenAPI_app_session_context_resp_data_parseFromJSON(cJSON *app_session_context_resp_dataJSON);
cJSON *OpenAPI_app_session_context_resp_data_convertToJSON(OpenAPI_app_session_context_resp_data_t *app_session_context_resp_data);
OpenAPI_app_session_context_resp_data_t *OpenAPI_app_session_context_resp_data_copy(OpenAPI_app_session_context_resp_data_t *dst, OpenAPI_app_session_context_resp_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_app_session_context_resp_data_H_ */

