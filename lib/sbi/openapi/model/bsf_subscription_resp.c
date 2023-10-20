
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "bsf_subscription_resp.h"

OpenAPI_bsf_subscription_resp_t *OpenAPI_bsf_subscription_resp_create(
    OpenAPI_list_t *events,
    char *notif_uri,
    char *notif_corre_id,
    char *supi,
    char *gpsi,
    OpenAPI_snssai_dnn_pair_t *snssai_dnn_pairs,
    OpenAPI_list_t *add_snssai_dnn_pairs,
    char *supp_feat,
    char *pcf_id,
    char *pcf_set_id,
    OpenAPI_binding_level_e bind_level,
    OpenAPI_list_t *event_notifs
)
{
    OpenAPI_bsf_subscription_resp_t *bsf_subscription_resp_local_var = ogs_malloc(sizeof(OpenAPI_bsf_subscription_resp_t));
    ogs_assert(bsf_subscription_resp_local_var);

    bsf_subscription_resp_local_var->events = events;
    bsf_subscription_resp_local_var->notif_uri = notif_uri;
    bsf_subscription_resp_local_var->notif_corre_id = notif_corre_id;
    bsf_subscription_resp_local_var->supi = supi;
    bsf_subscription_resp_local_var->gpsi = gpsi;
    bsf_subscription_resp_local_var->snssai_dnn_pairs = snssai_dnn_pairs;
    bsf_subscription_resp_local_var->add_snssai_dnn_pairs = add_snssai_dnn_pairs;
    bsf_subscription_resp_local_var->supp_feat = supp_feat;
    bsf_subscription_resp_local_var->pcf_id = pcf_id;
    bsf_subscription_resp_local_var->pcf_set_id = pcf_set_id;
    bsf_subscription_resp_local_var->bind_level = bind_level;
    bsf_subscription_resp_local_var->event_notifs = event_notifs;

    return bsf_subscription_resp_local_var;
}

void OpenAPI_bsf_subscription_resp_free(OpenAPI_bsf_subscription_resp_t *bsf_subscription_resp)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == bsf_subscription_resp) {
        return;
    }
    if (bsf_subscription_resp->events) {
        OpenAPI_list_free(bsf_subscription_resp->events);
        bsf_subscription_resp->events = NULL;
    }
    if (bsf_subscription_resp->notif_uri) {
        ogs_free(bsf_subscription_resp->notif_uri);
        bsf_subscription_resp->notif_uri = NULL;
    }
    if (bsf_subscription_resp->notif_corre_id) {
        ogs_free(bsf_subscription_resp->notif_corre_id);
        bsf_subscription_resp->notif_corre_id = NULL;
    }
    if (bsf_subscription_resp->supi) {
        ogs_free(bsf_subscription_resp->supi);
        bsf_subscription_resp->supi = NULL;
    }
    if (bsf_subscription_resp->gpsi) {
        ogs_free(bsf_subscription_resp->gpsi);
        bsf_subscription_resp->gpsi = NULL;
    }
    if (bsf_subscription_resp->snssai_dnn_pairs) {
        OpenAPI_snssai_dnn_pair_free(bsf_subscription_resp->snssai_dnn_pairs);
        bsf_subscription_resp->snssai_dnn_pairs = NULL;
    }
    if (bsf_subscription_resp->add_snssai_dnn_pairs) {
        OpenAPI_list_for_each(bsf_subscription_resp->add_snssai_dnn_pairs, node) {
            OpenAPI_snssai_dnn_pair_free(node->data);
        }
        OpenAPI_list_free(bsf_subscription_resp->add_snssai_dnn_pairs);
        bsf_subscription_resp->add_snssai_dnn_pairs = NULL;
    }
    if (bsf_subscription_resp->supp_feat) {
        ogs_free(bsf_subscription_resp->supp_feat);
        bsf_subscription_resp->supp_feat = NULL;
    }
    if (bsf_subscription_resp->pcf_id) {
        ogs_free(bsf_subscription_resp->pcf_id);
        bsf_subscription_resp->pcf_id = NULL;
    }
    if (bsf_subscription_resp->pcf_set_id) {
        ogs_free(bsf_subscription_resp->pcf_set_id);
        bsf_subscription_resp->pcf_set_id = NULL;
    }
    if (bsf_subscription_resp->event_notifs) {
        OpenAPI_list_for_each(bsf_subscription_resp->event_notifs, node) {
            OpenAPI_bsf_event_notification_free(node->data);
        }
        OpenAPI_list_free(bsf_subscription_resp->event_notifs);
        bsf_subscription_resp->event_notifs = NULL;
    }
    ogs_free(bsf_subscription_resp);
}

