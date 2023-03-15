
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nrf_info_served_pcf_info_value.h"

OpenAPI_nrf_info_served_pcf_info_value_t *OpenAPI_nrf_info_served_pcf_info_value_create(
    char *group_id,
    OpenAPI_list_t *dnn_list,
    OpenAPI_list_t *supi_ranges,
    OpenAPI_list_t *gpsi_ranges,
    char *rx_diam_host,
    char *rx_diam_realm,
    bool is_v2x_support_ind,
    int v2x_support_ind,
    bool is_prose_support_ind,
    int prose_support_ind,
    OpenAPI_pro_se_capability_t *prose_capability,
    OpenAPI_v2x_capability_t *v2x_capability
)
{
    OpenAPI_nrf_info_served_pcf_info_value_t *nrf_info_served_pcf_info_value_local_var = ogs_malloc(sizeof(OpenAPI_nrf_info_served_pcf_info_value_t));
    ogs_assert(nrf_info_served_pcf_info_value_local_var);

    nrf_info_served_pcf_info_value_local_var->group_id = group_id;
    nrf_info_served_pcf_info_value_local_var->dnn_list = dnn_list;
    nrf_info_served_pcf_info_value_local_var->supi_ranges = supi_ranges;
    nrf_info_served_pcf_info_value_local_var->gpsi_ranges = gpsi_ranges;
    nrf_info_served_pcf_info_value_local_var->rx_diam_host = rx_diam_host;
    nrf_info_served_pcf_info_value_local_var->rx_diam_realm = rx_diam_realm;
    nrf_info_served_pcf_info_value_local_var->is_v2x_support_ind = is_v2x_support_ind;
    nrf_info_served_pcf_info_value_local_var->v2x_support_ind = v2x_support_ind;
    nrf_info_served_pcf_info_value_local_var->is_prose_support_ind = is_prose_support_ind;
    nrf_info_served_pcf_info_value_local_var->prose_support_ind = prose_support_ind;
    nrf_info_served_pcf_info_value_local_var->prose_capability = prose_capability;
    nrf_info_served_pcf_info_value_local_var->v2x_capability = v2x_capability;

    return nrf_info_served_pcf_info_value_local_var;
}

void OpenAPI_nrf_info_served_pcf_info_value_free(OpenAPI_nrf_info_served_pcf_info_value_t *nrf_info_served_pcf_info_value)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == nrf_info_served_pcf_info_value) {
        return;
    }
    if (nrf_info_served_pcf_info_value->group_id) {
        ogs_free(nrf_info_served_pcf_info_value->group_id);
        nrf_info_served_pcf_info_value->group_id = NULL;
    }
    if (nrf_info_served_pcf_info_value->dnn_list) {
        OpenAPI_list_for_each(nrf_info_served_pcf_info_value->dnn_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(nrf_info_served_pcf_info_value->dnn_list);
        nrf_info_served_pcf_info_value->dnn_list = NULL;
    }
    if (nrf_info_served_pcf_info_value->supi_ranges) {
        OpenAPI_list_for_each(nrf_info_served_pcf_info_value->supi_ranges, node) {
            OpenAPI_supi_range_free(node->data);
        }
        OpenAPI_list_free(nrf_info_served_pcf_info_value->supi_ranges);
        nrf_info_served_pcf_info_value->supi_ranges = NULL;
    }
    if (nrf_info_served_pcf_info_value->gpsi_ranges) {
        OpenAPI_list_for_each(nrf_info_served_pcf_info_value->gpsi_ranges, node) {
            OpenAPI_identity_range_free(node->data);
        }
        OpenAPI_list_free(nrf_info_served_pcf_info_value->gpsi_ranges);
        nrf_info_served_pcf_info_value->gpsi_ranges = NULL;
    }
    if (nrf_info_served_pcf_info_value->rx_diam_host) {
        ogs_free(nrf_info_served_pcf_info_value->rx_diam_host);
        nrf_info_served_pcf_info_value->rx_diam_host = NULL;
    }
    if (nrf_info_served_pcf_info_value->rx_diam_realm) {
        ogs_free(nrf_info_served_pcf_info_value->rx_diam_realm);
        nrf_info_served_pcf_info_value->rx_diam_realm = NULL;
    }
    if (nrf_info_served_pcf_info_value->prose_capability) {
        OpenAPI_pro_se_capability_free(nrf_info_served_pcf_info_value->prose_capability);
        nrf_info_served_pcf_info_value->prose_capability = NULL;
    }
    if (nrf_info_served_pcf_info_value->v2x_capability) {
        OpenAPI_v2x_capability_free(nrf_info_served_pcf_info_value->v2x_capability);
        nrf_info_served_pcf_info_value->v2x_capability = NULL;
    }
    ogs_free(nrf_info_served_pcf_info_value);
}

