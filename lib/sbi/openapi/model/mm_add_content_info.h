/*
 * mm_add_content_info.h
 *
 * 
 */

#ifndef _OpenAPI_mm_add_content_info_H_
#define _OpenAPI_mm_add_content_info_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_mm_add_content_info_s OpenAPI_mm_add_content_info_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_mm_add_content_info_s {
    char *type_number;
    char *addtype_info;
    bool is_content_size;
    int content_size;
};

OpenAPI_mm_add_content_info_t *OpenAPI_mm_add_content_info_create(
    char *type_number,
    char *addtype_info,
    bool is_content_size,
    int content_size
);
void OpenAPI_mm_add_content_info_free(OpenAPI_mm_add_content_info_t *mm_add_content_info);
OpenAPI_mm_add_content_info_t *OpenAPI_mm_add_content_info_parseFromJSON(cJSON *mm_add_content_infoJSON);
cJSON *OpenAPI_mm_add_content_info_convertToJSON(OpenAPI_mm_add_content_info_t *mm_add_content_info);
OpenAPI_mm_add_content_info_t *OpenAPI_mm_add_content_info_copy(OpenAPI_mm_add_content_info_t *dst, OpenAPI_mm_add_content_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_mm_add_content_info_H_ */

