/*
 * pcf_for_ue_binding_patch.h
 *
 * Identifies the updates of an Individual PCF for a UE binding.
 */

#ifndef _OpenAPI_pcf_for_ue_binding_patch_H_
#define _OpenAPI_pcf_for_ue_binding_patch_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ip_end_point.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pcf_for_ue_binding_patch_s OpenAPI_pcf_for_ue_binding_patch_t;
typedef struct OpenAPI_pcf_for_ue_binding_patch_s {
    char *pcf_for_ue_fqdn;
    OpenAPI_list_t *pcf_for_ue_ip_end_points;
    char *pcf_id;
} OpenAPI_pcf_for_ue_binding_patch_t;

OpenAPI_pcf_for_ue_binding_patch_t *OpenAPI_pcf_for_ue_binding_patch_create(
    char *pcf_for_ue_fqdn,
    OpenAPI_list_t *pcf_for_ue_ip_end_points,
    char *pcf_id
);
void OpenAPI_pcf_for_ue_binding_patch_free(OpenAPI_pcf_for_ue_binding_patch_t *pcf_for_ue_binding_patch);
OpenAPI_pcf_for_ue_binding_patch_t *OpenAPI_pcf_for_ue_binding_patch_parseFromJSON(cJSON *pcf_for_ue_binding_patchJSON);
cJSON *OpenAPI_pcf_for_ue_binding_patch_convertToJSON(OpenAPI_pcf_for_ue_binding_patch_t *pcf_for_ue_binding_patch);
OpenAPI_pcf_for_ue_binding_patch_t *OpenAPI_pcf_for_ue_binding_patch_copy(OpenAPI_pcf_for_ue_binding_patch_t *dst, OpenAPI_pcf_for_ue_binding_patch_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pcf_for_ue_binding_patch_H_ */

