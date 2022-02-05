
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "amf_non3_gpp_access_registration.h"

OpenAPI_amf_non3_gpp_access_registration_t *OpenAPI_amf_non3_gpp_access_registration_create(
    char *amf_instance_id,
    char *supported_features,
    bool is_purge_flag,
    int purge_flag,
    char *pei,
    OpenAPI_ims_vo_ps_e ims_vo_ps,
    char *dereg_callback_uri,
    char *amf_service_name_dereg,
    char *pcscf_restoration_callback_uri,
    char *amf_service_name_pcscf_rest,
    OpenAPI_guami_t *guami,
    OpenAPI_list_t *backup_amf_info,
    OpenAPI_rat_type_e rat_type,
    bool is_urrp_indicator,
    int urrp_indicator,
    char *amf_ee_subscription_id,
    char *registration_time,
    OpenAPI_vgmlc_address_t *vgmlc_address,
    OpenAPI_context_info_t *context_info,
    bool is_no_ee_subscription_ind,
    int no_ee_subscription_ind,
    char *supi
)
{
    OpenAPI_amf_non3_gpp_access_registration_t *amf_non3_gpp_access_registration_local_var = ogs_malloc(sizeof(OpenAPI_amf_non3_gpp_access_registration_t));
    ogs_assert(amf_non3_gpp_access_registration_local_var);

    amf_non3_gpp_access_registration_local_var->amf_instance_id = amf_instance_id;
    amf_non3_gpp_access_registration_local_var->supported_features = supported_features;
    amf_non3_gpp_access_registration_local_var->is_purge_flag = is_purge_flag;
    amf_non3_gpp_access_registration_local_var->purge_flag = purge_flag;
    amf_non3_gpp_access_registration_local_var->pei = pei;
    amf_non3_gpp_access_registration_local_var->ims_vo_ps = ims_vo_ps;
    amf_non3_gpp_access_registration_local_var->dereg_callback_uri = dereg_callback_uri;
    amf_non3_gpp_access_registration_local_var->amf_service_name_dereg = amf_service_name_dereg;
    amf_non3_gpp_access_registration_local_var->pcscf_restoration_callback_uri = pcscf_restoration_callback_uri;
    amf_non3_gpp_access_registration_local_var->amf_service_name_pcscf_rest = amf_service_name_pcscf_rest;
    amf_non3_gpp_access_registration_local_var->guami = guami;
    amf_non3_gpp_access_registration_local_var->backup_amf_info = backup_amf_info;
    amf_non3_gpp_access_registration_local_var->rat_type = rat_type;
    amf_non3_gpp_access_registration_local_var->is_urrp_indicator = is_urrp_indicator;
    amf_non3_gpp_access_registration_local_var->urrp_indicator = urrp_indicator;
    amf_non3_gpp_access_registration_local_var->amf_ee_subscription_id = amf_ee_subscription_id;
    amf_non3_gpp_access_registration_local_var->registration_time = registration_time;
    amf_non3_gpp_access_registration_local_var->vgmlc_address = vgmlc_address;
    amf_non3_gpp_access_registration_local_var->context_info = context_info;
    amf_non3_gpp_access_registration_local_var->is_no_ee_subscription_ind = is_no_ee_subscription_ind;
    amf_non3_gpp_access_registration_local_var->no_ee_subscription_ind = no_ee_subscription_ind;
    amf_non3_gpp_access_registration_local_var->supi = supi;

    return amf_non3_gpp_access_registration_local_var;
}

void OpenAPI_amf_non3_gpp_access_registration_free(OpenAPI_amf_non3_gpp_access_registration_t *amf_non3_gpp_access_registration)
{
    if (NULL == amf_non3_gpp_access_registration) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(amf_non3_gpp_access_registration->amf_instance_id);
    ogs_free(amf_non3_gpp_access_registration->supported_features);
    ogs_free(amf_non3_gpp_access_registration->pei);
    ogs_free(amf_non3_gpp_access_registration->dereg_callback_uri);
    ogs_free(amf_non3_gpp_access_registration->amf_service_name_dereg);
    ogs_free(amf_non3_gpp_access_registration->pcscf_restoration_callback_uri);
    ogs_free(amf_non3_gpp_access_registration->amf_service_name_pcscf_rest);
    OpenAPI_guami_free(amf_non3_gpp_access_registration->guami);
    OpenAPI_list_for_each(amf_non3_gpp_access_registration->backup_amf_info, node) {
        OpenAPI_backup_amf_info_free(node->data);
    }
    OpenAPI_list_free(amf_non3_gpp_access_registration->backup_amf_info);
    ogs_free(amf_non3_gpp_access_registration->amf_ee_subscription_id);
    ogs_free(amf_non3_gpp_access_registration->registration_time);
    OpenAPI_vgmlc_address_free(amf_non3_gpp_access_registration->vgmlc_address);
    OpenAPI_context_info_free(amf_non3_gpp_access_registration->context_info);
    ogs_free(amf_non3_gpp_access_registration->supi);
    ogs_free(amf_non3_gpp_access_registration);
}

