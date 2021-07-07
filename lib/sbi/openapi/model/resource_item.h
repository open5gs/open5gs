/*
 * resource_item.h
 *
 * Identifies a subscription to policy data change notification when the change occurs in a fragment (subset of resource data) of a given resource.
 */

#ifndef _OpenAPI_resource_item_H_
#define _OpenAPI_resource_item_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_resource_item_s OpenAPI_resource_item_t;
typedef struct OpenAPI_resource_item_s {
    char *mon_resource_uri;
    OpenAPI_list_t *items;
} OpenAPI_resource_item_t;

OpenAPI_resource_item_t *OpenAPI_resource_item_create(
    char *mon_resource_uri,
    OpenAPI_list_t *items
);
void OpenAPI_resource_item_free(OpenAPI_resource_item_t *resource_item);
OpenAPI_resource_item_t *OpenAPI_resource_item_parseFromJSON(cJSON *resource_itemJSON);
cJSON *OpenAPI_resource_item_convertToJSON(OpenAPI_resource_item_t *resource_item);
OpenAPI_resource_item_t *OpenAPI_resource_item_copy(OpenAPI_resource_item_t *dst, OpenAPI_resource_item_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_resource_item_H_ */

