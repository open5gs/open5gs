/*
 * links_value_schema.h
 *
 * 
 */

#ifndef _OpenAPI_links_value_schema_H_
#define _OpenAPI_links_value_schema_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "link.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_links_value_schema_s OpenAPI_links_value_schema_t;
typedef struct OpenAPI_links_value_schema_s {
    char *href;
} OpenAPI_links_value_schema_t;

OpenAPI_links_value_schema_t *OpenAPI_links_value_schema_create(
    char *href
);
void OpenAPI_links_value_schema_free(OpenAPI_links_value_schema_t *links_value_schema);
OpenAPI_links_value_schema_t *OpenAPI_links_value_schema_parseFromJSON(cJSON *links_value_schemaJSON);
cJSON *OpenAPI_links_value_schema_convertToJSON(OpenAPI_links_value_schema_t *links_value_schema);
OpenAPI_links_value_schema_t *OpenAPI_links_value_schema_copy(OpenAPI_links_value_schema_t *dst, OpenAPI_links_value_schema_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_links_value_schema_H_ */

