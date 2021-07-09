/*
 * nssai_mapping.h
 *
 * 
 */

#ifndef _OpenAPI_nssai_mapping_H_
#define _OpenAPI_nssai_mapping_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_nssai_mapping_s OpenAPI_nssai_mapping_t;
typedef struct OpenAPI_nssai_mapping_s {
    struct OpenAPI_snssai_s *mapped_snssai;
    struct OpenAPI_snssai_s *h_snssai;
} OpenAPI_nssai_mapping_t;

OpenAPI_nssai_mapping_t *OpenAPI_nssai_mapping_create(
    OpenAPI_snssai_t *mapped_snssai,
    OpenAPI_snssai_t *h_snssai
);
void OpenAPI_nssai_mapping_free(OpenAPI_nssai_mapping_t *nssai_mapping);
OpenAPI_nssai_mapping_t *OpenAPI_nssai_mapping_parseFromJSON(cJSON *nssai_mappingJSON);
cJSON *OpenAPI_nssai_mapping_convertToJSON(OpenAPI_nssai_mapping_t *nssai_mapping);
OpenAPI_nssai_mapping_t *OpenAPI_nssai_mapping_copy(OpenAPI_nssai_mapping_t *dst, OpenAPI_nssai_mapping_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nssai_mapping_H_ */

