
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
    int v2x_support_ind
    )
{
    OpenAPI_pcf_info_t *pcf_info_local_var = OpenAPI_malloc(sizeof(OpenAPI_pcf_info_t));
    if (!pcf_info_local_var) {
        return NULL;
    }
    pcf_info_local_var->group_id = group_id;
    pcf_info_local_var->dnn_list = dnn_list;
    pcf_info_local_var->supi_ranges = supi_ranges;
    pcf_info_local_var->gpsi_ranges = gpsi_ranges;
    pcf_info_local_var->rx_diam_host = rx_diam_host;
    pcf_info_local_var->rx_diam_realm = rx_diam_realm;
    pcf_info_local_var->v2x_support_ind = v2x_support_ind;

    return pcf_info_local_var;
}

void OpenAPI_pcf_info_free(OpenAPI_pcf_info_t *pcf_info)
{
    if (NULL == pcf_info) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(pcf_info->group_id);
    OpenAPI_list_for_each(pcf_info->dnn_list, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(pcf_info->dnn_list);
    OpenAPI_list_for_each(pcf_info->supi_ranges, node) {
        OpenAPI_supi_range_free(node->data);
    }
    OpenAPI_list_free(pcf_info->supi_ranges);
    OpenAPI_list_for_each(pcf_info->gpsi_ranges, node) {
        OpenAPI_identity_range_free(node->data);
    }
    OpenAPI_list_free(pcf_info->gpsi_ranges);
    ogs_free(pcf_info->rx_diam_host);
    ogs_free(pcf_info->rx_diam_realm);
    ogs_free(pcf_info);
}

cJSON *OpenAPI_pcf_info_convertToJSON(OpenAPI_pcf_info_t *pcf_info)
{
    cJSON *item = NULL;

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
        cJSON *dnn_list = cJSON_AddArrayToObject(item, "dnnList");
        if (dnn_list == NULL) {
            ogs_error("OpenAPI_pcf_info_convertToJSON() failed [dnn_list]");
            goto end;
        }

        OpenAPI_lnode_t *dnn_list_node;
        OpenAPI_list_for_each(pcf_info->dnn_list, dnn_list_node)  {
            if (cJSON_AddStringToObject(dnn_list, "", (char*)dnn_list_node->data) == NULL) {
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

        OpenAPI_lnode_t *supi_ranges_node;
        if (pcf_info->supi_ranges) {
            OpenAPI_list_for_each(pcf_info->supi_ranges, supi_ranges_node) {
                cJSON *itemLocal = OpenAPI_supi_range_convertToJSON(supi_ranges_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_pcf_info_convertToJSON() failed [supi_ranges]");
                    goto end;
                }
                cJSON_AddItemToArray(supi_rangesList, itemLocal);
            }
        }
    }

    if (pcf_info->gpsi_ranges) {
        cJSON *gpsi_rangesList = cJSON_AddArrayToObject(item, "gpsiRanges");
        if (gpsi_rangesList == NULL) {
            ogs_error("OpenAPI_pcf_info_convertToJSON() failed [gpsi_ranges]");
            goto end;
        }

        OpenAPI_lnode_t *gpsi_ranges_node;
        if (pcf_info->gpsi_ranges) {
            OpenAPI_list_for_each(pcf_info->gpsi_ranges, gpsi_ranges_node) {
                cJSON *itemLocal = OpenAPI_identity_range_convertToJSON(gpsi_ranges_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_pcf_info_convertToJSON() failed [gpsi_ranges]");
                    goto end;
                }
                cJSON_AddItemToArray(gpsi_rangesList, itemLocal);
            }
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

    if (pcf_info->v2x_support_ind) {
        if (cJSON_AddBoolToObject(item, "v2xSupportInd", pcf_info->v2x_support_ind) == NULL) {
            ogs_error("OpenAPI_pcf_info_convertToJSON() failed [v2x_support_ind]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_pcf_info_t *OpenAPI_pcf_info_parseFromJSON(cJSON *pcf_infoJSON)
{
    OpenAPI_pcf_info_t *pcf_info_local_var = NULL;
    cJSON *group_id = cJSON_GetObjectItemCaseSensitive(pcf_infoJSON, "groupId");

    if (group_id) {
        if (!cJSON_IsString(group_id)) {
            ogs_error("OpenAPI_pcf_info_parseFromJSON() failed [group_id]");
            goto end;
        }
    }

    cJSON *dnn_list = cJSON_GetObjectItemCaseSensitive(pcf_infoJSON, "dnnList");

    OpenAPI_list_t *dnn_listList;
    if (dnn_list) {
        cJSON *dnn_list_local;
        if (!cJSON_IsArray(dnn_list)) {
            ogs_error("OpenAPI_pcf_info_parseFromJSON() failed [dnn_list]");
            goto end;
        }
        dnn_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(dnn_list_local, dnn_list) {
            if (!cJSON_IsString(dnn_list_local)) {
                ogs_error("OpenAPI_pcf_info_parseFromJSON() failed [dnn_list]");
                goto end;
            }
            OpenAPI_list_add(dnn_listList, ogs_strdup(dnn_list_local->valuestring));
        }
    }

    cJSON *supi_ranges = cJSON_GetObjectItemCaseSensitive(pcf_infoJSON, "supiRanges");

    OpenAPI_list_t *supi_rangesList;
    if (supi_ranges) {
        cJSON *supi_ranges_local_nonprimitive;
        if (!cJSON_IsArray(supi_ranges)) {
            ogs_error("OpenAPI_pcf_info_parseFromJSON() failed [supi_ranges]");
            goto end;
        }

        supi_rangesList = OpenAPI_list_create();

        cJSON_ArrayForEach(supi_ranges_local_nonprimitive, supi_ranges ) {
            if (!cJSON_IsObject(supi_ranges_local_nonprimitive)) {
                ogs_error("OpenAPI_pcf_info_parseFromJSON() failed [supi_ranges]");
                goto end;
            }
            OpenAPI_supi_range_t *supi_rangesItem = OpenAPI_supi_range_parseFromJSON(supi_ranges_local_nonprimitive);

            OpenAPI_list_add(supi_rangesList, supi_rangesItem);
        }
    }

    cJSON *gpsi_ranges = cJSON_GetObjectItemCaseSensitive(pcf_infoJSON, "gpsiRanges");

    OpenAPI_list_t *gpsi_rangesList;
    if (gpsi_ranges) {
        cJSON *gpsi_ranges_local_nonprimitive;
        if (!cJSON_IsArray(gpsi_ranges)) {
            ogs_error("OpenAPI_pcf_info_parseFromJSON() failed [gpsi_ranges]");
            goto end;
        }

        gpsi_rangesList = OpenAPI_list_create();

        cJSON_ArrayForEach(gpsi_ranges_local_nonprimitive, gpsi_ranges ) {
            if (!cJSON_IsObject(gpsi_ranges_local_nonprimitive)) {
                ogs_error("OpenAPI_pcf_info_parseFromJSON() failed [gpsi_ranges]");
                goto end;
            }
            OpenAPI_identity_range_t *gpsi_rangesItem = OpenAPI_identity_range_parseFromJSON(gpsi_ranges_local_nonprimitive);

            OpenAPI_list_add(gpsi_rangesList, gpsi_rangesItem);
        }
    }

    cJSON *rx_diam_host = cJSON_GetObjectItemCaseSensitive(pcf_infoJSON, "rxDiamHost");

    if (rx_diam_host) {
        if (!cJSON_IsString(rx_diam_host)) {
            ogs_error("OpenAPI_pcf_info_parseFromJSON() failed [rx_diam_host]");
            goto end;
        }
    }

    cJSON *rx_diam_realm = cJSON_GetObjectItemCaseSensitive(pcf_infoJSON, "rxDiamRealm");

    if (rx_diam_realm) {
        if (!cJSON_IsString(rx_diam_realm)) {
            ogs_error("OpenAPI_pcf_info_parseFromJSON() failed [rx_diam_realm]");
            goto end;
        }
    }

    cJSON *v2x_support_ind = cJSON_GetObjectItemCaseSensitive(pcf_infoJSON, "v2xSupportInd");

    if (v2x_support_ind) {
        if (!cJSON_IsBool(v2x_support_ind)) {
            ogs_error("OpenAPI_pcf_info_parseFromJSON() failed [v2x_support_ind]");
            goto end;
        }
    }

    pcf_info_local_var = OpenAPI_pcf_info_create (
        group_id ? ogs_strdup(group_id->valuestring) : NULL,
        dnn_list ? dnn_listList : NULL,
        supi_ranges ? supi_rangesList : NULL,
        gpsi_ranges ? gpsi_rangesList : NULL,
        rx_diam_host ? ogs_strdup(rx_diam_host->valuestring) : NULL,
        rx_diam_realm ? ogs_strdup(rx_diam_realm->valuestring) : NULL,
        v2x_support_ind ? v2x_support_ind->valueint : 0
        );

    return pcf_info_local_var;
end:
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

