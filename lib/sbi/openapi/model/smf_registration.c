
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
    OpenAPI_ip_address_t *pgw_ip_addr,
    bool is_epdg_ind,
    int epdg_ind,
    char *dereg_callback_uri,
    OpenAPI_registration_reason_e registration_reason,
    char *registration_time,
    OpenAPI_context_info_t *context_info,
    char *pcf_id,
    char *data_restoration_callback_uri,
    OpenAPI_list_t *reset_ids,
    bool is_udr_restart_ind,
    int udr_restart_ind,
    char *last_synchronization_time
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
    smf_registration_local_var->pgw_ip_addr = pgw_ip_addr;
    smf_registration_local_var->is_epdg_ind = is_epdg_ind;
    smf_registration_local_var->epdg_ind = epdg_ind;
    smf_registration_local_var->dereg_callback_uri = dereg_callback_uri;
    smf_registration_local_var->registration_reason = registration_reason;
    smf_registration_local_var->registration_time = registration_time;
    smf_registration_local_var->context_info = context_info;
    smf_registration_local_var->pcf_id = pcf_id;
    smf_registration_local_var->data_restoration_callback_uri = data_restoration_callback_uri;
    smf_registration_local_var->reset_ids = reset_ids;
    smf_registration_local_var->is_udr_restart_ind = is_udr_restart_ind;
    smf_registration_local_var->udr_restart_ind = udr_restart_ind;
    smf_registration_local_var->last_synchronization_time = last_synchronization_time;

    return smf_registration_local_var;
}

void OpenAPI_smf_registration_free(OpenAPI_smf_registration_t *smf_registration)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == smf_registration) {
        return;
    }
    if (smf_registration->smf_instance_id) {
        ogs_free(smf_registration->smf_instance_id);
        smf_registration->smf_instance_id = NULL;
    }
    if (smf_registration->smf_set_id) {
        ogs_free(smf_registration->smf_set_id);
        smf_registration->smf_set_id = NULL;
    }
    if (smf_registration->supported_features) {
        ogs_free(smf_registration->supported_features);
        smf_registration->supported_features = NULL;
    }
    if (smf_registration->single_nssai) {
        OpenAPI_snssai_free(smf_registration->single_nssai);
        smf_registration->single_nssai = NULL;
    }
    if (smf_registration->dnn) {
        ogs_free(smf_registration->dnn);
        smf_registration->dnn = NULL;
    }
    if (smf_registration->pcscf_restoration_callback_uri) {
        ogs_free(smf_registration->pcscf_restoration_callback_uri);
        smf_registration->pcscf_restoration_callback_uri = NULL;
    }
    if (smf_registration->plmn_id) {
        OpenAPI_plmn_id_free(smf_registration->plmn_id);
        smf_registration->plmn_id = NULL;
    }
    if (smf_registration->pgw_fqdn) {
        ogs_free(smf_registration->pgw_fqdn);
        smf_registration->pgw_fqdn = NULL;
    }
    if (smf_registration->pgw_ip_addr) {
        OpenAPI_ip_address_free(smf_registration->pgw_ip_addr);
        smf_registration->pgw_ip_addr = NULL;
    }
    if (smf_registration->dereg_callback_uri) {
        ogs_free(smf_registration->dereg_callback_uri);
        smf_registration->dereg_callback_uri = NULL;
    }
    if (smf_registration->registration_time) {
        ogs_free(smf_registration->registration_time);
        smf_registration->registration_time = NULL;
    }
    if (smf_registration->context_info) {
        OpenAPI_context_info_free(smf_registration->context_info);
        smf_registration->context_info = NULL;
    }
    if (smf_registration->pcf_id) {
        ogs_free(smf_registration->pcf_id);
        smf_registration->pcf_id = NULL;
    }
    if (smf_registration->data_restoration_callback_uri) {
        ogs_free(smf_registration->data_restoration_callback_uri);
        smf_registration->data_restoration_callback_uri = NULL;
    }
    if (smf_registration->reset_ids) {
        OpenAPI_list_for_each(smf_registration->reset_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(smf_registration->reset_ids);
        smf_registration->reset_ids = NULL;
    }
    if (smf_registration->last_synchronization_time) {
        ogs_free(smf_registration->last_synchronization_time);
        smf_registration->last_synchronization_time = NULL;
    }
    ogs_free(smf_registration);
}

