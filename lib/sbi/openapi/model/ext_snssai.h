/*
 * ext_snssai.h
 *
 * The sdRanges and wildcardSd attributes shall be exclusive from each other. If one of these attributes is present,  the sd attribute shall also be present and it shall contain one Slice Differentiator value within the range of SD  (if the sdRanges attribute is present) or with any value (if the wildcardSd attribute is present). 
 */

#ifndef _OpenAPI_ext_snssai_H_
#define _OpenAPI_ext_snssai_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "sd_range.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ext_snssai_s OpenAPI_ext_snssai_t;
typedef enum { OpenAPI_ext_snssai_WILDCARDSD_NULL = 0, OpenAPI_ext_snssai_WILDCARDSD__true } OpenAPI_ext_snssai_wildcard_sd_e;

char* OpenAPI_ext_snssai_wildcard_sd_ToString(OpenAPI_ext_snssai_wildcard_sd_e wildcard_sd);

OpenAPI_ext_snssai_wildcard_sd_e OpenAPI_ext_snssai_wildcard_sd_FromString(char* wildcard_sd);
typedef struct OpenAPI_ext_snssai_s {
    int sst;
    char *sd;
    OpenAPI_list_t *sd_ranges;
    OpenAPI_ext_snssai_wildcard_sd_e wildcard_sd;
} OpenAPI_ext_snssai_t;

OpenAPI_ext_snssai_t *OpenAPI_ext_snssai_create(
    int sst,
    char *sd,
    OpenAPI_list_t *sd_ranges,
    OpenAPI_ext_snssai_wildcard_sd_e wildcard_sd
);
void OpenAPI_ext_snssai_free(OpenAPI_ext_snssai_t *ext_snssai);
OpenAPI_ext_snssai_t *OpenAPI_ext_snssai_parseFromJSON(cJSON *ext_snssaiJSON);
cJSON *OpenAPI_ext_snssai_convertToJSON(OpenAPI_ext_snssai_t *ext_snssai);
OpenAPI_ext_snssai_t *OpenAPI_ext_snssai_copy(OpenAPI_ext_snssai_t *dst, OpenAPI_ext_snssai_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ext_snssai_H_ */

