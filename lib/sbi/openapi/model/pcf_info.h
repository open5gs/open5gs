/*
 * pcf_info.h
 *
 * Information of a PCF NF Instance
 */

#ifndef _OpenAPI_pcf_info_H_
#define _OpenAPI_pcf_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "identity_range.h"
#include "pro_se_capability.h"
#include "supi_range.h"
#include "v2x_capability.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pcf_info_s OpenAPI_pcf_info_t;
typedef struct OpenAPI_pcf_info_s {
    char *group_id;
    OpenAPI_list_t *dnn_list;
    OpenAPI_list_t *supi_ranges;
    OpenAPI_list_t *gpsi_ranges;
    char *rx_diam_host;
    char *rx_diam_realm;
    bool is_v2x_support_ind;
    int v2x_support_ind;
    bool is_prose_support_ind;
    int prose_support_ind;
    struct OpenAPI_pro_se_capability_s *prose_capability;
    struct OpenAPI_v2x_capability_s *v2x_capability;
} OpenAPI_pcf_info_t;

OpenAPI_pcf_info_t *OpenAPI_pcf_info_create(
    char *group_id,
    OpenAPI_list_t *dnn_list,
    OpenAPI_list_t *supi_ranges,
    OpenAPI_list_t *gpsi_ranges,
    char *rx_diam_host,
    char *rx_diam_realm,
    bool is_v2x_support_ind,
    int v2x_support_ind,
    bool is_prose_support_ind,
    int prose_support_ind,
    OpenAPI_pro_se_capability_t *prose_capability,
    OpenAPI_v2x_capability_t *v2x_capability
);
void OpenAPI_pcf_info_free(OpenAPI_pcf_info_t *pcf_info);
OpenAPI_pcf_info_t *OpenAPI_pcf_info_parseFromJSON(cJSON *pcf_infoJSON);
cJSON *OpenAPI_pcf_info_convertToJSON(OpenAPI_pcf_info_t *pcf_info);
OpenAPI_pcf_info_t *OpenAPI_pcf_info_copy(OpenAPI_pcf_info_t *dst, OpenAPI_pcf_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pcf_info_H_ */

