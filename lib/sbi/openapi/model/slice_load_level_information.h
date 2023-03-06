/*
 * slice_load_level_information.h
 *
 * Contains load level information applicable for one or several slices.
 */

#ifndef _OpenAPI_slice_load_level_information_H_
#define _OpenAPI_slice_load_level_information_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_slice_load_level_information_s OpenAPI_slice_load_level_information_t;
typedef struct OpenAPI_slice_load_level_information_s {
    int load_level_information;
    OpenAPI_list_t *snssais;
} OpenAPI_slice_load_level_information_t;

OpenAPI_slice_load_level_information_t *OpenAPI_slice_load_level_information_create(
    int load_level_information,
    OpenAPI_list_t *snssais
);
void OpenAPI_slice_load_level_information_free(OpenAPI_slice_load_level_information_t *slice_load_level_information);
OpenAPI_slice_load_level_information_t *OpenAPI_slice_load_level_information_parseFromJSON(cJSON *slice_load_level_informationJSON);
cJSON *OpenAPI_slice_load_level_information_convertToJSON(OpenAPI_slice_load_level_information_t *slice_load_level_information);
OpenAPI_slice_load_level_information_t *OpenAPI_slice_load_level_information_copy(OpenAPI_slice_load_level_information_t *dst, OpenAPI_slice_load_level_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_slice_load_level_information_H_ */

