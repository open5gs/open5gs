/*
 * cag_info_1.h
 *
 * contains CAG IDs.
 */

#ifndef _OpenAPI_cag_info_1_H_
#define _OpenAPI_cag_info_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_cag_info_1_s OpenAPI_cag_info_1_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_cag_info_1_s {
    OpenAPI_list_t *cag_list;
};

OpenAPI_cag_info_1_t *OpenAPI_cag_info_1_create(
    OpenAPI_list_t *cag_list
);
void OpenAPI_cag_info_1_free(OpenAPI_cag_info_1_t *cag_info_1);
OpenAPI_cag_info_1_t *OpenAPI_cag_info_1_parseFromJSON(cJSON *cag_info_1JSON);
cJSON *OpenAPI_cag_info_1_convertToJSON(OpenAPI_cag_info_1_t *cag_info_1);
OpenAPI_cag_info_1_t *OpenAPI_cag_info_1_copy(OpenAPI_cag_info_1_t *dst, OpenAPI_cag_info_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_cag_info_1_H_ */

