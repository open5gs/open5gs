
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "analytics_subscription.h"

OpenAPI_analytics_subscription_t *OpenAPI_analytics_subscription_create(
    char *nwdaf_id,
    char *nwdaf_set_id,
    OpenAPI_list_t *nwdaf_subscription_list
)
{
    OpenAPI_analytics_subscription_t *analytics_subscription_local_var = ogs_malloc(sizeof(OpenAPI_analytics_subscription_t));
    ogs_assert(analytics_subscription_local_var);

    analytics_subscription_local_var->nwdaf_id = nwdaf_id;
    analytics_subscription_local_var->nwdaf_set_id = nwdaf_set_id;
    analytics_subscription_local_var->nwdaf_subscription_list = nwdaf_subscription_list;

    return analytics_subscription_local_var;
}

void OpenAPI_analytics_subscription_free(OpenAPI_analytics_subscription_t *analytics_subscription)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == analytics_subscription) {
        return;
    }
    if (analytics_subscription->nwdaf_id) {
        ogs_free(analytics_subscription->nwdaf_id);
        analytics_subscription->nwdaf_id = NULL;
    }
    if (analytics_subscription->nwdaf_set_id) {
        ogs_free(analytics_subscription->nwdaf_set_id);
        analytics_subscription->nwdaf_set_id = NULL;
    }
    if (analytics_subscription->nwdaf_subscription_list) {
        OpenAPI_list_for_each(analytics_subscription->nwdaf_subscription_list, node) {
            OpenAPI_nwdaf_subscription_free(node->data);
        }
        OpenAPI_list_free(analytics_subscription->nwdaf_subscription_list);
        analytics_subscription->nwdaf_subscription_list = NULL;
    }
    ogs_free(analytics_subscription);
}

cJSON *OpenAPI_analytics_subscription_convertToJSON(OpenAPI_analytics_subscription_t *analytics_subscription)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (analytics_subscription == NULL) {
        ogs_error("OpenAPI_analytics_subscription_convertToJSON() failed [AnalyticsSubscription]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (analytics_subscription->nwdaf_id) {
    if (cJSON_AddStringToObject(item, "nwdafId", analytics_subscription->nwdaf_id) == NULL) {
        ogs_error("OpenAPI_analytics_subscription_convertToJSON() failed [nwdaf_id]");
        goto end;
    }
    }

    if (analytics_subscription->nwdaf_set_id) {
    if (cJSON_AddStringToObject(item, "nwdafSetId", analytics_subscription->nwdaf_set_id) == NULL) {
        ogs_error("OpenAPI_analytics_subscription_convertToJSON() failed [nwdaf_set_id]");
        goto end;
    }
    }

    if (!analytics_subscription->nwdaf_subscription_list) {
        ogs_error("OpenAPI_analytics_subscription_convertToJSON() failed [nwdaf_subscription_list]");
        return NULL;
    }
    cJSON *nwdaf_subscription_listList = cJSON_AddArrayToObject(item, "nwdafSubscriptionList");
    if (nwdaf_subscription_listList == NULL) {
        ogs_error("OpenAPI_analytics_subscription_convertToJSON() failed [nwdaf_subscription_list]");
        goto end;
    }
    OpenAPI_list_for_each(analytics_subscription->nwdaf_subscription_list, node) {
        cJSON *itemLocal = OpenAPI_nwdaf_subscription_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_analytics_subscription_convertToJSON() failed [nwdaf_subscription_list]");
            goto end;
        }
        cJSON_AddItemToArray(nwdaf_subscription_listList, itemLocal);
    }

end:
    return item;
}

OpenAPI_analytics_subscription_t *OpenAPI_analytics_subscription_parseFromJSON(cJSON *analytics_subscriptionJSON)
{
    OpenAPI_analytics_subscription_t *analytics_subscription_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *nwdaf_id = NULL;
    cJSON *nwdaf_set_id = NULL;
    cJSON *nwdaf_subscription_list = NULL;
    OpenAPI_list_t *nwdaf_subscription_listList = NULL;
    nwdaf_id = cJSON_GetObjectItemCaseSensitive(analytics_subscriptionJSON, "nwdafId");
    if (nwdaf_id) {
    if (!cJSON_IsString(nwdaf_id) && !cJSON_IsNull(nwdaf_id)) {
        ogs_error("OpenAPI_analytics_subscription_parseFromJSON() failed [nwdaf_id]");
        goto end;
    }
    }

    nwdaf_set_id = cJSON_GetObjectItemCaseSensitive(analytics_subscriptionJSON, "nwdafSetId");
    if (nwdaf_set_id) {
    if (!cJSON_IsString(nwdaf_set_id) && !cJSON_IsNull(nwdaf_set_id)) {
        ogs_error("OpenAPI_analytics_subscription_parseFromJSON() failed [nwdaf_set_id]");
        goto end;
    }
    }

    nwdaf_subscription_list = cJSON_GetObjectItemCaseSensitive(analytics_subscriptionJSON, "nwdafSubscriptionList");
    if (!nwdaf_subscription_list) {
        ogs_error("OpenAPI_analytics_subscription_parseFromJSON() failed [nwdaf_subscription_list]");
        goto end;
    }
        cJSON *nwdaf_subscription_list_local = NULL;
        if (!cJSON_IsArray(nwdaf_subscription_list)) {
            ogs_error("OpenAPI_analytics_subscription_parseFromJSON() failed [nwdaf_subscription_list]");
            goto end;
        }

        nwdaf_subscription_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(nwdaf_subscription_list_local, nwdaf_subscription_list) {
            if (!cJSON_IsObject(nwdaf_subscription_list_local)) {
                ogs_error("OpenAPI_analytics_subscription_parseFromJSON() failed [nwdaf_subscription_list]");
                goto end;
            }
            OpenAPI_nwdaf_subscription_t *nwdaf_subscription_listItem = OpenAPI_nwdaf_subscription_parseFromJSON(nwdaf_subscription_list_local);
            if (!nwdaf_subscription_listItem) {
                ogs_error("No nwdaf_subscription_listItem");
                goto end;
            }
            OpenAPI_list_add(nwdaf_subscription_listList, nwdaf_subscription_listItem);
        }

    analytics_subscription_local_var = OpenAPI_analytics_subscription_create (
        nwdaf_id && !cJSON_IsNull(nwdaf_id) ? ogs_strdup(nwdaf_id->valuestring) : NULL,
        nwdaf_set_id && !cJSON_IsNull(nwdaf_set_id) ? ogs_strdup(nwdaf_set_id->valuestring) : NULL,
        nwdaf_subscription_listList
    );

    return analytics_subscription_local_var;
end:
    if (nwdaf_subscription_listList) {
        OpenAPI_list_for_each(nwdaf_subscription_listList, node) {
            OpenAPI_nwdaf_subscription_free(node->data);
        }
        OpenAPI_list_free(nwdaf_subscription_listList);
        nwdaf_subscription_listList = NULL;
    }
    return NULL;
}

OpenAPI_analytics_subscription_t *OpenAPI_analytics_subscription_copy(OpenAPI_analytics_subscription_t *dst, OpenAPI_analytics_subscription_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_analytics_subscription_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_analytics_subscription_convertToJSON() failed");
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

    OpenAPI_analytics_subscription_free(dst);
    dst = OpenAPI_analytics_subscription_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

