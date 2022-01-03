
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "smf_registration.h"

OpenAPI_smf_registration_t *OpenAPI_smf_registration_create(
    char *smf_instance_id,
    char *smf_set_id,
    char *supported_features,
    int pdu_session_id,
    OpenAPI_snssai_t *single_nssai,
    char *dnn,
    bool is_emergency_services,
    int emergency_services,
    char *pcscf_restoration_callback_uri,
    OpenAPI_plmn_id_t *plmn_id,
    char *pgw_fqdn,
    bool is_epdg_ind,
    int epdg_ind,
    char *dereg_callback_uri,
    OpenAPI_registration_reason_e registration_reason,
    char *registration_time,
    OpenAPI_context_info_t *context_info
)
{
    OpenAPI_smf_registration_t *smf_registration_local_var = ogs_malloc(sizeof(OpenAPI_smf_registration_t));
    ogs_assert(smf_registration_local_var);

    smf_registration_local_var->smf_instance_id = smf_instance_id;
    smf_registration_local_var->smf_set_id = smf_set_id;
    smf_registration_local_var->supported_features = supported_features;
    smf_registration_local_var->pdu_session_id = pdu_session_id;
    smf_registration_local_var->single_nssai = single_nssai;
    smf_registration_local_var->dnn = dnn;
    smf_registration_local_var->is_emergency_services = is_emergency_services;
    smf_registration_local_var->emergency_services = emergency_services;
    smf_registration_local_var->pcscf_restoration_callback_uri = pcscf_restoration_callback_uri;
    smf_registration_local_var->plmn_id = plmn_id;
    smf_registration_local_var->pgw_fqdn = pgw_fqdn;
    smf_registration_local_var->is_epdg_ind = is_epdg_ind;
    smf_registration_local_var->epdg_ind = epdg_ind;
    smf_registration_local_var->dereg_callback_uri = dereg_callback_uri;
    smf_registration_local_var->registration_reason = registration_reason;
    smf_registration_local_var->registration_time = registration_time;
    smf_registration_local_var->context_info = context_info;

    return smf_registration_local_var;
}

void OpenAPI_smf_registration_free(OpenAPI_smf_registration_t *smf_registration)
{
    if (NULL == smf_registration) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(smf_registration->smf_instance_id);
    ogs_free(smf_registration->smf_set_id);
    ogs_free(smf_registration->supported_features);
    OpenAPI_snssai_free(smf_registration->single_nssai);
    ogs_free(smf_registration->dnn);
    ogs_free(smf_registration->pcscf_restoration_callback_uri);
    OpenAPI_plmn_id_free(smf_registration->plmn_id);
    ogs_free(smf_registration->pgw_fqdn);
    ogs_free(smf_registration->dereg_callback_uri);
    ogs_free(smf_registration->registration_time);
    OpenAPI_context_info_free(smf_registration->context_info);
    ogs_free(smf_registration);
}

