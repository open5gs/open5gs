/*
 * patch_item.h
 *
 * 
 */

#ifndef _OpenAPI_patch_item_H_
#define _OpenAPI_patch_item_H_

#include <string.h>
#include "../openapi/external/cJSON.h"
#include "../openapi/include/list.h"
#include "../openapi/model/patch_operation.h"

#include "../custom/any_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_patch_item_s OpenAPI_patch_item_t;
typedef struct OpenAPI_patch_item_s {
    OpenAPI_patch_operation_e op;
    char *path;
    char *from;
    OpenAPI_any_type_t *value;
} OpenAPI_patch_item_t;

OpenAPI_patch_item_t *OpenAPI_patch_item_create(
    OpenAPI_patch_operation_e op,
    char *path,
    char *from,
    OpenAPI_any_type_t *value
);
void OpenAPI_patch_item_free(OpenAPI_patch_item_t *patch_item);
OpenAPI_patch_item_t *OpenAPI_patch_item_parseFromJSON(cJSON *patch_itemJSON);
cJSON *OpenAPI_patch_item_convertToJSON(OpenAPI_patch_item_t *patch_item);
OpenAPI_patch_item_t *OpenAPI_patch_item_copy(OpenAPI_patch_item_t *dst, OpenAPI_patch_item_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_patch_item_H_ */

