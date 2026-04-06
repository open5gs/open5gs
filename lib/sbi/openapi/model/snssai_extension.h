/*
 * snssai_extension.h
 *
 * Extensions to the Snssai data type, sdRanges and wildcardSd shall not be present simultaneously 
 */

#ifndef _OpenAPI_snssai_extension_H_
#define _OpenAPI_snssai_extension_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_snssai_extension_s OpenAPI_snssai_extension_t;
#include "sd_range.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_snssai_extension_s {
    OpenAPI_list_t *sd_ranges;
    bool is_wildcard_sd;
    int wildcard_sd;
};

OpenAPI_snssai_extension_t *OpenAPI_snssai_extension_create(
    OpenAPI_list_t *sd_ranges,
    bool is_wildcard_sd,
    int wildcard_sd
);
void OpenAPI_snssai_extension_free(OpenAPI_snssai_extension_t *snssai_extension);
OpenAPI_snssai_extension_t *OpenAPI_snssai_extension_parseFromJSON(cJSON *snssai_extensionJSON);
cJSON *OpenAPI_snssai_extension_convertToJSON(OpenAPI_snssai_extension_t *snssai_extension);
OpenAPI_snssai_extension_t *OpenAPI_snssai_extension_copy(OpenAPI_snssai_extension_t *dst, OpenAPI_snssai_extension_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_snssai_extension_H_ */

