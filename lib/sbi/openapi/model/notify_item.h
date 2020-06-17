/*
 * notify_item.h
 *
 *
 */

#ifndef _OpenAPI_notify_item_H_
#define _OpenAPI_notify_item_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "change_item.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_notify_item_s OpenAPI_notify_item_t;
typedef struct OpenAPI_notify_item_s {
    char *resource_id;
    OpenAPI_list_t *changes;
} OpenAPI_notify_item_t;

OpenAPI_notify_item_t *OpenAPI_notify_item_create(
    char *resource_id,
    OpenAPI_list_t *changes
    );
void OpenAPI_notify_item_free(OpenAPI_notify_item_t *notify_item);
OpenAPI_notify_item_t *OpenAPI_notify_item_parseFromJSON(cJSON *notify_itemJSON);
cJSON *OpenAPI_notify_item_convertToJSON(OpenAPI_notify_item_t *notify_item);
OpenAPI_notify_item_t *OpenAPI_notify_item_copy(OpenAPI_notify_item_t *dst, OpenAPI_notify_item_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_notify_item_H_ */

