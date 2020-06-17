/*
 * pdu_session_context.h
 *
 *
 */

#ifndef _OpenAPI_pdu_session_context_H_
#define _OpenAPI_pdu_session_context_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "access_type.h"
#include "cn_assisted_ran_para.h"
#include "ebi_arp_mapping.h"
#include "sbi_binding_level.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pdu_session_context_s OpenAPI_pdu_session_context_t;
typedef struct OpenAPI_pdu_session_context_s {
    int pdu_session_id;
    char *sm_context_ref;
    struct OpenAPI_snssai_s *s_nssai;
    char *dnn;
    OpenAPI_access_type_e access_type;
    OpenAPI_access_type_e additional_access_type;
    OpenAPI_list_t *allocated_ebi_list;
    char *hsmf_id;
    char *hsmf_set_id;
    char *hsmf_service_set_id;
    OpenAPI_sbi_binding_level_e smf_binding;
    char *vsmf_id;
    char *vsmf_set_id;
    char *vsmf_service_set_id;
    OpenAPI_sbi_binding_level_e vsmf_binding;
    char *ismf_id;
    char *ismf_set_id;
    char *ismf_service_set_id;
    OpenAPI_sbi_binding_level_e ismf_binding;
    char *ns_instance;
    char *smf_service_instance_id;
    int ma_pdu_session;
    struct OpenAPI_cn_assisted_ran_para_s *cn_assisted_ran_para;
} OpenAPI_pdu_session_context_t;

OpenAPI_pdu_session_context_t *OpenAPI_pdu_session_context_create(
    int pdu_session_id,
    char *sm_context_ref,
    OpenAPI_snssai_t *s_nssai,
    char *dnn,
    OpenAPI_access_type_e access_type,
    OpenAPI_access_type_e additional_access_type,
    OpenAPI_list_t *allocated_ebi_list,
    char *hsmf_id,
    char *hsmf_set_id,
    char *hsmf_service_set_id,
    OpenAPI_sbi_binding_level_e smf_binding,
    char *vsmf_id,
    char *vsmf_set_id,
    char *vsmf_service_set_id,
    OpenAPI_sbi_binding_level_e vsmf_binding,
    char *ismf_id,
    char *ismf_set_id,
    char *ismf_service_set_id,
    OpenAPI_sbi_binding_level_e ismf_binding,
    char *ns_instance,
    char *smf_service_instance_id,
    int ma_pdu_session,
    OpenAPI_cn_assisted_ran_para_t *cn_assisted_ran_para
    );
void OpenAPI_pdu_session_context_free(OpenAPI_pdu_session_context_t *pdu_session_context);
OpenAPI_pdu_session_context_t *OpenAPI_pdu_session_context_parseFromJSON(cJSON *pdu_session_contextJSON);
cJSON *OpenAPI_pdu_session_context_convertToJSON(OpenAPI_pdu_session_context_t *pdu_session_context);
OpenAPI_pdu_session_context_t *OpenAPI_pdu_session_context_copy(OpenAPI_pdu_session_context_t *dst, OpenAPI_pdu_session_context_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pdu_session_context_H_ */

