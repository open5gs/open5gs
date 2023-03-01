/*
 * pcf_mbs_binding.h
 *
 * Represents an Individual PCF for an MBS Session binding.
 */

#ifndef _OpenAPI_pcf_mbs_binding_H_
#define _OpenAPI_pcf_mbs_binding_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "binding_level.h"
#include "ip_end_point.h"
#include "mbs_session_id.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pcf_mbs_binding_s OpenAPI_pcf_mbs_binding_t;
typedef struct OpenAPI_pcf_mbs_binding_s {
    struct OpenAPI_mbs_session_id_s *mbs_session_id;
    char *pcf_fqdn;
    OpenAPI_list_t *pcf_ip_end_points;
    char *pcf_id;
    char *pcf_set_id;
    OpenAPI_binding_level_e bind_level;
    char *recovery_time;
    char *supp_feat;
} OpenAPI_pcf_mbs_binding_t;

OpenAPI_pcf_mbs_binding_t *OpenAPI_pcf_mbs_binding_create(
    OpenAPI_mbs_session_id_t *mbs_session_id,
    char *pcf_fqdn,
    OpenAPI_list_t *pcf_ip_end_points,
    char *pcf_id,
    char *pcf_set_id,
    OpenAPI_binding_level_e bind_level,
    char *recovery_time,
    char *supp_feat
);
void OpenAPI_pcf_mbs_binding_free(OpenAPI_pcf_mbs_binding_t *pcf_mbs_binding);
OpenAPI_pcf_mbs_binding_t *OpenAPI_pcf_mbs_binding_parseFromJSON(cJSON *pcf_mbs_bindingJSON);
cJSON *OpenAPI_pcf_mbs_binding_convertToJSON(OpenAPI_pcf_mbs_binding_t *pcf_mbs_binding);
OpenAPI_pcf_mbs_binding_t *OpenAPI_pcf_mbs_binding_copy(OpenAPI_pcf_mbs_binding_t *dst, OpenAPI_pcf_mbs_binding_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pcf_mbs_binding_H_ */

