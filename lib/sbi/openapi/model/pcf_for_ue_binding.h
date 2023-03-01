/*
 * pcf_for_ue_binding.h
 *
 * Identifies an Individual PCF for a UE binding.
 */

#ifndef _OpenAPI_pcf_for_ue_binding_H_
#define _OpenAPI_pcf_for_ue_binding_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "binding_level.h"
#include "ip_end_point.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pcf_for_ue_binding_s OpenAPI_pcf_for_ue_binding_t;
typedef struct OpenAPI_pcf_for_ue_binding_s {
    char *supi;
    char *gpsi;
    char *pcf_for_ue_fqdn;
    OpenAPI_list_t *pcf_for_ue_ip_end_points;
    char *pcf_id;
    char *pcf_set_id;
    OpenAPI_binding_level_e bind_level;
    char *supp_feat;
} OpenAPI_pcf_for_ue_binding_t;

OpenAPI_pcf_for_ue_binding_t *OpenAPI_pcf_for_ue_binding_create(
    char *supi,
    char *gpsi,
    char *pcf_for_ue_fqdn,
    OpenAPI_list_t *pcf_for_ue_ip_end_points,
    char *pcf_id,
    char *pcf_set_id,
    OpenAPI_binding_level_e bind_level,
    char *supp_feat
);
void OpenAPI_pcf_for_ue_binding_free(OpenAPI_pcf_for_ue_binding_t *pcf_for_ue_binding);
OpenAPI_pcf_for_ue_binding_t *OpenAPI_pcf_for_ue_binding_parseFromJSON(cJSON *pcf_for_ue_bindingJSON);
cJSON *OpenAPI_pcf_for_ue_binding_convertToJSON(OpenAPI_pcf_for_ue_binding_t *pcf_for_ue_binding);
OpenAPI_pcf_for_ue_binding_t *OpenAPI_pcf_for_ue_binding_copy(OpenAPI_pcf_for_ue_binding_t *dst, OpenAPI_pcf_for_ue_binding_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pcf_for_ue_binding_H_ */

