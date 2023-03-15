
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nwdaf_subscription.h"

OpenAPI_nwdaf_subscription_t *OpenAPI_nwdaf_subscription_create(
    char *nwdaf_evt_subs_service_uri,
    OpenAPI_nnwdaf_events_subscription_t *nwdaf_events_subscription
)
{
    OpenAPI_nwdaf_subscription_t *nwdaf_subscription_local_var = ogs_malloc(sizeof(OpenAPI_nwdaf_subscription_t));
    ogs_assert(nwdaf_subscription_local_var);

    nwdaf_subscription_local_var->nwdaf_evt_subs_service_uri = nwdaf_evt_subs_service_uri;
    nwdaf_subscription_local_var->nwdaf_events_subscription = nwdaf_events_subscription;

    return nwdaf_subscription_local_var;
}

void OpenAPI_nwdaf_subscription_free(OpenAPI_nwdaf_subscription_t *nwdaf_subscription)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == nwdaf_subscription) {
        return;
    }
    if (nwdaf_subscription->nwdaf_evt_subs_service_uri) {
        ogs_free(nwdaf_subscription->nwdaf_evt_subs_service_uri);
        nwdaf_subscription->nwdaf_evt_subs_service_uri = NULL;
    }
    if (nwdaf_subscription->nwdaf_events_subscription) {
        OpenAPI_nnwdaf_events_subscription_free(nwdaf_subscription->nwdaf_events_subscription);
        nwdaf_subscription->nwdaf_events_subscription = NULL;
    }
    ogs_free(nwdaf_subscription);
}

cJSON *OpenAPI_nwdaf_subscription_convertToJSON(OpenAPI_nwdaf_subscription_t *nwdaf_subscription)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (nwdaf_subscription == NULL) {
        ogs_error("OpenAPI_nwdaf_subscription_convertToJSON() failed [NwdafSubscription]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!nwdaf_subscription->nwdaf_evt_subs_service_uri) {
        ogs_error("OpenAPI_nwdaf_subscription_convertToJSON() failed [nwdaf_evt_subs_service_uri]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "nwdafEvtSubsServiceUri", nwdaf_subscription->nwdaf_evt_subs_service_uri) == NULL) {
        ogs_error("OpenAPI_nwdaf_subscription_convertToJSON() failed [nwdaf_evt_subs_service_uri]");
        goto end;
    }

    if (!nwdaf_subscription->nwdaf_events_subscription) {
        ogs_error("OpenAPI_nwdaf_subscription_convertToJSON() failed [nwdaf_events_subscription]");
        return NULL;
    }
    cJSON *nwdaf_events_subscription_local_JSON = OpenAPI_nnwdaf_events_subscription_convertToJSON(nwdaf_subscription->nwdaf_events_subscription);
    if (nwdaf_events_subscription_local_JSON == NULL) {
        ogs_error("OpenAPI_nwdaf_subscription_convertToJSON() failed [nwdaf_events_subscription]");
        goto end;
    }
    cJSON_AddItemToObject(item, "nwdafEventsSubscription", nwdaf_events_subscription_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_nwdaf_subscription_convertToJSON() failed [nwdaf_events_subscription]");
        goto end;
    }

end:
    return item;
}

OpenAPI_nwdaf_subscription_t *OpenAPI_nwdaf_subscription_parseFromJSON(cJSON *nwdaf_subscriptionJSON)
{
    OpenAPI_nwdaf_subscription_t *nwdaf_subscription_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *nwdaf_evt_subs_service_uri = NULL;
    cJSON *nwdaf_events_subscription = NULL;
    OpenAPI_nnwdaf_events_subscription_t *nwdaf_events_subscription_local_nonprim = NULL;
    nwdaf_evt_subs_service_uri = cJSON_GetObjectItemCaseSensitive(nwdaf_subscriptionJSON, "nwdafEvtSubsServiceUri");
    if (!nwdaf_evt_subs_service_uri) {
        ogs_error("OpenAPI_nwdaf_subscription_parseFromJSON() failed [nwdaf_evt_subs_service_uri]");
        goto end;
    }
    if (!cJSON_IsString(nwdaf_evt_subs_service_uri)) {
        ogs_error("OpenAPI_nwdaf_subscription_parseFromJSON() failed [nwdaf_evt_subs_service_uri]");
        goto end;
    }

    nwdaf_events_subscription = cJSON_GetObjectItemCaseSensitive(nwdaf_subscriptionJSON, "nwdafEventsSubscription");
    if (!nwdaf_events_subscription) {
        ogs_error("OpenAPI_nwdaf_subscription_parseFromJSON() failed [nwdaf_events_subscription]");
        goto end;
    }
    nwdaf_events_subscription_local_nonprim = OpenAPI_nnwdaf_events_subscription_parseFromJSON(nwdaf_events_subscription);
    if (!nwdaf_events_subscription_local_nonprim) {
        ogs_error("OpenAPI_nnwdaf_events_subscription_parseFromJSON failed [nwdaf_events_subscription]");
        goto end;
    }

    nwdaf_subscription_local_var = OpenAPI_nwdaf_subscription_create (
        ogs_strdup(nwdaf_evt_subs_service_uri->valuestring),
        nwdaf_events_subscription_local_nonprim
    );

    return nwdaf_subscription_local_var;
end:
    if (nwdaf_events_subscription_local_nonprim) {
        OpenAPI_nnwdaf_events_subscription_free(nwdaf_events_subscription_local_nonprim);
        nwdaf_events_subscription_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_nwdaf_subscription_t *OpenAPI_nwdaf_subscription_copy(OpenAPI_nwdaf_subscription_t *dst, OpenAPI_nwdaf_subscription_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nwdaf_subscription_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nwdaf_subscription_convertToJSON() failed");
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

    OpenAPI_nwdaf_subscription_free(dst);
    dst = OpenAPI_nwdaf_subscription_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

