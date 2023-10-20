
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "bsf_subscription.h"

OpenAPI_bsf_subscription_t *OpenAPI_bsf_subscription_create(
    OpenAPI_list_t *events,
    char *notif_uri,
    char *notif_corre_id,
    char *supi,
    char *gpsi,
    OpenAPI_snssai_dnn_pair_t *snssai_dnn_pairs,
    OpenAPI_list_t *add_snssai_dnn_pairs,
    char *supp_feat
)
{
    OpenAPI_bsf_subscription_t *bsf_subscription_local_var = ogs_malloc(sizeof(OpenAPI_bsf_subscription_t));
    ogs_assert(bsf_subscription_local_var);

    bsf_subscription_local_var->events = events;
    bsf_subscription_local_var->notif_uri = notif_uri;
    bsf_subscription_local_var->notif_corre_id = notif_corre_id;
    bsf_subscription_local_var->supi = supi;
    bsf_subscription_local_var->gpsi = gpsi;
    bsf_subscription_local_var->snssai_dnn_pairs = snssai_dnn_pairs;
    bsf_subscription_local_var->add_snssai_dnn_pairs = add_snssai_dnn_pairs;
    bsf_subscription_local_var->supp_feat = supp_feat;

    return bsf_subscription_local_var;
}

void OpenAPI_bsf_subscription_free(OpenAPI_bsf_subscription_t *bsf_subscription)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == bsf_subscription) {
        return;
    }
    if (bsf_subscription->events) {
        OpenAPI_list_free(bsf_subscription->events);
        bsf_subscription->events = NULL;
    }
    if (bsf_subscription->notif_uri) {
        ogs_free(bsf_subscription->notif_uri);
        bsf_subscription->notif_uri = NULL;
    }
    if (bsf_subscription->notif_corre_id) {
        ogs_free(bsf_subscription->notif_corre_id);
        bsf_subscription->notif_corre_id = NULL;
    }
    if (bsf_subscription->supi) {
        ogs_free(bsf_subscription->supi);
        bsf_subscription->supi = NULL;
    }
    if (bsf_subscription->gpsi) {
        ogs_free(bsf_subscription->gpsi);
        bsf_subscription->gpsi = NULL;
    }
    if (bsf_subscription->snssai_dnn_pairs) {
        OpenAPI_snssai_dnn_pair_free(bsf_subscription->snssai_dnn_pairs);
        bsf_subscription->snssai_dnn_pairs = NULL;
    }
    if (bsf_subscription->add_snssai_dnn_pairs) {
        OpenAPI_list_for_each(bsf_subscription->add_snssai_dnn_pairs, node) {
            OpenAPI_snssai_dnn_pair_free(node->data);
        }
        OpenAPI_list_free(bsf_subscription->add_snssai_dnn_pairs);
        bsf_subscription->add_snssai_dnn_pairs = NULL;
    }
    if (bsf_subscription->supp_feat) {
        ogs_free(bsf_subscription->supp_feat);
        bsf_subscription->supp_feat = NULL;
    }
    ogs_free(bsf_subscription);
}

