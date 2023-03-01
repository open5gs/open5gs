/*
 * link.h
 *
 * It contains the URI of the linked resource.
 */

#ifndef _OpenAPI_link_H_
#define _OpenAPI_link_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_link_s OpenAPI_link_t;
typedef struct OpenAPI_link_s {
    char *href;
} OpenAPI_link_t;

OpenAPI_link_t *OpenAPI_link_create(
    char *href
);
void OpenAPI_link_free(OpenAPI_link_t *link);
OpenAPI_link_t *OpenAPI_link_parseFromJSON(cJSON *linkJSON);
cJSON *OpenAPI_link_convertToJSON(OpenAPI_link_t *link);
OpenAPI_link_t *OpenAPI_link_copy(OpenAPI_link_t *dst, OpenAPI_link_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_link_H_ */

