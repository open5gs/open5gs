/*
 * callback_uri_prefix_item.h
 *
 * callback URI prefix value to be used for specific notification types
 */

#ifndef _OpenAPI_callback_uri_prefix_item_H_
#define _OpenAPI_callback_uri_prefix_item_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_callback_uri_prefix_item_s OpenAPI_callback_uri_prefix_item_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_callback_uri_prefix_item_s {
    char *callback_uri_prefix;
    OpenAPI_list_t *notification_types;
};

OpenAPI_callback_uri_prefix_item_t *OpenAPI_callback_uri_prefix_item_create(
    char *callback_uri_prefix,
    OpenAPI_list_t *notification_types
);
void OpenAPI_callback_uri_prefix_item_free(OpenAPI_callback_uri_prefix_item_t *callback_uri_prefix_item);
OpenAPI_callback_uri_prefix_item_t *OpenAPI_callback_uri_prefix_item_parseFromJSON(cJSON *callback_uri_prefix_itemJSON);
cJSON *OpenAPI_callback_uri_prefix_item_convertToJSON(OpenAPI_callback_uri_prefix_item_t *callback_uri_prefix_item);
OpenAPI_callback_uri_prefix_item_t *OpenAPI_callback_uri_prefix_item_copy(OpenAPI_callback_uri_prefix_item_t *dst, OpenAPI_callback_uri_prefix_item_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_callback_uri_prefix_item_H_ */

