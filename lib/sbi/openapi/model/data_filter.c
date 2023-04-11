
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
    OpenAPI_list_t *ue_macs,
    bool is_any_ue_ind,
    int any_ue_ind,
    OpenAPI_list_t *dnn_snssai_infos
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
    data_filter_local_var->is_any_ue_ind = is_any_ue_ind;
    data_filter_local_var->any_ue_ind = any_ue_ind;
    data_filter_local_var->dnn_snssai_infos = dnn_snssai_infos;

    return data_filter_local_var;
}

void OpenAPI_data_filter_free(OpenAPI_data_filter_t *data_filter)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == data_filter) {
        return;
    }
    if (data_filter->data_ind) {
        OpenAPI_data_ind_free(data_filter->data_ind);
        data_filter->data_ind = NULL;
    }
    if (data_filter->dnns) {
        OpenAPI_list_for_each(data_filter->dnns, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(data_filter->dnns);
        data_filter->dnns = NULL;
    }
    if (data_filter->snssais) {
        OpenAPI_list_for_each(data_filter->snssais, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(data_filter->snssais);
        data_filter->snssais = NULL;
    }
    if (data_filter->internal_group_ids) {
        OpenAPI_list_for_each(data_filter->internal_group_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(data_filter->internal_group_ids);
        data_filter->internal_group_ids = NULL;
    }
    if (data_filter->supis) {
        OpenAPI_list_for_each(data_filter->supis, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(data_filter->supis);
        data_filter->supis = NULL;
    }
    if (data_filter->app_ids) {
        OpenAPI_list_for_each(data_filter->app_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(data_filter->app_ids);
        data_filter->app_ids = NULL;
    }
    if (data_filter->ue_ipv4s) {
        OpenAPI_list_for_each(data_filter->ue_ipv4s, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(data_filter->ue_ipv4s);
        data_filter->ue_ipv4s = NULL;
    }
    if (data_filter->ue_ipv6s) {
        OpenAPI_list_for_each(data_filter->ue_ipv6s, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(data_filter->ue_ipv6s);
        data_filter->ue_ipv6s = NULL;
    }
    if (data_filter->ue_macs) {
        OpenAPI_list_for_each(data_filter->ue_macs, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(data_filter->ue_macs);
        data_filter->ue_macs = NULL;
    }
    if (data_filter->dnn_snssai_infos) {
        OpenAPI_list_for_each(data_filter->dnn_snssai_infos, node) {
            OpenAPI_dnn_snssai_information_free(node->data);
        }
        OpenAPI_list_free(data_filter->dnn_snssai_infos);
        data_filter->dnn_snssai_infos = NULL;
    }
    ogs_free(data_filter);
}

cJSON *OpenAPI_data_filter_convertToJSON(OpenAPI_data_filter_t *data_filter)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (data_filter == NULL) {
        ogs_error("OpenAPI_data_filter_convertToJSON() failed [DataFilter]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!data_filter->data_ind) {
        ogs_error("OpenAPI_data_filter_convertToJSON() failed [data_ind]");
        return NULL;
    }
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
    cJSON *dnnsList = cJSON_AddArrayToObject(item, "dnns");
    if (dnnsList == NULL) {
        ogs_error("OpenAPI_data_filter_convertToJSON() failed [dnns]");
        goto end;
    }
    OpenAPI_list_for_each(data_filter->dnns, node) {
        if (cJSON_AddStringToObject(dnnsList, "", (char*)node->data) == NULL) {
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
    OpenAPI_list_for_each(data_filter->snssais, node) {
        cJSON *itemLocal = OpenAPI_snssai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_data_filter_convertToJSON() failed [snssais]");
            goto end;
        }
        cJSON_AddItemToArray(snssaisList, itemLocal);
    }
    }

    if (data_filter->internal_group_ids) {
    cJSON *internal_group_idsList = cJSON_AddArrayToObject(item, "internalGroupIds");
    if (internal_group_idsList == NULL) {
        ogs_error("OpenAPI_data_filter_convertToJSON() failed [internal_group_ids]");
        goto end;
    }
    OpenAPI_list_for_each(data_filter->internal_group_ids, node) {
        if (cJSON_AddStringToObject(internal_group_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_data_filter_convertToJSON() failed [internal_group_ids]");
            goto end;
        }
    }
    }

    if (data_filter->supis) {
    cJSON *supisList = cJSON_AddArrayToObject(item, "supis");
    if (supisList == NULL) {
        ogs_error("OpenAPI_data_filter_convertToJSON() failed [supis]");
        goto end;
    }
    OpenAPI_list_for_each(data_filter->supis, node) {
        if (cJSON_AddStringToObject(supisList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_data_filter_convertToJSON() failed [supis]");
            goto end;
        }
    }
    }

    if (data_filter->app_ids) {
    cJSON *app_idsList = cJSON_AddArrayToObject(item, "appIds");
    if (app_idsList == NULL) {
        ogs_error("OpenAPI_data_filter_convertToJSON() failed [app_ids]");
        goto end;
    }
    OpenAPI_list_for_each(data_filter->app_ids, node) {
        if (cJSON_AddStringToObject(app_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_data_filter_convertToJSON() failed [app_ids]");
            goto end;
        }
    }
    }

    if (data_filter->ue_ipv4s) {
    cJSON *ue_ipv4sList = cJSON_AddArrayToObject(item, "ueIpv4s");
    if (ue_ipv4sList == NULL) {
        ogs_error("OpenAPI_data_filter_convertToJSON() failed [ue_ipv4s]");
        goto end;
    }
    OpenAPI_list_for_each(data_filter->ue_ipv4s, node) {
        if (cJSON_AddStringToObject(ue_ipv4sList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_data_filter_convertToJSON() failed [ue_ipv4s]");
            goto end;
        }
    }
    }

    if (data_filter->ue_ipv6s) {
    cJSON *ue_ipv6sList = cJSON_AddArrayToObject(item, "ueIpv6s");
    if (ue_ipv6sList == NULL) {
        ogs_error("OpenAPI_data_filter_convertToJSON() failed [ue_ipv6s]");
        goto end;
    }
    OpenAPI_list_for_each(data_filter->ue_ipv6s, node) {
        if (cJSON_AddStringToObject(ue_ipv6sList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_data_filter_convertToJSON() failed [ue_ipv6s]");
            goto end;
        }
    }
    }

    if (data_filter->ue_macs) {
    cJSON *ue_macsList = cJSON_AddArrayToObject(item, "ueMacs");
    if (ue_macsList == NULL) {
        ogs_error("OpenAPI_data_filter_convertToJSON() failed [ue_macs]");
        goto end;
    }
    OpenAPI_list_for_each(data_filter->ue_macs, node) {
        if (cJSON_AddStringToObject(ue_macsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_data_filter_convertToJSON() failed [ue_macs]");
            goto end;
        }
    }
    }

    if (data_filter->is_any_ue_ind) {
    if (cJSON_AddBoolToObject(item, "anyUeInd", data_filter->any_ue_ind) == NULL) {
        ogs_error("OpenAPI_data_filter_convertToJSON() failed [any_ue_ind]");
        goto end;
    }
    }

    if (data_filter->dnn_snssai_infos) {
    cJSON *dnn_snssai_infosList = cJSON_AddArrayToObject(item, "dnnSnssaiInfos");
    if (dnn_snssai_infosList == NULL) {
        ogs_error("OpenAPI_data_filter_convertToJSON() failed [dnn_snssai_infos]");
        goto end;
    }
    OpenAPI_list_for_each(data_filter->dnn_snssai_infos, node) {
        cJSON *itemLocal = OpenAPI_dnn_snssai_information_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_data_filter_convertToJSON() failed [dnn_snssai_infos]");
            goto end;
        }
        cJSON_AddItemToArray(dnn_snssai_infosList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_data_filter_t *OpenAPI_data_filter_parseFromJSON(cJSON *data_filterJSON)
{
    OpenAPI_data_filter_t *data_filter_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *data_ind = NULL;
    OpenAPI_data_ind_t *data_ind_local_nonprim = NULL;
    cJSON *dnns = NULL;
    OpenAPI_list_t *dnnsList = NULL;
    cJSON *snssais = NULL;
    OpenAPI_list_t *snssaisList = NULL;
    cJSON *internal_group_ids = NULL;
    OpenAPI_list_t *internal_group_idsList = NULL;
    cJSON *supis = NULL;
    OpenAPI_list_t *supisList = NULL;
    cJSON *app_ids = NULL;
    OpenAPI_list_t *app_idsList = NULL;
    cJSON *ue_ipv4s = NULL;
    OpenAPI_list_t *ue_ipv4sList = NULL;
    cJSON *ue_ipv6s = NULL;
    OpenAPI_list_t *ue_ipv6sList = NULL;
    cJSON *ue_macs = NULL;
    OpenAPI_list_t *ue_macsList = NULL;
    cJSON *any_ue_ind = NULL;
    cJSON *dnn_snssai_infos = NULL;
    OpenAPI_list_t *dnn_snssai_infosList = NULL;
    data_ind = cJSON_GetObjectItemCaseSensitive(data_filterJSON, "dataInd");
    if (!data_ind) {
        ogs_error("OpenAPI_data_filter_parseFromJSON() failed [data_ind]");
        goto end;
    }
    data_ind_local_nonprim = OpenAPI_data_ind_parseFromJSON(data_ind);
    if (!data_ind_local_nonprim) {
        ogs_error("OpenAPI_data_ind_parseFromJSON failed [data_ind]");
        goto end;
    }

    dnns = cJSON_GetObjectItemCaseSensitive(data_filterJSON, "dnns");
    if (dnns) {
        cJSON *dnns_local = NULL;
        if (!cJSON_IsArray(dnns)) {
            ogs_error("OpenAPI_data_filter_parseFromJSON() failed [dnns]");
            goto end;
        }

        dnnsList = OpenAPI_list_create();

        cJSON_ArrayForEach(dnns_local, dnns) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(dnns_local)) {
                ogs_error("OpenAPI_data_filter_parseFromJSON() failed [dnns]");
                goto end;
            }
            OpenAPI_list_add(dnnsList, ogs_strdup(dnns_local->valuestring));
        }
    }

    snssais = cJSON_GetObjectItemCaseSensitive(data_filterJSON, "snssais");
    if (snssais) {
        cJSON *snssais_local = NULL;
        if (!cJSON_IsArray(snssais)) {
            ogs_error("OpenAPI_data_filter_parseFromJSON() failed [snssais]");
            goto end;
        }

        snssaisList = OpenAPI_list_create();

        cJSON_ArrayForEach(snssais_local, snssais) {
            if (!cJSON_IsObject(snssais_local)) {
                ogs_error("OpenAPI_data_filter_parseFromJSON() failed [snssais]");
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

    internal_group_ids = cJSON_GetObjectItemCaseSensitive(data_filterJSON, "internalGroupIds");
    if (internal_group_ids) {
        cJSON *internal_group_ids_local = NULL;
        if (!cJSON_IsArray(internal_group_ids)) {
            ogs_error("OpenAPI_data_filter_parseFromJSON() failed [internal_group_ids]");
            goto end;
        }

        internal_group_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(internal_group_ids_local, internal_group_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(internal_group_ids_local)) {
                ogs_error("OpenAPI_data_filter_parseFromJSON() failed [internal_group_ids]");
                goto end;
            }
            OpenAPI_list_add(internal_group_idsList, ogs_strdup(internal_group_ids_local->valuestring));
        }
    }

    supis = cJSON_GetObjectItemCaseSensitive(data_filterJSON, "supis");
    if (supis) {
        cJSON *supis_local = NULL;
        if (!cJSON_IsArray(supis)) {
            ogs_error("OpenAPI_data_filter_parseFromJSON() failed [supis]");
            goto end;
        }

        supisList = OpenAPI_list_create();

        cJSON_ArrayForEach(supis_local, supis) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(supis_local)) {
                ogs_error("OpenAPI_data_filter_parseFromJSON() failed [supis]");
                goto end;
            }
            OpenAPI_list_add(supisList, ogs_strdup(supis_local->valuestring));
        }
    }

    app_ids = cJSON_GetObjectItemCaseSensitive(data_filterJSON, "appIds");
    if (app_ids) {
        cJSON *app_ids_local = NULL;
        if (!cJSON_IsArray(app_ids)) {
            ogs_error("OpenAPI_data_filter_parseFromJSON() failed [app_ids]");
            goto end;
        }

        app_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(app_ids_local, app_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(app_ids_local)) {
                ogs_error("OpenAPI_data_filter_parseFromJSON() failed [app_ids]");
                goto end;
            }
            OpenAPI_list_add(app_idsList, ogs_strdup(app_ids_local->valuestring));
        }
    }

    ue_ipv4s = cJSON_GetObjectItemCaseSensitive(data_filterJSON, "ueIpv4s");
    if (ue_ipv4s) {
        cJSON *ue_ipv4s_local = NULL;
        if (!cJSON_IsArray(ue_ipv4s)) {
            ogs_error("OpenAPI_data_filter_parseFromJSON() failed [ue_ipv4s]");
            goto end;
        }

        ue_ipv4sList = OpenAPI_list_create();

        cJSON_ArrayForEach(ue_ipv4s_local, ue_ipv4s) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(ue_ipv4s_local)) {
                ogs_error("OpenAPI_data_filter_parseFromJSON() failed [ue_ipv4s]");
                goto end;
            }
            OpenAPI_list_add(ue_ipv4sList, ogs_strdup(ue_ipv4s_local->valuestring));
        }
    }

    ue_ipv6s = cJSON_GetObjectItemCaseSensitive(data_filterJSON, "ueIpv6s");
    if (ue_ipv6s) {
        cJSON *ue_ipv6s_local = NULL;
        if (!cJSON_IsArray(ue_ipv6s)) {
            ogs_error("OpenAPI_data_filter_parseFromJSON() failed [ue_ipv6s]");
            goto end;
        }

        ue_ipv6sList = OpenAPI_list_create();

        cJSON_ArrayForEach(ue_ipv6s_local, ue_ipv6s) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(ue_ipv6s_local)) {
                ogs_error("OpenAPI_data_filter_parseFromJSON() failed [ue_ipv6s]");
                goto end;
            }
            OpenAPI_list_add(ue_ipv6sList, ogs_strdup(ue_ipv6s_local->valuestring));
        }
    }

    ue_macs = cJSON_GetObjectItemCaseSensitive(data_filterJSON, "ueMacs");
    if (ue_macs) {
        cJSON *ue_macs_local = NULL;
        if (!cJSON_IsArray(ue_macs)) {
            ogs_error("OpenAPI_data_filter_parseFromJSON() failed [ue_macs]");
            goto end;
        }

        ue_macsList = OpenAPI_list_create();

        cJSON_ArrayForEach(ue_macs_local, ue_macs) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(ue_macs_local)) {
                ogs_error("OpenAPI_data_filter_parseFromJSON() failed [ue_macs]");
                goto end;
            }
            OpenAPI_list_add(ue_macsList, ogs_strdup(ue_macs_local->valuestring));
        }
    }

    any_ue_ind = cJSON_GetObjectItemCaseSensitive(data_filterJSON, "anyUeInd");
    if (any_ue_ind) {
    if (!cJSON_IsBool(any_ue_ind)) {
        ogs_error("OpenAPI_data_filter_parseFromJSON() failed [any_ue_ind]");
        goto end;
    }
    }

    dnn_snssai_infos = cJSON_GetObjectItemCaseSensitive(data_filterJSON, "dnnSnssaiInfos");
    if (dnn_snssai_infos) {
        cJSON *dnn_snssai_infos_local = NULL;
        if (!cJSON_IsArray(dnn_snssai_infos)) {
            ogs_error("OpenAPI_data_filter_parseFromJSON() failed [dnn_snssai_infos]");
            goto end;
        }

        dnn_snssai_infosList = OpenAPI_list_create();

        cJSON_ArrayForEach(dnn_snssai_infos_local, dnn_snssai_infos) {
            if (!cJSON_IsObject(dnn_snssai_infos_local)) {
                ogs_error("OpenAPI_data_filter_parseFromJSON() failed [dnn_snssai_infos]");
                goto end;
            }
            OpenAPI_dnn_snssai_information_t *dnn_snssai_infosItem = OpenAPI_dnn_snssai_information_parseFromJSON(dnn_snssai_infos_local);
            if (!dnn_snssai_infosItem) {
                ogs_error("No dnn_snssai_infosItem");
                goto end;
            }
            OpenAPI_list_add(dnn_snssai_infosList, dnn_snssai_infosItem);
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
        ue_macs ? ue_macsList : NULL,
        any_ue_ind ? true : false,
        any_ue_ind ? any_ue_ind->valueint : 0,
        dnn_snssai_infos ? dnn_snssai_infosList : NULL
    );

    return data_filter_local_var;
end:
    if (data_ind_local_nonprim) {
        OpenAPI_data_ind_free(data_ind_local_nonprim);
        data_ind_local_nonprim = NULL;
    }
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
    if (app_idsList) {
        OpenAPI_list_for_each(app_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(app_idsList);
        app_idsList = NULL;
    }
    if (ue_ipv4sList) {
        OpenAPI_list_for_each(ue_ipv4sList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ue_ipv4sList);
        ue_ipv4sList = NULL;
    }
    if (ue_ipv6sList) {
        OpenAPI_list_for_each(ue_ipv6sList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ue_ipv6sList);
        ue_ipv6sList = NULL;
    }
    if (ue_macsList) {
        OpenAPI_list_for_each(ue_macsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ue_macsList);
        ue_macsList = NULL;
    }
    if (dnn_snssai_infosList) {
        OpenAPI_list_for_each(dnn_snssai_infosList, node) {
            OpenAPI_dnn_snssai_information_free(node->data);
        }
        OpenAPI_list_free(dnn_snssai_infosList);
        dnn_snssai_infosList = NULL;
    }
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

