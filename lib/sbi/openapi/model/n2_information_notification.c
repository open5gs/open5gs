
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
    char *an_n2_i_pv4_addr,
    char *an_n2_i_pv6_addr
    )
{
    OpenAPI_n2_information_notification_t *n2_information_notification_local_var = OpenAPI_malloc(sizeof(OpenAPI_n2_information_notification_t));
    if (!n2_information_notification_local_var) {
        return NULL;
    }
    n2_information_notification_local_var->n2_notify_subscription_id = n2_notify_subscription_id;
    n2_information_notification_local_var->n2_info_container = n2_info_container;
    n2_information_notification_local_var->to_release_session_list = to_release_session_list;
    n2_information_notification_local_var->lcs_correlation_id = lcs_correlation_id;
    n2_information_notification_local_var->notify_reason = notify_reason;
    n2_information_notification_local_var->smf_change_info_list = smf_change_info_list;
    n2_information_notification_local_var->ran_node_id = ran_node_id;
    n2_information_notification_local_var->initial_amf_name = initial_amf_name;
    n2_information_notification_local_var->an_n2_i_pv4_addr = an_n2_i_pv4_addr;
    n2_information_notification_local_var->an_n2_i_pv6_addr = an_n2_i_pv6_addr;

    return n2_information_notification_local_var;
}

