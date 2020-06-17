
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "amf_event_subscription.h"

OpenAPI_amf_event_subscription_t *OpenAPI_amf_event_subscription_create(
    OpenAPI_list_t *event_list,
    char *event_notify_uri,
    char *notify_correlation_id,
    char *nf_id,
    char *subs_change_notify_uri,
    char *subs_change_notify_correlation_id,
    char *supi,
    char *group_id,
    char *gpsi,
    char *pei,
    int any_ue,
    OpenAPI_amf_event_mode_t *options
    )
{
    OpenAPI_amf_event_subscription_t *amf_event_subscription_local_var = OpenAPI_malloc(sizeof(OpenAPI_amf_event_subscription_t));
    if (!amf_event_subscription_local_var) {
        return NULL;
    }
    amf_event_subscription_local_var->event_list = event_list;
    amf_event_subscription_local_var->event_notify_uri = event_notify_uri;
    amf_event_subscription_local_var->notify_correlation_id = notify_correlation_id;
    amf_event_subscription_local_var->nf_id = nf_id;
    amf_event_subscription_local_var->subs_change_notify_uri = subs_change_notify_uri;
    amf_event_subscription_local_var->subs_change_notify_correlation_id = subs_change_notify_correlation_id;
    amf_event_subscription_local_var->supi = supi;
    amf_event_subscription_local_var->group_id = group_id;
    amf_event_subscription_local_var->gpsi = gpsi;
    amf_event_subscription_local_var->pei = pei;
    amf_event_subscription_local_var->any_ue = any_ue;
    amf_event_subscription_local_var->options = options;

    return amf_event_subscription_local_var;
}

void OpenAPI_amf_event_subscription_free(OpenAPI_amf_event_subscription_t *amf_event_subscription)
{
    if (NULL == amf_event_subscription) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(amf_event_subscription->event_list, node) {
        OpenAPI_amf_event_free(node->data);
    }
    OpenAPI_list_free(amf_event_subscription->event_list);
    ogs_free(amf_event_subscription->event_notify_uri);
    ogs_free(amf_event_subscription->notify_correlation_id);
    ogs_free(amf_event_subscription->nf_id);
    ogs_free(amf_event_subscription->subs_change_notify_uri);
    ogs_free(amf_event_subscription->subs_change_notify_correlation_id);
    ogs_free(amf_event_subscription->supi);
    ogs_free(amf_event_subscription->group_id);
    ogs_free(amf_event_subscription->gpsi);
    ogs_free(amf_event_subscription->pei);
    OpenAPI_amf_event_mode_free(amf_event_subscription->options);
    ogs_free(amf_event_subscription);
}