cJSON *OpenAPI_bsf_subscription_resp_convertToJSON(OpenAPI_bsf_subscription_resp_t *bsf_subscription_resp)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (bsf_subscription_resp == NULL) {
        ogs_error("OpenAPI_bsf_subscription_resp_convertToJSON() failed [BsfSubscriptionResp]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (bsf_subscription_resp->events == OpenAPI_bsf_event_NULL) {
        ogs_error("OpenAPI_bsf_subscription_resp_convertToJSON() failed [events]");
        return NULL;
    }
    cJSON *eventsList = cJSON_AddArrayToObject(item, "events");
    if (eventsList == NULL) {
        ogs_error("OpenAPI_bsf_subscription_resp_convertToJSON() failed [events]");
        goto end;
    }
    OpenAPI_list_for_each(bsf_subscription_resp->events, node) {
        if (cJSON_AddStringToObject(eventsList, "", OpenAPI_bsf_event_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_bsf_subscription_resp_convertToJSON() failed [events]");
            goto end;
        }
    }

    if (!bsf_subscription_resp->notif_uri) {
        ogs_error("OpenAPI_bsf_subscription_resp_convertToJSON() failed [notif_uri]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "notifUri", bsf_subscription_resp->notif_uri) == NULL) {
        ogs_error("OpenAPI_bsf_subscription_resp_convertToJSON() failed [notif_uri]");
        goto end;
    }

    if (!bsf_subscription_resp->notif_corre_id) {
        ogs_error("OpenAPI_bsf_subscription_resp_convertToJSON() failed [notif_corre_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "notifCorreId", bsf_subscription_resp->notif_corre_id) == NULL) {
        ogs_error("OpenAPI_bsf_subscription_resp_convertToJSON() failed [notif_corre_id]");
        goto end;
    }

    if (!bsf_subscription_resp->supi) {
        ogs_error("OpenAPI_bsf_subscription_resp_convertToJSON() failed [supi]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "supi", bsf_subscription_resp->supi) == NULL) {
        ogs_error("OpenAPI_bsf_subscription_resp_convertToJSON() failed [supi]");
        goto end;
    }

    if (bsf_subscription_resp->gpsi) {
    if (cJSON_AddStringToObject(item, "gpsi", bsf_subscription_resp->gpsi) == NULL) {
        ogs_error("OpenAPI_bsf_subscription_resp_convertToJSON() failed [gpsi]");
        goto end;
    }
    }

    if (bsf_subscription_resp->snssai_dnn_pairs) {
    cJSON *snssai_dnn_pairs_local_JSON = OpenAPI_snssai_dnn_pair_convertToJSON(bsf_subscription_resp->snssai_dnn_pairs);
    if (snssai_dnn_pairs_local_JSON == NULL) {
        ogs_error("OpenAPI_bsf_subscription_resp_convertToJSON() failed [snssai_dnn_pairs]");
        goto end;
    }
    cJSON_AddItemToObject(item, "snssaiDnnPairs", snssai_dnn_pairs_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_bsf_subscription_resp_convertToJSON() failed [snssai_dnn_pairs]");
        goto end;
    }
    }

    if (bsf_subscription_resp->add_snssai_dnn_pairs) {
    cJSON *add_snssai_dnn_pairsList = cJSON_AddArrayToObject(item, "addSnssaiDnnPairs");
    if (add_snssai_dnn_pairsList == NULL) {
        ogs_error("OpenAPI_bsf_subscription_resp_convertToJSON() failed [add_snssai_dnn_pairs]");
        goto end;
    }
    OpenAPI_list_for_each(bsf_subscription_resp->add_snssai_dnn_pairs, node) {
        cJSON *itemLocal = OpenAPI_snssai_dnn_pair_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_bsf_subscription_resp_convertToJSON() failed [add_snssai_dnn_pairs]");
            goto end;
        }
        cJSON_AddItemToArray(add_snssai_dnn_pairsList, itemLocal);
    }
    }

    if (bsf_subscription_resp->supp_feat) {
    if (cJSON_AddStringToObject(item, "suppFeat", bsf_subscription_resp->supp_feat) == NULL) {
        ogs_error("OpenAPI_bsf_subscription_resp_convertToJSON() failed [supp_feat]");
        goto end;
    }
    }

    if (bsf_subscription_resp->pcf_id) {
    if (cJSON_AddStringToObject(item, "pcfId", bsf_subscription_resp->pcf_id) == NULL) {
        ogs_error("OpenAPI_bsf_subscription_resp_convertToJSON() failed [pcf_id]");
        goto end;
    }
    }

    if (bsf_subscription_resp->pcf_set_id) {
    if (cJSON_AddStringToObject(item, "pcfSetId", bsf_subscription_resp->pcf_set_id) == NULL) {
        ogs_error("OpenAPI_bsf_subscription_resp_convertToJSON() failed [pcf_set_id]");
        goto end;
    }
    }

    if (bsf_subscription_resp->bind_level != OpenAPI_binding_level_NULL) {
    if (cJSON_AddStringToObject(item, "bindLevel", OpenAPI_binding_level_ToString(bsf_subscription_resp->bind_level)) == NULL) {
        ogs_error("OpenAPI_bsf_subscription_resp_convertToJSON() failed [bind_level]");
        goto end;
    }
    }

    if (!bsf_subscription_resp->event_notifs) {
        ogs_error("OpenAPI_bsf_subscription_resp_convertToJSON() failed [event_notifs]");
        return NULL;
    }
    cJSON *event_notifsList = cJSON_AddArrayToObject(item, "eventNotifs");
    if (event_notifsList == NULL) {
        ogs_error("OpenAPI_bsf_subscription_resp_convertToJSON() failed [event_notifs]");
        goto end;
    }
    OpenAPI_list_for_each(bsf_subscription_resp->event_notifs, node) {
        cJSON *itemLocal = OpenAPI_bsf_event_notification_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_bsf_subscription_resp_convertToJSON() failed [event_notifs]");
            goto end;
        }
        cJSON_AddItemToArray(event_notifsList, itemLocal);
    }

end:
    return item;
}

OpenAPI_bsf_subscription_resp_t *OpenAPI_bsf_subscription_resp_parseFromJSON(cJSON *bsf_subscription_respJSON)
{
    OpenAPI_bsf_subscription_resp_t *bsf_subscription_resp_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *events = NULL;
    OpenAPI_list_t *eventsList = NULL;
    cJSON *notif_uri = NULL;
    cJSON *notif_corre_id = NULL;
    cJSON *supi = NULL;
    cJSON *gpsi = NULL;
    cJSON *snssai_dnn_pairs = NULL;
    OpenAPI_snssai_dnn_pair_t *snssai_dnn_pairs_local_nonprim = NULL;
    cJSON *add_snssai_dnn_pairs = NULL;
    OpenAPI_list_t *add_snssai_dnn_pairsList = NULL;
    cJSON *supp_feat = NULL;
    cJSON *pcf_id = NULL;
    cJSON *pcf_set_id = NULL;
    cJSON *bind_level = NULL;
    OpenAPI_binding_level_e bind_levelVariable = 0;
    cJSON *event_notifs = NULL;
    OpenAPI_list_t *event_notifsList = NULL;
    events = cJSON_GetObjectItemCaseSensitive(bsf_subscription_respJSON, "events");
    if (!events) {
        ogs_error("OpenAPI_bsf_subscription_resp_parseFromJSON() failed [events]");
        goto end;
    }
        cJSON *events_local = NULL;
        if (!cJSON_IsArray(events)) {
            ogs_error("OpenAPI_bsf_subscription_resp_parseFromJSON() failed [events]");
            goto end;
        }

        eventsList = OpenAPI_list_create();

        cJSON_ArrayForEach(events_local, events) {
            OpenAPI_bsf_event_e localEnum = OpenAPI_bsf_event_NULL;
            if (!cJSON_IsString(events_local)) {
                ogs_error("OpenAPI_bsf_subscription_resp_parseFromJSON() failed [events]");
                goto end;
            }
            localEnum = OpenAPI_bsf_event_FromString(events_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"events\" is not supported. Ignoring it ...",
                         events_local->valuestring);
            } else {
                OpenAPI_list_add(eventsList, (void *)localEnum);
            }
        }
        if (eventsList->count == 0) {
            ogs_error("OpenAPI_bsf_subscription_resp_parseFromJSON() failed: Expected eventsList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }

    notif_uri = cJSON_GetObjectItemCaseSensitive(bsf_subscription_respJSON, "notifUri");
    if (!notif_uri) {
        ogs_error("OpenAPI_bsf_subscription_resp_parseFromJSON() failed [notif_uri]");
        goto end;
    }
    if (!cJSON_IsString(notif_uri)) {
        ogs_error("OpenAPI_bsf_subscription_resp_parseFromJSON() failed [notif_uri]");
        goto end;
    }

    notif_corre_id = cJSON_GetObjectItemCaseSensitive(bsf_subscription_respJSON, "notifCorreId");
    if (!notif_corre_id) {
        ogs_error("OpenAPI_bsf_subscription_resp_parseFromJSON() failed [notif_corre_id]");
        goto end;
    }
    if (!cJSON_IsString(notif_corre_id)) {
        ogs_error("OpenAPI_bsf_subscription_resp_parseFromJSON() failed [notif_corre_id]");
        goto end;
    }

    supi = cJSON_GetObjectItemCaseSensitive(bsf_subscription_respJSON, "supi");
    if (!supi) {
        ogs_error("OpenAPI_bsf_subscription_resp_parseFromJSON() failed [supi]");
        goto end;
    }
    if (!cJSON_IsString(supi)) {
        ogs_error("OpenAPI_bsf_subscription_resp_parseFromJSON() failed [supi]");
        goto end;
    }

    gpsi = cJSON_GetObjectItemCaseSensitive(bsf_subscription_respJSON, "gpsi");
    if (gpsi) {
    if (!cJSON_IsString(gpsi) && !cJSON_IsNull(gpsi)) {
        ogs_error("OpenAPI_bsf_subscription_resp_parseFromJSON() failed [gpsi]");
        goto end;
    }
    }

    snssai_dnn_pairs = cJSON_GetObjectItemCaseSensitive(bsf_subscription_respJSON, "snssaiDnnPairs");
    if (snssai_dnn_pairs) {
    snssai_dnn_pairs_local_nonprim = OpenAPI_snssai_dnn_pair_parseFromJSON(snssai_dnn_pairs);
    if (!snssai_dnn_pairs_local_nonprim) {
        ogs_error("OpenAPI_snssai_dnn_pair_parseFromJSON failed [snssai_dnn_pairs]");
        goto end;
    }
    }

    add_snssai_dnn_pairs = cJSON_GetObjectItemCaseSensitive(bsf_subscription_respJSON, "addSnssaiDnnPairs");
    if (add_snssai_dnn_pairs) {
        cJSON *add_snssai_dnn_pairs_local = NULL;
        if (!cJSON_IsArray(add_snssai_dnn_pairs)) {
            ogs_error("OpenAPI_bsf_subscription_resp_parseFromJSON() failed [add_snssai_dnn_pairs]");
            goto end;
        }

        add_snssai_dnn_pairsList = OpenAPI_list_create();

        cJSON_ArrayForEach(add_snssai_dnn_pairs_local, add_snssai_dnn_pairs) {
            if (!cJSON_IsObject(add_snssai_dnn_pairs_local)) {
                ogs_error("OpenAPI_bsf_subscription_resp_parseFromJSON() failed [add_snssai_dnn_pairs]");
                goto end;
            }
            OpenAPI_snssai_dnn_pair_t *add_snssai_dnn_pairsItem = OpenAPI_snssai_dnn_pair_parseFromJSON(add_snssai_dnn_pairs_local);
            if (!add_snssai_dnn_pairsItem) {
                ogs_error("No add_snssai_dnn_pairsItem");
                goto end;
            }
            OpenAPI_list_add(add_snssai_dnn_pairsList, add_snssai_dnn_pairsItem);
        }
    }

    supp_feat = cJSON_GetObjectItemCaseSensitive(bsf_subscription_respJSON, "suppFeat");
    if (supp_feat) {
    if (!cJSON_IsString(supp_feat) && !cJSON_IsNull(supp_feat)) {
        ogs_error("OpenAPI_bsf_subscription_resp_parseFromJSON() failed [supp_feat]");
        goto end;
    }
    }

    pcf_id = cJSON_GetObjectItemCaseSensitive(bsf_subscription_respJSON, "pcfId");
    if (pcf_id) {
    if (!cJSON_IsString(pcf_id) && !cJSON_IsNull(pcf_id)) {
        ogs_error("OpenAPI_bsf_subscription_resp_parseFromJSON() failed [pcf_id]");
        goto end;
    }
    }

    pcf_set_id = cJSON_GetObjectItemCaseSensitive(bsf_subscription_respJSON, "pcfSetId");
    if (pcf_set_id) {
    if (!cJSON_IsString(pcf_set_id) && !cJSON_IsNull(pcf_set_id)) {
        ogs_error("OpenAPI_bsf_subscription_resp_parseFromJSON() failed [pcf_set_id]");
        goto end;
    }
    }

    bind_level = cJSON_GetObjectItemCaseSensitive(bsf_subscription_respJSON, "bindLevel");
    if (bind_level) {
    if (!cJSON_IsString(bind_level)) {
        ogs_error("OpenAPI_bsf_subscription_resp_parseFromJSON() failed [bind_level]");
        goto end;
    }
    bind_levelVariable = OpenAPI_binding_level_FromString(bind_level->valuestring);
    }

    event_notifs = cJSON_GetObjectItemCaseSensitive(bsf_subscription_respJSON, "eventNotifs");
    if (!event_notifs) {
        ogs_error("OpenAPI_bsf_subscription_resp_parseFromJSON() failed [event_notifs]");
        goto end;
    }
        cJSON *event_notifs_local = NULL;
        if (!cJSON_IsArray(event_notifs)) {
            ogs_error("OpenAPI_bsf_subscription_resp_parseFromJSON() failed [event_notifs]");
            goto end;
        }

        event_notifsList = OpenAPI_list_create();

        cJSON_ArrayForEach(event_notifs_local, event_notifs) {
            if (!cJSON_IsObject(event_notifs_local)) {
                ogs_error("OpenAPI_bsf_subscription_resp_parseFromJSON() failed [event_notifs]");
                goto end;
            }
            OpenAPI_bsf_event_notification_t *event_notifsItem = OpenAPI_bsf_event_notification_parseFromJSON(event_notifs_local);
            if (!event_notifsItem) {
                ogs_error("No event_notifsItem");
                goto end;
            }
            OpenAPI_list_add(event_notifsList, event_notifsItem);
        }

    bsf_subscription_resp_local_var = OpenAPI_bsf_subscription_resp_create (
        eventsList,
        ogs_strdup(notif_uri->valuestring),
        ogs_strdup(notif_corre_id->valuestring),
        ogs_strdup(supi->valuestring),
        gpsi && !cJSON_IsNull(gpsi) ? ogs_strdup(gpsi->valuestring) : NULL,
        snssai_dnn_pairs ? snssai_dnn_pairs_local_nonprim : NULL,
        add_snssai_dnn_pairs ? add_snssai_dnn_pairsList : NULL,
        supp_feat && !cJSON_IsNull(supp_feat) ? ogs_strdup(supp_feat->valuestring) : NULL,
        pcf_id && !cJSON_IsNull(pcf_id) ? ogs_strdup(pcf_id->valuestring) : NULL,
        pcf_set_id && !cJSON_IsNull(pcf_set_id) ? ogs_strdup(pcf_set_id->valuestring) : NULL,
        bind_level ? bind_levelVariable : 0,
        event_notifsList
    );

    return bsf_subscription_resp_local_var;
end:
    if (eventsList) {
        OpenAPI_list_free(eventsList);
        eventsList = NULL;
    }
    if (snssai_dnn_pairs_local_nonprim) {
        OpenAPI_snssai_dnn_pair_free(snssai_dnn_pairs_local_nonprim);
        snssai_dnn_pairs_local_nonprim = NULL;
    }
    if (add_snssai_dnn_pairsList) {
        OpenAPI_list_for_each(add_snssai_dnn_pairsList, node) {
            OpenAPI_snssai_dnn_pair_free(node->data);
        }
        OpenAPI_list_free(add_snssai_dnn_pairsList);
        add_snssai_dnn_pairsList = NULL;
    }
    if (event_notifsList) {
        OpenAPI_list_for_each(event_notifsList, node) {
            OpenAPI_bsf_event_notification_free(node->data);
        }
        OpenAPI_list_free(event_notifsList);
        event_notifsList = NULL;
    }
    return NULL;
}

OpenAPI_bsf_subscription_resp_t *OpenAPI_bsf_subscription_resp_copy(OpenAPI_bsf_subscription_resp_t *dst, OpenAPI_bsf_subscription_resp_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_bsf_subscription_resp_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_bsf_subscription_resp_convertToJSON() failed");
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

    OpenAPI_bsf_subscription_resp_free(dst);
    dst = OpenAPI_bsf_subscription_resp_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

