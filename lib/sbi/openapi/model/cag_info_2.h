/*
 * cag_info_2.h
 *
 * contains CAG IDs.
 */

#ifndef _OpenAPI_cag_info_2_H_
#define _OpenAPI_cag_info_2_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_cag_info_2_s OpenAPI_cag_info_2_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_cag_info_2_s {
    OpenAPI_list_t *cag_list;
};

OpenAPI_cag_info_2_t *OpenAPI_cag_info_2_create(
    OpenAPI_list_t *cag_list
);
void OpenAPI_cag_info_2_free(OpenAPI_cag_info_2_t *cag_info_2);
OpenAPI_cag_info_2_t *OpenAPI_cag_info_2_parseFromJSON(cJSON *cag_info_2JSON);
cJSON *OpenAPI_cag_info_2_convertToJSON(OpenAPI_cag_info_2_t *cag_info_2);
OpenAPI_cag_info_2_t *OpenAPI_cag_info_2_copy(OpenAPI_cag_info_2_t *dst, OpenAPI_cag_info_2_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_cag_info_2_H_ */

