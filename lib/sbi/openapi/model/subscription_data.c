
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "subscription_data.h"

OpenAPI_subscription_data_t *OpenAPI_subscription_data_create(
    char *nf_status_notification_uri,
    char *req_nf_instance_id,
    OpenAPI_subscr_cond_t *subscr_cond,
    char *subscription_id,
    char *validity_time,
    OpenAPI_list_t *req_notif_events,
    OpenAPI_plmn_id_t *plmn_id,
    char *nid,
    OpenAPI_notif_condition_t *notif_condition,
    OpenAPI_nf_type_e req_nf_type,
    char *req_nf_fqdn,
    OpenAPI_list_t *req_snssais,
    OpenAPI_list_t *req_per_plmn_snssais,
    OpenAPI_list_t *req_plmn_list,
    OpenAPI_list_t *req_snpn_list,
    OpenAPI_list_t *serving_scope,
    char *requester_features,
    char *nrf_supported_features,
    char *hnrf_uri,
    bool is_onboarding_capability,
    int onboarding_capability,
    char *target_hni,
    char *preferred_locality
)
{
    OpenAPI_subscription_data_t *subscription_data_local_var = ogs_malloc(sizeof(OpenAPI_subscription_data_t));
    ogs_assert(subscription_data_local_var);

    subscription_data_local_var->nf_status_notification_uri = nf_status_notification_uri;
    subscription_data_local_var->req_nf_instance_id = req_nf_instance_id;
    subscription_data_local_var->subscr_cond = subscr_cond;
    subscription_data_local_var->subscription_id = subscription_id;
    subscription_data_local_var->validity_time = validity_time;
    subscription_data_local_var->req_notif_events = req_notif_events;
    subscription_data_local_var->plmn_id = plmn_id;
    subscription_data_local_var->nid = nid;
    subscription_data_local_var->notif_condition = notif_condition;
    subscription_data_local_var->req_nf_type = req_nf_type;
    subscription_data_local_var->req_nf_fqdn = req_nf_fqdn;
    subscription_data_local_var->req_snssais = req_snssais;
    subscription_data_local_var->req_per_plmn_snssais = req_per_plmn_snssais;
    subscription_data_local_var->req_plmn_list = req_plmn_list;
    subscription_data_local_var->req_snpn_list = req_snpn_list;
    subscription_data_local_var->serving_scope = serving_scope;
    subscription_data_local_var->requester_features = requester_features;
    subscription_data_local_var->nrf_supported_features = nrf_supported_features;
    subscription_data_local_var->hnrf_uri = hnrf_uri;
    subscription_data_local_var->is_onboarding_capability = is_onboarding_capability;
    subscription_data_local_var->onboarding_capability = onboarding_capability;
    subscription_data_local_var->target_hni = target_hni;
    subscription_data_local_var->preferred_locality = preferred_locality;

    return subscription_data_local_var;
}