cJSON *OpenAPI_amf_non3_gpp_access_registration_convertToJSON(OpenAPI_amf_non3_gpp_access_registration_t *amf_non3_gpp_access_registration)
{
    cJSON *item = NULL;

    if (amf_non3_gpp_access_registration == NULL) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_convertToJSON() failed [AmfNon3GppAccessRegistration]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddStringToObject(item, "amfInstanceId", amf_non3_gpp_access_registration->amf_instance_id) == NULL) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_convertToJSON() failed [amf_instance_id]");
        goto end;
    }

    if (amf_non3_gpp_access_registration->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", amf_non3_gpp_access_registration->supported_features) == NULL) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

    if (amf_non3_gpp_access_registration->is_purge_flag) {
    if (cJSON_AddBoolToObject(item, "purgeFlag", amf_non3_gpp_access_registration->purge_flag) == NULL) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_convertToJSON() failed [purge_flag]");
        goto end;
    }
    }

    if (amf_non3_gpp_access_registration->pei) {
    if (cJSON_AddStringToObject(item, "pei", amf_non3_gpp_access_registration->pei) == NULL) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_convertToJSON() failed [pei]");
        goto end;
    }
    }

    if (cJSON_AddStringToObject(item, "imsVoPs", OpenAPI_ims_vo_ps_ToString(amf_non3_gpp_access_registration->ims_vo_ps)) == NULL) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_convertToJSON() failed [ims_vo_ps]");
        goto end;
    }

    if (cJSON_AddStringToObject(item, "deregCallbackUri", amf_non3_gpp_access_registration->dereg_callback_uri) == NULL) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_convertToJSON() failed [dereg_callback_uri]");
        goto end;
    }

    if (amf_non3_gpp_access_registration->amf_service_name_dereg) {
    if (cJSON_AddStringToObject(item, "amfServiceNameDereg", amf_non3_gpp_access_registration->amf_service_name_dereg) == NULL) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_convertToJSON() failed [amf_service_name_dereg]");
        goto end;
    }
    }

    if (amf_non3_gpp_access_registration->pcscf_restoration_callback_uri) {
    if (cJSON_AddStringToObject(item, "pcscfRestorationCallbackUri", amf_non3_gpp_access_registration->pcscf_restoration_callback_uri) == NULL) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_convertToJSON() failed [pcscf_restoration_callback_uri]");
        goto end;
    }
    }

    if (amf_non3_gpp_access_registration->amf_service_name_pcscf_rest) {
    if (cJSON_AddStringToObject(item, "amfServiceNamePcscfRest", amf_non3_gpp_access_registration->amf_service_name_pcscf_rest) == NULL) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_convertToJSON() failed [amf_service_name_pcscf_rest]");
        goto end;
    }
    }

    cJSON *guami_local_JSON = OpenAPI_guami_convertToJSON(amf_non3_gpp_access_registration->guami);
    if (guami_local_JSON == NULL) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_convertToJSON() failed [guami]");
        goto end;
    }
    cJSON_AddItemToObject(item, "guami", guami_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_convertToJSON() failed [guami]");
        goto end;
    }

    if (amf_non3_gpp_access_registration->backup_amf_info) {
    cJSON *backup_amf_infoList = cJSON_AddArrayToObject(item, "backupAmfInfo");
    if (backup_amf_infoList == NULL) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_convertToJSON() failed [backup_amf_info]");
        goto end;
    }

    OpenAPI_lnode_t *backup_amf_info_node;
    if (amf_non3_gpp_access_registration->backup_amf_info) {
        OpenAPI_list_for_each(amf_non3_gpp_access_registration->backup_amf_info, backup_amf_info_node) {
            cJSON *itemLocal = OpenAPI_backup_amf_info_convertToJSON(backup_amf_info_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_amf_non3_gpp_access_registration_convertToJSON() failed [backup_amf_info]");
                goto end;
            }
            cJSON_AddItemToArray(backup_amf_infoList, itemLocal);
        }
    }
    }

    if (cJSON_AddStringToObject(item, "ratType", OpenAPI_rat_type_ToString(amf_non3_gpp_access_registration->rat_type)) == NULL) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_convertToJSON() failed [rat_type]");
        goto end;
    }

    if (amf_non3_gpp_access_registration->is_urrp_indicator) {
    if (cJSON_AddBoolToObject(item, "urrpIndicator", amf_non3_gpp_access_registration->urrp_indicator) == NULL) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_convertToJSON() failed [urrp_indicator]");
        goto end;
    }
    }

    if (amf_non3_gpp_access_registration->amf_ee_subscription_id) {
    if (cJSON_AddStringToObject(item, "amfEeSubscriptionId", amf_non3_gpp_access_registration->amf_ee_subscription_id) == NULL) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_convertToJSON() failed [amf_ee_subscription_id]");
        goto end;
    }
    }

    if (amf_non3_gpp_access_registration->registration_time) {
    if (cJSON_AddStringToObject(item, "registrationTime", amf_non3_gpp_access_registration->registration_time) == NULL) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_convertToJSON() failed [registration_time]");
        goto end;
    }
    }

    if (amf_non3_gpp_access_registration->vgmlc_address) {
    cJSON *vgmlc_address_local_JSON = OpenAPI_vgmlc_address_convertToJSON(amf_non3_gpp_access_registration->vgmlc_address);
    if (vgmlc_address_local_JSON == NULL) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_convertToJSON() failed [vgmlc_address]");
        goto end;
    }
    cJSON_AddItemToObject(item, "vgmlcAddress", vgmlc_address_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_convertToJSON() failed [vgmlc_address]");
        goto end;
    }
    }

    if (amf_non3_gpp_access_registration->context_info) {
    cJSON *context_info_local_JSON = OpenAPI_context_info_convertToJSON(amf_non3_gpp_access_registration->context_info);
    if (context_info_local_JSON == NULL) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_convertToJSON() failed [context_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "contextInfo", context_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_convertToJSON() failed [context_info]");
        goto end;
    }
    }

    if (amf_non3_gpp_access_registration->is_no_ee_subscription_ind) {
    if (cJSON_AddBoolToObject(item, "noEeSubscriptionInd", amf_non3_gpp_access_registration->no_ee_subscription_ind) == NULL) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_convertToJSON() failed [no_ee_subscription_ind]");
        goto end;
    }
    }

    if (amf_non3_gpp_access_registration->supi) {
    if (cJSON_AddStringToObject(item, "supi", amf_non3_gpp_access_registration->supi) == NULL) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_convertToJSON() failed [supi]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_amf_non3_gpp_access_registration_t *OpenAPI_amf_non3_gpp_access_registration_parseFromJSON(cJSON *amf_non3_gpp_access_registrationJSON)
{
    OpenAPI_amf_non3_gpp_access_registration_t *amf_non3_gpp_access_registration_local_var = NULL;
    cJSON *amf_instance_id = cJSON_GetObjectItemCaseSensitive(amf_non3_gpp_access_registrationJSON, "amfInstanceId");
    if (!amf_instance_id) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_parseFromJSON() failed [amf_instance_id]");
        goto end;
    }

    if (!cJSON_IsString(amf_instance_id)) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_parseFromJSON() failed [amf_instance_id]");
        goto end;
    }

    cJSON *supported_features = cJSON_GetObjectItemCaseSensitive(amf_non3_gpp_access_registrationJSON, "supportedFeatures");

    if (supported_features) {
    if (!cJSON_IsString(supported_features)) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    cJSON *purge_flag = cJSON_GetObjectItemCaseSensitive(amf_non3_gpp_access_registrationJSON, "purgeFlag");

    if (purge_flag) {
    if (!cJSON_IsBool(purge_flag)) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_parseFromJSON() failed [purge_flag]");
        goto end;
    }
    }

    cJSON *pei = cJSON_GetObjectItemCaseSensitive(amf_non3_gpp_access_registrationJSON, "pei");

    if (pei) {
    if (!cJSON_IsString(pei)) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_parseFromJSON() failed [pei]");
        goto end;
    }
    }

    cJSON *ims_vo_ps = cJSON_GetObjectItemCaseSensitive(amf_non3_gpp_access_registrationJSON, "imsVoPs");
    if (!ims_vo_ps) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_parseFromJSON() failed [ims_vo_ps]");
        goto end;
    }

    OpenAPI_ims_vo_ps_e ims_vo_psVariable;
    if (!cJSON_IsString(ims_vo_ps)) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_parseFromJSON() failed [ims_vo_ps]");
        goto end;
    }
    ims_vo_psVariable = OpenAPI_ims_vo_ps_FromString(ims_vo_ps->valuestring);

    cJSON *dereg_callback_uri = cJSON_GetObjectItemCaseSensitive(amf_non3_gpp_access_registrationJSON, "deregCallbackUri");
    if (!dereg_callback_uri) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_parseFromJSON() failed [dereg_callback_uri]");
        goto end;
    }

    if (!cJSON_IsString(dereg_callback_uri)) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_parseFromJSON() failed [dereg_callback_uri]");
        goto end;
    }

    cJSON *amf_service_name_dereg = cJSON_GetObjectItemCaseSensitive(amf_non3_gpp_access_registrationJSON, "amfServiceNameDereg");

    if (amf_service_name_dereg) {
    if (!cJSON_IsString(amf_service_name_dereg)) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_parseFromJSON() failed [amf_service_name_dereg]");
        goto end;
    }
    }

    cJSON *pcscf_restoration_callback_uri = cJSON_GetObjectItemCaseSensitive(amf_non3_gpp_access_registrationJSON, "pcscfRestorationCallbackUri");

    if (pcscf_restoration_callback_uri) {
    if (!cJSON_IsString(pcscf_restoration_callback_uri)) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_parseFromJSON() failed [pcscf_restoration_callback_uri]");
        goto end;
    }
    }

    cJSON *amf_service_name_pcscf_rest = cJSON_GetObjectItemCaseSensitive(amf_non3_gpp_access_registrationJSON, "amfServiceNamePcscfRest");

    if (amf_service_name_pcscf_rest) {
    if (!cJSON_IsString(amf_service_name_pcscf_rest)) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_parseFromJSON() failed [amf_service_name_pcscf_rest]");
        goto end;
    }
    }

    cJSON *guami = cJSON_GetObjectItemCaseSensitive(amf_non3_gpp_access_registrationJSON, "guami");
    if (!guami) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_parseFromJSON() failed [guami]");
        goto end;
    }

    OpenAPI_guami_t *guami_local_nonprim = NULL;
    guami_local_nonprim = OpenAPI_guami_parseFromJSON(guami);

    cJSON *backup_amf_info = cJSON_GetObjectItemCaseSensitive(amf_non3_gpp_access_registrationJSON, "backupAmfInfo");

    OpenAPI_list_t *backup_amf_infoList;
    if (backup_amf_info) {
    cJSON *backup_amf_info_local_nonprimitive;
    if (!cJSON_IsArray(backup_amf_info)){
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_parseFromJSON() failed [backup_amf_info]");
        goto end;
    }

    backup_amf_infoList = OpenAPI_list_create();

    cJSON_ArrayForEach(backup_amf_info_local_nonprimitive, backup_amf_info ) {
        if (!cJSON_IsObject(backup_amf_info_local_nonprimitive)) {
            ogs_error("OpenAPI_amf_non3_gpp_access_registration_parseFromJSON() failed [backup_amf_info]");
            goto end;
        }
        OpenAPI_backup_amf_info_t *backup_amf_infoItem = OpenAPI_backup_amf_info_parseFromJSON(backup_amf_info_local_nonprimitive);

        if (!backup_amf_infoItem) {
            ogs_error("No backup_amf_infoItem");
            OpenAPI_list_free(backup_amf_infoList);
            goto end;
        }

        OpenAPI_list_add(backup_amf_infoList, backup_amf_infoItem);
    }
    }

    cJSON *rat_type = cJSON_GetObjectItemCaseSensitive(amf_non3_gpp_access_registrationJSON, "ratType");
    if (!rat_type) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_parseFromJSON() failed [rat_type]");
        goto end;
    }

    OpenAPI_rat_type_e rat_typeVariable;
    if (!cJSON_IsString(rat_type)) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_parseFromJSON() failed [rat_type]");
        goto end;
    }
    rat_typeVariable = OpenAPI_rat_type_FromString(rat_type->valuestring);

    cJSON *urrp_indicator = cJSON_GetObjectItemCaseSensitive(amf_non3_gpp_access_registrationJSON, "urrpIndicator");

    if (urrp_indicator) {
    if (!cJSON_IsBool(urrp_indicator)) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_parseFromJSON() failed [urrp_indicator]");
        goto end;
    }
    }

    cJSON *amf_ee_subscription_id = cJSON_GetObjectItemCaseSensitive(amf_non3_gpp_access_registrationJSON, "amfEeSubscriptionId");

    if (amf_ee_subscription_id) {
    if (!cJSON_IsString(amf_ee_subscription_id)) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_parseFromJSON() failed [amf_ee_subscription_id]");
        goto end;
    }
    }

    cJSON *registration_time = cJSON_GetObjectItemCaseSensitive(amf_non3_gpp_access_registrationJSON, "registrationTime");

    if (registration_time) {
    if (!cJSON_IsString(registration_time)) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_parseFromJSON() failed [registration_time]");
        goto end;
    }
    }

    cJSON *vgmlc_address = cJSON_GetObjectItemCaseSensitive(amf_non3_gpp_access_registrationJSON, "vgmlcAddress");

    OpenAPI_vgmlc_address_t *vgmlc_address_local_nonprim = NULL;
    if (vgmlc_address) {
    vgmlc_address_local_nonprim = OpenAPI_vgmlc_address_parseFromJSON(vgmlc_address);
    }

    cJSON *context_info = cJSON_GetObjectItemCaseSensitive(amf_non3_gpp_access_registrationJSON, "contextInfo");

    OpenAPI_context_info_t *context_info_local_nonprim = NULL;
    if (context_info) {
    context_info_local_nonprim = OpenAPI_context_info_parseFromJSON(context_info);
    }

    cJSON *no_ee_subscription_ind = cJSON_GetObjectItemCaseSensitive(amf_non3_gpp_access_registrationJSON, "noEeSubscriptionInd");

    if (no_ee_subscription_ind) {
    if (!cJSON_IsBool(no_ee_subscription_ind)) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_parseFromJSON() failed [no_ee_subscription_ind]");
        goto end;
    }
    }

    cJSON *supi = cJSON_GetObjectItemCaseSensitive(amf_non3_gpp_access_registrationJSON, "supi");

    if (supi) {
    if (!cJSON_IsString(supi)) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_parseFromJSON() failed [supi]");
        goto end;
    }
    }

    amf_non3_gpp_access_registration_local_var = OpenAPI_amf_non3_gpp_access_registration_create (
        ogs_strdup(amf_instance_id->valuestring),
        supported_features ? ogs_strdup(supported_features->valuestring) : NULL,
        purge_flag ? true : false,
        purge_flag ? purge_flag->valueint : 0,
        pei ? ogs_strdup(pei->valuestring) : NULL,
        ims_vo_psVariable,
        ogs_strdup(dereg_callback_uri->valuestring),
        amf_service_name_dereg ? ogs_strdup(amf_service_name_dereg->valuestring) : NULL,
        pcscf_restoration_callback_uri ? ogs_strdup(pcscf_restoration_callback_uri->valuestring) : NULL,
        amf_service_name_pcscf_rest ? ogs_strdup(amf_service_name_pcscf_rest->valuestring) : NULL,
        guami_local_nonprim,
        backup_amf_info ? backup_amf_infoList : NULL,
        rat_typeVariable,
        urrp_indicator ? true : false,
        urrp_indicator ? urrp_indicator->valueint : 0,
        amf_ee_subscription_id ? ogs_strdup(amf_ee_subscription_id->valuestring) : NULL,
        registration_time ? ogs_strdup(registration_time->valuestring) : NULL,
        vgmlc_address ? vgmlc_address_local_nonprim : NULL,
        context_info ? context_info_local_nonprim : NULL,
        no_ee_subscription_ind ? true : false,
        no_ee_subscription_ind ? no_ee_subscription_ind->valueint : 0,
        supi ? ogs_strdup(supi->valuestring) : NULL
    );

    return amf_non3_gpp_access_registration_local_var;
end:
    return NULL;
}

OpenAPI_amf_non3_gpp_access_registration_t *OpenAPI_amf_non3_gpp_access_registration_copy(OpenAPI_amf_non3_gpp_access_registration_t *dst, OpenAPI_amf_non3_gpp_access_registration_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_amf_non3_gpp_access_registration_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_convertToJSON() failed");
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

    OpenAPI_amf_non3_gpp_access_registration_free(dst);
    dst = OpenAPI_amf_non3_gpp_access_registration_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

