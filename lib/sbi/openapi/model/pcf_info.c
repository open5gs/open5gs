
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pcf_info.h"

OpenAPI_pcf_info_t *OpenAPI_pcf_info_create(
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
    OpenAPI_pcf_info_t *pcf_info_local_var = ogs_malloc(sizeof(OpenAPI_pcf_info_t));
    ogs_assert(pcf_info_local_var);

    pcf_info_local_var->group_id = group_id;
    pcf_info_local_var->dnn_list = dnn_list;
    pcf_info_local_var->supi_ranges = supi_ranges;
    pcf_info_local_var->gpsi_ranges = gpsi_ranges;
    pcf_info_local_var->rx_diam_host = rx_diam_host;
    pcf_info_local_var->rx_diam_realm = rx_diam_realm;
    pcf_info_local_var->is_v2x_support_ind = is_v2x_support_ind;
    pcf_info_local_var->v2x_support_ind = v2x_support_ind;
    pcf_info_local_var->is_prose_support_ind = is_prose_support_ind;
    pcf_info_local_var->prose_support_ind = prose_support_ind;
    pcf_info_local_var->prose_capability = prose_capability;
    pcf_info_local_var->v2x_capability = v2x_capability;

    return pcf_info_local_var;
}

void OpenAPI_pcf_info_free(OpenAPI_pcf_info_t *pcf_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pcf_info) {
        return;
    }
    if (pcf_info->group_id) {
        ogs_free(pcf_info->group_id);
        pcf_info->group_id = NULL;
    }
    if (pcf_info->dnn_list) {
        OpenAPI_list_for_each(pcf_info->dnn_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(pcf_info->dnn_list);
        pcf_info->dnn_list = NULL;
    }
    if (pcf_info->supi_ranges) {
        OpenAPI_list_for_each(pcf_info->supi_ranges, node) {
            OpenAPI_supi_range_free(node->data);
        }
        OpenAPI_list_free(pcf_info->supi_ranges);
        pcf_info->supi_ranges = NULL;
    }
    if (pcf_info->gpsi_ranges) {
        OpenAPI_list_for_each(pcf_info->gpsi_ranges, node) {
            OpenAPI_identity_range_free(node->data);
        }
        OpenAPI_list_free(pcf_info->gpsi_ranges);
        pcf_info->gpsi_ranges = NULL;
    }
    if (pcf_info->rx_diam_host) {
        ogs_free(pcf_info->rx_diam_host);
        pcf_info->rx_diam_host = NULL;
    }
    if (pcf_info->rx_diam_realm) {
        ogs_free(pcf_info->rx_diam_realm);
        pcf_info->rx_diam_realm = NULL;
    }
    if (pcf_info->prose_capability) {
        OpenAPI_pro_se_capability_free(pcf_info->prose_capability);
        pcf_info->prose_capability = NULL;
    }
    if (pcf_info->v2x_capability) {
        OpenAPI_v2x_capability_free(pcf_info->v2x_capability);
        pcf_info->v2x_capability = NULL;
    }
    ogs_free(pcf_info);
}

