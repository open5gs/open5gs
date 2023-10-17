/*
 * nssaaf_info.h
 *
 * Information of a NSSAAF Instance
 */

#ifndef _OpenAPI_nssaaf_info_H_
#define _OpenAPI_nssaaf_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "internal_group_id_range.h"
#include "supi_range.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_nssaaf_info_s OpenAPI_nssaaf_info_t;
typedef struct OpenAPI_nssaaf_info_s {
    OpenAPI_list_t *supi_ranges;
    OpenAPI_list_t *internal_group_identifiers_ranges;
} OpenAPI_nssaaf_info_t;

OpenAPI_nssaaf_info_t *OpenAPI_nssaaf_info_create(
    OpenAPI_list_t *supi_ranges,
    OpenAPI_list_t *internal_group_identifiers_ranges
);
void OpenAPI_nssaaf_info_free(OpenAPI_nssaaf_info_t *nssaaf_info);
OpenAPI_nssaaf_info_t *OpenAPI_nssaaf_info_parseFromJSON(cJSON *nssaaf_infoJSON);
cJSON *OpenAPI_nssaaf_info_convertToJSON(OpenAPI_nssaaf_info_t *nssaaf_info);
OpenAPI_nssaaf_info_t *OpenAPI_nssaaf_info_copy(OpenAPI_nssaaf_info_t *dst, OpenAPI_nssaaf_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nssaaf_info_H_ */

