/*
 * locality_description_item.h
 *
 * Locality description item
 */

#ifndef _OpenAPI_locality_description_item_H_
#define _OpenAPI_locality_description_item_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_locality_description_item_s OpenAPI_locality_description_item_t;
#include "locality_type.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_locality_description_item_s {
    OpenAPI_locality_type_e locality_type;
    char *locality_value;
};

OpenAPI_locality_description_item_t *OpenAPI_locality_description_item_create(
    OpenAPI_locality_type_e locality_type,
    char *locality_value
);
void OpenAPI_locality_description_item_free(OpenAPI_locality_description_item_t *locality_description_item);
OpenAPI_locality_description_item_t *OpenAPI_locality_description_item_parseFromJSON(cJSON *locality_description_itemJSON);
cJSON *OpenAPI_locality_description_item_convertToJSON(OpenAPI_locality_description_item_t *locality_description_item);
OpenAPI_locality_description_item_t *OpenAPI_locality_description_item_copy(OpenAPI_locality_description_item_t *dst, OpenAPI_locality_description_item_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_locality_description_item_H_ */

