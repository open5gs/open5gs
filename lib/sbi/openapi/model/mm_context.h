/*
 * mm_context.h
 *
 *
 */

#ifndef _OpenAPI_mm_context_H_
#define _OpenAPI_mm_context_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "access_type.h"
#include "expected_ue_behavior.h"
#include "nas_security_mode.h"
#include "nssai_mapping.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_mm_context_s OpenAPI_mm_context_t;
typedef struct OpenAPI_mm_context_s {
    OpenAPI_access_type_e access_type;
    struct OpenAPI_nas_security_mode_s *nas_security_mode;
    int nas_downlink_count;
    int nas_uplink_count;
    char ue_security_capability;
    char s1_ue_network_capability;
    OpenAPI_list_t *allowed_nssai;
    OpenAPI_list_t *nssai_mapping_list;
    OpenAPI_list_t *ns_instance_list;
    struct OpenAPI_expected_ue_behavior_s *expected_u_ebehavior;
    char plmn_assi_ue_radio_cap_id;
    char man_assi_ue_radio_cap_id;
    char *ucmf_dic_entry_id;
} OpenAPI_mm_context_t;

OpenAPI_mm_context_t *OpenAPI_mm_context_create(
    OpenAPI_access_type_e access_type,
    OpenAPI_nas_security_mode_t *nas_security_mode,
    int nas_downlink_count,
    int nas_uplink_count,
    char ue_security_capability,
    char s1_ue_network_capability,
    OpenAPI_list_t *allowed_nssai,
    OpenAPI_list_t *nssai_mapping_list,
    OpenAPI_list_t *ns_instance_list,
    OpenAPI_expected_ue_behavior_t *expected_u_ebehavior,
    char plmn_assi_ue_radio_cap_id,
    char man_assi_ue_radio_cap_id,
    char *ucmf_dic_entry_id
    );
void OpenAPI_mm_context_free(OpenAPI_mm_context_t *mm_context);
OpenAPI_mm_context_t *OpenAPI_mm_context_parseFromJSON(cJSON *mm_contextJSON);
cJSON *OpenAPI_mm_context_convertToJSON(OpenAPI_mm_context_t *mm_context);
OpenAPI_mm_context_t *OpenAPI_mm_context_copy(OpenAPI_mm_context_t *dst, OpenAPI_mm_context_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_mm_context_H_ */

