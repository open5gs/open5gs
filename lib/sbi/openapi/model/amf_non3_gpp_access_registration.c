
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "amf_non3_gpp_access_registration.h"

OpenAPI_amf_non3_gpp_access_registration_t *OpenAPI_amf_non3_gpp_access_registration_create(
    char *amf_instance_id,
    char *supported_features,
    int purge_flag,
    char *pei,
    OpenAPI_ims_vo_ps_t *ims_vo_ps,
    char *dereg_callback_uri,
    char *amf_service_name_dereg,
    char *pcscf_restoration_callback_uri,
    char *amf_service_name_pcscf_rest,
    OpenAPI_guami_t *guami,
    OpenAPI_list_t *backup_amf_info,
    OpenAPI_rat_type_e rat_type,
    int urrp_indicator,
    char *amf_ee_subscription_id,
    char *nid,
    char *registration_time,
    char *vgmlc_address_ipv4,
    char *vgmlc_address_ipv6,
    char *vgmlc_fqdn
    )
{
    OpenAPI_amf_non3_gpp_access_registration_t *amf_non3_gpp_access_registration_local_var = OpenAPI_malloc(sizeof(OpenAPI_amf_non3_gpp_access_registration_t));
    if (!amf_non3_gpp_access_registration_local_var) {
        return NULL;
    }
    amf_non3_gpp_access_registration_local_var->amf_instance_id = amf_instance_id;
    amf_non3_gpp_access_registration_local_var->supported_features = supported_features;
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
    amf_non3_gpp_access_registration_local_var->urrp_indicator = urrp_indicator;
    amf_non3_gpp_access_registration_local_var->amf_ee_subscription_id = amf_ee_subscription_id;
    amf_non3_gpp_access_registration_local_var->nid = nid;
    amf_non3_gpp_access_registration_local_var->registration_time = registration_time;
    amf_non3_gpp_access_registration_local_var->vgmlc_address_ipv4 = vgmlc_address_ipv4;
    amf_non3_gpp_access_registration_local_var->vgmlc_address_ipv6 = vgmlc_address_ipv6;
    amf_non3_gpp_access_registration_local_var->vgmlc_fqdn = vgmlc_fqdn;

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
    OpenAPI_ims_vo_ps_free(amf_non3_gpp_access_registration->ims_vo_ps);
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
    ogs_free(amf_non3_gpp_access_registration->nid);
    ogs_free(amf_non3_gpp_access_registration->registration_time);
    ogs_free(amf_non3_gpp_access_registration->vgmlc_address_ipv4);
    ogs_free(amf_non3_gpp_access_registration->vgmlc_address_ipv6);
    ogs_free(amf_non3_gpp_access_registration->vgmlc_fqdn);
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
    if (!amf_non3_gpp_access_registration->amf_instance_id) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_convertToJSON() failed [amf_instance_id]");
        goto end;
    }
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

    if (amf_non3_gpp_access_registration->purge_flag) {
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

    if (!amf_non3_gpp_access_registration->ims_vo_ps) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_convertToJSON() failed [ims_vo_ps]");
        goto end;
    }
    cJSON *ims_vo_ps_local_JSON = OpenAPI_ims_vo_ps_convertToJSON(amf_non3_gpp_access_registration->ims_vo_ps);
    if (ims_vo_ps_local_JSON == NULL) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_convertToJSON() failed [ims_vo_ps]");
        goto end;
    }
    cJSON_AddItemToObject(item, "imsVoPs", ims_vo_ps_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_convertToJSON() failed [ims_vo_ps]");
        goto end;
    }

    if (!amf_non3_gpp_access_registration->dereg_callback_uri) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_convertToJSON() failed [dereg_callback_uri]");
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

    if (!amf_non3_gpp_access_registration->guami) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_convertToJSON() failed [guami]");
        goto end;
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

    if (!amf_non3_gpp_access_registration->rat_type) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_convertToJSON() failed [rat_type]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "ratType", OpenAPI_rat_type_ToString(amf_non3_gpp_access_registration->rat_type)) == NULL) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_convertToJSON() failed [rat_type]");
        goto end;
    }

    if (amf_non3_gpp_access_registration->urrp_indicator) {
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

    if (amf_non3_gpp_access_registration->nid) {
        if (cJSON_AddStringToObject(item, "nid", amf_non3_gpp_access_registration->nid) == NULL) {
            ogs_error("OpenAPI_amf_non3_gpp_access_registration_convertToJSON() failed [nid]");
            goto end;
        }
    }

    if (amf_non3_gpp_access_registration->registration_time) {
        if (cJSON_AddStringToObject(item, "registrationTime", amf_non3_gpp_access_registration->registration_time) == NULL) {
            ogs_error("OpenAPI_amf_non3_gpp_access_registration_convertToJSON() failed [registration_time]");
            goto end;
        }
    }

    if (amf_non3_gpp_access_registration->vgmlc_address_ipv4) {
        if (cJSON_AddStringToObject(item, "vgmlcAddressIpv4", amf_non3_gpp_access_registration->vgmlc_address_ipv4) == NULL) {
            ogs_error("OpenAPI_amf_non3_gpp_access_registration_convertToJSON() failed [vgmlc_address_ipv4]");
            goto end;
        }
    }

    if (amf_non3_gpp_access_registration->vgmlc_address_ipv6) {
        if (cJSON_AddStringToObject(item, "vgmlcAddressIpv6", amf_non3_gpp_access_registration->vgmlc_address_ipv6) == NULL) {
            ogs_error("OpenAPI_amf_non3_gpp_access_registration_convertToJSON() failed [vgmlc_address_ipv6]");
            goto end;
        }
    }

    if (amf_non3_gpp_access_registration->vgmlc_fqdn) {
        if (cJSON_AddStringToObject(item, "vgmlcFqdn", amf_non3_gpp_access_registration->vgmlc_fqdn) == NULL) {
            ogs_error("OpenAPI_amf_non3_gpp_access_registration_convertToJSON() failed [vgmlc_fqdn]");
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

    OpenAPI_ims_vo_ps_t *ims_vo_ps_local_nonprim = NULL;

    ims_vo_ps_local_nonprim = OpenAPI_ims_vo_ps_parseFromJSON(ims_vo_ps);

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
        if (!cJSON_IsArray(backup_amf_info)) {
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

    cJSON *nid = cJSON_GetObjectItemCaseSensitive(amf_non3_gpp_access_registrationJSON, "nid");

    if (nid) {
        if (!cJSON_IsString(nid)) {
            ogs_error("OpenAPI_amf_non3_gpp_access_registration_parseFromJSON() failed [nid]");
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

    cJSON *vgmlc_address_ipv4 = cJSON_GetObjectItemCaseSensitive(amf_non3_gpp_access_registrationJSON, "vgmlcAddressIpv4");

    if (vgmlc_address_ipv4) {
        if (!cJSON_IsString(vgmlc_address_ipv4)) {
            ogs_error("OpenAPI_amf_non3_gpp_access_registration_parseFromJSON() failed [vgmlc_address_ipv4]");
            goto end;
        }
    }

    cJSON *vgmlc_address_ipv6 = cJSON_GetObjectItemCaseSensitive(amf_non3_gpp_access_registrationJSON, "vgmlcAddressIpv6");

    if (vgmlc_address_ipv6) {
        if (!cJSON_IsString(vgmlc_address_ipv6)) {
            ogs_error("OpenAPI_amf_non3_gpp_access_registration_parseFromJSON() failed [vgmlc_address_ipv6]");
            goto end;
        }
    }

    cJSON *vgmlc_fqdn = cJSON_GetObjectItemCaseSensitive(amf_non3_gpp_access_registrationJSON, "vgmlcFqdn");

    if (vgmlc_fqdn) {
        if (!cJSON_IsString(vgmlc_fqdn)) {
            ogs_error("OpenAPI_amf_non3_gpp_access_registration_parseFromJSON() failed [vgmlc_fqdn]");
            goto end;
        }
    }

    amf_non3_gpp_access_registration_local_var = OpenAPI_amf_non3_gpp_access_registration_create (
        ogs_strdup(amf_instance_id->valuestring),
        supported_features ? ogs_strdup(supported_features->valuestring) : NULL,
        purge_flag ? purge_flag->valueint : 0,
        pei ? ogs_strdup(pei->valuestring) : NULL,
        ims_vo_ps_local_nonprim,
        ogs_strdup(dereg_callback_uri->valuestring),
        amf_service_name_dereg ? ogs_strdup(amf_service_name_dereg->valuestring) : NULL,
        pcscf_restoration_callback_uri ? ogs_strdup(pcscf_restoration_callback_uri->valuestring) : NULL,
        amf_service_name_pcscf_rest ? ogs_strdup(amf_service_name_pcscf_rest->valuestring) : NULL,
        guami_local_nonprim,
        backup_amf_info ? backup_amf_infoList : NULL,
        rat_typeVariable,
        urrp_indicator ? urrp_indicator->valueint : 0,
        amf_ee_subscription_id ? ogs_strdup(amf_ee_subscription_id->valuestring) : NULL,
        nid ? ogs_strdup(nid->valuestring) : NULL,
        registration_time ? ogs_strdup(registration_time->valuestring) : NULL,
        vgmlc_address_ipv4 ? ogs_strdup(vgmlc_address_ipv4->valuestring) : NULL,
        vgmlc_address_ipv6 ? ogs_strdup(vgmlc_address_ipv6->valuestring) : NULL,
        vgmlc_fqdn ? ogs_strdup(vgmlc_fqdn->valuestring) : NULL
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

