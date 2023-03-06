/*
 * pcf_for_ue_info.h
 *
 * Contains the information of the PCF for a UE.
 */

#ifndef _OpenAPI_pcf_for_ue_info_H_
#define _OpenAPI_pcf_for_ue_info_H_

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

typedef struct OpenAPI_pcf_for_ue_info_s OpenAPI_pcf_for_ue_info_t;
typedef struct OpenAPI_pcf_for_ue_info_s {
    char *pcf_fqdn;
    OpenAPI_list_t *pcf_ip_end_points;
    char *pcf_id;
    char *pcf_set_id;
    OpenAPI_binding_level_e bind_level;
} OpenAPI_pcf_for_ue_info_t;

OpenAPI_pcf_for_ue_info_t *OpenAPI_pcf_for_ue_info_create(
    char *pcf_fqdn,
    OpenAPI_list_t *pcf_ip_end_points,
    char *pcf_id,
    char *pcf_set_id,
    OpenAPI_binding_level_e bind_level
);
void OpenAPI_pcf_for_ue_info_free(OpenAPI_pcf_for_ue_info_t *pcf_for_ue_info);
OpenAPI_pcf_for_ue_info_t *OpenAPI_pcf_for_ue_info_parseFromJSON(cJSON *pcf_for_ue_infoJSON);
cJSON *OpenAPI_pcf_for_ue_info_convertToJSON(OpenAPI_pcf_for_ue_info_t *pcf_for_ue_info);
OpenAPI_pcf_for_ue_info_t *OpenAPI_pcf_for_ue_info_copy(OpenAPI_pcf_for_ue_info_t *dst, OpenAPI_pcf_for_ue_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pcf_for_ue_info_H_ */

