/*
 * slice_replacement_mapping.h
 *
 * Represents the mapping between a replaced S-NSSAI in serving PLMN to its alternative S-NSSAI 
 */

#ifndef _OpenAPI_slice_replacement_mapping_H_
#define _OpenAPI_slice_replacement_mapping_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_slice_replacement_mapping_s OpenAPI_slice_replacement_mapping_t;
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_slice_replacement_mapping_s {
    struct OpenAPI_snssai_s *replaced_snssai;
    struct OpenAPI_snssai_s *alt_snssai;
};

OpenAPI_slice_replacement_mapping_t *OpenAPI_slice_replacement_mapping_create(
    OpenAPI_snssai_t *replaced_snssai,
    OpenAPI_snssai_t *alt_snssai
);
void OpenAPI_slice_replacement_mapping_free(OpenAPI_slice_replacement_mapping_t *slice_replacement_mapping);
OpenAPI_slice_replacement_mapping_t *OpenAPI_slice_replacement_mapping_parseFromJSON(cJSON *slice_replacement_mappingJSON);
cJSON *OpenAPI_slice_replacement_mapping_convertToJSON(OpenAPI_slice_replacement_mapping_t *slice_replacement_mapping);
OpenAPI_slice_replacement_mapping_t *OpenAPI_slice_replacement_mapping_copy(OpenAPI_slice_replacement_mapping_t *dst, OpenAPI_slice_replacement_mapping_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_slice_replacement_mapping_H_ */