cJSON *OpenAPI_nrf_info_served_pcf_info_value_convertToJSON(OpenAPI_nrf_info_served_pcf_info_value_t *nrf_info_served_pcf_info_value)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (nrf_info_served_pcf_info_value == NULL) {
        ogs_error("OpenAPI_nrf_info_served_pcf_info_value_convertToJSON() failed [NrfInfo_servedPcfInfo_value]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (nrf_info_served_pcf_info_value->group_id) {
    if (cJSON_AddStringToObject(item, "groupId", nrf_info_served_pcf_info_value->group_id) == NULL) {
        ogs_error("OpenAPI_nrf_info_served_pcf_info_value_convertToJSON() failed [group_id]");
        goto end;
    }
    }

    if (nrf_info_served_pcf_info_value->dnn_list) {
    cJSON *dnn_listList = cJSON_AddArrayToObject(item, "dnnList");
    if (dnn_listList == NULL) {
        ogs_error("OpenAPI_nrf_info_served_pcf_info_value_convertToJSON() failed [dnn_list]");
        goto end;
    }
    OpenAPI_list_for_each(nrf_info_served_pcf_info_value->dnn_list, node) {
        if (cJSON_AddStringToObject(dnn_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_nrf_info_served_pcf_info_value_convertToJSON() failed [dnn_list]");
            goto end;
        }
    }
    }

    if (nrf_info_served_pcf_info_value->supi_ranges) {
    cJSON *supi_rangesList = cJSON_AddArrayToObject(item, "supiRanges");
    if (supi_rangesList == NULL) {
        ogs_error("OpenAPI_nrf_info_served_pcf_info_value_convertToJSON() failed [supi_ranges]");
        goto end;
    }
    OpenAPI_list_for_each(nrf_info_served_pcf_info_value->supi_ranges, node) {
        cJSON *itemLocal = OpenAPI_supi_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nrf_info_served_pcf_info_value_convertToJSON() failed [supi_ranges]");
            goto end;
        }
        cJSON_AddItemToArray(supi_rangesList, itemLocal);
    }
    }

    if (nrf_info_served_pcf_info_value->gpsi_ranges) {
    cJSON *gpsi_rangesList = cJSON_AddArrayToObject(item, "gpsiRanges");
    if (gpsi_rangesList == NULL) {
        ogs_error("OpenAPI_nrf_info_served_pcf_info_value_convertToJSON() failed [gpsi_ranges]");
        goto end;
    }
    OpenAPI_list_for_each(nrf_info_served_pcf_info_value->gpsi_ranges, node) {
        cJSON *itemLocal = OpenAPI_identity_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nrf_info_served_pcf_info_value_convertToJSON() failed [gpsi_ranges]");
            goto end;
        }
        cJSON_AddItemToArray(gpsi_rangesList, itemLocal);
    }
    }

    if (nrf_info_served_pcf_info_value->rx_diam_host) {
    if (cJSON_AddStringToObject(item, "rxDiamHost", nrf_info_served_pcf_info_value->rx_diam_host) == NULL) {
        ogs_error("OpenAPI_nrf_info_served_pcf_info_value_convertToJSON() failed [rx_diam_host]");
        goto end;
    }
    }

    if (nrf_info_served_pcf_info_value->rx_diam_realm) {
    if (cJSON_AddStringToObject(item, "rxDiamRealm", nrf_info_served_pcf_info_value->rx_diam_realm) == NULL) {
        ogs_error("OpenAPI_nrf_info_served_pcf_info_value_convertToJSON() failed [rx_diam_realm]");
        goto end;
    }
    }

    if (nrf_info_served_pcf_info_value->is_v2x_support_ind) {
    if (cJSON_AddBoolToObject(item, "v2xSupportInd", nrf_info_served_pcf_info_value->v2x_support_ind) == NULL) {
        ogs_error("OpenAPI_nrf_info_served_pcf_info_value_convertToJSON() failed [v2x_support_ind]");
        goto end;
    }
    }

    if (nrf_info_served_pcf_info_value->is_prose_support_ind) {
    if (cJSON_AddBoolToObject(item, "proseSupportInd", nrf_info_served_pcf_info_value->prose_support_ind) == NULL) {
        ogs_error("OpenAPI_nrf_info_served_pcf_info_value_convertToJSON() failed [prose_support_ind]");
        goto end;
    }
    }

    if (nrf_info_served_pcf_info_value->prose_capability) {
    cJSON *prose_capability_local_JSON = OpenAPI_pro_se_capability_convertToJSON(nrf_info_served_pcf_info_value->prose_capability);
    if (prose_capability_local_JSON == NULL) {
        ogs_error("OpenAPI_nrf_info_served_pcf_info_value_convertToJSON() failed [prose_capability]");
        goto end;
    }
    cJSON_AddItemToObject(item, "proseCapability", prose_capability_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_nrf_info_served_pcf_info_value_convertToJSON() failed [prose_capability]");
        goto end;
    }
    }

    if (nrf_info_served_pcf_info_value->v2x_capability) {
    cJSON *v2x_capability_local_JSON = OpenAPI_v2x_capability_convertToJSON(nrf_info_served_pcf_info_value->v2x_capability);
    if (v2x_capability_local_JSON == NULL) {
        ogs_error("OpenAPI_nrf_info_served_pcf_info_value_convertToJSON() failed [v2x_capability]");
        goto end;
    }
    cJSON_AddItemToObject(item, "v2xCapability", v2x_capability_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_nrf_info_served_pcf_info_value_convertToJSON() failed [v2x_capability]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_nrf_info_served_pcf_info_value_t *OpenAPI_nrf_info_served_pcf_info_value_parseFromJSON(cJSON *nrf_info_served_pcf_info_valueJSON)
{
    OpenAPI_nrf_info_served_pcf_info_value_t *nrf_info_served_pcf_info_value_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *group_id = NULL;
    cJSON *dnn_list = NULL;
    OpenAPI_list_t *dnn_listList = NULL;
    cJSON *supi_ranges = NULL;
    OpenAPI_list_t *supi_rangesList = NULL;
    cJSON *gpsi_ranges = NULL;
    OpenAPI_list_t *gpsi_rangesList = NULL;
    cJSON *rx_diam_host = NULL;
    cJSON *rx_diam_realm = NULL;
    cJSON *v2x_support_ind = NULL;
    cJSON *prose_support_ind = NULL;
    cJSON *prose_capability = NULL;
    OpenAPI_pro_se_capability_t *prose_capability_local_nonprim = NULL;
    cJSON *v2x_capability = NULL;
    OpenAPI_v2x_capability_t *v2x_capability_local_nonprim = NULL;
    group_id = cJSON_GetObjectItemCaseSensitive(nrf_info_served_pcf_info_valueJSON, "groupId");
    if (group_id) {
    if (!cJSON_IsString(group_id) && !cJSON_IsNull(group_id)) {
        ogs_error("OpenAPI_nrf_info_served_pcf_info_value_parseFromJSON() failed [group_id]");
        goto end;
    }
    }

    dnn_list = cJSON_GetObjectItemCaseSensitive(nrf_info_served_pcf_info_valueJSON, "dnnList");
    if (dnn_list) {
        cJSON *dnn_list_local = NULL;
        if (!cJSON_IsArray(dnn_list)) {
            ogs_error("OpenAPI_nrf_info_served_pcf_info_value_parseFromJSON() failed [dnn_list]");
            goto end;
        }

        dnn_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(dnn_list_local, dnn_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(dnn_list_local)) {
                ogs_error("OpenAPI_nrf_info_served_pcf_info_value_parseFromJSON() failed [dnn_list]");
                goto end;
            }
            OpenAPI_list_add(dnn_listList, ogs_strdup(dnn_list_local->valuestring));
        }
    }

    supi_ranges = cJSON_GetObjectItemCaseSensitive(nrf_info_served_pcf_info_valueJSON, "supiRanges");
    if (supi_ranges) {
        cJSON *supi_ranges_local = NULL;
        if (!cJSON_IsArray(supi_ranges)) {
            ogs_error("OpenAPI_nrf_info_served_pcf_info_value_parseFromJSON() failed [supi_ranges]");
            goto end;
        }

        supi_rangesList = OpenAPI_list_create();

        cJSON_ArrayForEach(supi_ranges_local, supi_ranges) {
            if (!cJSON_IsObject(supi_ranges_local)) {
                ogs_error("OpenAPI_nrf_info_served_pcf_info_value_parseFromJSON() failed [supi_ranges]");
                goto end;
            }
            OpenAPI_supi_range_t *supi_rangesItem = OpenAPI_supi_range_parseFromJSON(supi_ranges_local);
            if (!supi_rangesItem) {
                ogs_error("No supi_rangesItem");
                goto end;
            }
            OpenAPI_list_add(supi_rangesList, supi_rangesItem);
        }
    }

    gpsi_ranges = cJSON_GetObjectItemCaseSensitive(nrf_info_served_pcf_info_valueJSON, "gpsiRanges");
    if (gpsi_ranges) {
        cJSON *gpsi_ranges_local = NULL;
        if (!cJSON_IsArray(gpsi_ranges)) {
            ogs_error("OpenAPI_nrf_info_served_pcf_info_value_parseFromJSON() failed [gpsi_ranges]");
            goto end;
        }

        gpsi_rangesList = OpenAPI_list_create();

        cJSON_ArrayForEach(gpsi_ranges_local, gpsi_ranges) {
            if (!cJSON_IsObject(gpsi_ranges_local)) {
                ogs_error("OpenAPI_nrf_info_served_pcf_info_value_parseFromJSON() failed [gpsi_ranges]");
                goto end;
            }
            OpenAPI_identity_range_t *gpsi_rangesItem = OpenAPI_identity_range_parseFromJSON(gpsi_ranges_local);
            if (!gpsi_rangesItem) {
                ogs_error("No gpsi_rangesItem");
                goto end;
            }
            OpenAPI_list_add(gpsi_rangesList, gpsi_rangesItem);
        }
    }

    rx_diam_host = cJSON_GetObjectItemCaseSensitive(nrf_info_served_pcf_info_valueJSON, "rxDiamHost");
    if (rx_diam_host) {
    if (!cJSON_IsString(rx_diam_host) && !cJSON_IsNull(rx_diam_host)) {
        ogs_error("OpenAPI_nrf_info_served_pcf_info_value_parseFromJSON() failed [rx_diam_host]");
        goto end;
    }
    }

    rx_diam_realm = cJSON_GetObjectItemCaseSensitive(nrf_info_served_pcf_info_valueJSON, "rxDiamRealm");
    if (rx_diam_realm) {
    if (!cJSON_IsString(rx_diam_realm) && !cJSON_IsNull(rx_diam_realm)) {
        ogs_error("OpenAPI_nrf_info_served_pcf_info_value_parseFromJSON() failed [rx_diam_realm]");
        goto end;
    }
    }

    v2x_support_ind = cJSON_GetObjectItemCaseSensitive(nrf_info_served_pcf_info_valueJSON, "v2xSupportInd");
    if (v2x_support_ind) {
    if (!cJSON_IsBool(v2x_support_ind)) {
        ogs_error("OpenAPI_nrf_info_served_pcf_info_value_parseFromJSON() failed [v2x_support_ind]");
        goto end;
    }
    }

    prose_support_ind = cJSON_GetObjectItemCaseSensitive(nrf_info_served_pcf_info_valueJSON, "proseSupportInd");
    if (prose_support_ind) {
    if (!cJSON_IsBool(prose_support_ind)) {
        ogs_error("OpenAPI_nrf_info_served_pcf_info_value_parseFromJSON() failed [prose_support_ind]");
        goto end;
    }
    }

    prose_capability = cJSON_GetObjectItemCaseSensitive(nrf_info_served_pcf_info_valueJSON, "proseCapability");
    if (prose_capability) {
    prose_capability_local_nonprim = OpenAPI_pro_se_capability_parseFromJSON(prose_capability);
    if (!prose_capability_local_nonprim) {
        ogs_error("OpenAPI_pro_se_capability_parseFromJSON failed [prose_capability]");
        goto end;
    }
    }

    v2x_capability = cJSON_GetObjectItemCaseSensitive(nrf_info_served_pcf_info_valueJSON, "v2xCapability");
    if (v2x_capability) {
    v2x_capability_local_nonprim = OpenAPI_v2x_capability_parseFromJSON(v2x_capability);
    if (!v2x_capability_local_nonprim) {
        ogs_error("OpenAPI_v2x_capability_parseFromJSON failed [v2x_capability]");
        goto end;
    }
    }

    nrf_info_served_pcf_info_value_local_var = OpenAPI_nrf_info_served_pcf_info_value_create (
        group_id && !cJSON_IsNull(group_id) ? ogs_strdup(group_id->valuestring) : NULL,
        dnn_list ? dnn_listList : NULL,
        supi_ranges ? supi_rangesList : NULL,
        gpsi_ranges ? gpsi_rangesList : NULL,
        rx_diam_host && !cJSON_IsNull(rx_diam_host) ? ogs_strdup(rx_diam_host->valuestring) : NULL,
        rx_diam_realm && !cJSON_IsNull(rx_diam_realm) ? ogs_strdup(rx_diam_realm->valuestring) : NULL,
        v2x_support_ind ? true : false,
        v2x_support_ind ? v2x_support_ind->valueint : 0,
        prose_support_ind ? true : false,
        prose_support_ind ? prose_support_ind->valueint : 0,
        prose_capability ? prose_capability_local_nonprim : NULL,
        v2x_capability ? v2x_capability_local_nonprim : NULL
    );

    return nrf_info_served_pcf_info_value_local_var;
end:
    if (dnn_listList) {
        OpenAPI_list_for_each(dnn_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(dnn_listList);
        dnn_listList = NULL;
    }
    if (supi_rangesList) {
        OpenAPI_list_for_each(supi_rangesList, node) {
            OpenAPI_supi_range_free(node->data);
        }
        OpenAPI_list_free(supi_rangesList);
        supi_rangesList = NULL;
    }
    if (gpsi_rangesList) {
        OpenAPI_list_for_each(gpsi_rangesList, node) {
            OpenAPI_identity_range_free(node->data);
        }
        OpenAPI_list_free(gpsi_rangesList);
        gpsi_rangesList = NULL;
    }
    if (prose_capability_local_nonprim) {
        OpenAPI_pro_se_capability_free(prose_capability_local_nonprim);
        prose_capability_local_nonprim = NULL;
    }
    if (v2x_capability_local_nonprim) {
        OpenAPI_v2x_capability_free(v2x_capability_local_nonprim);
        v2x_capability_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_nrf_info_served_pcf_info_value_t *OpenAPI_nrf_info_served_pcf_info_value_copy(OpenAPI_nrf_info_served_pcf_info_value_t *dst, OpenAPI_nrf_info_served_pcf_info_value_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nrf_info_served_pcf_info_value_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nrf_info_served_pcf_info_value_convertToJSON() failed");
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

    OpenAPI_nrf_info_served_pcf_info_value_free(dst);
    dst = OpenAPI_nrf_info_served_pcf_info_value_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

