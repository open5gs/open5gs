/*
 * sm_context_release_data.h
 *
 * 
 */

#ifndef _OpenAPI_sm_context_release_data_H_
#define _OpenAPI_sm_context_release_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "cause.h"
#include "n2_sm_info_type.h"
#include "ng_ap_cause.h"
#include "ref_to_binary_data.h"
#include "user_location.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_sm_context_release_data_s OpenAPI_sm_context_release_data_t;
typedef struct OpenAPI_sm_context_release_data_s {
    OpenAPI_cause_e cause;
    struct OpenAPI_ng_ap_cause_s *ng_ap_cause;
    bool is__5g_mm_cause_value;
    int _5g_mm_cause_value;
    struct OpenAPI_user_location_s *ue_location;
    char *ue_time_zone;
    struct OpenAPI_user_location_s *add_ue_location;
    bool is_vsmf_release_only;
    int vsmf_release_only;
    struct OpenAPI_ref_to_binary_data_s *n2_sm_info;
    OpenAPI_n2_sm_info_type_e n2_sm_info_type;
    bool is_ismf_release_only;
    int ismf_release_only;
} OpenAPI_sm_context_release_data_t;

OpenAPI_sm_context_release_data_t *OpenAPI_sm_context_release_data_create(
    OpenAPI_cause_e cause,
    OpenAPI_ng_ap_cause_t *ng_ap_cause,
    bool is__5g_mm_cause_value,
    int _5g_mm_cause_value,
    OpenAPI_user_location_t *ue_location,
    char *ue_time_zone,
    OpenAPI_user_location_t *add_ue_location,
    bool is_vsmf_release_only,
    int vsmf_release_only,
    OpenAPI_ref_to_binary_data_t *n2_sm_info,
    OpenAPI_n2_sm_info_type_e n2_sm_info_type,
    bool is_ismf_release_only,
    int ismf_release_only
);
void OpenAPI_sm_context_release_data_free(OpenAPI_sm_context_release_data_t *sm_context_release_data);
OpenAPI_sm_context_release_data_t *OpenAPI_sm_context_release_data_parseFromJSON(cJSON *sm_context_release_dataJSON);
cJSON *OpenAPI_sm_context_release_data_convertToJSON(OpenAPI_sm_context_release_data_t *sm_context_release_data);
OpenAPI_sm_context_release_data_t *OpenAPI_sm_context_release_data_copy(OpenAPI_sm_context_release_data_t *dst, OpenAPI_sm_context_release_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sm_context_release_data_H_ */

