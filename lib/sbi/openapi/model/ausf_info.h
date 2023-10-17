/*
 * ausf_info.h
 *
 * Information of an AUSF NF Instance
 */

#ifndef _OpenAPI_ausf_info_H_
#define _OpenAPI_ausf_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "suci_info.h"
#include "supi_range.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ausf_info_s OpenAPI_ausf_info_t;
typedef struct OpenAPI_ausf_info_s {
    char *group_id;
    OpenAPI_list_t *supi_ranges;
    OpenAPI_list_t *routing_indicators;
    OpenAPI_list_t *suci_infos;
} OpenAPI_ausf_info_t;

OpenAPI_ausf_info_t *OpenAPI_ausf_info_create(
    char *group_id,
    OpenAPI_list_t *supi_ranges,
    OpenAPI_list_t *routing_indicators,
    OpenAPI_list_t *suci_infos
);
void OpenAPI_ausf_info_free(OpenAPI_ausf_info_t *ausf_info);
OpenAPI_ausf_info_t *OpenAPI_ausf_info_parseFromJSON(cJSON *ausf_infoJSON);
cJSON *OpenAPI_ausf_info_convertToJSON(OpenAPI_ausf_info_t *ausf_info);
OpenAPI_ausf_info_t *OpenAPI_ausf_info_copy(OpenAPI_ausf_info_t *dst, OpenAPI_ausf_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ausf_info_H_ */

