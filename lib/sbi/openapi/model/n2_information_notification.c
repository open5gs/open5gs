
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "n2_information_notification.h"

OpenAPI_n2_information_notification_t *OpenAPI_n2_information_notification_create(
    char *n2_notify_subscription_id,
    OpenAPI_n2_info_container_t *n2_info_container,
    OpenAPI_list_t *to_release_session_list,
    char *lcs_correlation_id,
    OpenAPI_n2_info_notify_reason_e notify_reason,
    OpenAPI_list_t *smf_change_info_list,
    OpenAPI_global_ran_node_id_t *ran_node_id,
    char *initial_amf_name,
    char *an_n2_ipv4_addr,
    char *an_n2_ipv6_addr,
    OpenAPI_guami_t *guami,
    bool is_notify_source_ng_ran,
    int notify_source_ng_ran
)
{
    OpenAPI_n2_information_notification_t *n2_information_notification_local_var = ogs_malloc(sizeof(OpenAPI_n2_information_notification_t));
    ogs_assert(n2_information_notification_local_var);

    n2_information_notification_local_var->n2_notify_subscription_id = n2_notify_subscription_id;
    n2_information_notification_local_var->n2_info_container = n2_info_container;
    n2_information_notification_local_var->to_release_session_list = to_release_session_list;
    n2_information_notification_local_var->lcs_correlation_id = lcs_correlation_id;
    n2_information_notification_local_var->notify_reason = notify_reason;
    n2_information_notification_local_var->smf_change_info_list = smf_change_info_list;
    n2_information_notification_local_var->ran_node_id = ran_node_id;
    n2_information_notification_local_var->initial_amf_name = initial_amf_name;
    n2_information_notification_local_var->an_n2_ipv4_addr = an_n2_ipv4_addr;
    n2_information_notification_local_var->an_n2_ipv6_addr = an_n2_ipv6_addr;
    n2_information_notification_local_var->guami = guami;
    n2_information_notification_local_var->is_notify_source_ng_ran = is_notify_source_ng_ran;
    n2_information_notification_local_var->notify_source_ng_ran = notify_source_ng_ran;

    return n2_information_notification_local_var;
}

void OpenAPI_n2_information_notification_free(OpenAPI_n2_information_notification_t *n2_information_notification)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == n2_information_notification) {
        return;
    }
    if (n2_information_notification->n2_notify_subscription_id) {
        ogs_free(n2_information_notification->n2_notify_subscription_id);
        n2_information_notification->n2_notify_subscription_id = NULL;
    }
    if (n2_information_notification->n2_info_container) {
        OpenAPI_n2_info_container_free(n2_information_notification->n2_info_container);
        n2_information_notification->n2_info_container = NULL;
    }
    if (n2_information_notification->to_release_session_list) {
        OpenAPI_list_for_each(n2_information_notification->to_release_session_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(n2_information_notification->to_release_session_list);
        n2_information_notification->to_release_session_list = NULL;
    }
    if (n2_information_notification->lcs_correlation_id) {
        ogs_free(n2_information_notification->lcs_correlation_id);
        n2_information_notification->lcs_correlation_id = NULL;
    }
    if (n2_information_notification->smf_change_info_list) {
        OpenAPI_list_for_each(n2_information_notification->smf_change_info_list, node) {
            OpenAPI_smf_change_info_free(node->data);
        }
        OpenAPI_list_free(n2_information_notification->smf_change_info_list);
        n2_information_notification->smf_change_info_list = NULL;
    }
    if (n2_information_notification->ran_node_id) {
        OpenAPI_global_ran_node_id_free(n2_information_notification->ran_node_id);
        n2_information_notification->ran_node_id = NULL;
    }
    if (n2_information_notification->initial_amf_name) {
        ogs_free(n2_information_notification->initial_amf_name);
        n2_information_notification->initial_amf_name = NULL;
    }
    if (n2_information_notification->an_n2_ipv4_addr) {
        ogs_free(n2_information_notification->an_n2_ipv4_addr);
        n2_information_notification->an_n2_ipv4_addr = NULL;
    }
    if (n2_information_notification->an_n2_ipv6_addr) {
        ogs_free(n2_information_notification->an_n2_ipv6_addr);
        n2_information_notification->an_n2_ipv6_addr = NULL;
    }
    if (n2_information_notification->guami) {
        OpenAPI_guami_free(n2_information_notification->guami);
        n2_information_notification->guami = NULL;
    }
    ogs_free(n2_information_notification);
}