cJSON *OpenAPI_bsf_subscription_convertToJSON(OpenAPI_bsf_subscription_t *bsf_subscription)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (bsf_subscription == NULL) {
        ogs_error("OpenAPI_bsf_subscription_convertToJSON() failed [BsfSubscription]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (bsf_subscription->events == OpenAPI_bsf_event_NULL) {
        ogs_error("OpenAPI_bsf_subscription_convertToJSON() failed [events]");
        return NULL;
    }
    cJSON *eventsList = cJSON_AddArrayToObject(item, "events");
    if (eventsList == NULL) {
        ogs_error("OpenAPI_bsf_subscription_convertToJSON() failed [events]");
        goto end;
    }
    OpenAPI_list_for_each(bsf_subscription->events, node) {
        if (cJSON_AddStringToObject(eventsList, "", OpenAPI_bsf_event_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_bsf_subscription_convertToJSON() failed [events]");
            goto end;
        }
    }

    if (!bsf_subscription->notif_uri) {
        ogs_error("OpenAPI_bsf_subscription_convertToJSON() failed [notif_uri]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "notifUri", bsf_subscription->notif_uri) == NULL) {
        ogs_error("OpenAPI_bsf_subscription_convertToJSON() failed [notif_uri]");
        goto end;
    }

    if (!bsf_subscription->notif_corre_id) {
        ogs_error("OpenAPI_bsf_subscription_convertToJSON() failed [notif_corre_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "notifCorreId", bsf_subscription->notif_corre_id) == NULL) {
        ogs_error("OpenAPI_bsf_subscription_convertToJSON() failed [notif_corre_id]");
        goto end;
    }

    if (!bsf_subscription->supi) {
        ogs_error("OpenAPI_bsf_subscription_convertToJSON() failed [supi]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "supi", bsf_subscription->supi) == NULL) {
        ogs_error("OpenAPI_bsf_subscription_convertToJSON() failed [supi]");
        goto end;
    }

    if (bsf_subscription->gpsi) {
    if (cJSON_AddStringToObject(item, "gpsi", bsf_subscription->gpsi) == NULL) {
        ogs_error("OpenAPI_bsf_subscription_convertToJSON() failed [gpsi]");
        goto end;
    }
    }

    if (bsf_subscription->snssai_dnn_pairs) {
    cJSON *snssai_dnn_pairs_local_JSON = OpenAPI_snssai_dnn_pair_convertToJSON(bsf_subscription->snssai_dnn_pairs);
    if (snssai_dnn_pairs_local_JSON == NULL) {
        ogs_error("OpenAPI_bsf_subscription_convertToJSON() failed [snssai_dnn_pairs]");
        goto end;
    }
    cJSON_AddItemToObject(item, "snssaiDnnPairs", snssai_dnn_pairs_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_bsf_subscription_convertToJSON() failed [snssai_dnn_pairs]");
        goto end;
    }
    }

    if (bsf_subscription->add_snssai_dnn_pairs) {
    cJSON *add_snssai_dnn_pairsList = cJSON_AddArrayToObject(item, "addSnssaiDnnPairs");
    if (add_snssai_dnn_pairsList == NULL) {
        ogs_error("OpenAPI_bsf_subscription_convertToJSON() failed [add_snssai_dnn_pairs]");
        goto end;
    }
    OpenAPI_list_for_each(bsf_subscription->add_snssai_dnn_pairs, node) {
        cJSON *itemLocal = OpenAPI_snssai_dnn_pair_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_bsf_subscription_convertToJSON() failed [add_snssai_dnn_pairs]");
            goto end;
        }
        cJSON_AddItemToArray(add_snssai_dnn_pairsList, itemLocal);
    }
    }

    if (bsf_subscription->supp_feat) {
    if (cJSON_AddStringToObject(item, "suppFeat", bsf_subscription->supp_feat) == NULL) {
        ogs_error("OpenAPI_bsf_subscription_convertToJSON() failed [supp_feat]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_bsf_subscription_t *OpenAPI_bsf_subscription_parseFromJSON(cJSON *bsf_subscriptionJSON)
{
    OpenAPI_bsf_subscription_t *bsf_subscription_local_var = NULL;
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
    events = cJSON_GetObjectItemCaseSensitive(bsf_subscriptionJSON, "events");
    if (!events) {
        ogs_error("OpenAPI_bsf_subscription_parseFromJSON() failed [events]");
        goto end;
    }
        cJSON *events_local = NULL;
        if (!cJSON_IsArray(events)) {
            ogs_error("OpenAPI_bsf_subscription_parseFromJSON() failed [events]");
            goto end;
        }

        eventsList = OpenAPI_list_create();

        cJSON_ArrayForEach(events_local, events) {
            OpenAPI_bsf_event_e localEnum = OpenAPI_bsf_event_NULL;
            if (!cJSON_IsString(events_local)) {
                ogs_error("OpenAPI_bsf_subscription_parseFromJSON() failed [events]");
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
            ogs_error("OpenAPI_bsf_subscription_parseFromJSON() failed: Expected eventsList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }

    notif_uri = cJSON_GetObjectItemCaseSensitive(bsf_subscriptionJSON, "notifUri");
    if (!notif_uri) {
        ogs_error("OpenAPI_bsf_subscription_parseFromJSON() failed [notif_uri]");
        goto end;
    }
    if (!cJSON_IsString(notif_uri)) {
        ogs_error("OpenAPI_bsf_subscription_parseFromJSON() failed [notif_uri]");
        goto end;
    }

    notif_corre_id = cJSON_GetObjectItemCaseSensitive(bsf_subscriptionJSON, "notifCorreId");
    if (!notif_corre_id) {
        ogs_error("OpenAPI_bsf_subscription_parseFromJSON() failed [notif_corre_id]");
        goto end;
    }
    if (!cJSON_IsString(notif_corre_id)) {
        ogs_error("OpenAPI_bsf_subscription_parseFromJSON() failed [notif_corre_id]");
        goto end;
    }

    supi = cJSON_GetObjectItemCaseSensitive(bsf_subscriptionJSON, "supi");
    if (!supi) {
        ogs_error("OpenAPI_bsf_subscription_parseFromJSON() failed [supi]");
        goto end;
    }
    if (!cJSON_IsString(supi)) {
        ogs_error("OpenAPI_bsf_subscription_parseFromJSON() failed [supi]");
        goto end;
    }

    gpsi = cJSON_GetObjectItemCaseSensitive(bsf_subscriptionJSON, "gpsi");
    if (gpsi) {
    if (!cJSON_IsString(gpsi) && !cJSON_IsNull(gpsi)) {
        ogs_error("OpenAPI_bsf_subscription_parseFromJSON() failed [gpsi]");
        goto end;
    }
    }

    snssai_dnn_pairs = cJSON_GetObjectItemCaseSensitive(bsf_subscriptionJSON, "snssaiDnnPairs");
    if (snssai_dnn_pairs) {
    snssai_dnn_pairs_local_nonprim = OpenAPI_snssai_dnn_pair_parseFromJSON(snssai_dnn_pairs);
    if (!snssai_dnn_pairs_local_nonprim) {
        ogs_error("OpenAPI_snssai_dnn_pair_parseFromJSON failed [snssai_dnn_pairs]");
        goto end;
    }
    }

    add_snssai_dnn_pairs = cJSON_GetObjectItemCaseSensitive(bsf_subscriptionJSON, "addSnssaiDnnPairs");
    if (add_snssai_dnn_pairs) {
        cJSON *add_snssai_dnn_pairs_local = NULL;
        if (!cJSON_IsArray(add_snssai_dnn_pairs)) {
            ogs_error("OpenAPI_bsf_subscription_parseFromJSON() failed [add_snssai_dnn_pairs]");
            goto end;
        }

        add_snssai_dnn_pairsList = OpenAPI_list_create();

        cJSON_ArrayForEach(add_snssai_dnn_pairs_local, add_snssai_dnn_pairs) {
            if (!cJSON_IsObject(add_snssai_dnn_pairs_local)) {
                ogs_error("OpenAPI_bsf_subscription_parseFromJSON() failed [add_snssai_dnn_pairs]");
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

    supp_feat = cJSON_GetObjectItemCaseSensitive(bsf_subscriptionJSON, "suppFeat");
    if (supp_feat) {
    if (!cJSON_IsString(supp_feat) && !cJSON_IsNull(supp_feat)) {
        ogs_error("OpenAPI_bsf_subscription_parseFromJSON() failed [supp_feat]");
        goto end;
    }
    }

    bsf_subscription_local_var = OpenAPI_bsf_subscription_create (
        eventsList,
        ogs_strdup(notif_uri->valuestring),
        ogs_strdup(notif_corre_id->valuestring),
        ogs_strdup(supi->valuestring),
        gpsi && !cJSON_IsNull(gpsi) ? ogs_strdup(gpsi->valuestring) : NULL,
        snssai_dnn_pairs ? snssai_dnn_pairs_local_nonprim : NULL,
        add_snssai_dnn_pairs ? add_snssai_dnn_pairsList : NULL,
        supp_feat && !cJSON_IsNull(supp_feat) ? ogs_strdup(supp_feat->valuestring) : NULL
    );

    return bsf_subscription_local_var;
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
    return NULL;
}

OpenAPI_bsf_subscription_t *OpenAPI_bsf_subscription_copy(OpenAPI_bsf_subscription_t *dst, OpenAPI_bsf_subscription_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_bsf_subscription_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_bsf_subscription_convertToJSON() failed");
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

    OpenAPI_bsf_subscription_free(dst);
    dst = OpenAPI_bsf_subscription_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

