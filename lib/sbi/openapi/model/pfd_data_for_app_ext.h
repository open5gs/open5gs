/*
 * pfd_data_for_app_ext.h
 *
 *
 */

#ifndef _OpenAPI_pfd_data_for_app_ext_H_
#define _OpenAPI_pfd_data_for_app_ext_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "pfd_content.h"
#include "pfd_data_for_app.h"
#include "pfd_data_for_app_ext_all_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pfd_data_for_app_ext_s OpenAPI_pfd_data_for_app_ext_t;
typedef struct OpenAPI_pfd_data_for_app_ext_s {
    char *application_id;
    OpenAPI_list_t *pfds;
    char *caching_time;
    char *supp_feat;
} OpenAPI_pfd_data_for_app_ext_t;

OpenAPI_pfd_data_for_app_ext_t *OpenAPI_pfd_data_for_app_ext_create(
    char *application_id,
    OpenAPI_list_t *pfds,
    char *caching_time,
    char *supp_feat
    );
void OpenAPI_pfd_data_for_app_ext_free(OpenAPI_pfd_data_for_app_ext_t *pfd_data_for_app_ext);
OpenAPI_pfd_data_for_app_ext_t *OpenAPI_pfd_data_for_app_ext_parseFromJSON(cJSON *pfd_data_for_app_extJSON);
cJSON *OpenAPI_pfd_data_for_app_ext_convertToJSON(OpenAPI_pfd_data_for_app_ext_t *pfd_data_for_app_ext);
OpenAPI_pfd_data_for_app_ext_t *OpenAPI_pfd_data_for_app_ext_copy(OpenAPI_pfd_data_for_app_ext_t *dst, OpenAPI_pfd_data_for_app_ext_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pfd_data_for_app_ext_H_ */

