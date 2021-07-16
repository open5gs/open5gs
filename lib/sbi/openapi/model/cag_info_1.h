/*
 * cag_info_1.h
 *
 * 
 */

#ifndef _OpenAPI_cag_info_1_H_
#define _OpenAPI_cag_info_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_cag_info_1_s OpenAPI_cag_info_1_t;
typedef struct OpenAPI_cag_info_1_s {
    OpenAPI_list_t *allowed_cag_list;
    bool is_cag_only_indicator;
    int cag_only_indicator;
} OpenAPI_cag_info_1_t;

OpenAPI_cag_info_1_t *OpenAPI_cag_info_1_create(
    OpenAPI_list_t *allowed_cag_list,
    bool is_cag_only_indicator,
    int cag_only_indicator
);
void OpenAPI_cag_info_1_free(OpenAPI_cag_info_1_t *cag_info_1);
OpenAPI_cag_info_1_t *OpenAPI_cag_info_1_parseFromJSON(cJSON *cag_info_1JSON);
cJSON *OpenAPI_cag_info_1_convertToJSON(OpenAPI_cag_info_1_t *cag_info_1);
OpenAPI_cag_info_1_t *OpenAPI_cag_info_1_copy(OpenAPI_cag_info_1_t *dst, OpenAPI_cag_info_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_cag_info_1_H_ */