cJSON *OpenAPI_smf_registration_convertToJSON(OpenAPI_smf_registration_t *smf_registration)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (smf_registration == NULL) {
        ogs_error("OpenAPI_smf_registration_convertToJSON() failed [SmfRegistration]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!smf_registration->smf_instance_id) {
        ogs_error("OpenAPI_smf_registration_convertToJSON() failed [smf_instance_id]");
        return NULL;
    }
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

    if (!smf_registration->single_nssai) {
        ogs_error("OpenAPI_smf_registration_convertToJSON() failed [single_nssai]");
        return NULL;
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

    if (!smf_registration->plmn_id) {
        ogs_error("OpenAPI_smf_registration_convertToJSON() failed [plmn_id]");
        return NULL;
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

    if (smf_registration->pgw_ip_addr) {
    cJSON *pgw_ip_addr_local_JSON = OpenAPI_ip_address_convertToJSON(smf_registration->pgw_ip_addr);
    if (pgw_ip_addr_local_JSON == NULL) {
        ogs_error("OpenAPI_smf_registration_convertToJSON() failed [pgw_ip_addr]");
        goto end;
    }
    cJSON_AddItemToObject(item, "pgwIpAddr", pgw_ip_addr_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_smf_registration_convertToJSON() failed [pgw_ip_addr]");
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

    if (smf_registration->registration_reason != OpenAPI_registration_reason_NULL) {
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

    if (smf_registration->pcf_id) {
    if (cJSON_AddStringToObject(item, "pcfId", smf_registration->pcf_id) == NULL) {
        ogs_error("OpenAPI_smf_registration_convertToJSON() failed [pcf_id]");
        goto end;
    }
    }

    if (smf_registration->data_restoration_callback_uri) {
    if (cJSON_AddStringToObject(item, "dataRestorationCallbackUri", smf_registration->data_restoration_callback_uri) == NULL) {
        ogs_error("OpenAPI_smf_registration_convertToJSON() failed [data_restoration_callback_uri]");
        goto end;
    }
    }

    if (smf_registration->reset_ids) {
    cJSON *reset_idsList = cJSON_AddArrayToObject(item, "resetIds");
    if (reset_idsList == NULL) {
        ogs_error("OpenAPI_smf_registration_convertToJSON() failed [reset_ids]");
        goto end;
    }
    OpenAPI_list_for_each(smf_registration->reset_ids, node) {
        if (cJSON_AddStringToObject(reset_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_smf_registration_convertToJSON() failed [reset_ids]");
            goto end;
        }
    }
    }

    if (smf_registration->is_udr_restart_ind) {
    if (cJSON_AddBoolToObject(item, "udrRestartInd", smf_registration->udr_restart_ind) == NULL) {
        ogs_error("OpenAPI_smf_registration_convertToJSON() failed [udr_restart_ind]");
        goto end;
    }
    }

    if (smf_registration->last_synchronization_time) {
    if (cJSON_AddStringToObject(item, "lastSynchronizationTime", smf_registration->last_synchronization_time) == NULL) {
        ogs_error("OpenAPI_smf_registration_convertToJSON() failed [last_synchronization_time]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_smf_registration_t *OpenAPI_smf_registration_parseFromJSON(cJSON *smf_registrationJSON)
{
    OpenAPI_smf_registration_t *smf_registration_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *smf_instance_id = NULL;
    cJSON *smf_set_id = NULL;
    cJSON *supported_features = NULL;
    cJSON *pdu_session_id = NULL;
    cJSON *single_nssai = NULL;
    OpenAPI_snssai_t *single_nssai_local_nonprim = NULL;
    cJSON *dnn = NULL;
    cJSON *emergency_services = NULL;
    cJSON *pcscf_restoration_callback_uri = NULL;
    cJSON *plmn_id = NULL;
    OpenAPI_plmn_id_t *plmn_id_local_nonprim = NULL;
    cJSON *pgw_fqdn = NULL;
    cJSON *pgw_ip_addr = NULL;
    OpenAPI_ip_address_t *pgw_ip_addr_local_nonprim = NULL;
    cJSON *epdg_ind = NULL;
    cJSON *dereg_callback_uri = NULL;
    cJSON *registration_reason = NULL;
    OpenAPI_registration_reason_e registration_reasonVariable = 0;
    cJSON *registration_time = NULL;
    cJSON *context_info = NULL;
    OpenAPI_context_info_t *context_info_local_nonprim = NULL;
    cJSON *pcf_id = NULL;
    cJSON *data_restoration_callback_uri = NULL;
    cJSON *reset_ids = NULL;
    OpenAPI_list_t *reset_idsList = NULL;
    cJSON *udr_restart_ind = NULL;
    cJSON *last_synchronization_time = NULL;
    smf_instance_id = cJSON_GetObjectItemCaseSensitive(smf_registrationJSON, "smfInstanceId");
    if (!smf_instance_id) {
        ogs_error("OpenAPI_smf_registration_parseFromJSON() failed [smf_instance_id]");
        goto end;
    }
    if (!cJSON_IsString(smf_instance_id)) {
        ogs_error("OpenAPI_smf_registration_parseFromJSON() failed [smf_instance_id]");
        goto end;
    }

    smf_set_id = cJSON_GetObjectItemCaseSensitive(smf_registrationJSON, "smfSetId");
    if (smf_set_id) {
    if (!cJSON_IsString(smf_set_id) && !cJSON_IsNull(smf_set_id)) {
        ogs_error("OpenAPI_smf_registration_parseFromJSON() failed [smf_set_id]");
        goto end;
    }
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(smf_registrationJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_smf_registration_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    pdu_session_id = cJSON_GetObjectItemCaseSensitive(smf_registrationJSON, "pduSessionId");
    if (!pdu_session_id) {
        ogs_error("OpenAPI_smf_registration_parseFromJSON() failed [pdu_session_id]");
        goto end;
    }
    if (!cJSON_IsNumber(pdu_session_id)) {
        ogs_error("OpenAPI_smf_registration_parseFromJSON() failed [pdu_session_id]");
        goto end;
    }

    single_nssai = cJSON_GetObjectItemCaseSensitive(smf_registrationJSON, "singleNssai");
    if (!single_nssai) {
        ogs_error("OpenAPI_smf_registration_parseFromJSON() failed [single_nssai]");
        goto end;
    }
    single_nssai_local_nonprim = OpenAPI_snssai_parseFromJSON(single_nssai);
    if (!single_nssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [single_nssai]");
        goto end;
    }

    dnn = cJSON_GetObjectItemCaseSensitive(smf_registrationJSON, "dnn");
    if (dnn) {
    if (!cJSON_IsString(dnn) && !cJSON_IsNull(dnn)) {
        ogs_error("OpenAPI_smf_registration_parseFromJSON() failed [dnn]");
        goto end;
    }
    }

    emergency_services = cJSON_GetObjectItemCaseSensitive(smf_registrationJSON, "emergencyServices");
    if (emergency_services) {
    if (!cJSON_IsBool(emergency_services)) {
        ogs_error("OpenAPI_smf_registration_parseFromJSON() failed [emergency_services]");
        goto end;
    }
    }

    pcscf_restoration_callback_uri = cJSON_GetObjectItemCaseSensitive(smf_registrationJSON, "pcscfRestorationCallbackUri");
    if (pcscf_restoration_callback_uri) {
    if (!cJSON_IsString(pcscf_restoration_callback_uri) && !cJSON_IsNull(pcscf_restoration_callback_uri)) {
        ogs_error("OpenAPI_smf_registration_parseFromJSON() failed [pcscf_restoration_callback_uri]");
        goto end;
    }
    }

    plmn_id = cJSON_GetObjectItemCaseSensitive(smf_registrationJSON, "plmnId");
    if (!plmn_id) {
        ogs_error("OpenAPI_smf_registration_parseFromJSON() failed [plmn_id]");
        goto end;
    }
    plmn_id_local_nonprim = OpenAPI_plmn_id_parseFromJSON(plmn_id);
    if (!plmn_id_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_parseFromJSON failed [plmn_id]");
        goto end;
    }

    pgw_fqdn = cJSON_GetObjectItemCaseSensitive(smf_registrationJSON, "pgwFqdn");
    if (pgw_fqdn) {
    if (!cJSON_IsString(pgw_fqdn) && !cJSON_IsNull(pgw_fqdn)) {
        ogs_error("OpenAPI_smf_registration_parseFromJSON() failed [pgw_fqdn]");
        goto end;
    }
    }

    pgw_ip_addr = cJSON_GetObjectItemCaseSensitive(smf_registrationJSON, "pgwIpAddr");
    if (pgw_ip_addr) {
    pgw_ip_addr_local_nonprim = OpenAPI_ip_address_parseFromJSON(pgw_ip_addr);
    if (!pgw_ip_addr_local_nonprim) {
        ogs_error("OpenAPI_ip_address_parseFromJSON failed [pgw_ip_addr]");
        goto end;
    }
    }

    epdg_ind = cJSON_GetObjectItemCaseSensitive(smf_registrationJSON, "epdgInd");
    if (epdg_ind) {
    if (!cJSON_IsBool(epdg_ind)) {
        ogs_error("OpenAPI_smf_registration_parseFromJSON() failed [epdg_ind]");
        goto end;
    }
    }

    dereg_callback_uri = cJSON_GetObjectItemCaseSensitive(smf_registrationJSON, "deregCallbackUri");
    if (dereg_callback_uri) {
    if (!cJSON_IsString(dereg_callback_uri) && !cJSON_IsNull(dereg_callback_uri)) {
        ogs_error("OpenAPI_smf_registration_parseFromJSON() failed [dereg_callback_uri]");
        goto end;
    }
    }

    registration_reason = cJSON_GetObjectItemCaseSensitive(smf_registrationJSON, "registrationReason");
    if (registration_reason) {
    if (!cJSON_IsString(registration_reason)) {
        ogs_error("OpenAPI_smf_registration_parseFromJSON() failed [registration_reason]");
        goto end;
    }
    registration_reasonVariable = OpenAPI_registration_reason_FromString(registration_reason->valuestring);
    }

    registration_time = cJSON_GetObjectItemCaseSensitive(smf_registrationJSON, "registrationTime");
    if (registration_time) {
    if (!cJSON_IsString(registration_time) && !cJSON_IsNull(registration_time)) {
        ogs_error("OpenAPI_smf_registration_parseFromJSON() failed [registration_time]");
        goto end;
    }
    }

    context_info = cJSON_GetObjectItemCaseSensitive(smf_registrationJSON, "contextInfo");
    if (context_info) {
    context_info_local_nonprim = OpenAPI_context_info_parseFromJSON(context_info);
    if (!context_info_local_nonprim) {
        ogs_error("OpenAPI_context_info_parseFromJSON failed [context_info]");
        goto end;
    }
    }

    pcf_id = cJSON_GetObjectItemCaseSensitive(smf_registrationJSON, "pcfId");
    if (pcf_id) {
    if (!cJSON_IsString(pcf_id) && !cJSON_IsNull(pcf_id)) {
        ogs_error("OpenAPI_smf_registration_parseFromJSON() failed [pcf_id]");
        goto end;
    }
    }

    data_restoration_callback_uri = cJSON_GetObjectItemCaseSensitive(smf_registrationJSON, "dataRestorationCallbackUri");
    if (data_restoration_callback_uri) {
    if (!cJSON_IsString(data_restoration_callback_uri) && !cJSON_IsNull(data_restoration_callback_uri)) {
        ogs_error("OpenAPI_smf_registration_parseFromJSON() failed [data_restoration_callback_uri]");
        goto end;
    }
    }

    reset_ids = cJSON_GetObjectItemCaseSensitive(smf_registrationJSON, "resetIds");
    if (reset_ids) {
        cJSON *reset_ids_local = NULL;
        if (!cJSON_IsArray(reset_ids)) {
            ogs_error("OpenAPI_smf_registration_parseFromJSON() failed [reset_ids]");
            goto end;
        }

        reset_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(reset_ids_local, reset_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(reset_ids_local)) {
                ogs_error("OpenAPI_smf_registration_parseFromJSON() failed [reset_ids]");
                goto end;
            }
            OpenAPI_list_add(reset_idsList, ogs_strdup(reset_ids_local->valuestring));
        }
    }

    udr_restart_ind = cJSON_GetObjectItemCaseSensitive(smf_registrationJSON, "udrRestartInd");
    if (udr_restart_ind) {
    if (!cJSON_IsBool(udr_restart_ind)) {
        ogs_error("OpenAPI_smf_registration_parseFromJSON() failed [udr_restart_ind]");
        goto end;
    }
    }

    last_synchronization_time = cJSON_GetObjectItemCaseSensitive(smf_registrationJSON, "lastSynchronizationTime");
    if (last_synchronization_time) {
    if (!cJSON_IsString(last_synchronization_time) && !cJSON_IsNull(last_synchronization_time)) {
        ogs_error("OpenAPI_smf_registration_parseFromJSON() failed [last_synchronization_time]");
        goto end;
    }
    }

    smf_registration_local_var = OpenAPI_smf_registration_create (
        ogs_strdup(smf_instance_id->valuestring),
        smf_set_id && !cJSON_IsNull(smf_set_id) ? ogs_strdup(smf_set_id->valuestring) : NULL,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL,
        
        pdu_session_id->valuedouble,
        single_nssai_local_nonprim,
        dnn && !cJSON_IsNull(dnn) ? ogs_strdup(dnn->valuestring) : NULL,
        emergency_services ? true : false,
        emergency_services ? emergency_services->valueint : 0,
        pcscf_restoration_callback_uri && !cJSON_IsNull(pcscf_restoration_callback_uri) ? ogs_strdup(pcscf_restoration_callback_uri->valuestring) : NULL,
        plmn_id_local_nonprim,
        pgw_fqdn && !cJSON_IsNull(pgw_fqdn) ? ogs_strdup(pgw_fqdn->valuestring) : NULL,
        pgw_ip_addr ? pgw_ip_addr_local_nonprim : NULL,
        epdg_ind ? true : false,
        epdg_ind ? epdg_ind->valueint : 0,
        dereg_callback_uri && !cJSON_IsNull(dereg_callback_uri) ? ogs_strdup(dereg_callback_uri->valuestring) : NULL,
        registration_reason ? registration_reasonVariable : 0,
        registration_time && !cJSON_IsNull(registration_time) ? ogs_strdup(registration_time->valuestring) : NULL,
        context_info ? context_info_local_nonprim : NULL,
        pcf_id && !cJSON_IsNull(pcf_id) ? ogs_strdup(pcf_id->valuestring) : NULL,
        data_restoration_callback_uri && !cJSON_IsNull(data_restoration_callback_uri) ? ogs_strdup(data_restoration_callback_uri->valuestring) : NULL,
        reset_ids ? reset_idsList : NULL,
        udr_restart_ind ? true : false,
        udr_restart_ind ? udr_restart_ind->valueint : 0,
        last_synchronization_time && !cJSON_IsNull(last_synchronization_time) ? ogs_strdup(last_synchronization_time->valuestring) : NULL
    );

    return smf_registration_local_var;
end:
    if (single_nssai_local_nonprim) {
        OpenAPI_snssai_free(single_nssai_local_nonprim);
        single_nssai_local_nonprim = NULL;
    }
    if (plmn_id_local_nonprim) {
        OpenAPI_plmn_id_free(plmn_id_local_nonprim);
        plmn_id_local_nonprim = NULL;
    }
    if (pgw_ip_addr_local_nonprim) {
        OpenAPI_ip_address_free(pgw_ip_addr_local_nonprim);
        pgw_ip_addr_local_nonprim = NULL;
    }
    if (context_info_local_nonprim) {
        OpenAPI_context_info_free(context_info_local_nonprim);
        context_info_local_nonprim = NULL;
    }
    if (reset_idsList) {
        OpenAPI_list_for_each(reset_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(reset_idsList);
        reset_idsList = NULL;
    }
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

