/*
 * udm_info.h
 *
 * Information of an UDM NF Instance
 */

#ifndef _OpenAPI_udm_info_H_
#define _OpenAPI_udm_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "identity_range.h"
#include "internal_group_id_range.h"
#include "suci_info.h"
#include "supi_range.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_udm_info_s OpenAPI_udm_info_t;
typedef struct OpenAPI_udm_info_s {
    char *group_id;
    OpenAPI_list_t *supi_ranges;
    OpenAPI_list_t *gpsi_ranges;
    OpenAPI_list_t *external_group_identifiers_ranges;
    OpenAPI_list_t *routing_indicators;
    OpenAPI_list_t *internal_group_identifiers_ranges;
    OpenAPI_list_t *suci_infos;
} OpenAPI_udm_info_t;

OpenAPI_udm_info_t *OpenAPI_udm_info_create(
    char *group_id,
    OpenAPI_list_t *supi_ranges,
    OpenAPI_list_t *gpsi_ranges,
    OpenAPI_list_t *external_group_identifiers_ranges,
    OpenAPI_list_t *routing_indicators,
    OpenAPI_list_t *internal_group_identifiers_ranges,
    OpenAPI_list_t *suci_infos
);
void OpenAPI_udm_info_free(OpenAPI_udm_info_t *udm_info);
OpenAPI_udm_info_t *OpenAPI_udm_info_parseFromJSON(cJSON *udm_infoJSON);
cJSON *OpenAPI_udm_info_convertToJSON(OpenAPI_udm_info_t *udm_info);
OpenAPI_udm_info_t *OpenAPI_udm_info_copy(OpenAPI_udm_info_t *dst, OpenAPI_udm_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_udm_info_H_ */

