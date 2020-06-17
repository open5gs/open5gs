/*
 * area_of_validity.h
 *
 *
 */

#ifndef _OpenAPI_area_of_validity_H_
#define _OpenAPI_area_of_validity_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "tai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_area_of_validity_s OpenAPI_area_of_validity_t;
typedef struct OpenAPI_area_of_validity_s {
    OpenAPI_list_t *tai_list;
} OpenAPI_area_of_validity_t;

OpenAPI_area_of_validity_t *OpenAPI_area_of_validity_create(
    OpenAPI_list_t *tai_list
    );
void OpenAPI_area_of_validity_free(OpenAPI_area_of_validity_t *area_of_validity);
OpenAPI_area_of_validity_t *OpenAPI_area_of_validity_parseFromJSON(cJSON *area_of_validityJSON);
cJSON *OpenAPI_area_of_validity_convertToJSON(OpenAPI_area_of_validity_t *area_of_validity);
OpenAPI_area_of_validity_t *OpenAPI_area_of_validity_copy(OpenAPI_area_of_validity_t *dst, OpenAPI_area_of_validity_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_area_of_validity_H_ */

