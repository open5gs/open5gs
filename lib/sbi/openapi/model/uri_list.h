/*
 * uri_list.h
 *
 * Represents a set of URIs following the 3GPP hypermedia format (containing a \&quot;_links\&quot; attribute). 
 */

#ifndef _OpenAPI_uri_list_H_
#define _OpenAPI_uri_list_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "links_value_schema.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_uri_list_s OpenAPI_uri_list_t;
typedef struct OpenAPI_uri_list_s {
    OpenAPI_list_t* _links;
    bool is_total_item_count;
    int total_item_count;
} OpenAPI_uri_list_t;

OpenAPI_uri_list_t *OpenAPI_uri_list_create(
    OpenAPI_list_t* _links,
    bool is_total_item_count,
    int total_item_count
);
void OpenAPI_uri_list_free(OpenAPI_uri_list_t *uri_list);
OpenAPI_uri_list_t *OpenAPI_uri_list_parseFromJSON(cJSON *uri_listJSON);
cJSON *OpenAPI_uri_list_convertToJSON(OpenAPI_uri_list_t *uri_list);
OpenAPI_uri_list_t *OpenAPI_uri_list_copy(OpenAPI_uri_list_t *dst, OpenAPI_uri_list_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_uri_list_H_ */

