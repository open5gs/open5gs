/*
 * notification_item.h
 *
 * Identifies a data change notification when the change occurs in a fragment (subset of resource data) of a given resource.
 */

#ifndef _OpenAPI_notification_item_H_
#define _OpenAPI_notification_item_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "updated_item.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_notification_item_s OpenAPI_notification_item_t;
typedef struct OpenAPI_notification_item_s {
    char *resource_id;
    OpenAPI_list_t *notif_items;
} OpenAPI_notification_item_t;

OpenAPI_notification_item_t *OpenAPI_notification_item_create(
    char *resource_id,
    OpenAPI_list_t *notif_items
    );
void OpenAPI_notification_item_free(OpenAPI_notification_item_t *notification_item);
OpenAPI_notification_item_t *OpenAPI_notification_item_parseFromJSON(cJSON *notification_itemJSON);
cJSON *OpenAPI_notification_item_convertToJSON(OpenAPI_notification_item_t *notification_item);
OpenAPI_notification_item_t *OpenAPI_notification_item_copy(OpenAPI_notification_item_t *dst, OpenAPI_notification_item_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_notification_item_H_ */

