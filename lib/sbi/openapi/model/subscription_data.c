
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "subscription_data.h"

OpenAPI_subscription_data_t *OpenAPI_subscription_data_create(
    char *nf_status_notification_uri,
    char *req_nf_instance_id,
    OpenAPI_subscription_data_subscr_cond_t *subscr_cond,
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
    OpenAPI_list_t *serving_scope
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

    return subscription_data_local_var;
}

void OpenAPI_subscription_data_free(OpenAPI_subscription_data_t *subscription_data)
{
    if (NULL == subscription_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(subscription_data->nf_status_notification_uri);
    ogs_free(subscription_data->req_nf_instance_id);
    OpenAPI_subscription_data_subscr_cond_free(subscription_data->subscr_cond);
    ogs_free(subscription_data->subscription_id);
    ogs_free(subscription_data->validity_time);
    OpenAPI_list_free(subscription_data->req_notif_events);
    OpenAPI_plmn_id_free(subscription_data->plmn_id);
    ogs_free(subscription_data->nid);
    OpenAPI_notif_condition_free(subscription_data->notif_condition);
    ogs_free(subscription_data->req_nf_fqdn);
    OpenAPI_list_for_each(subscription_data->req_snssais, node) {
        OpenAPI_snssai_free(node->data);
    }
    OpenAPI_list_free(subscription_data->req_snssais);
    OpenAPI_list_for_each(subscription_data->req_per_plmn_snssais, node) {
        OpenAPI_plmn_snssai_free(node->data);
    }
    OpenAPI_list_free(subscription_data->req_per_plmn_snssais);
    OpenAPI_list_for_each(subscription_data->req_plmn_list, node) {
        OpenAPI_plmn_id_free(node->data);
    }
    OpenAPI_list_free(subscription_data->req_plmn_list);
    OpenAPI_list_for_each(subscription_data->req_snpn_list, node) {
        OpenAPI_plmn_id_nid_free(node->data);
    }
    OpenAPI_list_free(subscription_data->req_snpn_list);
    OpenAPI_list_for_each(subscription_data->serving_scope, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(subscription_data->serving_scope);
    ogs_free(subscription_data);
}

cJSON *OpenAPI_subscription_data_convertToJSON(OpenAPI_subscription_data_t *subscription_data)
{
    cJSON *item = NULL;

    if (subscription_data == NULL) {
        ogs_error("OpenAPI_subscription_data_convertToJSON() failed [SubscriptionData]");
        return NULL;
    }

    item = cJSON_CreateObject();
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
    cJSON *subscr_cond_local_JSON = OpenAPI_subscription_data_subscr_cond_convertToJSON(subscription_data->subscr_cond);
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

    if (subscription_data->req_notif_events) {
    cJSON *req_notif_events = cJSON_AddArrayToObject(item, "reqNotifEvents");
    if (req_notif_events == NULL) {
        ogs_error("OpenAPI_subscription_data_convertToJSON() failed [req_notif_events]");
        goto end;
    }
    OpenAPI_lnode_t *req_notif_events_node;
    OpenAPI_list_for_each(subscription_data->req_notif_events, req_notif_events_node) {
        if (cJSON_AddStringToObject(req_notif_events, "", OpenAPI_notification_event_type_ToString((intptr_t)req_notif_events_node->data)) == NULL) {
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

    if (subscription_data->req_nf_type) {
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

    OpenAPI_lnode_t *req_snssais_node;
    if (subscription_data->req_snssais) {
        OpenAPI_list_for_each(subscription_data->req_snssais, req_snssais_node) {
            cJSON *itemLocal = OpenAPI_snssai_convertToJSON(req_snssais_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_subscription_data_convertToJSON() failed [req_snssais]");
                goto end;
            }
            cJSON_AddItemToArray(req_snssaisList, itemLocal);
        }
    }
    }

    if (subscription_data->req_per_plmn_snssais) {
    cJSON *req_per_plmn_snssaisList = cJSON_AddArrayToObject(item, "reqPerPlmnSnssais");
    if (req_per_plmn_snssaisList == NULL) {
        ogs_error("OpenAPI_subscription_data_convertToJSON() failed [req_per_plmn_snssais]");
        goto end;
    }

    OpenAPI_lnode_t *req_per_plmn_snssais_node;
    if (subscription_data->req_per_plmn_snssais) {
        OpenAPI_list_for_each(subscription_data->req_per_plmn_snssais, req_per_plmn_snssais_node) {
            cJSON *itemLocal = OpenAPI_plmn_snssai_convertToJSON(req_per_plmn_snssais_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_subscription_data_convertToJSON() failed [req_per_plmn_snssais]");
                goto end;
            }
            cJSON_AddItemToArray(req_per_plmn_snssaisList, itemLocal);
        }
    }
    }

    if (subscription_data->req_plmn_list) {
    cJSON *req_plmn_listList = cJSON_AddArrayToObject(item, "reqPlmnList");
    if (req_plmn_listList == NULL) {
        ogs_error("OpenAPI_subscription_data_convertToJSON() failed [req_plmn_list]");
        goto end;
    }

    OpenAPI_lnode_t *req_plmn_list_node;
    if (subscription_data->req_plmn_list) {
        OpenAPI_list_for_each(subscription_data->req_plmn_list, req_plmn_list_node) {
            cJSON *itemLocal = OpenAPI_plmn_id_convertToJSON(req_plmn_list_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_subscription_data_convertToJSON() failed [req_plmn_list]");
                goto end;
            }
            cJSON_AddItemToArray(req_plmn_listList, itemLocal);
        }
    }
    }

    if (subscription_data->req_snpn_list) {
    cJSON *req_snpn_listList = cJSON_AddArrayToObject(item, "reqSnpnList");
    if (req_snpn_listList == NULL) {
        ogs_error("OpenAPI_subscription_data_convertToJSON() failed [req_snpn_list]");
        goto end;
    }

    OpenAPI_lnode_t *req_snpn_list_node;
    if (subscription_data->req_snpn_list) {
        OpenAPI_list_for_each(subscription_data->req_snpn_list, req_snpn_list_node) {
            cJSON *itemLocal = OpenAPI_plmn_id_nid_convertToJSON(req_snpn_list_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_subscription_data_convertToJSON() failed [req_snpn_list]");
                goto end;
            }
            cJSON_AddItemToArray(req_snpn_listList, itemLocal);
        }
    }
    }

    if (subscription_data->serving_scope) {
    cJSON *serving_scope = cJSON_AddArrayToObject(item, "servingScope");
    if (serving_scope == NULL) {
        ogs_error("OpenAPI_subscription_data_convertToJSON() failed [serving_scope]");
        goto end;
    }

    OpenAPI_lnode_t *serving_scope_node;
    OpenAPI_list_for_each(subscription_data->serving_scope, serving_scope_node)  {
    if (cJSON_AddStringToObject(serving_scope, "", (char*)serving_scope_node->data) == NULL) {
        ogs_error("OpenAPI_subscription_data_convertToJSON() failed [serving_scope]");
        goto end;
    }
                    }
    }

end:
    return item;
}

OpenAPI_subscription_data_t *OpenAPI_subscription_data_parseFromJSON(cJSON *subscription_dataJSON)
{
    OpenAPI_subscription_data_t *subscription_data_local_var = NULL;
    cJSON *nf_status_notification_uri = cJSON_GetObjectItemCaseSensitive(subscription_dataJSON, "nfStatusNotificationUri");
    if (!nf_status_notification_uri) {
        ogs_error("OpenAPI_subscription_data_parseFromJSON() failed [nf_status_notification_uri]");
        goto end;
    }

    if (!cJSON_IsString(nf_status_notification_uri)) {
        ogs_error("OpenAPI_subscription_data_parseFromJSON() failed [nf_status_notification_uri]");
        goto end;
    }

    cJSON *req_nf_instance_id = cJSON_GetObjectItemCaseSensitive(subscription_dataJSON, "reqNfInstanceId");

    if (req_nf_instance_id) {
    if (!cJSON_IsString(req_nf_instance_id)) {
        ogs_error("OpenAPI_subscription_data_parseFromJSON() failed [req_nf_instance_id]");
        goto end;
    }
    }

    cJSON *subscr_cond = cJSON_GetObjectItemCaseSensitive(subscription_dataJSON, "subscrCond");

    OpenAPI_subscription_data_subscr_cond_t *subscr_cond_local_nonprim = NULL;
    if (subscr_cond) {
    subscr_cond_local_nonprim = OpenAPI_subscription_data_subscr_cond_parseFromJSON(subscr_cond);
    }

    cJSON *subscription_id = cJSON_GetObjectItemCaseSensitive(subscription_dataJSON, "subscriptionId");

    if (subscription_id) {
    if (!cJSON_IsString(subscription_id)) {
        ogs_error("OpenAPI_subscription_data_parseFromJSON() failed [subscription_id]");
        goto end;
    }
    }

    cJSON *validity_time = cJSON_GetObjectItemCaseSensitive(subscription_dataJSON, "validityTime");

    if (validity_time) {
    if (!cJSON_IsString(validity_time)) {
        ogs_error("OpenAPI_subscription_data_parseFromJSON() failed [validity_time]");
        goto end;
    }
    }

    cJSON *req_notif_events = cJSON_GetObjectItemCaseSensitive(subscription_dataJSON, "reqNotifEvents");

    OpenAPI_list_t *req_notif_eventsList;
    if (req_notif_events) {
    cJSON *req_notif_events_local_nonprimitive;
    if (!cJSON_IsArray(req_notif_events)) {
        ogs_error("OpenAPI_subscription_data_parseFromJSON() failed [req_notif_events]");
        goto end;
    }

    req_notif_eventsList = OpenAPI_list_create();

    cJSON_ArrayForEach(req_notif_events_local_nonprimitive, req_notif_events ) {
        if (!cJSON_IsString(req_notif_events_local_nonprimitive)){
            ogs_error("OpenAPI_subscription_data_parseFromJSON() failed [req_notif_events]");
            goto end;
        }

        OpenAPI_list_add(req_notif_eventsList, (void *)OpenAPI_notification_event_type_FromString(req_notif_events_local_nonprimitive->valuestring));
    }
    }

    cJSON *plmn_id = cJSON_GetObjectItemCaseSensitive(subscription_dataJSON, "plmnId");

    OpenAPI_plmn_id_t *plmn_id_local_nonprim = NULL;
    if (plmn_id) {
    plmn_id_local_nonprim = OpenAPI_plmn_id_parseFromJSON(plmn_id);
    }

    cJSON *nid = cJSON_GetObjectItemCaseSensitive(subscription_dataJSON, "nid");

    if (nid) {
    if (!cJSON_IsString(nid)) {
        ogs_error("OpenAPI_subscription_data_parseFromJSON() failed [nid]");
        goto end;
    }
    }

    cJSON *notif_condition = cJSON_GetObjectItemCaseSensitive(subscription_dataJSON, "notifCondition");

    OpenAPI_notif_condition_t *notif_condition_local_nonprim = NULL;
    if (notif_condition) {
    notif_condition_local_nonprim = OpenAPI_notif_condition_parseFromJSON(notif_condition);
    }

    cJSON *req_nf_type = cJSON_GetObjectItemCaseSensitive(subscription_dataJSON, "reqNfType");

    OpenAPI_nf_type_e req_nf_typeVariable;
    if (req_nf_type) {
    if (!cJSON_IsString(req_nf_type)) {
        ogs_error("OpenAPI_subscription_data_parseFromJSON() failed [req_nf_type]");
        goto end;
    }
    req_nf_typeVariable = OpenAPI_nf_type_FromString(req_nf_type->valuestring);
    }

    cJSON *req_nf_fqdn = cJSON_GetObjectItemCaseSensitive(subscription_dataJSON, "reqNfFqdn");

    if (req_nf_fqdn) {
    if (!cJSON_IsString(req_nf_fqdn)) {
        ogs_error("OpenAPI_subscription_data_parseFromJSON() failed [req_nf_fqdn]");
        goto end;
    }
    }

    cJSON *req_snssais = cJSON_GetObjectItemCaseSensitive(subscription_dataJSON, "reqSnssais");

    OpenAPI_list_t *req_snssaisList;
    if (req_snssais) {
    cJSON *req_snssais_local_nonprimitive;
    if (!cJSON_IsArray(req_snssais)){
        ogs_error("OpenAPI_subscription_data_parseFromJSON() failed [req_snssais]");
        goto end;
    }

    req_snssaisList = OpenAPI_list_create();

    cJSON_ArrayForEach(req_snssais_local_nonprimitive, req_snssais ) {
        if (!cJSON_IsObject(req_snssais_local_nonprimitive)) {
            ogs_error("OpenAPI_subscription_data_parseFromJSON() failed [req_snssais]");
            goto end;
        }
        OpenAPI_snssai_t *req_snssaisItem = OpenAPI_snssai_parseFromJSON(req_snssais_local_nonprimitive);

        if (!req_snssaisItem) {
            ogs_error("No req_snssaisItem");
            OpenAPI_list_free(req_snssaisList);
            goto end;
        }

        OpenAPI_list_add(req_snssaisList, req_snssaisItem);
    }
    }

    cJSON *req_per_plmn_snssais = cJSON_GetObjectItemCaseSensitive(subscription_dataJSON, "reqPerPlmnSnssais");

    OpenAPI_list_t *req_per_plmn_snssaisList;
    if (req_per_plmn_snssais) {
    cJSON *req_per_plmn_snssais_local_nonprimitive;
    if (!cJSON_IsArray(req_per_plmn_snssais)){
        ogs_error("OpenAPI_subscription_data_parseFromJSON() failed [req_per_plmn_snssais]");
        goto end;
    }

    req_per_plmn_snssaisList = OpenAPI_list_create();

    cJSON_ArrayForEach(req_per_plmn_snssais_local_nonprimitive, req_per_plmn_snssais ) {
        if (!cJSON_IsObject(req_per_plmn_snssais_local_nonprimitive)) {
            ogs_error("OpenAPI_subscription_data_parseFromJSON() failed [req_per_plmn_snssais]");
            goto end;
        }
        OpenAPI_plmn_snssai_t *req_per_plmn_snssaisItem = OpenAPI_plmn_snssai_parseFromJSON(req_per_plmn_snssais_local_nonprimitive);

        if (!req_per_plmn_snssaisItem) {
            ogs_error("No req_per_plmn_snssaisItem");
            OpenAPI_list_free(req_per_plmn_snssaisList);
            goto end;
        }

        OpenAPI_list_add(req_per_plmn_snssaisList, req_per_plmn_snssaisItem);
    }
    }

    cJSON *req_plmn_list = cJSON_GetObjectItemCaseSensitive(subscription_dataJSON, "reqPlmnList");

    OpenAPI_list_t *req_plmn_listList;
    if (req_plmn_list) {
    cJSON *req_plmn_list_local_nonprimitive;
    if (!cJSON_IsArray(req_plmn_list)){
        ogs_error("OpenAPI_subscription_data_parseFromJSON() failed [req_plmn_list]");
        goto end;
    }

    req_plmn_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(req_plmn_list_local_nonprimitive, req_plmn_list ) {
        if (!cJSON_IsObject(req_plmn_list_local_nonprimitive)) {
            ogs_error("OpenAPI_subscription_data_parseFromJSON() failed [req_plmn_list]");
            goto end;
        }
        OpenAPI_plmn_id_t *req_plmn_listItem = OpenAPI_plmn_id_parseFromJSON(req_plmn_list_local_nonprimitive);

        if (!req_plmn_listItem) {
            ogs_error("No req_plmn_listItem");
            OpenAPI_list_free(req_plmn_listList);
            goto end;
        }

        OpenAPI_list_add(req_plmn_listList, req_plmn_listItem);
    }
    }

    cJSON *req_snpn_list = cJSON_GetObjectItemCaseSensitive(subscription_dataJSON, "reqSnpnList");

    OpenAPI_list_t *req_snpn_listList;
    if (req_snpn_list) {
    cJSON *req_snpn_list_local_nonprimitive;
    if (!cJSON_IsArray(req_snpn_list)){
        ogs_error("OpenAPI_subscription_data_parseFromJSON() failed [req_snpn_list]");
        goto end;
    }

    req_snpn_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(req_snpn_list_local_nonprimitive, req_snpn_list ) {
        if (!cJSON_IsObject(req_snpn_list_local_nonprimitive)) {
            ogs_error("OpenAPI_subscription_data_parseFromJSON() failed [req_snpn_list]");
            goto end;
        }
        OpenAPI_plmn_id_nid_t *req_snpn_listItem = OpenAPI_plmn_id_nid_parseFromJSON(req_snpn_list_local_nonprimitive);

        if (!req_snpn_listItem) {
            ogs_error("No req_snpn_listItem");
            OpenAPI_list_free(req_snpn_listList);
            goto end;
        }

        OpenAPI_list_add(req_snpn_listList, req_snpn_listItem);
    }
    }

    cJSON *serving_scope = cJSON_GetObjectItemCaseSensitive(subscription_dataJSON, "servingScope");

    OpenAPI_list_t *serving_scopeList;
    if (serving_scope) {
    cJSON *serving_scope_local;
    if (!cJSON_IsArray(serving_scope)) {
        ogs_error("OpenAPI_subscription_data_parseFromJSON() failed [serving_scope]");
        goto end;
    }
    serving_scopeList = OpenAPI_list_create();

    cJSON_ArrayForEach(serving_scope_local, serving_scope) {
    if (!cJSON_IsString(serving_scope_local)) {
        ogs_error("OpenAPI_subscription_data_parseFromJSON() failed [serving_scope]");
        goto end;
    }
    OpenAPI_list_add(serving_scopeList , ogs_strdup(serving_scope_local->valuestring));
    }
    }

    subscription_data_local_var = OpenAPI_subscription_data_create (
        ogs_strdup(nf_status_notification_uri->valuestring),
        req_nf_instance_id ? ogs_strdup(req_nf_instance_id->valuestring) : NULL,
        subscr_cond ? subscr_cond_local_nonprim : NULL,
        subscription_id ? ogs_strdup(subscription_id->valuestring) : NULL,
        validity_time ? ogs_strdup(validity_time->valuestring) : NULL,
        req_notif_events ? req_notif_eventsList : NULL,
        plmn_id ? plmn_id_local_nonprim : NULL,
        nid ? ogs_strdup(nid->valuestring) : NULL,
        notif_condition ? notif_condition_local_nonprim : NULL,
        req_nf_type ? req_nf_typeVariable : 0,
        req_nf_fqdn ? ogs_strdup(req_nf_fqdn->valuestring) : NULL,
        req_snssais ? req_snssaisList : NULL,
        req_per_plmn_snssais ? req_per_plmn_snssaisList : NULL,
        req_plmn_list ? req_plmn_listList : NULL,
        req_snpn_list ? req_snpn_listList : NULL,
        serving_scope ? serving_scopeList : NULL
    );

    return subscription_data_local_var;
end:
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