void OpenAPI_subscription_data_free(OpenAPI_subscription_data_t *subscription_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == subscription_data) {
        return;
    }
    if (subscription_data->nf_status_notification_uri) {
        ogs_free(subscription_data->nf_status_notification_uri);
        subscription_data->nf_status_notification_uri = NULL;
    }
    if (subscription_data->req_nf_instance_id) {
        ogs_free(subscription_data->req_nf_instance_id);
        subscription_data->req_nf_instance_id = NULL;
    }
    if (subscription_data->subscr_cond) {
        OpenAPI_subscr_cond_free(subscription_data->subscr_cond);
        subscription_data->subscr_cond = NULL;
    }
    if (subscription_data->subscription_id) {
        ogs_free(subscription_data->subscription_id);
        subscription_data->subscription_id = NULL;
    }
    if (subscription_data->validity_time) {
        ogs_free(subscription_data->validity_time);
        subscription_data->validity_time = NULL;
    }
    if (subscription_data->req_notif_events) {
        OpenAPI_list_free(subscription_data->req_notif_events);
        subscription_data->req_notif_events = NULL;
    }
    if (subscription_data->plmn_id) {
        OpenAPI_plmn_id_free(subscription_data->plmn_id);
        subscription_data->plmn_id = NULL;
    }
    if (subscription_data->nid) {
        ogs_free(subscription_data->nid);
        subscription_data->nid = NULL;
    }
    if (subscription_data->notif_condition) {
        OpenAPI_notif_condition_free(subscription_data->notif_condition);
        subscription_data->notif_condition = NULL;
    }
    if (subscription_data->req_nf_fqdn) {
        ogs_free(subscription_data->req_nf_fqdn);
        subscription_data->req_nf_fqdn = NULL;
    }
    if (subscription_data->req_snssais) {
        OpenAPI_list_for_each(subscription_data->req_snssais, node) {
            OpenAPI_ext_snssai_free(node->data);
        }
        OpenAPI_list_free(subscription_data->req_snssais);
        subscription_data->req_snssais = NULL;
    }
    if (subscription_data->req_per_plmn_snssais) {
        OpenAPI_list_for_each(subscription_data->req_per_plmn_snssais, node) {
            OpenAPI_plmn_snssai_free(node->data);
        }
        OpenAPI_list_free(subscription_data->req_per_plmn_snssais);
        subscription_data->req_per_plmn_snssais = NULL;
    }
    if (subscription_data->req_plmn_list) {
        OpenAPI_list_for_each(subscription_data->req_plmn_list, node) {
            OpenAPI_plmn_id_free(node->data);
        }
        OpenAPI_list_free(subscription_data->req_plmn_list);
        subscription_data->req_plmn_list = NULL;
    }
    if (subscription_data->req_snpn_list) {
        OpenAPI_list_for_each(subscription_data->req_snpn_list, node) {
            OpenAPI_plmn_id_nid_free(node->data);
        }
        OpenAPI_list_free(subscription_data->req_snpn_list);
        subscription_data->req_snpn_list = NULL;
    }
    if (subscription_data->serving_scope) {
        OpenAPI_list_for_each(subscription_data->serving_scope, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(subscription_data->serving_scope);
        subscription_data->serving_scope = NULL;
    }
    if (subscription_data->requester_features) {
        ogs_free(subscription_data->requester_features);
        subscription_data->requester_features = NULL;
    }
    if (subscription_data->nrf_supported_features) {
        ogs_free(subscription_data->nrf_supported_features);
        subscription_data->nrf_supported_features = NULL;
    }
    if (subscription_data->hnrf_uri) {
        ogs_free(subscription_data->hnrf_uri);
        subscription_data->hnrf_uri = NULL;
    }
    if (subscription_data->target_hni) {
        ogs_free(subscription_data->target_hni);
        subscription_data->target_hni = NULL;
    }
    if (subscription_data->preferred_locality) {
        ogs_free(subscription_data->preferred_locality);
        subscription_data->preferred_locality = NULL;
    }
    ogs_free(subscription_data);
}