cJSON *OpenAPI_smf_registration_convertToJSON(OpenAPI_smf_registration_t *smf_registration)
{
    cJSON *item = NULL;

    if (smf_registration == NULL) {
        ogs_error("OpenAPI_smf_registration_convertToJSON() failed [SmfRegistration]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddStringToObject(item, "smfInstanceId", smf_registration->smf_instance_id) == NULL) {
        ogs_error("OpenAPI_smf_registration_convertToJSON() failed [smf_instance_id]");
        goto end;
    }

    if (smf_registration->smf_set_id) {
    if (cJSON_AddStringToObject(item, "smfSetId", smf_registration->smf_set_id) == NULL) {
        ogs_error("OpenAPI_smf_registration_convertToJSON() failed [smf_set_id]");
        goto end;
    }
    }

    if (smf_registration->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", smf_registration->supported_features) == NULL) {
        ogs_error("OpenAPI_smf_registration_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

    if (cJSON_AddNumberToObject(item, "pduSessionId", smf_registration->pdu_session_id) == NULL) {
        ogs_error("OpenAPI_smf_registration_convertToJSON() failed [pdu_session_id]");
        goto end;
    }

    cJSON *single_nssai_local_JSON = OpenAPI_snssai_convertToJSON(smf_registration->single_nssai);
    if (single_nssai_local_JSON == NULL) {
        ogs_error("OpenAPI_smf_registration_convertToJSON() failed [single_nssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "singleNssai", single_nssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_smf_registration_convertToJSON() failed [single_nssai]");
        goto end;
    }

    if (smf_registration->dnn) {
    if (cJSON_AddStringToObject(item, "dnn", smf_registration->dnn) == NULL) {
        ogs_error("OpenAPI_smf_registration_convertToJSON() failed [dnn]");
        goto end;
    }
    }

    if (smf_registration->is_emergency_services) {
    if (cJSON_AddBoolToObject(item, "emergencyServices", smf_registration->emergency_services) == NULL) {
        ogs_error("OpenAPI_smf_registration_convertToJSON() failed [emergency_services]");
        goto end;
    }
    }

    if (smf_registration->pcscf_restoration_callback_uri) {
    if (cJSON_AddStringToObject(item, "pcscfRestorationCallbackUri", smf_registration->pcscf_restoration_callback_uri) == NULL) {
        ogs_error("OpenAPI_smf_registration_convertToJSON() failed [pcscf_restoration_callback_uri]");
        goto end;
    }
    }

    cJSON *plmn_id_local_JSON = OpenAPI_plmn_id_convertToJSON(smf_registration->plmn_id);
    if (plmn_id_local_JSON == NULL) {
        ogs_error("OpenAPI_smf_registration_convertToJSON() failed [plmn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "plmnId", plmn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_smf_registration_convertToJSON() failed [plmn_id]");
        goto end;
    }

    if (smf_registration->pgw_fqdn) {
    if (cJSON_AddStringToObject(item, "pgwFqdn", smf_registration->pgw_fqdn) == NULL) {
        ogs_error("OpenAPI_smf_registration_convertToJSON() failed [pgw_fqdn]");
        goto end;
    }
    }

    if (smf_registration->is_epdg_ind) {
    if (cJSON_AddBoolToObject(item, "epdgInd", smf_registration->epdg_ind) == NULL) {
        ogs_error("OpenAPI_smf_registration_convertToJSON() failed [epdg_ind]");
        goto end;
    }
    }

    if (smf_registration->dereg_callback_uri) {
    if (cJSON_AddStringToObject(item, "deregCallbackUri", smf_registration->dereg_callback_uri) == NULL) {
        ogs_error("OpenAPI_smf_registration_convertToJSON() failed [dereg_callback_uri]");
        goto end;
    }
    }

    if (smf_registration->registration_reason) {
    if (cJSON_AddStringToObject(item, "registrationReason", OpenAPI_registration_reason_ToString(smf_registration->registration_reason)) == NULL) {
        ogs_error("OpenAPI_smf_registration_convertToJSON() failed [registration_reason]");
        goto end;
    }
    }

    if (smf_registration->registration_time) {
    if (cJSON_AddStringToObject(item, "registrationTime", smf_registration->registration_time) == NULL) {
        ogs_error("OpenAPI_smf_registration_convertToJSON() failed [registration_time]");
        goto end;
    }
    }

    if (smf_registration->context_info) {
    cJSON *context_info_local_JSON = OpenAPI_context_info_convertToJSON(smf_registration->context_info);
    if (context_info_local_JSON == NULL) {
        ogs_error("OpenAPI_smf_registration_convertToJSON() failed [context_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "contextInfo", context_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_smf_registration_convertToJSON() failed [context_info]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_smf_registration_t *OpenAPI_smf_registration_parseFromJSON(cJSON *smf_registrationJSON)
{
    OpenAPI_smf_registration_t *smf_registration_local_var = NULL;
    cJSON *smf_instance_id = cJSON_GetObjectItemCaseSensitive(smf_registrationJSON, "smfInstanceId");
    if (!smf_instance_id) {
        ogs_error("OpenAPI_smf_registration_parseFromJSON() failed [smf_instance_id]");
        goto end;
    }

    if (!cJSON_IsString(smf_instance_id)) {
        ogs_error("OpenAPI_smf_registration_parseFromJSON() failed [smf_instance_id]");
        goto end;
    }

    cJSON *smf_set_id = cJSON_GetObjectItemCaseSensitive(smf_registrationJSON, "smfSetId");

    if (smf_set_id) {
    if (!cJSON_IsString(smf_set_id)) {
        ogs_error("OpenAPI_smf_registration_parseFromJSON() failed [smf_set_id]");
        goto end;
    }
    }

    cJSON *supported_features = cJSON_GetObjectItemCaseSensitive(smf_registrationJSON, "supportedFeatures");

    if (supported_features) {
    if (!cJSON_IsString(supported_features)) {
        ogs_error("OpenAPI_smf_registration_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    cJSON *pdu_session_id = cJSON_GetObjectItemCaseSensitive(smf_registrationJSON, "pduSessionId");
    if (!pdu_session_id) {
        ogs_error("OpenAPI_smf_registration_parseFromJSON() failed [pdu_session_id]");
        goto end;
    }

    if (!cJSON_IsNumber(pdu_session_id)) {
        ogs_error("OpenAPI_smf_registration_parseFromJSON() failed [pdu_session_id]");
        goto end;
    }

    cJSON *single_nssai = cJSON_GetObjectItemCaseSensitive(smf_registrationJSON, "singleNssai");
    if (!single_nssai) {
        ogs_error("OpenAPI_smf_registration_parseFromJSON() failed [single_nssai]");
        goto end;
    }

    OpenAPI_snssai_t *single_nssai_local_nonprim = NULL;
    single_nssai_local_nonprim = OpenAPI_snssai_parseFromJSON(single_nssai);

    cJSON *dnn = cJSON_GetObjectItemCaseSensitive(smf_registrationJSON, "dnn");

    if (dnn) {
    if (!cJSON_IsString(dnn)) {
        ogs_error("OpenAPI_smf_registration_parseFromJSON() failed [dnn]");
        goto end;
    }
    }

    cJSON *emergency_services = cJSON_GetObjectItemCaseSensitive(smf_registrationJSON, "emergencyServices");

    if (emergency_services) {
    if (!cJSON_IsBool(emergency_services)) {
        ogs_error("OpenAPI_smf_registration_parseFromJSON() failed [emergency_services]");
        goto end;
    }
    }

    cJSON *pcscf_restoration_callback_uri = cJSON_GetObjectItemCaseSensitive(smf_registrationJSON, "pcscfRestorationCallbackUri");

    if (pcscf_restoration_callback_uri) {
    if (!cJSON_IsString(pcscf_restoration_callback_uri)) {
        ogs_error("OpenAPI_smf_registration_parseFromJSON() failed [pcscf_restoration_callback_uri]");
        goto end;
    }
    }

    cJSON *plmn_id = cJSON_GetObjectItemCaseSensitive(smf_registrationJSON, "plmnId");
    if (!plmn_id) {
        ogs_error("OpenAPI_smf_registration_parseFromJSON() failed [plmn_id]");
        goto end;
    }

    OpenAPI_plmn_id_t *plmn_id_local_nonprim = NULL;
    plmn_id_local_nonprim = OpenAPI_plmn_id_parseFromJSON(plmn_id);

    cJSON *pgw_fqdn = cJSON_GetObjectItemCaseSensitive(smf_registrationJSON, "pgwFqdn");

    if (pgw_fqdn) {
    if (!cJSON_IsString(pgw_fqdn)) {
        ogs_error("OpenAPI_smf_registration_parseFromJSON() failed [pgw_fqdn]");
        goto end;
    }
    }

    cJSON *epdg_ind = cJSON_GetObjectItemCaseSensitive(smf_registrationJSON, "epdgInd");

    if (epdg_ind) {
    if (!cJSON_IsBool(epdg_ind)) {
        ogs_error("OpenAPI_smf_registration_parseFromJSON() failed [epdg_ind]");
        goto end;
    }
    }

    cJSON *dereg_callback_uri = cJSON_GetObjectItemCaseSensitive(smf_registrationJSON, "deregCallbackUri");

    if (dereg_callback_uri) {
    if (!cJSON_IsString(dereg_callback_uri)) {
        ogs_error("OpenAPI_smf_registration_parseFromJSON() failed [dereg_callback_uri]");
        goto end;
    }
    }

    cJSON *registration_reason = cJSON_GetObjectItemCaseSensitive(smf_registrationJSON, "registrationReason");

    OpenAPI_registration_reason_e registration_reasonVariable;
    if (registration_reason) {
    if (!cJSON_IsString(registration_reason)) {
        ogs_error("OpenAPI_smf_registration_parseFromJSON() failed [registration_reason]");
        goto end;
    }
    registration_reasonVariable = OpenAPI_registration_reason_FromString(registration_reason->valuestring);
    }

    cJSON *registration_time = cJSON_GetObjectItemCaseSensitive(smf_registrationJSON, "registrationTime");

    if (registration_time) {
    if (!cJSON_IsString(registration_time)) {
        ogs_error("OpenAPI_smf_registration_parseFromJSON() failed [registration_time]");
        goto end;
    }
    }

    cJSON *context_info = cJSON_GetObjectItemCaseSensitive(smf_registrationJSON, "contextInfo");

    OpenAPI_context_info_t *context_info_local_nonprim = NULL;
    if (context_info) {
    context_info_local_nonprim = OpenAPI_context_info_parseFromJSON(context_info);
    }

    smf_registration_local_var = OpenAPI_smf_registration_create (
        ogs_strdup(smf_instance_id->valuestring),
        smf_set_id ? ogs_strdup(smf_set_id->valuestring) : NULL,
        supported_features ? ogs_strdup(supported_features->valuestring) : NULL,
        
        pdu_session_id->valuedouble,
        single_nssai_local_nonprim,
        dnn ? ogs_strdup(dnn->valuestring) : NULL,
        emergency_services ? true : false,
        emergency_services ? emergency_services->valueint : 0,
        pcscf_restoration_callback_uri ? ogs_strdup(pcscf_restoration_callback_uri->valuestring) : NULL,
        plmn_id_local_nonprim,
        pgw_fqdn ? ogs_strdup(pgw_fqdn->valuestring) : NULL,
        epdg_ind ? true : false,
        epdg_ind ? epdg_ind->valueint : 0,
        dereg_callback_uri ? ogs_strdup(dereg_callback_uri->valuestring) : NULL,
        registration_reason ? registration_reasonVariable : 0,
        registration_time ? ogs_strdup(registration_time->valuestring) : NULL,
        context_info ? context_info_local_nonprim : NULL
    );

    return smf_registration_local_var;
end:
    return NULL;
}

OpenAPI_smf_registration_t *OpenAPI_smf_registration_copy(OpenAPI_smf_registration_t *dst, OpenAPI_smf_registration_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_smf_registration_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_smf_registration_convertToJSON() failed");
        return NULL;
    }

    content = cJSON_Print(item);
    cJSON_Delete(item);

    if (!content) {
        ogs_error("cJSON_Print() failed");
        return NULL;
    }

    item = cJSON_Parse(content);
    ogs_free(content);
    if (!item) {
        ogs_error("cJSON_Parse() failed");
        return NULL;
    }

    OpenAPI_smf_registration_free(dst);
    dst = OpenAPI_smf_registration_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

