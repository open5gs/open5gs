/*
 * pfd_data_for_app.h
 *
 *
 */

#ifndef _OpenAPI_pfd_data_for_app_H_
#define _OpenAPI_pfd_data_for_app_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "pfd_content.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pfd_data_for_app_s OpenAPI_pfd_data_for_app_t;
typedef struct OpenAPI_pfd_data_for_app_s {
    char *application_id;
    OpenAPI_list_t *pfds;
    char *caching_time;
} OpenAPI_pfd_data_for_app_t;

OpenAPI_pfd_data_for_app_t *OpenAPI_pfd_data_for_app_create(
    char *application_id,
    OpenAPI_list_t *pfds,
    char *caching_time
    );
void OpenAPI_pfd_data_for_app_free(OpenAPI_pfd_data_for_app_t *pfd_data_for_app);
OpenAPI_pfd_data_for_app_t *OpenAPI_pfd_data_for_app_parseFromJSON(cJSON *pfd_data_for_appJSON);
cJSON *OpenAPI_pfd_data_for_app_convertToJSON(OpenAPI_pfd_data_for_app_t *pfd_data_for_app);
OpenAPI_pfd_data_for_app_t *OpenAPI_pfd_data_for_app_copy(OpenAPI_pfd_data_for_app_t *dst, OpenAPI_pfd_data_for_app_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pfd_data_for_app_H_ */

