/*
 * slice_info_for_ue_configuration_update.h
 *
 * 
 */

#ifndef _OpenAPI_slice_info_for_ue_configuration_update_H_
#define _OpenAPI_slice_info_for_ue_configuration_update_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "allowed_nssai.h"
#include "mapping_of_snssai.h"
#include "snssai.h"
#include "subscribed_snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_slice_info_for_ue_configuration_update_s OpenAPI_slice_info_for_ue_configuration_update_t;
typedef struct OpenAPI_slice_info_for_ue_configuration_update_s {
    OpenAPI_list_t *subscribed_nssai;
    struct OpenAPI_allowed_nssai_s *allowed_nssai_current_access;
    struct OpenAPI_allowed_nssai_s *allowed_nssai_other_access;
    bool is_default_configured_snssai_ind;
    int default_configured_snssai_ind;
    OpenAPI_list_t *requested_nssai;
    OpenAPI_list_t *mapping_of_nssai;
} OpenAPI_slice_info_for_ue_configuration_update_t;

OpenAPI_slice_info_for_ue_configuration_update_t *OpenAPI_slice_info_for_ue_configuration_update_create(
    OpenAPI_list_t *subscribed_nssai,
    OpenAPI_allowed_nssai_t *allowed_nssai_current_access,
    OpenAPI_allowed_nssai_t *allowed_nssai_other_access,
    bool is_default_configured_snssai_ind,
    int default_configured_snssai_ind,
    OpenAPI_list_t *requested_nssai,
    OpenAPI_list_t *mapping_of_nssai
);
void OpenAPI_slice_info_for_ue_configuration_update_free(OpenAPI_slice_info_for_ue_configuration_update_t *slice_info_for_ue_configuration_update);
OpenAPI_slice_info_for_ue_configuration_update_t *OpenAPI_slice_info_for_ue_configuration_update_parseFromJSON(cJSON *slice_info_for_ue_configuration_updateJSON);
cJSON *OpenAPI_slice_info_for_ue_configuration_update_convertToJSON(OpenAPI_slice_info_for_ue_configuration_update_t *slice_info_for_ue_configuration_update);
OpenAPI_slice_info_for_ue_configuration_update_t *OpenAPI_slice_info_for_ue_configuration_update_copy(OpenAPI_slice_info_for_ue_configuration_update_t *dst, OpenAPI_slice_info_for_ue_configuration_update_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_slice_info_for_ue_configuration_update_H_ */