cJSON *OpenAPI_subscription_data_convertToJSON(OpenAPI_subscription_data_t *subscription_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (subscription_data == NULL) {
        ogs_error("OpenAPI_subscription_data_convertToJSON() failed [SubscriptionData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!subscription_data->nf_status_notification_uri) {
        ogs_error("OpenAPI_subscription_data_convertToJSON() failed [nf_status_notification_uri]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "nfStatusNotificationUri", subscription_data->nf_status_notification_uri) == NULL) {
        ogs_error("OpenAPI_subscription_data_convertToJSON() failed [nf_status_notification_uri]");
        goto end;
    }

    if (subscription_data->req_nf_instance_id) {
    if (cJSON_AddStringToObject(item, "reqNfInstanceId", subscription_data->req_nf_instance_id) == NULL) {
        ogs_error("OpenAPI_subscription_data_convertToJSON() failed [req_nf_instance_id]");
        goto end;
    }
    }

    if (subscription_data->subscr_cond) {
    cJSON *subscr_cond_local_JSON = OpenAPI_subscr_cond_convertToJSON(subscription_data->subscr_cond);
    if (subscr_cond_local_JSON == NULL) {
        ogs_error("OpenAPI_subscription_data_convertToJSON() failed [subscr_cond]");
        goto end;
    }
    cJSON_AddItemToObject(item, "subscrCond", subscr_cond_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_subscription_data_convertToJSON() failed [subscr_cond]");
        goto end;
    }
    }

    if (subscription_data->subscription_id) {
    if (cJSON_AddStringToObject(item, "subscriptionId", subscription_data->subscription_id) == NULL) {
        ogs_error("OpenAPI_subscription_data_convertToJSON() failed [subscription_id]");
        goto end;
    }
    }

    if (subscription_data->validity_time) {
    if (cJSON_AddStringToObject(item, "validityTime", subscription_data->validity_time) == NULL) {
        ogs_error("OpenAPI_subscription_data_convertToJSON() failed [validity_time]");
        goto end;
    }
    }

    if (subscription_data->req_notif_events != OpenAPI_notification_event_type_NULL) {
    cJSON *req_notif_eventsList = cJSON_AddArrayToObject(item, "reqNotifEvents");
    if (req_notif_eventsList == NULL) {
        ogs_error("OpenAPI_subscription_data_convertToJSON() failed [req_notif_events]");
        goto end;
    }
    OpenAPI_list_for_each(subscription_data->req_notif_events, node) {
        if (cJSON_AddStringToObject(req_notif_eventsList, "", OpenAPI_notification_event_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_subscription_data_convertToJSON() failed [req_notif_events]");
            goto end;
        }
    }
    }

    if (subscription_data->plmn_id) {
    cJSON *plmn_id_local_JSON = OpenAPI_plmn_id_convertToJSON(subscription_data->plmn_id);
    if (plmn_id_local_JSON == NULL) {
        ogs_error("OpenAPI_subscription_data_convertToJSON() failed [plmn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "plmnId", plmn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_subscription_data_convertToJSON() failed [plmn_id]");
        goto end;
    }
    }

    if (subscription_data->nid) {
    if (cJSON_AddStringToObject(item, "nid", subscription_data->nid) == NULL) {
        ogs_error("OpenAPI_subscription_data_convertToJSON() failed [nid]");
        goto end;
    }
    }

    if (subscription_data->notif_condition) {
    cJSON *notif_condition_local_JSON = OpenAPI_notif_condition_convertToJSON(subscription_data->notif_condition);
    if (notif_condition_local_JSON == NULL) {
        ogs_error("OpenAPI_subscription_data_convertToJSON() failed [notif_condition]");
        goto end;
    }
    cJSON_AddItemToObject(item, "notifCondition", notif_condition_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_subscription_data_convertToJSON() failed [notif_condition]");
        goto end;
    }
    }

    if (subscription_data->req_nf_type != OpenAPI_nf_type_NULL) {
    if (cJSON_AddStringToObject(item, "reqNfType", OpenAPI_nf_type_ToString(subscription_data->req_nf_type)) == NULL) {
        ogs_error("OpenAPI_subscription_data_convertToJSON() failed [req_nf_type]");
        goto end;
    }
    }

    if (subscription_data->req_nf_fqdn) {
    if (cJSON_AddStringToObject(item, "reqNfFqdn", subscription_data->req_nf_fqdn) == NULL) {
        ogs_error("OpenAPI_subscription_data_convertToJSON() failed [req_nf_fqdn]");
        goto end;
    }
    }

    if (subscription_data->req_snssais) {
    cJSON *req_snssaisList = cJSON_AddArrayToObject(item, "reqSnssais");
    if (req_snssaisList == NULL) {
        ogs_error("OpenAPI_subscription_data_convertToJSON() failed [req_snssais]");
        goto end;
    }
    OpenAPI_list_for_each(subscription_data->req_snssais, node) {
        cJSON *itemLocal = OpenAPI_ext_snssai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_subscription_data_convertToJSON() failed [req_snssais]");
            goto end;
        }
        cJSON_AddItemToArray(req_snssaisList, itemLocal);
    }
    }

    if (subscription_data->req_per_plmn_snssais) {
    cJSON *req_per_plmn_snssaisList = cJSON_AddArrayToObject(item, "reqPerPlmnSnssais");
    if (req_per_plmn_snssaisList == NULL) {
        ogs_error("OpenAPI_subscription_data_convertToJSON() failed [req_per_plmn_snssais]");
        goto end;
    }
    OpenAPI_list_for_each(subscription_data->req_per_plmn_snssais, node) {
        cJSON *itemLocal = OpenAPI_plmn_snssai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_subscription_data_convertToJSON() failed [req_per_plmn_snssais]");
            goto end;
        }
        cJSON_AddItemToArray(req_per_plmn_snssaisList, itemLocal);
    }
    }

    if (subscription_data->req_plmn_list) {
    cJSON *req_plmn_listList = cJSON_AddArrayToObject(item, "reqPlmnList");
    if (req_plmn_listList == NULL) {
        ogs_error("OpenAPI_subscription_data_convertToJSON() failed [req_plmn_list]");
        goto end;
    }
    OpenAPI_list_for_each(subscription_data->req_plmn_list, node) {
        cJSON *itemLocal = OpenAPI_plmn_id_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_subscription_data_convertToJSON() failed [req_plmn_list]");
            goto end;
        }
        cJSON_AddItemToArray(req_plmn_listList, itemLocal);
    }
    }

    if (subscription_data->req_snpn_list) {
    cJSON *req_snpn_listList = cJSON_AddArrayToObject(item, "reqSnpnList");
    if (req_snpn_listList == NULL) {
        ogs_error("OpenAPI_subscription_data_convertToJSON() failed [req_snpn_list]");
        goto end;
    }
    OpenAPI_list_for_each(subscription_data->req_snpn_list, node) {
        cJSON *itemLocal = OpenAPI_plmn_id_nid_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_subscription_data_convertToJSON() failed [req_snpn_list]");
            goto end;
        }
        cJSON_AddItemToArray(req_snpn_listList, itemLocal);
    }
    }

    if (subscription_data->serving_scope) {
    cJSON *serving_scopeList = cJSON_AddArrayToObject(item, "servingScope");
    if (serving_scopeList == NULL) {
        ogs_error("OpenAPI_subscription_data_convertToJSON() failed [serving_scope]");
        goto end;
    }
    OpenAPI_list_for_each(subscription_data->serving_scope, node) {
        if (cJSON_AddStringToObject(serving_scopeList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_subscription_data_convertToJSON() failed [serving_scope]");
            goto end;
        }
    }
    }

    if (subscription_data->requester_features) {
    if (cJSON_AddStringToObject(item, "requesterFeatures", subscription_data->requester_features) == NULL) {
        ogs_error("OpenAPI_subscription_data_convertToJSON() failed [requester_features]");
        goto end;
    }
    }

    if (subscription_data->nrf_supported_features) {
    if (cJSON_AddStringToObject(item, "nrfSupportedFeatures", subscription_data->nrf_supported_features) == NULL) {
        ogs_error("OpenAPI_subscription_data_convertToJSON() failed [nrf_supported_features]");
        goto end;
    }
    }

    if (subscription_data->hnrf_uri) {
    if (cJSON_AddStringToObject(item, "hnrfUri", subscription_data->hnrf_uri) == NULL) {
        ogs_error("OpenAPI_subscription_data_convertToJSON() failed [hnrf_uri]");
        goto end;
    }
    }

    if (subscription_data->is_onboarding_capability) {
    if (cJSON_AddBoolToObject(item, "onboardingCapability", subscription_data->onboarding_capability) == NULL) {
        ogs_error("OpenAPI_subscription_data_convertToJSON() failed [onboarding_capability]");
        goto end;
    }
    }

    if (subscription_data->target_hni) {
    if (cJSON_AddStringToObject(item, "targetHni", subscription_data->target_hni) == NULL) {
        ogs_error("OpenAPI_subscription_data_convertToJSON() failed [target_hni]");
        goto end;
    }
    }

    if (subscription_data->preferred_locality) {
    if (cJSON_AddStringToObject(item, "preferredLocality", subscription_data->preferred_locality) == NULL) {
        ogs_error("OpenAPI_subscription_data_convertToJSON() failed [preferred_locality]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_subscription_data_t *OpenAPI_subscription_data_parseFromJSON(cJSON *subscription_dataJSON)
{
    OpenAPI_subscription_data_t *subscription_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *nf_status_notification_uri = NULL;
    cJSON *req_nf_instance_id = NULL;
    cJSON *subscr_cond = NULL;
    OpenAPI_subscr_cond_t *subscr_cond_local_nonprim = NULL;
    cJSON *subscription_id = NULL;
    cJSON *validity_time = NULL;
    cJSON *req_notif_events = NULL;
    OpenAPI_list_t *req_notif_eventsList = NULL;
    cJSON *plmn_id = NULL;
    OpenAPI_plmn_id_t *plmn_id_local_nonprim = NULL;
    cJSON *nid = NULL;
    cJSON *notif_condition = NULL;
    OpenAPI_notif_condition_t *notif_condition_local_nonprim = NULL;
    cJSON *req_nf_type = NULL;
    OpenAPI_nf_type_e req_nf_typeVariable = 0;
    cJSON *req_nf_fqdn = NULL;
    cJSON *req_snssais = NULL;
    OpenAPI_list_t *req_snssaisList = NULL;
    cJSON *req_per_plmn_snssais = NULL;
    OpenAPI_list_t *req_per_plmn_snssaisList = NULL;
    cJSON *req_plmn_list = NULL;
    OpenAPI_list_t *req_plmn_listList = NULL;
    cJSON *req_snpn_list = NULL;
    OpenAPI_list_t *req_snpn_listList = NULL;
    cJSON *serving_scope = NULL;
    OpenAPI_list_t *serving_scopeList = NULL;
    cJSON *requester_features = NULL;
    cJSON *nrf_supported_features = NULL;
    cJSON *hnrf_uri = NULL;
    cJSON *onboarding_capability = NULL;
    cJSON *target_hni = NULL;
    cJSON *preferred_locality = NULL;
    nf_status_notification_uri = cJSON_GetObjectItemCaseSensitive(subscription_dataJSON, "nfStatusNotificationUri");
    if (!nf_status_notification_uri) {
        ogs_error("OpenAPI_subscription_data_parseFromJSON() failed [nf_status_notification_uri]");
        goto end;
    }
    if (!cJSON_IsString(nf_status_notification_uri)) {
        ogs_error("OpenAPI_subscription_data_parseFromJSON() failed [nf_status_notification_uri]");
        goto end;
    }

    req_nf_instance_id = cJSON_GetObjectItemCaseSensitive(subscription_dataJSON, "reqNfInstanceId");
    if (req_nf_instance_id) {
    if (!cJSON_IsString(req_nf_instance_id) && !cJSON_IsNull(req_nf_instance_id)) {
        ogs_error("OpenAPI_subscription_data_parseFromJSON() failed [req_nf_instance_id]");
        goto end;
    }
    }

    subscr_cond = cJSON_GetObjectItemCaseSensitive(subscription_dataJSON, "subscrCond");
    if (subscr_cond) {
    subscr_cond_local_nonprim = OpenAPI_subscr_cond_parseFromJSON(subscr_cond);
    if (!subscr_cond_local_nonprim) {
        ogs_error("OpenAPI_subscr_cond_parseFromJSON failed [subscr_cond]");
        goto end;
    }
    }

    subscription_id = cJSON_GetObjectItemCaseSensitive(subscription_dataJSON, "subscriptionId");
    if (subscription_id) {
    if (!cJSON_IsString(subscription_id) && !cJSON_IsNull(subscription_id)) {
        ogs_error("OpenAPI_subscription_data_parseFromJSON() failed [subscription_id]");
        goto end;
    }
    }

    validity_time = cJSON_GetObjectItemCaseSensitive(subscription_dataJSON, "validityTime");
    if (validity_time) {
    if (!cJSON_IsString(validity_time) && !cJSON_IsNull(validity_time)) {
        ogs_error("OpenAPI_subscription_data_parseFromJSON() failed [validity_time]");
        goto end;
    }
    }

    req_notif_events = cJSON_GetObjectItemCaseSensitive(subscription_dataJSON, "reqNotifEvents");
    if (req_notif_events) {
        cJSON *req_notif_events_local = NULL;
        if (!cJSON_IsArray(req_notif_events)) {
            ogs_error("OpenAPI_subscription_data_parseFromJSON() failed [req_notif_events]");
            goto end;
        }

        req_notif_eventsList = OpenAPI_list_create();

        cJSON_ArrayForEach(req_notif_events_local, req_notif_events) {
            OpenAPI_notification_event_type_e localEnum = OpenAPI_notification_event_type_NULL;
            if (!cJSON_IsString(req_notif_events_local)) {
                ogs_error("OpenAPI_subscription_data_parseFromJSON() failed [req_notif_events]");
                goto end;
            }
            localEnum = OpenAPI_notification_event_type_FromString(req_notif_events_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"req_notif_events\" is not supported. Ignoring it ...",
                         req_notif_events_local->valuestring);
            } else {
                OpenAPI_list_add(req_notif_eventsList, (void *)localEnum);
            }
        }
        if (req_notif_eventsList->count == 0) {
            ogs_error("OpenAPI_subscription_data_parseFromJSON() failed: Expected req_notif_eventsList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    plmn_id = cJSON_GetObjectItemCaseSensitive(subscription_dataJSON, "plmnId");
    if (plmn_id) {
    plmn_id_local_nonprim = OpenAPI_plmn_id_parseFromJSON(plmn_id);
    if (!plmn_id_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_parseFromJSON failed [plmn_id]");
        goto end;
    }
    }

    nid = cJSON_GetObjectItemCaseSensitive(subscription_dataJSON, "nid");
    if (nid) {
    if (!cJSON_IsString(nid) && !cJSON_IsNull(nid)) {
        ogs_error("OpenAPI_subscription_data_parseFromJSON() failed [nid]");
        goto end;
    }
    }

    notif_condition = cJSON_GetObjectItemCaseSensitive(subscription_dataJSON, "notifCondition");
    if (notif_condition) {
    notif_condition_local_nonprim = OpenAPI_notif_condition_parseFromJSON(notif_condition);
    if (!notif_condition_local_nonprim) {
        ogs_error("OpenAPI_notif_condition_parseFromJSON failed [notif_condition]");
        goto end;
    }
    }

    req_nf_type = cJSON_GetObjectItemCaseSensitive(subscription_dataJSON, "reqNfType");
    if (req_nf_type) {
    if (!cJSON_IsString(req_nf_type)) {
        ogs_error("OpenAPI_subscription_data_parseFromJSON() failed [req_nf_type]");
        goto end;
    }
    req_nf_typeVariable = OpenAPI_nf_type_FromString(req_nf_type->valuestring);
    }

    req_nf_fqdn = cJSON_GetObjectItemCaseSensitive(subscription_dataJSON, "reqNfFqdn");
    if (req_nf_fqdn) {
    if (!cJSON_IsString(req_nf_fqdn) && !cJSON_IsNull(req_nf_fqdn)) {
        ogs_error("OpenAPI_subscription_data_parseFromJSON() failed [req_nf_fqdn]");
        goto end;
    }
    }

    req_snssais = cJSON_GetObjectItemCaseSensitive(subscription_dataJSON, "reqSnssais");
    if (req_snssais) {
        cJSON *req_snssais_local = NULL;
        if (!cJSON_IsArray(req_snssais)) {
            ogs_error("OpenAPI_subscription_data_parseFromJSON() failed [req_snssais]");
            goto end;
        }

        req_snssaisList = OpenAPI_list_create();

        cJSON_ArrayForEach(req_snssais_local, req_snssais) {
            if (!cJSON_IsObject(req_snssais_local)) {
                ogs_error("OpenAPI_subscription_data_parseFromJSON() failed [req_snssais]");
                goto end;
            }
            OpenAPI_ext_snssai_t *req_snssaisItem = OpenAPI_ext_snssai_parseFromJSON(req_snssais_local);
            if (!req_snssaisItem) {
                ogs_error("No req_snssaisItem");
                goto end;
            }
            OpenAPI_list_add(req_snssaisList, req_snssaisItem);
        }
    }

    req_per_plmn_snssais = cJSON_GetObjectItemCaseSensitive(subscription_dataJSON, "reqPerPlmnSnssais");
    if (req_per_plmn_snssais) {
        cJSON *req_per_plmn_snssais_local = NULL;
        if (!cJSON_IsArray(req_per_plmn_snssais)) {
            ogs_error("OpenAPI_subscription_data_parseFromJSON() failed [req_per_plmn_snssais]");
            goto end;
        }

        req_per_plmn_snssaisList = OpenAPI_list_create();

        cJSON_ArrayForEach(req_per_plmn_snssais_local, req_per_plmn_snssais) {
            if (!cJSON_IsObject(req_per_plmn_snssais_local)) {
                ogs_error("OpenAPI_subscription_data_parseFromJSON() failed [req_per_plmn_snssais]");
                goto end;
            }
            OpenAPI_plmn_snssai_t *req_per_plmn_snssaisItem = OpenAPI_plmn_snssai_parseFromJSON(req_per_plmn_snssais_local);
            if (!req_per_plmn_snssaisItem) {
                ogs_error("No req_per_plmn_snssaisItem");
                goto end;
            }
            OpenAPI_list_add(req_per_plmn_snssaisList, req_per_plmn_snssaisItem);
        }
    }

    req_plmn_list = cJSON_GetObjectItemCaseSensitive(subscription_dataJSON, "reqPlmnList");
    if (req_plmn_list) {
        cJSON *req_plmn_list_local = NULL;
        if (!cJSON_IsArray(req_plmn_list)) {
            ogs_error("OpenAPI_subscription_data_parseFromJSON() failed [req_plmn_list]");
            goto end;
        }

        req_plmn_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(req_plmn_list_local, req_plmn_list) {
            if (!cJSON_IsObject(req_plmn_list_local)) {
                ogs_error("OpenAPI_subscription_data_parseFromJSON() failed [req_plmn_list]");
                goto end;
            }
            OpenAPI_plmn_id_t *req_plmn_listItem = OpenAPI_plmn_id_parseFromJSON(req_plmn_list_local);
            if (!req_plmn_listItem) {
                ogs_error("No req_plmn_listItem");
                goto end;
            }
            OpenAPI_list_add(req_plmn_listList, req_plmn_listItem);
        }
    }

    req_snpn_list = cJSON_GetObjectItemCaseSensitive(subscription_dataJSON, "reqSnpnList");
    if (req_snpn_list) {
        cJSON *req_snpn_list_local = NULL;
        if (!cJSON_IsArray(req_snpn_list)) {
            ogs_error("OpenAPI_subscription_data_parseFromJSON() failed [req_snpn_list]");
            goto end;
        }

        req_snpn_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(req_snpn_list_local, req_snpn_list) {
            if (!cJSON_IsObject(req_snpn_list_local)) {
                ogs_error("OpenAPI_subscription_data_parseFromJSON() failed [req_snpn_list]");
                goto end;
            }
            OpenAPI_plmn_id_nid_t *req_snpn_listItem = OpenAPI_plmn_id_nid_parseFromJSON(req_snpn_list_local);
            if (!req_snpn_listItem) {
                ogs_error("No req_snpn_listItem");
                goto end;
            }
            OpenAPI_list_add(req_snpn_listList, req_snpn_listItem);
        }
    }

    serving_scope = cJSON_GetObjectItemCaseSensitive(subscription_dataJSON, "servingScope");
    if (serving_scope) {
        cJSON *serving_scope_local = NULL;
        if (!cJSON_IsArray(serving_scope)) {
            ogs_error("OpenAPI_subscription_data_parseFromJSON() failed [serving_scope]");
            goto end;
        }

        serving_scopeList = OpenAPI_list_create();

        cJSON_ArrayForEach(serving_scope_local, serving_scope) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(serving_scope_local)) {
                ogs_error("OpenAPI_subscription_data_parseFromJSON() failed [serving_scope]");
                goto end;
            }
            OpenAPI_list_add(serving_scopeList, ogs_strdup(serving_scope_local->valuestring));
        }
    }

    requester_features = cJSON_GetObjectItemCaseSensitive(subscription_dataJSON, "requesterFeatures");
    if (requester_features) {
    if (!cJSON_IsString(requester_features) && !cJSON_IsNull(requester_features)) {
        ogs_error("OpenAPI_subscription_data_parseFromJSON() failed [requester_features]");
        goto end;
    }
    }

    nrf_supported_features = cJSON_GetObjectItemCaseSensitive(subscription_dataJSON, "nrfSupportedFeatures");
    if (nrf_supported_features) {
    if (!cJSON_IsString(nrf_supported_features) && !cJSON_IsNull(nrf_supported_features)) {
        ogs_error("OpenAPI_subscription_data_parseFromJSON() failed [nrf_supported_features]");
        goto end;
    }
    }

    hnrf_uri = cJSON_GetObjectItemCaseSensitive(subscription_dataJSON, "hnrfUri");
    if (hnrf_uri) {
    if (!cJSON_IsString(hnrf_uri) && !cJSON_IsNull(hnrf_uri)) {
        ogs_error("OpenAPI_subscription_data_parseFromJSON() failed [hnrf_uri]");
        goto end;
    }
    }

    onboarding_capability = cJSON_GetObjectItemCaseSensitive(subscription_dataJSON, "onboardingCapability");
    if (onboarding_capability) {
    if (!cJSON_IsBool(onboarding_capability)) {
        ogs_error("OpenAPI_subscription_data_parseFromJSON() failed [onboarding_capability]");
        goto end;
    }
    }

    target_hni = cJSON_GetObjectItemCaseSensitive(subscription_dataJSON, "targetHni");
    if (target_hni) {
    if (!cJSON_IsString(target_hni) && !cJSON_IsNull(target_hni)) {
        ogs_error("OpenAPI_subscription_data_parseFromJSON() failed [target_hni]");
        goto end;
    }
    }

    preferred_locality = cJSON_GetObjectItemCaseSensitive(subscription_dataJSON, "preferredLocality");
    if (preferred_locality) {
    if (!cJSON_IsString(preferred_locality) && !cJSON_IsNull(preferred_locality)) {
        ogs_error("OpenAPI_subscription_data_parseFromJSON() failed [preferred_locality]");
        goto end;
    }
    }

    subscription_data_local_var = OpenAPI_subscription_data_create (
        ogs_strdup(nf_status_notification_uri->valuestring),
        req_nf_instance_id && !cJSON_IsNull(req_nf_instance_id) ? ogs_strdup(req_nf_instance_id->valuestring) : NULL,
        subscr_cond ? subscr_cond_local_nonprim : NULL,
        subscription_id && !cJSON_IsNull(subscription_id) ? ogs_strdup(subscription_id->valuestring) : NULL,
        validity_time && !cJSON_IsNull(validity_time) ? ogs_strdup(validity_time->valuestring) : NULL,
        req_notif_events ? req_notif_eventsList : NULL,
        plmn_id ? plmn_id_local_nonprim : NULL,
        nid && !cJSON_IsNull(nid) ? ogs_strdup(nid->valuestring) : NULL,
        notif_condition ? notif_condition_local_nonprim : NULL,
        req_nf_type ? req_nf_typeVariable : 0,
        req_nf_fqdn && !cJSON_IsNull(req_nf_fqdn) ? ogs_strdup(req_nf_fqdn->valuestring) : NULL,
        req_snssais ? req_snssaisList : NULL,
        req_per_plmn_snssais ? req_per_plmn_snssaisList : NULL,
        req_plmn_list ? req_plmn_listList : NULL,
        req_snpn_list ? req_snpn_listList : NULL,
        serving_scope ? serving_scopeList : NULL,
        requester_features && !cJSON_IsNull(requester_features) ? ogs_strdup(requester_features->valuestring) : NULL,
        nrf_supported_features && !cJSON_IsNull(nrf_supported_features) ? ogs_strdup(nrf_supported_features->valuestring) : NULL,
        hnrf_uri && !cJSON_IsNull(hnrf_uri) ? ogs_strdup(hnrf_uri->valuestring) : NULL,
        onboarding_capability ? true : false,
        onboarding_capability ? onboarding_capability->valueint : 0,
        target_hni && !cJSON_IsNull(target_hni) ? ogs_strdup(target_hni->valuestring) : NULL,
        preferred_locality && !cJSON_IsNull(preferred_locality) ? ogs_strdup(preferred_locality->valuestring) : NULL
    );

    return subscription_data_local_var;
end:
    if (subscr_cond_local_nonprim) {
        OpenAPI_subscr_cond_free(subscr_cond_local_nonprim);
        subscr_cond_local_nonprim = NULL;
    }
    if (req_notif_eventsList) {
        OpenAPI_list_free(req_notif_eventsList);
        req_notif_eventsList = NULL;
    }
    if (plmn_id_local_nonprim) {
        OpenAPI_plmn_id_free(plmn_id_local_nonprim);
        plmn_id_local_nonprim = NULL;
    }
    if (notif_condition_local_nonprim) {
        OpenAPI_notif_condition_free(notif_condition_local_nonprim);
        notif_condition_local_nonprim = NULL;
    }
    if (req_snssaisList) {
        OpenAPI_list_for_each(req_snssaisList, node) {
            OpenAPI_ext_snssai_free(node->data);
        }
        OpenAPI_list_free(req_snssaisList);
        req_snssaisList = NULL;
    }
    if (req_per_plmn_snssaisList) {
        OpenAPI_list_for_each(req_per_plmn_snssaisList, node) {
            OpenAPI_plmn_snssai_free(node->data);
        }
        OpenAPI_list_free(req_per_plmn_snssaisList);
        req_per_plmn_snssaisList = NULL;
    }
    if (req_plmn_listList) {
        OpenAPI_list_for_each(req_plmn_listList, node) {
            OpenAPI_plmn_id_free(node->data);
        }
        OpenAPI_list_free(req_plmn_listList);
        req_plmn_listList = NULL;
    }
    if (req_snpn_listList) {
        OpenAPI_list_for_each(req_snpn_listList, node) {
            OpenAPI_plmn_id_nid_free(node->data);
        }
        OpenAPI_list_free(req_snpn_listList);
        req_snpn_listList = NULL;
    }
    if (serving_scopeList) {
        OpenAPI_list_for_each(serving_scopeList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(serving_scopeList);
        serving_scopeList = NULL;
    }
    return NULL;
}

OpenAPI_subscription_data_t *OpenAPI_subscription_data_copy(OpenAPI_subscription_data_t *dst, OpenAPI_subscription_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_subscription_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_subscription_data_convertToJSON() failed");
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

    OpenAPI_subscription_data_free(dst);
    dst = OpenAPI_subscription_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

