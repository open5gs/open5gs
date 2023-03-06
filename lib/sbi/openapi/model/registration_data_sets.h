/*
 * registration_data_sets.h
 *
 * 
 */

#ifndef _OpenAPI_registration_data_sets_H_
#define _OpenAPI_registration_data_sets_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "amf3_gpp_access_registration.h"
#include "amf_non3_gpp_access_registration.h"
#include "ip_sm_gw_registration.h"
#include "nwdaf_registration_info.h"
#include "smf_registration_info.h"
#include "smsf_registration.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_registration_data_sets_s OpenAPI_registration_data_sets_t;
typedef struct OpenAPI_registration_data_sets_s {
    struct OpenAPI_amf3_gpp_access_registration_s *amf3_gpp;
    struct OpenAPI_amf_non3_gpp_access_registration_s *amf_non3_gpp;
    struct OpenAPI_smf_registration_info_s *smf_registration;
    struct OpenAPI_smsf_registration_s *smsf3_gpp;
    struct OpenAPI_smsf_registration_s *smsf_non3_gpp;
    struct OpenAPI_ip_sm_gw_registration_s *ip_sm_gw;
    struct OpenAPI_nwdaf_registration_info_s *nwdaf_registration;
} OpenAPI_registration_data_sets_t;

OpenAPI_registration_data_sets_t *OpenAPI_registration_data_sets_create(
    OpenAPI_amf3_gpp_access_registration_t *amf3_gpp,
    OpenAPI_amf_non3_gpp_access_registration_t *amf_non3_gpp,
    OpenAPI_smf_registration_info_t *smf_registration,
    OpenAPI_smsf_registration_t *smsf3_gpp,
    OpenAPI_smsf_registration_t *smsf_non3_gpp,
    OpenAPI_ip_sm_gw_registration_t *ip_sm_gw,
    OpenAPI_nwdaf_registration_info_t *nwdaf_registration
);
void OpenAPI_registration_data_sets_free(OpenAPI_registration_data_sets_t *registration_data_sets);
OpenAPI_registration_data_sets_t *OpenAPI_registration_data_sets_parseFromJSON(cJSON *registration_data_setsJSON);
cJSON *OpenAPI_registration_data_sets_convertToJSON(OpenAPI_registration_data_sets_t *registration_data_sets);
OpenAPI_registration_data_sets_t *OpenAPI_registration_data_sets_copy(OpenAPI_registration_data_sets_t *dst, OpenAPI_registration_data_sets_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_registration_data_sets_H_ */

