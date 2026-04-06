/*
 * locality_description.h
 *
 * Locality description
 */

#ifndef _OpenAPI_locality_description_H_
#define _OpenAPI_locality_description_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_locality_description_s OpenAPI_locality_description_t;
#include "locality_description_item.h"
#include "locality_type.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_locality_description_s {
    OpenAPI_locality_type_e locality_type;
    char *locality_value;
    OpenAPI_list_t *addl_loc_descr_items;
};

OpenAPI_locality_description_t *OpenAPI_locality_description_create(
    OpenAPI_locality_type_e locality_type,
    char *locality_value,
    OpenAPI_list_t *addl_loc_descr_items
);
void OpenAPI_locality_description_free(OpenAPI_locality_description_t *locality_description);
OpenAPI_locality_description_t *OpenAPI_locality_description_parseFromJSON(cJSON *locality_descriptionJSON);
cJSON *OpenAPI_locality_description_convertToJSON(OpenAPI_locality_description_t *locality_description);
OpenAPI_locality_description_t *OpenAPI_locality_description_copy(OpenAPI_locality_description_t *dst, OpenAPI_locality_description_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_locality_description_H_ */

