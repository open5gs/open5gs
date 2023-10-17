/*
 * updated_item.h
 *
 * Identifies a fragment of a resource.
 */

#ifndef _OpenAPI_updated_item_H_
#define _OpenAPI_updated_item_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "any_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_updated_item_s OpenAPI_updated_item_t;
typedef struct OpenAPI_updated_item_s {
    char *item;
    bool is_value_null;
    OpenAPI_any_type_t *value;
} OpenAPI_updated_item_t;

OpenAPI_updated_item_t *OpenAPI_updated_item_create(
    char *item,
    bool is_value_null,
    OpenAPI_any_type_t *value
);
void OpenAPI_updated_item_free(OpenAPI_updated_item_t *updated_item);
OpenAPI_updated_item_t *OpenAPI_updated_item_parseFromJSON(cJSON *updated_itemJSON);
cJSON *OpenAPI_updated_item_convertToJSON(OpenAPI_updated_item_t *updated_item);
OpenAPI_updated_item_t *OpenAPI_updated_item_copy(OpenAPI_updated_item_t *dst, OpenAPI_updated_item_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_updated_item_H_ */

