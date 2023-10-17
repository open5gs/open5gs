/*
 * slice_info_for_registration.h
 *
 * Contains the slice information requested during a Registration procedure
 */

#ifndef _OpenAPI_slice_info_for_registration_H_
#define _OpenAPI_slice_info_for_registration_H_

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

typedef struct OpenAPI_slice_info_for_registration_s OpenAPI_slice_info_for_registration_t;
typedef struct OpenAPI_slice_info_for_registration_s {
    OpenAPI_list_t *subscribed_nssai;
    struct OpenAPI_allowed_nssai_s *allowed_nssai_current_access;
    struct OpenAPI_allowed_nssai_s *allowed_nssai_other_access;
    OpenAPI_list_t *s_nssai_for_mapping;
    OpenAPI_list_t *requested_nssai;
    bool is_default_configured_snssai_ind;
    int default_configured_snssai_ind;
    OpenAPI_list_t *mapping_of_nssai;
    bool is_request_mapping;
    int request_mapping;
    bool is_ue_sup_nssrg_ind;
    int ue_sup_nssrg_ind;
    bool is_suppress_nssrg_ind;
    int suppress_nssrg_ind;
    bool is_nsag_supported;
    int nsag_supported;
} OpenAPI_slice_info_for_registration_t;

OpenAPI_slice_info_for_registration_t *OpenAPI_slice_info_for_registration_create(
    OpenAPI_list_t *subscribed_nssai,
    OpenAPI_allowed_nssai_t *allowed_nssai_current_access,
    OpenAPI_allowed_nssai_t *allowed_nssai_other_access,
    OpenAPI_list_t *s_nssai_for_mapping,
    OpenAPI_list_t *requested_nssai,
    bool is_default_configured_snssai_ind,
    int default_configured_snssai_ind,
    OpenAPI_list_t *mapping_of_nssai,
    bool is_request_mapping,
    int request_mapping,
    bool is_ue_sup_nssrg_ind,
    int ue_sup_nssrg_ind,
    bool is_suppress_nssrg_ind,
    int suppress_nssrg_ind,
    bool is_nsag_supported,
    int nsag_supported
);
void OpenAPI_slice_info_for_registration_free(OpenAPI_slice_info_for_registration_t *slice_info_for_registration);
OpenAPI_slice_info_for_registration_t *OpenAPI_slice_info_for_registration_parseFromJSON(cJSON *slice_info_for_registrationJSON);
cJSON *OpenAPI_slice_info_for_registration_convertToJSON(OpenAPI_slice_info_for_registration_t *slice_info_for_registration);
OpenAPI_slice_info_for_registration_t *OpenAPI_slice_info_for_registration_copy(OpenAPI_slice_info_for_registration_t *dst, OpenAPI_slice_info_for_registration_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_slice_info_for_registration_H_ */

