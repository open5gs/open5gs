/*
 * udr_info.h
 *
 * Information of an UDR NF Instance
 */

#ifndef _OpenAPI_udr_info_H_
#define _OpenAPI_udr_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "data_set_id.h"
#include "identity_range.h"
#include "supi_range.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_udr_info_s OpenAPI_udr_info_t;
typedef struct OpenAPI_udr_info_s {
    char *group_id;
    OpenAPI_list_t *supi_ranges;
    OpenAPI_list_t *gpsi_ranges;
    OpenAPI_list_t *external_group_identifiers_ranges;
    OpenAPI_list_t *supported_data_sets;
} OpenAPI_udr_info_t;

OpenAPI_udr_info_t *OpenAPI_udr_info_create(
    char *group_id,
    OpenAPI_list_t *supi_ranges,
    OpenAPI_list_t *gpsi_ranges,
    OpenAPI_list_t *external_group_identifiers_ranges,
    OpenAPI_list_t *supported_data_sets
);
void OpenAPI_udr_info_free(OpenAPI_udr_info_t *udr_info);
OpenAPI_udr_info_t *OpenAPI_udr_info_parseFromJSON(cJSON *udr_infoJSON);
cJSON *OpenAPI_udr_info_convertToJSON(OpenAPI_udr_info_t *udr_info);
OpenAPI_udr_info_t *OpenAPI_udr_info_copy(OpenAPI_udr_info_t *dst, OpenAPI_udr_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_udr_info_H_ */