cJSON *OpenAPI_pcf_info_convertToJSON(OpenAPI_pcf_info_t *pcf_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pcf_info == NULL) {
        ogs_error("OpenAPI_pcf_info_convertToJSON() failed [PcfInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (pcf_info->group_id) {
    if (cJSON_AddStringToObject(item, "groupId", pcf_info->group_id) == NULL) {
        ogs_error("OpenAPI_pcf_info_convertToJSON() failed [group_id]");
        goto end;
    }
    }

    if (pcf_info->dnn_list) {
    cJSON *dnn_listList = cJSON_AddArrayToObject(item, "dnnList");
    if (dnn_listList == NULL) {
        ogs_error("OpenAPI_pcf_info_convertToJSON() failed [dnn_list]");
        goto end;
    }
    OpenAPI_list_for_each(pcf_info->dnn_list, node) {
        if (cJSON_AddStringToObject(dnn_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_pcf_info_convertToJSON() failed [dnn_list]");
            goto end;
        }
    }
    }

    if (pcf_info->supi_ranges) {
    cJSON *supi_rangesList = cJSON_AddArrayToObject(item, "supiRanges");
    if (supi_rangesList == NULL) {
        ogs_error("OpenAPI_pcf_info_convertToJSON() failed [supi_ranges]");
        goto end;
    }
    OpenAPI_list_for_each(pcf_info->supi_ranges, node) {
        cJSON *itemLocal = OpenAPI_supi_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_pcf_info_convertToJSON() failed [supi_ranges]");
            goto end;
        }
        cJSON_AddItemToArray(supi_rangesList, itemLocal);
    }
    }

    if (pcf_info->gpsi_ranges) {
    cJSON *gpsi_rangesList = cJSON_AddArrayToObject(item, "gpsiRanges");
    if (gpsi_rangesList == NULL) {
        ogs_error("OpenAPI_pcf_info_convertToJSON() failed [gpsi_ranges]");
        goto end;
    }
    OpenAPI_list_for_each(pcf_info->gpsi_ranges, node) {
        cJSON *itemLocal = OpenAPI_identity_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_pcf_info_convertToJSON() failed [gpsi_ranges]");
            goto end;
        }
        cJSON_AddItemToArray(gpsi_rangesList, itemLocal);
    }
    }

    if (pcf_info->rx_diam_host) {
    if (cJSON_AddStringToObject(item, "rxDiamHost", pcf_info->rx_diam_host) == NULL) {
        ogs_error("OpenAPI_pcf_info_convertToJSON() failed [rx_diam_host]");
        goto end;
    }
    }

    if (pcf_info->rx_diam_realm) {
    if (cJSON_AddStringToObject(item, "rxDiamRealm", pcf_info->rx_diam_realm) == NULL) {
        ogs_error("OpenAPI_pcf_info_convertToJSON() failed [rx_diam_realm]");
        goto end;
    }
    }

    if (pcf_info->is_v2x_support_ind) {
    if (cJSON_AddBoolToObject(item, "v2xSupportInd", pcf_info->v2x_support_ind) == NULL) {
        ogs_error("OpenAPI_pcf_info_convertToJSON() failed [v2x_support_ind]");
        goto end;
    }
    }

    if (pcf_info->is_prose_support_ind) {
    if (cJSON_AddBoolToObject(item, "proseSupportInd", pcf_info->prose_support_ind) == NULL) {
        ogs_error("OpenAPI_pcf_info_convertToJSON() failed [prose_support_ind]");
        goto end;
    }
    }

    if (pcf_info->prose_capability) {
    cJSON *prose_capability_local_JSON = OpenAPI_pro_se_capability_convertToJSON(pcf_info->prose_capability);
    if (prose_capability_local_JSON == NULL) {
        ogs_error("OpenAPI_pcf_info_convertToJSON() failed [prose_capability]");
        goto end;
    }
    cJSON_AddItemToObject(item, "proseCapability", prose_capability_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pcf_info_convertToJSON() failed [prose_capability]");
        goto end;
    }
    }

    if (pcf_info->v2x_capability) {
    cJSON *v2x_capability_local_JSON = OpenAPI_v2x_capability_convertToJSON(pcf_info->v2x_capability);
    if (v2x_capability_local_JSON == NULL) {
        ogs_error("OpenAPI_pcf_info_convertToJSON() failed [v2x_capability]");
        goto end;
    }
    cJSON_AddItemToObject(item, "v2xCapability", v2x_capability_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pcf_info_convertToJSON() failed [v2x_capability]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_pcf_info_t *OpenAPI_pcf_info_parseFromJSON(cJSON *pcf_infoJSON)
{
    OpenAPI_pcf_info_t *pcf_info_local_var = NULL;
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
    group_id = cJSON_GetObjectItemCaseSensitive(pcf_infoJSON, "groupId");
    if (group_id) {
    if (!cJSON_IsString(group_id) && !cJSON_IsNull(group_id)) {
        ogs_error("OpenAPI_pcf_info_parseFromJSON() failed [group_id]");
        goto end;
    }
    }

    dnn_list = cJSON_GetObjectItemCaseSensitive(pcf_infoJSON, "dnnList");
    if (dnn_list) {
        cJSON *dnn_list_local = NULL;
        if (!cJSON_IsArray(dnn_list)) {
            ogs_error("OpenAPI_pcf_info_parseFromJSON() failed [dnn_list]");
            goto end;
        }

        dnn_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(dnn_list_local, dnn_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(dnn_list_local)) {
                ogs_error("OpenAPI_pcf_info_parseFromJSON() failed [dnn_list]");
                goto end;
            }
            OpenAPI_list_add(dnn_listList, ogs_strdup(dnn_list_local->valuestring));
        }
    }

    supi_ranges = cJSON_GetObjectItemCaseSensitive(pcf_infoJSON, "supiRanges");
    if (supi_ranges) {
        cJSON *supi_ranges_local = NULL;
        if (!cJSON_IsArray(supi_ranges)) {
            ogs_error("OpenAPI_pcf_info_parseFromJSON() failed [supi_ranges]");
            goto end;
        }

        supi_rangesList = OpenAPI_list_create();

        cJSON_ArrayForEach(supi_ranges_local, supi_ranges) {
            if (!cJSON_IsObject(supi_ranges_local)) {
                ogs_error("OpenAPI_pcf_info_parseFromJSON() failed [supi_ranges]");
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

    gpsi_ranges = cJSON_GetObjectItemCaseSensitive(pcf_infoJSON, "gpsiRanges");
    if (gpsi_ranges) {
        cJSON *gpsi_ranges_local = NULL;
        if (!cJSON_IsArray(gpsi_ranges)) {
            ogs_error("OpenAPI_pcf_info_parseFromJSON() failed [gpsi_ranges]");
            goto end;
        }

        gpsi_rangesList = OpenAPI_list_create();

        cJSON_ArrayForEach(gpsi_ranges_local, gpsi_ranges) {
            if (!cJSON_IsObject(gpsi_ranges_local)) {
                ogs_error("OpenAPI_pcf_info_parseFromJSON() failed [gpsi_ranges]");
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

    rx_diam_host = cJSON_GetObjectItemCaseSensitive(pcf_infoJSON, "rxDiamHost");
    if (rx_diam_host) {
    if (!cJSON_IsString(rx_diam_host) && !cJSON_IsNull(rx_diam_host)) {
        ogs_error("OpenAPI_pcf_info_parseFromJSON() failed [rx_diam_host]");
        goto end;
    }
    }

    rx_diam_realm = cJSON_GetObjectItemCaseSensitive(pcf_infoJSON, "rxDiamRealm");
    if (rx_diam_realm) {
    if (!cJSON_IsString(rx_diam_realm) && !cJSON_IsNull(rx_diam_realm)) {
        ogs_error("OpenAPI_pcf_info_parseFromJSON() failed [rx_diam_realm]");
        goto end;
    }
    }

    v2x_support_ind = cJSON_GetObjectItemCaseSensitive(pcf_infoJSON, "v2xSupportInd");
    if (v2x_support_ind) {
    if (!cJSON_IsBool(v2x_support_ind)) {
        ogs_error("OpenAPI_pcf_info_parseFromJSON() failed [v2x_support_ind]");
        goto end;
    }
    }

    prose_support_ind = cJSON_GetObjectItemCaseSensitive(pcf_infoJSON, "proseSupportInd");
    if (prose_support_ind) {
    if (!cJSON_IsBool(prose_support_ind)) {
        ogs_error("OpenAPI_pcf_info_parseFromJSON() failed [prose_support_ind]");
        goto end;
    }
    }

    prose_capability = cJSON_GetObjectItemCaseSensitive(pcf_infoJSON, "proseCapability");
    if (prose_capability) {
    prose_capability_local_nonprim = OpenAPI_pro_se_capability_parseFromJSON(prose_capability);
    if (!prose_capability_local_nonprim) {
        ogs_error("OpenAPI_pro_se_capability_parseFromJSON failed [prose_capability]");
        goto end;
    }
    }

    v2x_capability = cJSON_GetObjectItemCaseSensitive(pcf_infoJSON, "v2xCapability");
    if (v2x_capability) {
    v2x_capability_local_nonprim = OpenAPI_v2x_capability_parseFromJSON(v2x_capability);
    if (!v2x_capability_local_nonprim) {
        ogs_error("OpenAPI_v2x_capability_parseFromJSON failed [v2x_capability]");
        goto end;
    }
    }

    pcf_info_local_var = OpenAPI_pcf_info_create (
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

    return pcf_info_local_var;
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

OpenAPI_pcf_info_t *OpenAPI_pcf_info_copy(OpenAPI_pcf_info_t *dst, OpenAPI_pcf_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pcf_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pcf_info_convertToJSON() failed");
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

    OpenAPI_pcf_info_free(dst);
    dst = OpenAPI_pcf_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