cJSON *OpenAPI_n2_information_notification_convertToJSON(OpenAPI_n2_information_notification_t *n2_information_notification)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (n2_information_notification == NULL) {
        ogs_error("OpenAPI_n2_information_notification_convertToJSON() failed [N2InformationNotification]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!n2_information_notification->n2_notify_subscription_id) {
        ogs_error("OpenAPI_n2_information_notification_convertToJSON() failed [n2_notify_subscription_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "n2NotifySubscriptionId", n2_information_notification->n2_notify_subscription_id) == NULL) {
        ogs_error("OpenAPI_n2_information_notification_convertToJSON() failed [n2_notify_subscription_id]");
        goto end;
    }

    if (n2_information_notification->n2_info_container) {
    cJSON *n2_info_container_local_JSON = OpenAPI_n2_info_container_convertToJSON(n2_information_notification->n2_info_container);
    if (n2_info_container_local_JSON == NULL) {
        ogs_error("OpenAPI_n2_information_notification_convertToJSON() failed [n2_info_container]");
        goto end;
    }
    cJSON_AddItemToObject(item, "n2InfoContainer", n2_info_container_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_n2_information_notification_convertToJSON() failed [n2_info_container]");
        goto end;
    }
    }

    if (n2_information_notification->to_release_session_list) {
    cJSON *to_release_session_listList = cJSON_AddArrayToObject(item, "toReleaseSessionList");
    if (to_release_session_listList == NULL) {
        ogs_error("OpenAPI_n2_information_notification_convertToJSON() failed [to_release_session_list]");
        goto end;
    }
    OpenAPI_list_for_each(n2_information_notification->to_release_session_list, node) {
        if (node->data == NULL) {
            ogs_error("OpenAPI_n2_information_notification_convertToJSON() failed [to_release_session_list]");
            goto end;
        }
        if (cJSON_AddNumberToObject(to_release_session_listList, "", *(double *)node->data) == NULL) {
            ogs_error("OpenAPI_n2_information_notification_convertToJSON() failed [to_release_session_list]");
            goto end;
        }
    }
    }

    if (n2_information_notification->lcs_correlation_id) {
    if (cJSON_AddStringToObject(item, "lcsCorrelationId", n2_information_notification->lcs_correlation_id) == NULL) {
        ogs_error("OpenAPI_n2_information_notification_convertToJSON() failed [lcs_correlation_id]");
        goto end;
    }
    }

    if (n2_information_notification->notify_reason != OpenAPI_n2_info_notify_reason_NULL) {
    if (cJSON_AddStringToObject(item, "notifyReason", OpenAPI_n2_info_notify_reason_ToString(n2_information_notification->notify_reason)) == NULL) {
        ogs_error("OpenAPI_n2_information_notification_convertToJSON() failed [notify_reason]");
        goto end;
    }
    }

    if (n2_information_notification->smf_change_info_list) {
    cJSON *smf_change_info_listList = cJSON_AddArrayToObject(item, "smfChangeInfoList");
    if (smf_change_info_listList == NULL) {
        ogs_error("OpenAPI_n2_information_notification_convertToJSON() failed [smf_change_info_list]");
        goto end;
    }
    OpenAPI_list_for_each(n2_information_notification->smf_change_info_list, node) {
        cJSON *itemLocal = OpenAPI_smf_change_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_n2_information_notification_convertToJSON() failed [smf_change_info_list]");
            goto end;
        }
        cJSON_AddItemToArray(smf_change_info_listList, itemLocal);
    }
    }

    if (n2_information_notification->ran_node_id) {
    cJSON *ran_node_id_local_JSON = OpenAPI_global_ran_node_id_convertToJSON(n2_information_notification->ran_node_id);
    if (ran_node_id_local_JSON == NULL) {
        ogs_error("OpenAPI_n2_information_notification_convertToJSON() failed [ran_node_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ranNodeId", ran_node_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_n2_information_notification_convertToJSON() failed [ran_node_id]");
        goto end;
    }
    }

    if (n2_information_notification->initial_amf_name) {
    if (cJSON_AddStringToObject(item, "initialAmfName", n2_information_notification->initial_amf_name) == NULL) {
        ogs_error("OpenAPI_n2_information_notification_convertToJSON() failed [initial_amf_name]");
        goto end;
    }
    }

    if (n2_information_notification->an_n2_ipv4_addr) {
    if (cJSON_AddStringToObject(item, "anN2IPv4Addr", n2_information_notification->an_n2_ipv4_addr) == NULL) {
        ogs_error("OpenAPI_n2_information_notification_convertToJSON() failed [an_n2_ipv4_addr]");
        goto end;
    }
    }

    if (n2_information_notification->an_n2_ipv6_addr) {
    if (cJSON_AddStringToObject(item, "anN2IPv6Addr", n2_information_notification->an_n2_ipv6_addr) == NULL) {
        ogs_error("OpenAPI_n2_information_notification_convertToJSON() failed [an_n2_ipv6_addr]");
        goto end;
    }
    }

    if (n2_information_notification->guami) {
    cJSON *guami_local_JSON = OpenAPI_guami_convertToJSON(n2_information_notification->guami);
    if (guami_local_JSON == NULL) {
        ogs_error("OpenAPI_n2_information_notification_convertToJSON() failed [guami]");
        goto end;
    }
    cJSON_AddItemToObject(item, "guami", guami_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_n2_information_notification_convertToJSON() failed [guami]");
        goto end;
    }
    }

    if (n2_information_notification->is_notify_source_ng_ran) {
    if (cJSON_AddBoolToObject(item, "notifySourceNgRan", n2_information_notification->notify_source_ng_ran) == NULL) {
        ogs_error("OpenAPI_n2_information_notification_convertToJSON() failed [notify_source_ng_ran]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_n2_information_notification_t *OpenAPI_n2_information_notification_parseFromJSON(cJSON *n2_information_notificationJSON)
{
    OpenAPI_n2_information_notification_t *n2_information_notification_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *n2_notify_subscription_id = NULL;
    cJSON *n2_info_container = NULL;
    OpenAPI_n2_info_container_t *n2_info_container_local_nonprim = NULL;
    cJSON *to_release_session_list = NULL;
    OpenAPI_list_t *to_release_session_listList = NULL;
    cJSON *lcs_correlation_id = NULL;
    cJSON *notify_reason = NULL;
    OpenAPI_n2_info_notify_reason_e notify_reasonVariable = 0;
    cJSON *smf_change_info_list = NULL;
    OpenAPI_list_t *smf_change_info_listList = NULL;
    cJSON *ran_node_id = NULL;
    OpenAPI_global_ran_node_id_t *ran_node_id_local_nonprim = NULL;
    cJSON *initial_amf_name = NULL;
    cJSON *an_n2_ipv4_addr = NULL;
    cJSON *an_n2_ipv6_addr = NULL;
    cJSON *guami = NULL;
    OpenAPI_guami_t *guami_local_nonprim = NULL;
    cJSON *notify_source_ng_ran = NULL;
    n2_notify_subscription_id = cJSON_GetObjectItemCaseSensitive(n2_information_notificationJSON, "n2NotifySubscriptionId");
    if (!n2_notify_subscription_id) {
        ogs_error("OpenAPI_n2_information_notification_parseFromJSON() failed [n2_notify_subscription_id]");
        goto end;
    }
    if (!cJSON_IsString(n2_notify_subscription_id)) {
        ogs_error("OpenAPI_n2_information_notification_parseFromJSON() failed [n2_notify_subscription_id]");
        goto end;
    }

    n2_info_container = cJSON_GetObjectItemCaseSensitive(n2_information_notificationJSON, "n2InfoContainer");
    if (n2_info_container) {
    n2_info_container_local_nonprim = OpenAPI_n2_info_container_parseFromJSON(n2_info_container);
    if (!n2_info_container_local_nonprim) {
        ogs_error("OpenAPI_n2_info_container_parseFromJSON failed [n2_info_container]");
        goto end;
    }
    }

    to_release_session_list = cJSON_GetObjectItemCaseSensitive(n2_information_notificationJSON, "toReleaseSessionList");
    if (to_release_session_list) {
        cJSON *to_release_session_list_local = NULL;
        if (!cJSON_IsArray(to_release_session_list)) {
            ogs_error("OpenAPI_n2_information_notification_parseFromJSON() failed [to_release_session_list]");
            goto end;
        }

        to_release_session_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(to_release_session_list_local, to_release_session_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsNumber(to_release_session_list_local)) {
                ogs_error("OpenAPI_n2_information_notification_parseFromJSON() failed [to_release_session_list]");
                goto end;
            }
            localDouble = (double *)ogs_calloc(1, sizeof(double));
            if (!localDouble) {
                ogs_error("OpenAPI_n2_information_notification_parseFromJSON() failed [to_release_session_list]");
                goto end;
            }
            *localDouble = to_release_session_list_local->valuedouble;
            OpenAPI_list_add(to_release_session_listList, localDouble);
        }
    }

    lcs_correlation_id = cJSON_GetObjectItemCaseSensitive(n2_information_notificationJSON, "lcsCorrelationId");
    if (lcs_correlation_id) {
    if (!cJSON_IsString(lcs_correlation_id) && !cJSON_IsNull(lcs_correlation_id)) {
        ogs_error("OpenAPI_n2_information_notification_parseFromJSON() failed [lcs_correlation_id]");
        goto end;
    }
    }

    notify_reason = cJSON_GetObjectItemCaseSensitive(n2_information_notificationJSON, "notifyReason");
    if (notify_reason) {
    if (!cJSON_IsString(notify_reason)) {
        ogs_error("OpenAPI_n2_information_notification_parseFromJSON() failed [notify_reason]");
        goto end;
    }
    notify_reasonVariable = OpenAPI_n2_info_notify_reason_FromString(notify_reason->valuestring);
    }

    smf_change_info_list = cJSON_GetObjectItemCaseSensitive(n2_information_notificationJSON, "smfChangeInfoList");
    if (smf_change_info_list) {
        cJSON *smf_change_info_list_local = NULL;
        if (!cJSON_IsArray(smf_change_info_list)) {
            ogs_error("OpenAPI_n2_information_notification_parseFromJSON() failed [smf_change_info_list]");
            goto end;
        }

        smf_change_info_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(smf_change_info_list_local, smf_change_info_list) {
            if (!cJSON_IsObject(smf_change_info_list_local)) {
                ogs_error("OpenAPI_n2_information_notification_parseFromJSON() failed [smf_change_info_list]");
                goto end;
            }
            OpenAPI_smf_change_info_t *smf_change_info_listItem = OpenAPI_smf_change_info_parseFromJSON(smf_change_info_list_local);
            if (!smf_change_info_listItem) {
                ogs_error("No smf_change_info_listItem");
                goto end;
            }
            OpenAPI_list_add(smf_change_info_listList, smf_change_info_listItem);
        }
    }

    ran_node_id = cJSON_GetObjectItemCaseSensitive(n2_information_notificationJSON, "ranNodeId");
    if (ran_node_id) {
    ran_node_id_local_nonprim = OpenAPI_global_ran_node_id_parseFromJSON(ran_node_id);
    if (!ran_node_id_local_nonprim) {
        ogs_error("OpenAPI_global_ran_node_id_parseFromJSON failed [ran_node_id]");
        goto end;
    }
    }

    initial_amf_name = cJSON_GetObjectItemCaseSensitive(n2_information_notificationJSON, "initialAmfName");
    if (initial_amf_name) {
    if (!cJSON_IsString(initial_amf_name) && !cJSON_IsNull(initial_amf_name)) {
        ogs_error("OpenAPI_n2_information_notification_parseFromJSON() failed [initial_amf_name]");
        goto end;
    }
    }

    an_n2_ipv4_addr = cJSON_GetObjectItemCaseSensitive(n2_information_notificationJSON, "anN2IPv4Addr");
    if (an_n2_ipv4_addr) {
    if (!cJSON_IsString(an_n2_ipv4_addr) && !cJSON_IsNull(an_n2_ipv4_addr)) {
        ogs_error("OpenAPI_n2_information_notification_parseFromJSON() failed [an_n2_ipv4_addr]");
        goto end;
    }
    }

    an_n2_ipv6_addr = cJSON_GetObjectItemCaseSensitive(n2_information_notificationJSON, "anN2IPv6Addr");
    if (an_n2_ipv6_addr) {
    if (!cJSON_IsString(an_n2_ipv6_addr) && !cJSON_IsNull(an_n2_ipv6_addr)) {
        ogs_error("OpenAPI_n2_information_notification_parseFromJSON() failed [an_n2_ipv6_addr]");
        goto end;
    }
    }

    guami = cJSON_GetObjectItemCaseSensitive(n2_information_notificationJSON, "guami");
    if (guami) {
    guami_local_nonprim = OpenAPI_guami_parseFromJSON(guami);
    if (!guami_local_nonprim) {
        ogs_error("OpenAPI_guami_parseFromJSON failed [guami]");
        goto end;
    }
    }

    notify_source_ng_ran = cJSON_GetObjectItemCaseSensitive(n2_information_notificationJSON, "notifySourceNgRan");
    if (notify_source_ng_ran) {
    if (!cJSON_IsBool(notify_source_ng_ran)) {
        ogs_error("OpenAPI_n2_information_notification_parseFromJSON() failed [notify_source_ng_ran]");
        goto end;
    }
    }

    n2_information_notification_local_var = OpenAPI_n2_information_notification_create (
        ogs_strdup(n2_notify_subscription_id->valuestring),
        n2_info_container ? n2_info_container_local_nonprim : NULL,
        to_release_session_list ? to_release_session_listList : NULL,
        lcs_correlation_id && !cJSON_IsNull(lcs_correlation_id) ? ogs_strdup(lcs_correlation_id->valuestring) : NULL,
        notify_reason ? notify_reasonVariable : 0,
        smf_change_info_list ? smf_change_info_listList : NULL,
        ran_node_id ? ran_node_id_local_nonprim : NULL,
        initial_amf_name && !cJSON_IsNull(initial_amf_name) ? ogs_strdup(initial_amf_name->valuestring) : NULL,
        an_n2_ipv4_addr && !cJSON_IsNull(an_n2_ipv4_addr) ? ogs_strdup(an_n2_ipv4_addr->valuestring) : NULL,
        an_n2_ipv6_addr && !cJSON_IsNull(an_n2_ipv6_addr) ? ogs_strdup(an_n2_ipv6_addr->valuestring) : NULL,
        guami ? guami_local_nonprim : NULL,
        notify_source_ng_ran ? true : false,
        notify_source_ng_ran ? notify_source_ng_ran->valueint : 0
    );

    return n2_information_notification_local_var;
end:
    if (n2_info_container_local_nonprim) {
        OpenAPI_n2_info_container_free(n2_info_container_local_nonprim);
        n2_info_container_local_nonprim = NULL;
    }
    if (to_release_session_listList) {
        OpenAPI_list_for_each(to_release_session_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(to_release_session_listList);
        to_release_session_listList = NULL;
    }
    if (smf_change_info_listList) {
        OpenAPI_list_for_each(smf_change_info_listList, node) {
            OpenAPI_smf_change_info_free(node->data);
        }
        OpenAPI_list_free(smf_change_info_listList);
        smf_change_info_listList = NULL;
    }
    if (ran_node_id_local_nonprim) {
        OpenAPI_global_ran_node_id_free(ran_node_id_local_nonprim);
        ran_node_id_local_nonprim = NULL;
    }
    if (guami_local_nonprim) {
        OpenAPI_guami_free(guami_local_nonprim);
        guami_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_n2_information_notification_t *OpenAPI_n2_information_notification_copy(OpenAPI_n2_information_notification_t *dst, OpenAPI_n2_information_notification_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_n2_information_notification_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_n2_information_notification_convertToJSON() failed");
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

    OpenAPI_n2_information_notification_free(dst);
    dst = OpenAPI_n2_information_notification_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

