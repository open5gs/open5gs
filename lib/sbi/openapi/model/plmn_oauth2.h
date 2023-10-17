/*
 * plmn_oauth2.h
 *
 * Oauth2.0 required indication for a given PLMN ID
 */

#ifndef _OpenAPI_plmn_oauth2_H_
#define _OpenAPI_plmn_oauth2_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "plmn_id.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_plmn_oauth2_s OpenAPI_plmn_oauth2_t;
typedef struct OpenAPI_plmn_oauth2_s {
    OpenAPI_list_t *oauth2_required_plmn_id_list;
    OpenAPI_list_t *oauth2_not_required_plmn_id_list;
} OpenAPI_plmn_oauth2_t;

OpenAPI_plmn_oauth2_t *OpenAPI_plmn_oauth2_create(
    OpenAPI_list_t *oauth2_required_plmn_id_list,
    OpenAPI_list_t *oauth2_not_required_plmn_id_list
);
void OpenAPI_plmn_oauth2_free(OpenAPI_plmn_oauth2_t *plmn_oauth2);
OpenAPI_plmn_oauth2_t *OpenAPI_plmn_oauth2_parseFromJSON(cJSON *plmn_oauth2JSON);
cJSON *OpenAPI_plmn_oauth2_convertToJSON(OpenAPI_plmn_oauth2_t *plmn_oauth2);
OpenAPI_plmn_oauth2_t *OpenAPI_plmn_oauth2_copy(OpenAPI_plmn_oauth2_t *dst, OpenAPI_plmn_oauth2_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_plmn_oauth2_H_ */

