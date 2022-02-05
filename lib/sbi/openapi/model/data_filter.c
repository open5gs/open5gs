
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "data_filter.h"

OpenAPI_data_filter_t *OpenAPI_data_filter_create(
    OpenAPI_data_ind_t *data_ind,
    OpenAPI_list_t *dnns,
    OpenAPI_list_t *snssais,
    OpenAPI_list_t *internal_group_ids,
    OpenAPI_list_t *supis,
    OpenAPI_list_t *app_ids,
    OpenAPI_list_t *ue_ipv4s,
    OpenAPI_list_t *ue_ipv6s,
    OpenAPI_list_t *ue_macs
)
{
    OpenAPI_data_filter_t *data_filter_local_var = ogs_malloc(sizeof(OpenAPI_data_filter_t));
    ogs_assert(data_filter_local_var);

    data_filter_local_var->data_ind = data_ind;
    data_filter_local_var->dnns = dnns;
    data_filter_local_var->snssais = snssais;
    data_filter_local_var->internal_group_ids = internal_group_ids;
    data_filter_local_var->supis = supis;
    data_filter_local_var->app_ids = app_ids;
    data_filter_local_var->ue_ipv4s = ue_ipv4s;
    data_filter_local_var->ue_ipv6s = ue_ipv6s;
    data_filter_local_var->ue_macs = ue_macs;

    return data_filter_local_var;
}

