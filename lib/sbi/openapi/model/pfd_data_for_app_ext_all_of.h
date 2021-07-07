/*
 * pfd_data_for_app_ext_all_of.h
 *
 * 
 */

#ifndef _OpenAPI_pfd_data_for_app_ext_all_of_H_
#define _OpenAPI_pfd_data_for_app_ext_all_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pfd_data_for_app_ext_all_of_s OpenAPI_pfd_data_for_app_ext_all_of_t;
typedef struct OpenAPI_pfd_data_for_app_ext_all_of_s {
    char *supp_feat;
} OpenAPI_pfd_data_for_app_ext_all_of_t;

OpenAPI_pfd_data_for_app_ext_all_of_t *OpenAPI_pfd_data_for_app_ext_all_of_create(
    char *supp_feat
);
void OpenAPI_pfd_data_for_app_ext_all_of_free(OpenAPI_pfd_data_for_app_ext_all_of_t *pfd_data_for_app_ext_all_of);
OpenAPI_pfd_data_for_app_ext_all_of_t *OpenAPI_pfd_data_for_app_ext_all_of_parseFromJSON(cJSON *pfd_data_for_app_ext_all_ofJSON);
cJSON *OpenAPI_pfd_data_for_app_ext_all_of_convertToJSON(OpenAPI_pfd_data_for_app_ext_all_of_t *pfd_data_for_app_ext_all_of);
OpenAPI_pfd_data_for_app_ext_all_of_t *OpenAPI_pfd_data_for_app_ext_all_of_copy(OpenAPI_pfd_data_for_app_ext_all_of_t *dst, OpenAPI_pfd_data_for_app_ext_all_of_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pfd_data_for_app_ext_all_of_H_ */

