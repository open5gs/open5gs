/*
 * smf_registration.h
 *
 *
 */

#ifndef _OpenAPI_smf_registration_H_
#define _OpenAPI_smf_registration_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "plmn_id.h"
#include "registration_reason.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_smf_registration_s OpenAPI_smf_registration_t;
typedef struct OpenAPI_smf_registration_s {
    char *smf_instance_id;
    char *smf_set_id;
    char *supported_features;
    int pdu_session_id;
    struct OpenAPI_snssai_s *single_nssai;
    char *dnn;
    int emergency_services;
    char *pcscf_restoration_callback_uri;
    struct OpenAPI_plmn_id_s *plmn_id;
    char *pgw_fqdn;
    int epdg_ind;
    char *dereg_callback_uri;
    struct OpenAPI_registration_reason_s *registration_reason;
    char *registration_time;
} OpenAPI_smf_registration_t;

OpenAPI_smf_registration_t *OpenAPI_smf_registration_create(
    char *smf_instance_id,
    char *smf_set_id,
    char *supported_features,
    int pdu_session_id,
    OpenAPI_snssai_t *single_nssai,
    char *dnn,
    int emergency_services,
    char *pcscf_restoration_callback_uri,
    OpenAPI_plmn_id_t *plmn_id,
    char *pgw_fqdn,
    int epdg_ind,
    char *dereg_callback_uri,
    OpenAPI_registration_reason_t *registration_reason,
    char *registration_time
    );
void OpenAPI_smf_registration_free(OpenAPI_smf_registration_t *smf_registration);
OpenAPI_smf_registration_t *OpenAPI_smf_registration_parseFromJSON(cJSON *smf_registrationJSON);
cJSON *OpenAPI_smf_registration_convertToJSON(OpenAPI_smf_registration_t *smf_registration);
OpenAPI_smf_registration_t *OpenAPI_smf_registration_copy(OpenAPI_smf_registration_t *dst, OpenAPI_smf_registration_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_smf_registration_H_ */

