
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
    char *supported_features
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

    return traffic_influ_sub_local_var;
}

void OpenAPI_traffic_influ_sub_free(OpenAPI_traffic_influ_sub_t *traffic_influ_sub)
{
    if (NULL == traffic_influ_sub) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(traffic_influ_sub->dnns, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(traffic_influ_sub->dnns);
    OpenAPI_list_for_each(traffic_influ_sub->snssais, node) {
        OpenAPI_snssai_free(node->data);
    }
    OpenAPI_list_free(traffic_influ_sub->snssais);
    OpenAPI_list_for_each(traffic_influ_sub->internal_group_ids, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(traffic_influ_sub->internal_group_ids);
    OpenAPI_list_for_each(traffic_influ_sub->supis, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(traffic_influ_sub->supis);
    ogs_free(traffic_influ_sub->notification_uri);
    ogs_free(traffic_influ_sub->expiry);
    ogs_free(traffic_influ_sub->supported_features);
    ogs_free(traffic_influ_sub);
}

cJSON *OpenAPI_traffic_influ_sub_convertToJSON(OpenAPI_traffic_influ_sub_t *traffic_influ_sub)
{
    cJSON *item = NULL;

    if (traffic_influ_sub == NULL) {
        ogs_error("OpenAPI_traffic_influ_sub_convertToJSON() failed [TrafficInfluSub]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (traffic_influ_sub->dnns) {
    cJSON *dnns = cJSON_AddArrayToObject(item, "dnns");
    if (dnns == NULL) {
        ogs_error("OpenAPI_traffic_influ_sub_convertToJSON() failed [dnns]");
        goto end;
    }

    OpenAPI_lnode_t *dnns_node;
    OpenAPI_list_for_each(traffic_influ_sub->dnns, dnns_node)  {
    if (cJSON_AddStringToObject(dnns, "", (char*)dnns_node->data) == NULL) {
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

    OpenAPI_lnode_t *snssais_node;
    if (traffic_influ_sub->snssais) {
        OpenAPI_list_for_each(traffic_influ_sub->snssais, snssais_node) {
            cJSON *itemLocal = OpenAPI_snssai_convertToJSON(snssais_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_traffic_influ_sub_convertToJSON() failed [snssais]");
                goto end;
            }
            cJSON_AddItemToArray(snssaisList, itemLocal);
        }
    }
    }

    if (traffic_influ_sub->internal_group_ids) {
    cJSON *internal_group_ids = cJSON_AddArrayToObject(item, "internalGroupIds");
    if (internal_group_ids == NULL) {
        ogs_error("OpenAPI_traffic_influ_sub_convertToJSON() failed [internal_group_ids]");
        goto end;
    }

    OpenAPI_lnode_t *internal_group_ids_node;
    OpenAPI_list_for_each(traffic_influ_sub->internal_group_ids, internal_group_ids_node)  {
    if (cJSON_AddStringToObject(internal_group_ids, "", (char*)internal_group_ids_node->data) == NULL) {
        ogs_error("OpenAPI_traffic_influ_sub_convertToJSON() failed [internal_group_ids]");
        goto end;
    }
                    }
    }

    if (traffic_influ_sub->supis) {
    cJSON *supis = cJSON_AddArrayToObject(item, "supis");
    if (supis == NULL) {
        ogs_error("OpenAPI_traffic_influ_sub_convertToJSON() failed [supis]");
        goto end;
    }

    OpenAPI_lnode_t *supis_node;
    OpenAPI_list_for_each(traffic_influ_sub->supis, supis_node)  {
    if (cJSON_AddStringToObject(supis, "", (char*)supis_node->data) == NULL) {
        ogs_error("OpenAPI_traffic_influ_sub_convertToJSON() failed [supis]");
        goto end;
    }
                    }
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

end:
    return item;
}

OpenAPI_traffic_influ_sub_t *OpenAPI_traffic_influ_sub_parseFromJSON(cJSON *traffic_influ_subJSON)
{
    OpenAPI_traffic_influ_sub_t *traffic_influ_sub_local_var = NULL;
    cJSON *dnns = cJSON_GetObjectItemCaseSensitive(traffic_influ_subJSON, "dnns");

    OpenAPI_list_t *dnnsList;
    if (dnns) {
    cJSON *dnns_local;
    if (!cJSON_IsArray(dnns)) {
        ogs_error("OpenAPI_traffic_influ_sub_parseFromJSON() failed [dnns]");
        goto end;
    }
    dnnsList = OpenAPI_list_create();

    cJSON_ArrayForEach(dnns_local, dnns) {
    if (!cJSON_IsString(dnns_local)) {
        ogs_error("OpenAPI_traffic_influ_sub_parseFromJSON() failed [dnns]");
        goto end;
    }
    OpenAPI_list_add(dnnsList , ogs_strdup(dnns_local->valuestring));
    }
    }

    cJSON *snssais = cJSON_GetObjectItemCaseSensitive(traffic_influ_subJSON, "snssais");

    OpenAPI_list_t *snssaisList;
    if (snssais) {
    cJSON *snssais_local_nonprimitive;
    if (!cJSON_IsArray(snssais)){
        ogs_error("OpenAPI_traffic_influ_sub_parseFromJSON() failed [snssais]");
        goto end;
    }

    snssaisList = OpenAPI_list_create();

    cJSON_ArrayForEach(snssais_local_nonprimitive, snssais ) {
        if (!cJSON_IsObject(snssais_local_nonprimitive)) {
            ogs_error("OpenAPI_traffic_influ_sub_parseFromJSON() failed [snssais]");
            goto end;
        }
        OpenAPI_snssai_t *snssaisItem = OpenAPI_snssai_parseFromJSON(snssais_local_nonprimitive);

        if (!snssaisItem) {
            ogs_error("No snssaisItem");
            OpenAPI_list_free(snssaisList);
            goto end;
        }

        OpenAPI_list_add(snssaisList, snssaisItem);
    }
    }

    cJSON *internal_group_ids = cJSON_GetObjectItemCaseSensitive(traffic_influ_subJSON, "internalGroupIds");

    OpenAPI_list_t *internal_group_idsList;
    if (internal_group_ids) {
    cJSON *internal_group_ids_local;
    if (!cJSON_IsArray(internal_group_ids)) {
        ogs_error("OpenAPI_traffic_influ_sub_parseFromJSON() failed [internal_group_ids]");
        goto end;
    }
    internal_group_idsList = OpenAPI_list_create();

    cJSON_ArrayForEach(internal_group_ids_local, internal_group_ids) {
    if (!cJSON_IsString(internal_group_ids_local)) {
        ogs_error("OpenAPI_traffic_influ_sub_parseFromJSON() failed [internal_group_ids]");
        goto end;
    }
    OpenAPI_list_add(internal_group_idsList , ogs_strdup(internal_group_ids_local->valuestring));
    }
    }

    cJSON *supis = cJSON_GetObjectItemCaseSensitive(traffic_influ_subJSON, "supis");

    OpenAPI_list_t *supisList;
    if (supis) {
    cJSON *supis_local;
    if (!cJSON_IsArray(supis)) {
        ogs_error("OpenAPI_traffic_influ_sub_parseFromJSON() failed [supis]");
        goto end;
    }
    supisList = OpenAPI_list_create();

    cJSON_ArrayForEach(supis_local, supis) {
    if (!cJSON_IsString(supis_local)) {
        ogs_error("OpenAPI_traffic_influ_sub_parseFromJSON() failed [supis]");
        goto end;
    }
    OpenAPI_list_add(supisList , ogs_strdup(supis_local->valuestring));
    }
    }

    cJSON *notification_uri = cJSON_GetObjectItemCaseSensitive(traffic_influ_subJSON, "notificationUri");
    if (!notification_uri) {
        ogs_error("OpenAPI_traffic_influ_sub_parseFromJSON() failed [notification_uri]");
        goto end;
    }

    if (!cJSON_IsString(notification_uri)) {
        ogs_error("OpenAPI_traffic_influ_sub_parseFromJSON() failed [notification_uri]");
        goto end;
    }

    cJSON *expiry = cJSON_GetObjectItemCaseSensitive(traffic_influ_subJSON, "expiry");

    if (expiry) {
    if (!cJSON_IsString(expiry)) {
        ogs_error("OpenAPI_traffic_influ_sub_parseFromJSON() failed [expiry]");
        goto end;
    }
    }

    cJSON *supported_features = cJSON_GetObjectItemCaseSensitive(traffic_influ_subJSON, "supportedFeatures");

    if (supported_features) {
    if (!cJSON_IsString(supported_features)) {
        ogs_error("OpenAPI_traffic_influ_sub_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    traffic_influ_sub_local_var = OpenAPI_traffic_influ_sub_create (
        dnns ? dnnsList : NULL,
        snssais ? snssaisList : NULL,
        internal_group_ids ? internal_group_idsList : NULL,
        supis ? supisList : NULL,
        ogs_strdup(notification_uri->valuestring),
        expiry ? ogs_strdup(expiry->valuestring) : NULL,
        supported_features ? ogs_strdup(supported_features->valuestring) : NULL
    );

    return traffic_influ_sub_local_var;
end:
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

