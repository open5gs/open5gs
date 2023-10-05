/*
 * change_item.h
 *
 * It contains data which need to be changed.
 */

#ifndef _OpenAPI_change_item_H_
#define _OpenAPI_change_item_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "any_type.h"
#include "change_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_change_item_s OpenAPI_change_item_t;
typedef struct OpenAPI_change_item_s {
    OpenAPI_change_type_e op;
    char *path;
    char *from;
    bool is_orig_value_null;
    OpenAPI_any_type_t *orig_value;
    bool is_new_value_null;
    OpenAPI_any_type_t *new_value;
} OpenAPI_change_item_t;

OpenAPI_change_item_t *OpenAPI_change_item_create(
    OpenAPI_change_type_e op,
    char *path,
    char *from,
    bool is_orig_value_null,
    OpenAPI_any_type_t *orig_value,
    bool is_new_value_null,
    OpenAPI_any_type_t *new_value
);
void OpenAPI_change_item_free(OpenAPI_change_item_t *change_item);
OpenAPI_change_item_t *OpenAPI_change_item_parseFromJSON(cJSON *change_itemJSON);
cJSON *OpenAPI_change_item_convertToJSON(OpenAPI_change_item_t *change_item);
OpenAPI_change_item_t *OpenAPI_change_item_copy(OpenAPI_change_item_t *dst, OpenAPI_change_item_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_change_item_H_ */

