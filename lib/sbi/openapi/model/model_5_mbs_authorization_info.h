/*
 * model_5_mbs_authorization_info.h
 *
 * 
 */

#ifndef _OpenAPI_model_5_mbs_authorization_info_H_
#define _OpenAPI_model_5_mbs_authorization_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "mbs_session_id.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_model_5_mbs_authorization_info_s OpenAPI_model_5_mbs_authorization_info_t;
typedef struct OpenAPI_model_5_mbs_authorization_info_s {
    OpenAPI_list_t *_5mbs_session_ids;
} OpenAPI_model_5_mbs_authorization_info_t;

OpenAPI_model_5_mbs_authorization_info_t *OpenAPI_model_5_mbs_authorization_info_create(
    OpenAPI_list_t *_5mbs_session_ids
);
void OpenAPI_model_5_mbs_authorization_info_free(OpenAPI_model_5_mbs_authorization_info_t *model_5_mbs_authorization_info);
OpenAPI_model_5_mbs_authorization_info_t *OpenAPI_model_5_mbs_authorization_info_parseFromJSON(cJSON *model_5_mbs_authorization_infoJSON);
cJSON *OpenAPI_model_5_mbs_authorization_info_convertToJSON(OpenAPI_model_5_mbs_authorization_info_t *model_5_mbs_authorization_info);
OpenAPI_model_5_mbs_authorization_info_t *OpenAPI_model_5_mbs_authorization_info_copy(OpenAPI_model_5_mbs_authorization_info_t *dst, OpenAPI_model_5_mbs_authorization_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_model_5_mbs_authorization_info_H_ */

