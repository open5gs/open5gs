/*
 * change_item.h
 *
 *
 */

#ifndef _OpenAPI_change_item_H_
#define _OpenAPI_change_item_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "change_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_change_item_s OpenAPI_change_item_t;
typedef struct OpenAPI_change_item_s {
    OpenAPI_change_type_e op;
    char *path;
    char *from;
    char *orig_value;
    char *new_value;
} OpenAPI_change_item_t;

OpenAPI_change_item_t *OpenAPI_change_item_create(
    OpenAPI_change_type_e op,
    char *path,
    char *from,
    char *orig_value,
    char *new_value
    );
void OpenAPI_change_item_free(OpenAPI_change_item_t *change_item);
OpenAPI_change_item_t *OpenAPI_change_item_parseFromJSON(cJSON *change_itemJSON);
cJSON *OpenAPI_change_item_convertToJSON(OpenAPI_change_item_t *change_item);
OpenAPI_change_item_t *OpenAPI_change_item_copy(OpenAPI_change_item_t *dst, OpenAPI_change_item_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_change_item_H_ */