cJSON *OpenAPI_amf_event_subscription_convertToJSON(OpenAPI_amf_event_subscription_t *amf_event_subscription)
{
    cJSON *item = NULL;

    if (amf_event_subscription == NULL) {
        ogs_error("OpenAPI_amf_event_subscription_convertToJSON() failed [AmfEventSubscription]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!amf_event_subscription->event_list) {
        ogs_error("OpenAPI_amf_event_subscription_convertToJSON() failed [event_list]");
        goto end;
    }
    cJSON *event_listList = cJSON_AddArrayToObject(item, "eventList");
    if (event_listList == NULL) {
        ogs_error("OpenAPI_amf_event_subscription_convertToJSON() failed [event_list]");
        goto end;
    }

    OpenAPI_lnode_t *event_list_node;
    if (amf_event_subscription->event_list) {
        OpenAPI_list_for_each(amf_event_subscription->event_list, event_list_node) {
            cJSON *itemLocal = OpenAPI_amf_event_convertToJSON(event_list_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_amf_event_subscription_convertToJSON() failed [event_list]");
                goto end;
            }
            cJSON_AddItemToArray(event_listList, itemLocal);
        }
    }

    if (!amf_event_subscription->event_notify_uri) {
        ogs_error("OpenAPI_amf_event_subscription_convertToJSON() failed [event_notify_uri]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "eventNotifyUri", amf_event_subscription->event_notify_uri) == NULL) {
        ogs_error("OpenAPI_amf_event_subscription_convertToJSON() failed [event_notify_uri]");
        goto end;
    }

    if (!amf_event_subscription->notify_correlation_id) {
        ogs_error("OpenAPI_amf_event_subscription_convertToJSON() failed [notify_correlation_id]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "notifyCorrelationId", amf_event_subscription->notify_correlation_id) == NULL) {
        ogs_error("OpenAPI_amf_event_subscription_convertToJSON() failed [notify_correlation_id]");
        goto end;
    }

    if (!amf_event_subscription->nf_id) {
        ogs_error("OpenAPI_amf_event_subscription_convertToJSON() failed [nf_id]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "nfId", amf_event_subscription->nf_id) == NULL) {
        ogs_error("OpenAPI_amf_event_subscription_convertToJSON() failed [nf_id]");
        goto end;
    }

    if (amf_event_subscription->subs_change_notify_uri) {
        if (cJSON_AddStringToObject(item, "subsChangeNotifyUri", amf_event_subscription->subs_change_notify_uri) == NULL) {
            ogs_error("OpenAPI_amf_event_subscription_convertToJSON() failed [subs_change_notify_uri]");
            goto end;
        }
    }

    if (amf_event_subscription->subs_change_notify_correlation_id) {
        if (cJSON_AddStringToObject(item, "subsChangeNotifyCorrelationId", amf_event_subscription->subs_change_notify_correlation_id) == NULL) {
            ogs_error("OpenAPI_amf_event_subscription_convertToJSON() failed [subs_change_notify_correlation_id]");
            goto end;
        }
    }

    if (amf_event_subscription->supi) {
        if (cJSON_AddStringToObject(item, "supi", amf_event_subscription->supi) == NULL) {
            ogs_error("OpenAPI_amf_event_subscription_convertToJSON() failed [supi]");
            goto end;
        }
    }

    if (amf_event_subscription->group_id) {
        if (cJSON_AddStringToObject(item, "groupId", amf_event_subscription->group_id) == NULL) {
            ogs_error("OpenAPI_amf_event_subscription_convertToJSON() failed [group_id]");
            goto end;
        }
    }

    if (amf_event_subscription->gpsi) {
        if (cJSON_AddStringToObject(item, "gpsi", amf_event_subscription->gpsi) == NULL) {
            ogs_error("OpenAPI_amf_event_subscription_convertToJSON() failed [gpsi]");
            goto end;
        }
    }

    if (amf_event_subscription->pei) {
        if (cJSON_AddStringToObject(item, "pei", amf_event_subscription->pei) == NULL) {
            ogs_error("OpenAPI_amf_event_subscription_convertToJSON() failed [pei]");
            goto end;
        }
    }

    if (amf_event_subscription->any_ue) {
        if (cJSON_AddBoolToObject(item, "anyUE", amf_event_subscription->any_ue) == NULL) {
            ogs_error("OpenAPI_amf_event_subscription_convertToJSON() failed [any_ue]");
            goto end;
        }
    }

    if (amf_event_subscription->options) {
        cJSON *options_local_JSON = OpenAPI_amf_event_mode_convertToJSON(amf_event_subscription->options);
        if (options_local_JSON == NULL) {
            ogs_error("OpenAPI_amf_event_subscription_convertToJSON() failed [options]");
            goto end;
        }
        cJSON_AddItemToObject(item, "options", options_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_amf_event_subscription_convertToJSON() failed [options]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_amf_event_subscription_t *OpenAPI_amf_event_subscription_parseFromJSON(cJSON *amf_event_subscriptionJSON)
{
    OpenAPI_amf_event_subscription_t *amf_event_subscription_local_var = NULL;
    cJSON *event_list = cJSON_GetObjectItemCaseSensitive(amf_event_subscriptionJSON, "eventList");
    if (!event_list) {
        ogs_error("OpenAPI_amf_event_subscription_parseFromJSON() failed [event_list]");
        goto end;
    }

    OpenAPI_list_t *event_listList;

    cJSON *event_list_local_nonprimitive;
    if (!cJSON_IsArray(event_list)) {
        ogs_error("OpenAPI_amf_event_subscription_parseFromJSON() failed [event_list]");
        goto end;
    }

    event_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(event_list_local_nonprimitive, event_list ) {
        if (!cJSON_IsObject(event_list_local_nonprimitive)) {
            ogs_error("OpenAPI_amf_event_subscription_parseFromJSON() failed [event_list]");
            goto end;
        }
        OpenAPI_amf_event_t *event_listItem = OpenAPI_amf_event_parseFromJSON(event_list_local_nonprimitive);

        OpenAPI_list_add(event_listList, event_listItem);
    }

    cJSON *event_notify_uri = cJSON_GetObjectItemCaseSensitive(amf_event_subscriptionJSON, "eventNotifyUri");
    if (!event_notify_uri) {
        ogs_error("OpenAPI_amf_event_subscription_parseFromJSON() failed [event_notify_uri]");
        goto end;
    }


    if (!cJSON_IsString(event_notify_uri)) {
        ogs_error("OpenAPI_amf_event_subscription_parseFromJSON() failed [event_notify_uri]");
        goto end;
    }

    cJSON *notify_correlation_id = cJSON_GetObjectItemCaseSensitive(amf_event_subscriptionJSON, "notifyCorrelationId");
    if (!notify_correlation_id) {
        ogs_error("OpenAPI_amf_event_subscription_parseFromJSON() failed [notify_correlation_id]");
        goto end;
    }


    if (!cJSON_IsString(notify_correlation_id)) {
        ogs_error("OpenAPI_amf_event_subscription_parseFromJSON() failed [notify_correlation_id]");
        goto end;
    }

    cJSON *nf_id = cJSON_GetObjectItemCaseSensitive(amf_event_subscriptionJSON, "nfId");
    if (!nf_id) {
        ogs_error("OpenAPI_amf_event_subscription_parseFromJSON() failed [nf_id]");
        goto end;
    }


    if (!cJSON_IsString(nf_id)) {
        ogs_error("OpenAPI_amf_event_subscription_parseFromJSON() failed [nf_id]");
        goto end;
    }

    cJSON *subs_change_notify_uri = cJSON_GetObjectItemCaseSensitive(amf_event_subscriptionJSON, "subsChangeNotifyUri");

    if (subs_change_notify_uri) {
        if (!cJSON_IsString(subs_change_notify_uri)) {
            ogs_error("OpenAPI_amf_event_subscription_parseFromJSON() failed [subs_change_notify_uri]");
            goto end;
        }
    }

    cJSON *subs_change_notify_correlation_id = cJSON_GetObjectItemCaseSensitive(amf_event_subscriptionJSON, "subsChangeNotifyCorrelationId");

    if (subs_change_notify_correlation_id) {
        if (!cJSON_IsString(subs_change_notify_correlation_id)) {
            ogs_error("OpenAPI_amf_event_subscription_parseFromJSON() failed [subs_change_notify_correlation_id]");
            goto end;
        }
    }

    cJSON *supi = cJSON_GetObjectItemCaseSensitive(amf_event_subscriptionJSON, "supi");

    if (supi) {
        if (!cJSON_IsString(supi)) {
            ogs_error("OpenAPI_amf_event_subscription_parseFromJSON() failed [supi]");
            goto end;
        }
    }

    cJSON *group_id = cJSON_GetObjectItemCaseSensitive(amf_event_subscriptionJSON, "groupId");

    if (group_id) {
        if (!cJSON_IsString(group_id)) {
            ogs_error("OpenAPI_amf_event_subscription_parseFromJSON() failed [group_id]");
            goto end;
        }
    }

    cJSON *gpsi = cJSON_GetObjectItemCaseSensitive(amf_event_subscriptionJSON, "gpsi");

    if (gpsi) {
        if (!cJSON_IsString(gpsi)) {
            ogs_error("OpenAPI_amf_event_subscription_parseFromJSON() failed [gpsi]");
            goto end;
        }
    }

    cJSON *pei = cJSON_GetObjectItemCaseSensitive(amf_event_subscriptionJSON, "pei");

    if (pei) {
        if (!cJSON_IsString(pei)) {
            ogs_error("OpenAPI_amf_event_subscription_parseFromJSON() failed [pei]");
            goto end;
        }
    }

    cJSON *any_ue = cJSON_GetObjectItemCaseSensitive(amf_event_subscriptionJSON, "anyUE");

    if (any_ue) {
        if (!cJSON_IsBool(any_ue)) {
            ogs_error("OpenAPI_amf_event_subscription_parseFromJSON() failed [any_ue]");
            goto end;
        }
    }

    cJSON *options = cJSON_GetObjectItemCaseSensitive(amf_event_subscriptionJSON, "options");

    OpenAPI_amf_event_mode_t *options_local_nonprim = NULL;
    if (options) {
        options_local_nonprim = OpenAPI_amf_event_mode_parseFromJSON(options);
    }

    amf_event_subscription_local_var = OpenAPI_amf_event_subscription_create (
        event_listList,
        ogs_strdup(event_notify_uri->valuestring),
        ogs_strdup(notify_correlation_id->valuestring),
        ogs_strdup(nf_id->valuestring),
        subs_change_notify_uri ? ogs_strdup(subs_change_notify_uri->valuestring) : NULL,
        subs_change_notify_correlation_id ? ogs_strdup(subs_change_notify_correlation_id->valuestring) : NULL,
        supi ? ogs_strdup(supi->valuestring) : NULL,
        group_id ? ogs_strdup(group_id->valuestring) : NULL,
        gpsi ? ogs_strdup(gpsi->valuestring) : NULL,
        pei ? ogs_strdup(pei->valuestring) : NULL,
        any_ue ? any_ue->valueint : 0,
        options ? options_local_nonprim : NULL
        );

    return amf_event_subscription_local_var;
end:
    return NULL;
}

OpenAPI_amf_event_subscription_t *OpenAPI_amf_event_subscription_copy(OpenAPI_amf_event_subscription_t *dst, OpenAPI_amf_event_subscription_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_amf_event_subscription_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_amf_event_subscription_convertToJSON() failed");
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

    OpenAPI_amf_event_subscription_free(dst);
    dst = OpenAPI_amf_event_subscription_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