void OpenAPI_data_filter_free(OpenAPI_data_filter_t *data_filter)
{
    if (NULL == data_filter) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_data_ind_free(data_filter->data_ind);
    OpenAPI_list_for_each(data_filter->dnns, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(data_filter->dnns);
    OpenAPI_list_for_each(data_filter->snssais, node) {
        OpenAPI_snssai_free(node->data);
    }
    OpenAPI_list_free(data_filter->snssais);
    OpenAPI_list_for_each(data_filter->internal_group_ids, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(data_filter->internal_group_ids);
    OpenAPI_list_for_each(data_filter->supis, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(data_filter->supis);
    OpenAPI_list_for_each(data_filter->app_ids, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(data_filter->app_ids);
    OpenAPI_list_for_each(data_filter->ue_ipv4s, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(data_filter->ue_ipv4s);
    OpenAPI_list_for_each(data_filter->ue_ipv6s, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(data_filter->ue_ipv6s);
    OpenAPI_list_for_each(data_filter->ue_macs, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(data_filter->ue_macs);
    ogs_free(data_filter);
}

cJSON *OpenAPI_data_filter_convertToJSON(OpenAPI_data_filter_t *data_filter)
{
    cJSON *item = NULL;

    if (data_filter == NULL) {
        ogs_error("OpenAPI_data_filter_convertToJSON() failed [DataFilter]");
        return NULL;
    }

    item = cJSON_CreateObject();
    cJSON *data_ind_local_JSON = OpenAPI_data_ind_convertToJSON(data_filter->data_ind);
    if (data_ind_local_JSON == NULL) {
        ogs_error("OpenAPI_data_filter_convertToJSON() failed [data_ind]");
        goto end;
    }
    cJSON_AddItemToObject(item, "dataInd", data_ind_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_data_filter_convertToJSON() failed [data_ind]");
        goto end;
    }

    if (data_filter->dnns) {
    cJSON *dnns = cJSON_AddArrayToObject(item, "dnns");
    if (dnns == NULL) {
        ogs_error("OpenAPI_data_filter_convertToJSON() failed [dnns]");
        goto end;
    }

    OpenAPI_lnode_t *dnns_node;
    OpenAPI_list_for_each(data_filter->dnns, dnns_node)  {
    if (cJSON_AddStringToObject(dnns, "", (char*)dnns_node->data) == NULL) {
        ogs_error("OpenAPI_data_filter_convertToJSON() failed [dnns]");
        goto end;
    }
                    }
    }

    if (data_filter->snssais) {
    cJSON *snssaisList = cJSON_AddArrayToObject(item, "snssais");
    if (snssaisList == NULL) {
        ogs_error("OpenAPI_data_filter_convertToJSON() failed [snssais]");
        goto end;
    }

    OpenAPI_lnode_t *snssais_node;
    if (data_filter->snssais) {
        OpenAPI_list_for_each(data_filter->snssais, snssais_node) {
            cJSON *itemLocal = OpenAPI_snssai_convertToJSON(snssais_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_data_filter_convertToJSON() failed [snssais]");
                goto end;
            }
            cJSON_AddItemToArray(snssaisList, itemLocal);
        }
    }
    }

    if (data_filter->internal_group_ids) {
    cJSON *internal_group_ids = cJSON_AddArrayToObject(item, "internalGroupIds");
    if (internal_group_ids == NULL) {
        ogs_error("OpenAPI_data_filter_convertToJSON() failed [internal_group_ids]");
        goto end;
    }

    OpenAPI_lnode_t *internal_group_ids_node;
    OpenAPI_list_for_each(data_filter->internal_group_ids, internal_group_ids_node)  {
    if (cJSON_AddStringToObject(internal_group_ids, "", (char*)internal_group_ids_node->data) == NULL) {
        ogs_error("OpenAPI_data_filter_convertToJSON() failed [internal_group_ids]");
        goto end;
    }
                    }
    }

    if (data_filter->supis) {
    cJSON *supis = cJSON_AddArrayToObject(item, "supis");
    if (supis == NULL) {
        ogs_error("OpenAPI_data_filter_convertToJSON() failed [supis]");
        goto end;
    }

    OpenAPI_lnode_t *supis_node;
    OpenAPI_list_for_each(data_filter->supis, supis_node)  {
    if (cJSON_AddStringToObject(supis, "", (char*)supis_node->data) == NULL) {
        ogs_error("OpenAPI_data_filter_convertToJSON() failed [supis]");
        goto end;
    }
                    }
    }

    if (data_filter->app_ids) {
    cJSON *app_ids = cJSON_AddArrayToObject(item, "appIds");
    if (app_ids == NULL) {
        ogs_error("OpenAPI_data_filter_convertToJSON() failed [app_ids]");
        goto end;
    }

    OpenAPI_lnode_t *app_ids_node;
    OpenAPI_list_for_each(data_filter->app_ids, app_ids_node)  {
    if (cJSON_AddStringToObject(app_ids, "", (char*)app_ids_node->data) == NULL) {
        ogs_error("OpenAPI_data_filter_convertToJSON() failed [app_ids]");
        goto end;
    }
                    }
    }

    if (data_filter->ue_ipv4s) {
    cJSON *ue_ipv4s = cJSON_AddArrayToObject(item, "ueIpv4s");
    if (ue_ipv4s == NULL) {
        ogs_error("OpenAPI_data_filter_convertToJSON() failed [ue_ipv4s]");
        goto end;
    }

    OpenAPI_lnode_t *ue_ipv4s_node;
    OpenAPI_list_for_each(data_filter->ue_ipv4s, ue_ipv4s_node)  {
    if (cJSON_AddStringToObject(ue_ipv4s, "", (char*)ue_ipv4s_node->data) == NULL) {
        ogs_error("OpenAPI_data_filter_convertToJSON() failed [ue_ipv4s]");
        goto end;
    }
                    }
    }

    if (data_filter->ue_ipv6s) {
    cJSON *ue_ipv6s = cJSON_AddArrayToObject(item, "ueIpv6s");
    if (ue_ipv6s == NULL) {
        ogs_error("OpenAPI_data_filter_convertToJSON() failed [ue_ipv6s]");
        goto end;
    }

    OpenAPI_lnode_t *ue_ipv6s_node;
    OpenAPI_list_for_each(data_filter->ue_ipv6s, ue_ipv6s_node)  {
    if (cJSON_AddStringToObject(ue_ipv6s, "", (char*)ue_ipv6s_node->data) == NULL) {
        ogs_error("OpenAPI_data_filter_convertToJSON() failed [ue_ipv6s]");
        goto end;
    }
                    }
    }

    if (data_filter->ue_macs) {
    cJSON *ue_macs = cJSON_AddArrayToObject(item, "ueMacs");
    if (ue_macs == NULL) {
        ogs_error("OpenAPI_data_filter_convertToJSON() failed [ue_macs]");
        goto end;
    }

    OpenAPI_lnode_t *ue_macs_node;
    OpenAPI_list_for_each(data_filter->ue_macs, ue_macs_node)  {
    if (cJSON_AddStringToObject(ue_macs, "", (char*)ue_macs_node->data) == NULL) {
        ogs_error("OpenAPI_data_filter_convertToJSON() failed [ue_macs]");
        goto end;
    }
                    }
    }

end:
    return item;
}

OpenAPI_data_filter_t *OpenAPI_data_filter_parseFromJSON(cJSON *data_filterJSON)
{
    OpenAPI_data_filter_t *data_filter_local_var = NULL;
    cJSON *data_ind = cJSON_GetObjectItemCaseSensitive(data_filterJSON, "dataInd");
    if (!data_ind) {
        ogs_error("OpenAPI_data_filter_parseFromJSON() failed [data_ind]");
        goto end;
    }

    OpenAPI_data_ind_t *data_ind_local_nonprim = NULL;
    data_ind_local_nonprim = OpenAPI_data_ind_parseFromJSON(data_ind);

    cJSON *dnns = cJSON_GetObjectItemCaseSensitive(data_filterJSON, "dnns");

    OpenAPI_list_t *dnnsList;
    if (dnns) {
    cJSON *dnns_local;
    if (!cJSON_IsArray(dnns)) {
        ogs_error("OpenAPI_data_filter_parseFromJSON() failed [dnns]");
        goto end;
    }
    dnnsList = OpenAPI_list_create();

    cJSON_ArrayForEach(dnns_local, dnns) {
    if (!cJSON_IsString(dnns_local)) {
        ogs_error("OpenAPI_data_filter_parseFromJSON() failed [dnns]");
        goto end;
    }
    OpenAPI_list_add(dnnsList , ogs_strdup(dnns_local->valuestring));
    }
    }

    cJSON *snssais = cJSON_GetObjectItemCaseSensitive(data_filterJSON, "snssais");

    OpenAPI_list_t *snssaisList;
    if (snssais) {
    cJSON *snssais_local_nonprimitive;
    if (!cJSON_IsArray(snssais)){
        ogs_error("OpenAPI_data_filter_parseFromJSON() failed [snssais]");
        goto end;
    }

    snssaisList = OpenAPI_list_create();

    cJSON_ArrayForEach(snssais_local_nonprimitive, snssais ) {
        if (!cJSON_IsObject(snssais_local_nonprimitive)) {
            ogs_error("OpenAPI_data_filter_parseFromJSON() failed [snssais]");
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

    cJSON *internal_group_ids = cJSON_GetObjectItemCaseSensitive(data_filterJSON, "internalGroupIds");

    OpenAPI_list_t *internal_group_idsList;
    if (internal_group_ids) {
    cJSON *internal_group_ids_local;
    if (!cJSON_IsArray(internal_group_ids)) {
        ogs_error("OpenAPI_data_filter_parseFromJSON() failed [internal_group_ids]");
        goto end;
    }
    internal_group_idsList = OpenAPI_list_create();

    cJSON_ArrayForEach(internal_group_ids_local, internal_group_ids) {
    if (!cJSON_IsString(internal_group_ids_local)) {
        ogs_error("OpenAPI_data_filter_parseFromJSON() failed [internal_group_ids]");
        goto end;
    }
    OpenAPI_list_add(internal_group_idsList , ogs_strdup(internal_group_ids_local->valuestring));
    }
    }

    cJSON *supis = cJSON_GetObjectItemCaseSensitive(data_filterJSON, "supis");

    OpenAPI_list_t *supisList;
    if (supis) {
    cJSON *supis_local;
    if (!cJSON_IsArray(supis)) {
        ogs_error("OpenAPI_data_filter_parseFromJSON() failed [supis]");
        goto end;
    }
    supisList = OpenAPI_list_create();

    cJSON_ArrayForEach(supis_local, supis) {
    if (!cJSON_IsString(supis_local)) {
        ogs_error("OpenAPI_data_filter_parseFromJSON() failed [supis]");
        goto end;
    }
    OpenAPI_list_add(supisList , ogs_strdup(supis_local->valuestring));
    }
    }

    cJSON *app_ids = cJSON_GetObjectItemCaseSensitive(data_filterJSON, "appIds");

    OpenAPI_list_t *app_idsList;
    if (app_ids) {
    cJSON *app_ids_local;
    if (!cJSON_IsArray(app_ids)) {
        ogs_error("OpenAPI_data_filter_parseFromJSON() failed [app_ids]");
        goto end;
    }
    app_idsList = OpenAPI_list_create();

    cJSON_ArrayForEach(app_ids_local, app_ids) {
    if (!cJSON_IsString(app_ids_local)) {
        ogs_error("OpenAPI_data_filter_parseFromJSON() failed [app_ids]");
        goto end;
    }
    OpenAPI_list_add(app_idsList , ogs_strdup(app_ids_local->valuestring));
    }
    }

    cJSON *ue_ipv4s = cJSON_GetObjectItemCaseSensitive(data_filterJSON, "ueIpv4s");

    OpenAPI_list_t *ue_ipv4sList;
    if (ue_ipv4s) {
    cJSON *ue_ipv4s_local;
    if (!cJSON_IsArray(ue_ipv4s)) {
        ogs_error("OpenAPI_data_filter_parseFromJSON() failed [ue_ipv4s]");
        goto end;
    }
    ue_ipv4sList = OpenAPI_list_create();

    cJSON_ArrayForEach(ue_ipv4s_local, ue_ipv4s) {
    if (!cJSON_IsString(ue_ipv4s_local)) {
        ogs_error("OpenAPI_data_filter_parseFromJSON() failed [ue_ipv4s]");
        goto end;
    }
    OpenAPI_list_add(ue_ipv4sList , ogs_strdup(ue_ipv4s_local->valuestring));
    }
    }

    cJSON *ue_ipv6s = cJSON_GetObjectItemCaseSensitive(data_filterJSON, "ueIpv6s");

    OpenAPI_list_t *ue_ipv6sList;
    if (ue_ipv6s) {
    cJSON *ue_ipv6s_local;
    if (!cJSON_IsArray(ue_ipv6s)) {
        ogs_error("OpenAPI_data_filter_parseFromJSON() failed [ue_ipv6s]");
        goto end;
    }
    ue_ipv6sList = OpenAPI_list_create();

    cJSON_ArrayForEach(ue_ipv6s_local, ue_ipv6s) {
    if (!cJSON_IsString(ue_ipv6s_local)) {
        ogs_error("OpenAPI_data_filter_parseFromJSON() failed [ue_ipv6s]");
        goto end;
    }
    OpenAPI_list_add(ue_ipv6sList , ogs_strdup(ue_ipv6s_local->valuestring));
    }
    }

    cJSON *ue_macs = cJSON_GetObjectItemCaseSensitive(data_filterJSON, "ueMacs");

    OpenAPI_list_t *ue_macsList;
    if (ue_macs) {
    cJSON *ue_macs_local;
    if (!cJSON_IsArray(ue_macs)) {
        ogs_error("OpenAPI_data_filter_parseFromJSON() failed [ue_macs]");
        goto end;
    }
    ue_macsList = OpenAPI_list_create();

    cJSON_ArrayForEach(ue_macs_local, ue_macs) {
    if (!cJSON_IsString(ue_macs_local)) {
        ogs_error("OpenAPI_data_filter_parseFromJSON() failed [ue_macs]");
        goto end;
    }
    OpenAPI_list_add(ue_macsList , ogs_strdup(ue_macs_local->valuestring));
    }
    }

    data_filter_local_var = OpenAPI_data_filter_create (
        data_ind_local_nonprim,
        dnns ? dnnsList : NULL,
        snssais ? snssaisList : NULL,
        internal_group_ids ? internal_group_idsList : NULL,
        supis ? supisList : NULL,
        app_ids ? app_idsList : NULL,
        ue_ipv4s ? ue_ipv4sList : NULL,
        ue_ipv6s ? ue_ipv6sList : NULL,
        ue_macs ? ue_macsList : NULL
    );

    return data_filter_local_var;
end:
    return NULL;
}

OpenAPI_data_filter_t *OpenAPI_data_filter_copy(OpenAPI_data_filter_t *dst, OpenAPI_data_filter_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_data_filter_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_data_filter_convertToJSON() failed");
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

    OpenAPI_data_filter_free(dst);
    dst = OpenAPI_data_filter_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

