/*
 * mapping_of_snssai.h
 *
 *
 */

#ifndef _OpenAPI_mapping_of_snssai_H_
#define _OpenAPI_mapping_of_snssai_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_mapping_of_snssai_s OpenAPI_mapping_of_snssai_t;
typedef struct OpenAPI_mapping_of_snssai_s {
    struct OpenAPI_snssai_s *serving_snssai;
    struct OpenAPI_snssai_s *home_snssai;
} OpenAPI_mapping_of_snssai_t;

OpenAPI_mapping_of_snssai_t *OpenAPI_mapping_of_snssai_create(
    OpenAPI_snssai_t *serving_snssai,
    OpenAPI_snssai_t *home_snssai
    );
void OpenAPI_mapping_of_snssai_free(OpenAPI_mapping_of_snssai_t *mapping_of_snssai);
OpenAPI_mapping_of_snssai_t *OpenAPI_mapping_of_snssai_parseFromJSON(cJSON *mapping_of_snssaiJSON);
cJSON *OpenAPI_mapping_of_snssai_convertToJSON(OpenAPI_mapping_of_snssai_t *mapping_of_snssai);
OpenAPI_mapping_of_snssai_t *OpenAPI_mapping_of_snssai_copy(OpenAPI_mapping_of_snssai_t *dst, OpenAPI_mapping_of_snssai_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_mapping_of_snssai_H_ */

