
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "traffic_influ_sub.h"

OpenAPI_traffic_influ_sub_t *OpenAPI_traffic_influ_sub_create(
    OpenAPI_list_t *dnns,
    OpenAPI_list_t *snssais,
    OpenAPI_list_t *internal_group_ids,
    OpenAPI_list_t *supis,
    char *notification_uri,
    char *expiry,
    char *supported_features,
    OpenAPI_list_t *reset_ids
)
{
    OpenAPI_traffic_influ_sub_t *traffic_influ_sub_local_var = ogs_malloc(sizeof(OpenAPI_traffic_influ_sub_t));
    ogs_assert(traffic_influ_sub_local_var);

    traffic_influ_sub_local_var->dnns = dnns;
    traffic_influ_sub_local_var->snssais = snssais;
    traffic_influ_sub_local_var->internal_group_ids = internal_group_ids;
    traffic_influ_sub_local_var->supis = supis;
    traffic_influ_sub_local_var->notification_uri = notification_uri;
    traffic_influ_sub_local_var->expiry = expiry;
    traffic_influ_sub_local_var->supported_features = supported_features;
    traffic_influ_sub_local_var->reset_ids = reset_ids;

    return traffic_influ_sub_local_var;
}

void OpenAPI_traffic_influ_sub_free(OpenAPI_traffic_influ_sub_t *traffic_influ_sub)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == traffic_influ_sub) {
        return;
    }
    if (traffic_influ_sub->dnns) {
        OpenAPI_list_for_each(traffic_influ_sub->dnns, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(traffic_influ_sub->dnns);
        traffic_influ_sub->dnns = NULL;
    }
    if (traffic_influ_sub->snssais) {
        OpenAPI_list_for_each(traffic_influ_sub->snssais, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(traffic_influ_sub->snssais);
        traffic_influ_sub->snssais = NULL;
    }
    if (traffic_influ_sub->internal_group_ids) {
        OpenAPI_list_for_each(traffic_influ_sub->internal_group_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(traffic_influ_sub->internal_group_ids);
        traffic_influ_sub->internal_group_ids = NULL;
    }
    if (traffic_influ_sub->supis) {
        OpenAPI_list_for_each(traffic_influ_sub->supis, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(traffic_influ_sub->supis);
        traffic_influ_sub->supis = NULL;
    }
    if (traffic_influ_sub->notification_uri) {
        ogs_free(traffic_influ_sub->notification_uri);
        traffic_influ_sub->notification_uri = NULL;
    }
    if (traffic_influ_sub->expiry) {
        ogs_free(traffic_influ_sub->expiry);
        traffic_influ_sub->expiry = NULL;
    }
    if (traffic_influ_sub->supported_features) {
        ogs_free(traffic_influ_sub->supported_features);
        traffic_influ_sub->supported_features = NULL;
    }
    if (traffic_influ_sub->reset_ids) {
        OpenAPI_list_for_each(traffic_influ_sub->reset_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(traffic_influ_sub->reset_ids);
        traffic_influ_sub->reset_ids = NULL;
    }
    ogs_free(traffic_influ_sub);
}

cJSON *OpenAPI_traffic_influ_sub_convertToJSON(OpenAPI_traffic_influ_sub_t *traffic_influ_sub)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (traffic_influ_sub == NULL) {
        ogs_error("OpenAPI_traffic_influ_sub_convertToJSON() failed [TrafficInfluSub]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (traffic_influ_sub->dnns) {
    cJSON *dnnsList = cJSON_AddArrayToObject(item, "dnns");
    if (dnnsList == NULL) {
        ogs_error("OpenAPI_traffic_influ_sub_convertToJSON() failed [dnns]");
        goto end;
    }
    OpenAPI_list_for_each(traffic_influ_sub->dnns, node) {
        if (cJSON_AddStringToObject(dnnsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_traffic_influ_sub_convertToJSON() failed [dnns]");
            goto end;
        }
    }
    }

    if (traffic_influ_sub->snssais) {
    cJSON *snssaisList = cJSON_AddArrayToObject(item, "snssais");
    if (snssaisList == NULL) {
        ogs_error("OpenAPI_traffic_influ_sub_convertToJSON() failed [snssais]");
        goto end;
    }
    OpenAPI_list_for_each(traffic_influ_sub->snssais, node) {
        cJSON *itemLocal = OpenAPI_snssai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_traffic_influ_sub_convertToJSON() failed [snssais]");
            goto end;
        }
        cJSON_AddItemToArray(snssaisList, itemLocal);
    }
    }

    if (traffic_influ_sub->internal_group_ids) {
    cJSON *internal_group_idsList = cJSON_AddArrayToObject(item, "internalGroupIds");
    if (internal_group_idsList == NULL) {
        ogs_error("OpenAPI_traffic_influ_sub_convertToJSON() failed [internal_group_ids]");
        goto end;
    }
    OpenAPI_list_for_each(traffic_influ_sub->internal_group_ids, node) {
        if (cJSON_AddStringToObject(internal_group_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_traffic_influ_sub_convertToJSON() failed [internal_group_ids]");
            goto end;
        }
    }
    }

    if (traffic_influ_sub->supis) {
    cJSON *supisList = cJSON_AddArrayToObject(item, "supis");
    if (supisList == NULL) {
        ogs_error("OpenAPI_traffic_influ_sub_convertToJSON() failed [supis]");
        goto end;
    }
    OpenAPI_list_for_each(traffic_influ_sub->supis, node) {
        if (cJSON_AddStringToObject(supisList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_traffic_influ_sub_convertToJSON() failed [supis]");
            goto end;
        }
    }
    }

    if (!traffic_influ_sub->notification_uri) {
        ogs_error("OpenAPI_traffic_influ_sub_convertToJSON() failed [notification_uri]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "notificationUri", traffic_influ_sub->notification_uri) == NULL) {
        ogs_error("OpenAPI_traffic_influ_sub_convertToJSON() failed [notification_uri]");
        goto end;
    }

    if (traffic_influ_sub->expiry) {
    if (cJSON_AddStringToObject(item, "expiry", traffic_influ_sub->expiry) == NULL) {
        ogs_error("OpenAPI_traffic_influ_sub_convertToJSON() failed [expiry]");
        goto end;
    }
    }

    if (traffic_influ_sub->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", traffic_influ_sub->supported_features) == NULL) {
        ogs_error("OpenAPI_traffic_influ_sub_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

    if (traffic_influ_sub->reset_ids) {
    cJSON *reset_idsList = cJSON_AddArrayToObject(item, "resetIds");
    if (reset_idsList == NULL) {
        ogs_error("OpenAPI_traffic_influ_sub_convertToJSON() failed [reset_ids]");
        goto end;
    }
    OpenAPI_list_for_each(traffic_influ_sub->reset_ids, node) {
        if (cJSON_AddStringToObject(reset_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_traffic_influ_sub_convertToJSON() failed [reset_ids]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_traffic_influ_sub_t *OpenAPI_traffic_influ_sub_parseFromJSON(cJSON *traffic_influ_subJSON)
{
    OpenAPI_traffic_influ_sub_t *traffic_influ_sub_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *dnns = NULL;
    OpenAPI_list_t *dnnsList = NULL;
    cJSON *snssais = NULL;
    OpenAPI_list_t *snssaisList = NULL;
    cJSON *internal_group_ids = NULL;
    OpenAPI_list_t *internal_group_idsList = NULL;
    cJSON *supis = NULL;
    OpenAPI_list_t *supisList = NULL;
    cJSON *notification_uri = NULL;
    cJSON *expiry = NULL;
    cJSON *supported_features = NULL;
    cJSON *reset_ids = NULL;
    OpenAPI_list_t *reset_idsList = NULL;
    dnns = cJSON_GetObjectItemCaseSensitive(traffic_influ_subJSON, "dnns");
    if (dnns) {
        cJSON *dnns_local = NULL;
        if (!cJSON_IsArray(dnns)) {
            ogs_error("OpenAPI_traffic_influ_sub_parseFromJSON() failed [dnns]");
            goto end;
        }

        dnnsList = OpenAPI_list_create();

        cJSON_ArrayForEach(dnns_local, dnns) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(dnns_local)) {
                ogs_error("OpenAPI_traffic_influ_sub_parseFromJSON() failed [dnns]");
                goto end;
            }
            OpenAPI_list_add(dnnsList, ogs_strdup(dnns_local->valuestring));
        }
    }

    snssais = cJSON_GetObjectItemCaseSensitive(traffic_influ_subJSON, "snssais");
    if (snssais) {
        cJSON *snssais_local = NULL;
        if (!cJSON_IsArray(snssais)) {
            ogs_error("OpenAPI_traffic_influ_sub_parseFromJSON() failed [snssais]");
            goto end;
        }

        snssaisList = OpenAPI_list_create();

        cJSON_ArrayForEach(snssais_local, snssais) {
            if (!cJSON_IsObject(snssais_local)) {
                ogs_error("OpenAPI_traffic_influ_sub_parseFromJSON() failed [snssais]");
                goto end;
            }
            OpenAPI_snssai_t *snssaisItem = OpenAPI_snssai_parseFromJSON(snssais_local);
            if (!snssaisItem) {
                ogs_error("No snssaisItem");
                goto end;
            }
            OpenAPI_list_add(snssaisList, snssaisItem);
        }
    }

    internal_group_ids = cJSON_GetObjectItemCaseSensitive(traffic_influ_subJSON, "internalGroupIds");
    if (internal_group_ids) {
        cJSON *internal_group_ids_local = NULL;
        if (!cJSON_IsArray(internal_group_ids)) {
            ogs_error("OpenAPI_traffic_influ_sub_parseFromJSON() failed [internal_group_ids]");
            goto end;
        }

        internal_group_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(internal_group_ids_local, internal_group_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(internal_group_ids_local)) {
                ogs_error("OpenAPI_traffic_influ_sub_parseFromJSON() failed [internal_group_ids]");
                goto end;
            }
            OpenAPI_list_add(internal_group_idsList, ogs_strdup(internal_group_ids_local->valuestring));
        }
    }

    supis = cJSON_GetObjectItemCaseSensitive(traffic_influ_subJSON, "supis");
    if (supis) {
        cJSON *supis_local = NULL;
        if (!cJSON_IsArray(supis)) {
            ogs_error("OpenAPI_traffic_influ_sub_parseFromJSON() failed [supis]");
            goto end;
        }

        supisList = OpenAPI_list_create();

        cJSON_ArrayForEach(supis_local, supis) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(supis_local)) {
                ogs_error("OpenAPI_traffic_influ_sub_parseFromJSON() failed [supis]");
                goto end;
            }
            OpenAPI_list_add(supisList, ogs_strdup(supis_local->valuestring));
        }
    }

    notification_uri = cJSON_GetObjectItemCaseSensitive(traffic_influ_subJSON, "notificationUri");
    if (!notification_uri) {
        ogs_error("OpenAPI_traffic_influ_sub_parseFromJSON() failed [notification_uri]");
        goto end;
    }
    if (!cJSON_IsString(notification_uri)) {
        ogs_error("OpenAPI_traffic_influ_sub_parseFromJSON() failed [notification_uri]");
        goto end;
    }

    expiry = cJSON_GetObjectItemCaseSensitive(traffic_influ_subJSON, "expiry");
    if (expiry) {
    if (!cJSON_IsString(expiry) && !cJSON_IsNull(expiry)) {
        ogs_error("OpenAPI_traffic_influ_sub_parseFromJSON() failed [expiry]");
        goto end;
    }
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(traffic_influ_subJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_traffic_influ_sub_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    reset_ids = cJSON_GetObjectItemCaseSensitive(traffic_influ_subJSON, "resetIds");
    if (reset_ids) {
        cJSON *reset_ids_local = NULL;
        if (!cJSON_IsArray(reset_ids)) {
            ogs_error("OpenAPI_traffic_influ_sub_parseFromJSON() failed [reset_ids]");
            goto end;
        }

        reset_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(reset_ids_local, reset_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(reset_ids_local)) {
                ogs_error("OpenAPI_traffic_influ_sub_parseFromJSON() failed [reset_ids]");
                goto end;
            }
            OpenAPI_list_add(reset_idsList, ogs_strdup(reset_ids_local->valuestring));
        }
    }

    traffic_influ_sub_local_var = OpenAPI_traffic_influ_sub_create (
        dnns ? dnnsList : NULL,
        snssais ? snssaisList : NULL,
        internal_group_ids ? internal_group_idsList : NULL,
        supis ? supisList : NULL,
        ogs_strdup(notification_uri->valuestring),
        expiry && !cJSON_IsNull(expiry) ? ogs_strdup(expiry->valuestring) : NULL,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL,
        reset_ids ? reset_idsList : NULL
    );

    return traffic_influ_sub_local_var;
end:
    if (dnnsList) {
        OpenAPI_list_for_each(dnnsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(dnnsList);
        dnnsList = NULL;
    }
    if (snssaisList) {
        OpenAPI_list_for_each(snssaisList, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(snssaisList);
        snssaisList = NULL;
    }
    if (internal_group_idsList) {
        OpenAPI_list_for_each(internal_group_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(internal_group_idsList);
        internal_group_idsList = NULL;
    }
    if (supisList) {
        OpenAPI_list_for_each(supisList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(supisList);
        supisList = NULL;
    }
    if (reset_idsList) {
        OpenAPI_list_for_each(reset_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(reset_idsList);
        reset_idsList = NULL;
    }
    return NULL;
}

OpenAPI_traffic_influ_sub_t *OpenAPI_traffic_influ_sub_copy(OpenAPI_traffic_influ_sub_t *dst, OpenAPI_traffic_influ_sub_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_traffic_influ_sub_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_traffic_influ_sub_convertToJSON() failed");
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

    OpenAPI_traffic_influ_sub_free(dst);
    dst = OpenAPI_traffic_influ_sub_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

