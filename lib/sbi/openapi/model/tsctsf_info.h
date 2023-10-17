/*
 * tsctsf_info.h
 *
 * Information of a TSCTSF NF Instance
 */

#ifndef _OpenAPI_tsctsf_info_H_
#define _OpenAPI_tsctsf_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "identity_range.h"
#include "internal_group_id_range.h"
#include "snssai_tsctsf_info_item.h"
#include "supi_range.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_tsctsf_info_s OpenAPI_tsctsf_info_t;
typedef struct OpenAPI_tsctsf_info_s {
    OpenAPI_list_t* s_nssai_info_list;
    OpenAPI_list_t *external_group_identifiers_ranges;
    OpenAPI_list_t *supi_ranges;
    OpenAPI_list_t *gpsi_ranges;
    OpenAPI_list_t *internal_group_identifiers_ranges;
} OpenAPI_tsctsf_info_t;

OpenAPI_tsctsf_info_t *OpenAPI_tsctsf_info_create(
    OpenAPI_list_t* s_nssai_info_list,
    OpenAPI_list_t *external_group_identifiers_ranges,
    OpenAPI_list_t *supi_ranges,
    OpenAPI_list_t *gpsi_ranges,
    OpenAPI_list_t *internal_group_identifiers_ranges
);
void OpenAPI_tsctsf_info_free(OpenAPI_tsctsf_info_t *tsctsf_info);
OpenAPI_tsctsf_info_t *OpenAPI_tsctsf_info_parseFromJSON(cJSON *tsctsf_infoJSON);
cJSON *OpenAPI_tsctsf_info_convertToJSON(OpenAPI_tsctsf_info_t *tsctsf_info);
OpenAPI_tsctsf_info_t *OpenAPI_tsctsf_info_copy(OpenAPI_tsctsf_info_t *dst, OpenAPI_tsctsf_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_tsctsf_info_H_ */