void OpenAPI_n2_information_notification_free(OpenAPI_n2_information_notification_t *n2_information_notification)
{
    if (NULL == n2_information_notification) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(n2_information_notification->n2_notify_subscription_id);
    OpenAPI_n2_info_container_free(n2_information_notification->n2_info_container);
    OpenAPI_list_for_each(n2_information_notification->to_release_session_list, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(n2_information_notification->to_release_session_list);
    ogs_free(n2_information_notification->lcs_correlation_id);
    OpenAPI_list_for_each(n2_information_notification->smf_change_info_list, node) {
        OpenAPI_smf_change_info_free(node->data);
    }
    OpenAPI_list_free(n2_information_notification->smf_change_info_list);
    OpenAPI_global_ran_node_id_free(n2_information_notification->ran_node_id);
    ogs_free(n2_information_notification->initial_amf_name);
    ogs_free(n2_information_notification->an_n2_i_pv4_addr);
    ogs_free(n2_information_notification->an_n2_i_pv6_addr);
    ogs_free(n2_information_notification);
}

cJSON *OpenAPI_n2_information_notification_convertToJSON(OpenAPI_n2_information_notification_t *n2_information_notification)
{
    cJSON *item = NULL;

    if (n2_information_notification == NULL) {
        ogs_error("OpenAPI_n2_information_notification_convertToJSON() failed [N2InformationNotification]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!n2_information_notification->n2_notify_subscription_id) {
        ogs_error("OpenAPI_n2_information_notification_convertToJSON() failed [n2_notify_subscription_id]");
        goto end;
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
        cJSON *to_release_session_list = cJSON_AddArrayToObject(item, "toReleaseSessionList");
        if (to_release_session_list == NULL) {
            ogs_error("OpenAPI_n2_information_notification_convertToJSON() failed [to_release_session_list]");
            goto end;
        }

        OpenAPI_lnode_t *to_release_session_list_node;
        OpenAPI_list_for_each(n2_information_notification->to_release_session_list, to_release_session_list_node)  {
            if (cJSON_AddNumberToObject(to_release_session_list, "", *(double *)to_release_session_list_node->data) == NULL) {
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

    if (n2_information_notification->notify_reason) {
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

        OpenAPI_lnode_t *smf_change_info_list_node;
        if (n2_information_notification->smf_change_info_list) {
            OpenAPI_list_for_each(n2_information_notification->smf_change_info_list, smf_change_info_list_node) {
                cJSON *itemLocal = OpenAPI_smf_change_info_convertToJSON(smf_change_info_list_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_n2_information_notification_convertToJSON() failed [smf_change_info_list]");
                    goto end;
                }
                cJSON_AddItemToArray(smf_change_info_listList, itemLocal);
            }
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

    if (n2_information_notification->an_n2_i_pv4_addr) {
        if (cJSON_AddStringToObject(item, "anN2IPv4Addr", n2_information_notification->an_n2_i_pv4_addr) == NULL) {
            ogs_error("OpenAPI_n2_information_notification_convertToJSON() failed [an_n2_i_pv4_addr]");
            goto end;
        }
    }

    if (n2_information_notification->an_n2_i_pv6_addr) {
        if (cJSON_AddStringToObject(item, "anN2IPv6Addr", n2_information_notification->an_n2_i_pv6_addr) == NULL) {
            ogs_error("OpenAPI_n2_information_notification_convertToJSON() failed [an_n2_i_pv6_addr]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_n2_information_notification_t *OpenAPI_n2_information_notification_parseFromJSON(cJSON *n2_information_notificationJSON)
{
    OpenAPI_n2_information_notification_t *n2_information_notification_local_var = NULL;
    cJSON *n2_notify_subscription_id = cJSON_GetObjectItemCaseSensitive(n2_information_notificationJSON, "n2NotifySubscriptionId");
    if (!n2_notify_subscription_id) {
        ogs_error("OpenAPI_n2_information_notification_parseFromJSON() failed [n2_notify_subscription_id]");
        goto end;
    }


    if (!cJSON_IsString(n2_notify_subscription_id)) {
        ogs_error("OpenAPI_n2_information_notification_parseFromJSON() failed [n2_notify_subscription_id]");
        goto end;
    }

    cJSON *n2_info_container = cJSON_GetObjectItemCaseSensitive(n2_information_notificationJSON, "n2InfoContainer");

    OpenAPI_n2_info_container_t *n2_info_container_local_nonprim = NULL;
    if (n2_info_container) {
        n2_info_container_local_nonprim = OpenAPI_n2_info_container_parseFromJSON(n2_info_container);
    }

    cJSON *to_release_session_list = cJSON_GetObjectItemCaseSensitive(n2_information_notificationJSON, "toReleaseSessionList");

    OpenAPI_list_t *to_release_session_listList;
    if (to_release_session_list) {
        cJSON *to_release_session_list_local;
        if (!cJSON_IsArray(to_release_session_list)) {
            ogs_error("OpenAPI_n2_information_notification_parseFromJSON() failed [to_release_session_list]");
            goto end;
        }
        to_release_session_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(to_release_session_list_local, to_release_session_list) {
            if (!cJSON_IsNumber(to_release_session_list_local)) {
                ogs_error("OpenAPI_n2_information_notification_parseFromJSON() failed [to_release_session_list]");
                goto end;
            }
            OpenAPI_list_add(to_release_session_listList, &to_release_session_list_local->valuedouble);
        }
    }

    cJSON *lcs_correlation_id = cJSON_GetObjectItemCaseSensitive(n2_information_notificationJSON, "lcsCorrelationId");

    if (lcs_correlation_id) {
        if (!cJSON_IsString(lcs_correlation_id)) {
            ogs_error("OpenAPI_n2_information_notification_parseFromJSON() failed [lcs_correlation_id]");
            goto end;
        }
    }

    cJSON *notify_reason = cJSON_GetObjectItemCaseSensitive(n2_information_notificationJSON, "notifyReason");

    OpenAPI_n2_info_notify_reason_e notify_reasonVariable;
    if (notify_reason) {
        if (!cJSON_IsString(notify_reason)) {
            ogs_error("OpenAPI_n2_information_notification_parseFromJSON() failed [notify_reason]");
            goto end;
        }
        notify_reasonVariable = OpenAPI_n2_info_notify_reason_FromString(notify_reason->valuestring);
    }

    cJSON *smf_change_info_list = cJSON_GetObjectItemCaseSensitive(n2_information_notificationJSON, "smfChangeInfoList");

    OpenAPI_list_t *smf_change_info_listList;
    if (smf_change_info_list) {
        cJSON *smf_change_info_list_local_nonprimitive;
        if (!cJSON_IsArray(smf_change_info_list)) {
            ogs_error("OpenAPI_n2_information_notification_parseFromJSON() failed [smf_change_info_list]");
            goto end;
        }

        smf_change_info_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(smf_change_info_list_local_nonprimitive, smf_change_info_list ) {
            if (!cJSON_IsObject(smf_change_info_list_local_nonprimitive)) {
                ogs_error("OpenAPI_n2_information_notification_parseFromJSON() failed [smf_change_info_list]");
                goto end;
            }
            OpenAPI_smf_change_info_t *smf_change_info_listItem = OpenAPI_smf_change_info_parseFromJSON(smf_change_info_list_local_nonprimitive);

            OpenAPI_list_add(smf_change_info_listList, smf_change_info_listItem);
        }
    }

    cJSON *ran_node_id = cJSON_GetObjectItemCaseSensitive(n2_information_notificationJSON, "ranNodeId");

    OpenAPI_global_ran_node_id_t *ran_node_id_local_nonprim = NULL;
    if (ran_node_id) {
        ran_node_id_local_nonprim = OpenAPI_global_ran_node_id_parseFromJSON(ran_node_id);
    }

    cJSON *initial_amf_name = cJSON_GetObjectItemCaseSensitive(n2_information_notificationJSON, "initialAmfName");

    if (initial_amf_name) {
        if (!cJSON_IsString(initial_amf_name)) {
            ogs_error("OpenAPI_n2_information_notification_parseFromJSON() failed [initial_amf_name]");
            goto end;
        }
    }

    cJSON *an_n2_i_pv4_addr = cJSON_GetObjectItemCaseSensitive(n2_information_notificationJSON, "anN2IPv4Addr");

    if (an_n2_i_pv4_addr) {
        if (!cJSON_IsString(an_n2_i_pv4_addr)) {
            ogs_error("OpenAPI_n2_information_notification_parseFromJSON() failed [an_n2_i_pv4_addr]");
            goto end;
        }
    }

    cJSON *an_n2_i_pv6_addr = cJSON_GetObjectItemCaseSensitive(n2_information_notificationJSON, "anN2IPv6Addr");

    if (an_n2_i_pv6_addr) {
        if (!cJSON_IsString(an_n2_i_pv6_addr)) {
            ogs_error("OpenAPI_n2_information_notification_parseFromJSON() failed [an_n2_i_pv6_addr]");
            goto end;
        }
    }

    n2_information_notification_local_var = OpenAPI_n2_information_notification_create (
        ogs_strdup(n2_notify_subscription_id->valuestring),
        n2_info_container ? n2_info_container_local_nonprim : NULL,
        to_release_session_list ? to_release_session_listList : NULL,
        lcs_correlation_id ? ogs_strdup(lcs_correlation_id->valuestring) : NULL,
        notify_reason ? notify_reasonVariable : 0,
        smf_change_info_list ? smf_change_info_listList : NULL,
        ran_node_id ? ran_node_id_local_nonprim : NULL,
        initial_amf_name ? ogs_strdup(initial_amf_name->valuestring) : NULL,
        an_n2_i_pv4_addr ? ogs_strdup(an_n2_i_pv4_addr->valuestring) : NULL,
        an_n2_i_pv6_addr ? ogs_strdup(an_n2_i_pv6_addr->valuestring) : NULL
        );

    return n2_information_notification_local_var;
end:
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

