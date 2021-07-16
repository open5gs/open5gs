/*
 * ext_snssai.h
 *
 * 
 */

#ifndef _OpenAPI_ext_snssai_H_
#define _OpenAPI_ext_snssai_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "sd_range.h"
#include "snssai.h"
#include "snssai_extension.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ext_snssai_s OpenAPI_ext_snssai_t;
typedef struct OpenAPI_ext_snssai_s {
    int sst;
    char *sd;
    OpenAPI_list_t *sd_ranges;
    bool is_wildcard_sd;
    int wildcard_sd;
} OpenAPI_ext_snssai_t;

OpenAPI_ext_snssai_t *OpenAPI_ext_snssai_create(
    int sst,
    char *sd,
    OpenAPI_list_t *sd_ranges,
    bool is_wildcard_sd,
    int wildcard_sd
);
void OpenAPI_ext_snssai_free(OpenAPI_ext_snssai_t *ext_snssai);
OpenAPI_ext_snssai_t *OpenAPI_ext_snssai_parseFromJSON(cJSON *ext_snssaiJSON);
cJSON *OpenAPI_ext_snssai_convertToJSON(OpenAPI_ext_snssai_t *ext_snssai);
OpenAPI_ext_snssai_t *OpenAPI_ext_snssai_copy(OpenAPI_ext_snssai_t *dst, OpenAPI_ext_snssai_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ext_snssai_H_ */

