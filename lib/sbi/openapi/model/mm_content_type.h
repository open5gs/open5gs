/*
 * mm_content_type.h
 *
 * 
 */

#ifndef _OpenAPI_mm_content_type_H_
#define _OpenAPI_mm_content_type_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_mm_content_type_s OpenAPI_mm_content_type_t;
#include "mm_add_content_info.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_mm_content_type_s {
    char *type_number;
    char *addtype_info;
    bool is_content_size;
    int content_size;
    OpenAPI_list_t *mm_add_content_info;
};

OpenAPI_mm_content_type_t *OpenAPI_mm_content_type_create(
    char *type_number,
    char *addtype_info,
    bool is_content_size,
    int content_size,
    OpenAPI_list_t *mm_add_content_info
);
void OpenAPI_mm_content_type_free(OpenAPI_mm_content_type_t *mm_content_type);
OpenAPI_mm_content_type_t *OpenAPI_mm_content_type_parseFromJSON(cJSON *mm_content_typeJSON);
cJSON *OpenAPI_mm_content_type_convertToJSON(OpenAPI_mm_content_type_t *mm_content_type);
OpenAPI_mm_content_type_t *OpenAPI_mm_content_type_copy(OpenAPI_mm_content_type_t *dst, OpenAPI_mm_content_type_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_mm_content_type_H_ */

