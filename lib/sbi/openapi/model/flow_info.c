
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "flow_info.h"

OpenAPI_flow_info_t *OpenAPI_flow_info_create(
    int flow_id,
    OpenAPI_list_t *flow_descriptions,
    char *tos_tc,
    OpenAPI_list_t *mpx_media_ul_infos,
    OpenAPI_list_t *mpx_media_dl_infos
)
{
    OpenAPI_flow_info_t *flow_info_local_var = ogs_malloc(sizeof(OpenAPI_flow_info_t));
    ogs_assert(flow_info_local_var);

    flow_info_local_var->flow_id = flow_id;
    flow_info_local_var->flow_descriptions = flow_descriptions;
    flow_info_local_var->tos_tc = tos_tc;
    flow_info_local_var->mpx_media_ul_infos = mpx_media_ul_infos;
    flow_info_local_var->mpx_media_dl_infos = mpx_media_dl_infos;

    return flow_info_local_var;
}

void OpenAPI_flow_info_free(OpenAPI_flow_info_t *flow_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == flow_info) {
        return;
    }
    if (flow_info->flow_descriptions) {
        OpenAPI_list_for_each(flow_info->flow_descriptions, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(flow_info->flow_descriptions);
        flow_info->flow_descriptions = NULL;
    }
    if (flow_info->tos_tc) {
        ogs_free(flow_info->tos_tc);
        flow_info->tos_tc = NULL;
    }
    if (flow_info->mpx_media_ul_infos) {
        OpenAPI_list_for_each(flow_info->mpx_media_ul_infos, node) {
            OpenAPI_mpx_media_info_free(node->data);
        }
        OpenAPI_list_free(flow_info->mpx_media_ul_infos);
        flow_info->mpx_media_ul_infos = NULL;
    }
    if (flow_info->mpx_media_dl_infos) {
        OpenAPI_list_for_each(flow_info->mpx_media_dl_infos, node) {
            OpenAPI_mpx_media_info_free(node->data);
        }
        OpenAPI_list_free(flow_info->mpx_media_dl_infos);
        flow_info->mpx_media_dl_infos = NULL;
    }
    ogs_free(flow_info);
}

cJSON *OpenAPI_flow_info_convertToJSON(OpenAPI_flow_info_t *flow_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (flow_info == NULL) {
        ogs_error("OpenAPI_flow_info_convertToJSON() failed [FlowInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "flowId", flow_info->flow_id) == NULL) {
        ogs_error("OpenAPI_flow_info_convertToJSON() failed [flow_id]");
        goto end;
    }

    if (flow_info->flow_descriptions) {
    cJSON *flow_descriptionsList = cJSON_AddArrayToObject(item, "flowDescriptions");
    if (flow_descriptionsList == NULL) {
        ogs_error("OpenAPI_flow_info_convertToJSON() failed [flow_descriptions]");
        goto end;
    }
    OpenAPI_list_for_each(flow_info->flow_descriptions, node) {
        if (cJSON_AddStringToObject(flow_descriptionsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_flow_info_convertToJSON() failed [flow_descriptions]");
            goto end;
        }
    }
    }

    if (flow_info->tos_tc) {
    if (cJSON_AddStringToObject(item, "tosTC", flow_info->tos_tc) == NULL) {
        ogs_error("OpenAPI_flow_info_convertToJSON() failed [tos_tc]");
        goto end;
    }
    }

    if (flow_info->mpx_media_ul_infos) {
    cJSON *mpx_media_ul_infosList = cJSON_AddArrayToObject(item, "mpxMediaUlInfos");
    if (mpx_media_ul_infosList == NULL) {
        ogs_error("OpenAPI_flow_info_convertToJSON() failed [mpx_media_ul_infos]");
        goto end;
    }
    OpenAPI_list_for_each(flow_info->mpx_media_ul_infos, node) {
        cJSON *itemLocal = OpenAPI_mpx_media_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_flow_info_convertToJSON() failed [mpx_media_ul_infos]");
            goto end;
        }
        cJSON_AddItemToArray(mpx_media_ul_infosList, itemLocal);
    }
    }

    if (flow_info->mpx_media_dl_infos) {
    cJSON *mpx_media_dl_infosList = cJSON_AddArrayToObject(item, "mpxMediaDlInfos");
    if (mpx_media_dl_infosList == NULL) {
        ogs_error("OpenAPI_flow_info_convertToJSON() failed [mpx_media_dl_infos]");
        goto end;
    }
    OpenAPI_list_for_each(flow_info->mpx_media_dl_infos, node) {
        cJSON *itemLocal = OpenAPI_mpx_media_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_flow_info_convertToJSON() failed [mpx_media_dl_infos]");
            goto end;
        }
        cJSON_AddItemToArray(mpx_media_dl_infosList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_flow_info_t *OpenAPI_flow_info_parseFromJSON(cJSON *flow_infoJSON)
{
    OpenAPI_flow_info_t *flow_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *flow_id = NULL;
    cJSON *flow_descriptions = NULL;
    OpenAPI_list_t *flow_descriptionsList = NULL;
    cJSON *tos_tc = NULL;
    cJSON *mpx_media_ul_infos = NULL;
    OpenAPI_list_t *mpx_media_ul_infosList = NULL;
    cJSON *mpx_media_dl_infos = NULL;
    OpenAPI_list_t *mpx_media_dl_infosList = NULL;
    flow_id = cJSON_GetObjectItemCaseSensitive(flow_infoJSON, "flowId");
    if (!flow_id) {
        ogs_error("OpenAPI_flow_info_parseFromJSON() failed [flow_id]");
        goto end;
    }
    if (!cJSON_IsNumber(flow_id)) {
        ogs_error("OpenAPI_flow_info_parseFromJSON() failed [flow_id]");
        goto end;
    }

    flow_descriptions = cJSON_GetObjectItemCaseSensitive(flow_infoJSON, "flowDescriptions");
    if (flow_descriptions) {
        cJSON *flow_descriptions_local = NULL;
        if (!cJSON_IsArray(flow_descriptions)) {
            ogs_error("OpenAPI_flow_info_parseFromJSON() failed [flow_descriptions]");
            goto end;
        }

        flow_descriptionsList = OpenAPI_list_create();

        cJSON_ArrayForEach(flow_descriptions_local, flow_descriptions) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(flow_descriptions_local)) {
                ogs_error("OpenAPI_flow_info_parseFromJSON() failed [flow_descriptions]");
                goto end;
            }
            OpenAPI_list_add(flow_descriptionsList, ogs_strdup(flow_descriptions_local->valuestring));
        }
    }

    tos_tc = cJSON_GetObjectItemCaseSensitive(flow_infoJSON, "tosTC");
    if (tos_tc) {
    if (!cJSON_IsString(tos_tc) && !cJSON_IsNull(tos_tc)) {
        ogs_error("OpenAPI_flow_info_parseFromJSON() failed [tos_tc]");
        goto end;
    }
    }

    mpx_media_ul_infos = cJSON_GetObjectItemCaseSensitive(flow_infoJSON, "mpxMediaUlInfos");
    if (mpx_media_ul_infos) {
        cJSON *mpx_media_ul_infos_local = NULL;
        if (!cJSON_IsArray(mpx_media_ul_infos)) {
            ogs_error("OpenAPI_flow_info_parseFromJSON() failed [mpx_media_ul_infos]");
            goto end;
        }

        mpx_media_ul_infosList = OpenAPI_list_create();

        cJSON_ArrayForEach(mpx_media_ul_infos_local, mpx_media_ul_infos) {
            if (!cJSON_IsObject(mpx_media_ul_infos_local)) {
                ogs_error("OpenAPI_flow_info_parseFromJSON() failed [mpx_media_ul_infos]");
                goto end;
            }
            OpenAPI_mpx_media_info_t *mpx_media_ul_infosItem = OpenAPI_mpx_media_info_parseFromJSON(mpx_media_ul_infos_local);
            if (!mpx_media_ul_infosItem) {
                ogs_error("No mpx_media_ul_infosItem");
                goto end;
            }
            OpenAPI_list_add(mpx_media_ul_infosList, mpx_media_ul_infosItem);
        }
    }

    mpx_media_dl_infos = cJSON_GetObjectItemCaseSensitive(flow_infoJSON, "mpxMediaDlInfos");
    if (mpx_media_dl_infos) {
        cJSON *mpx_media_dl_infos_local = NULL;
        if (!cJSON_IsArray(mpx_media_dl_infos)) {
            ogs_error("OpenAPI_flow_info_parseFromJSON() failed [mpx_media_dl_infos]");
            goto end;
        }

        mpx_media_dl_infosList = OpenAPI_list_create();

        cJSON_ArrayForEach(mpx_media_dl_infos_local, mpx_media_dl_infos) {
            if (!cJSON_IsObject(mpx_media_dl_infos_local)) {
                ogs_error("OpenAPI_flow_info_parseFromJSON() failed [mpx_media_dl_infos]");
                goto end;
            }
            OpenAPI_mpx_media_info_t *mpx_media_dl_infosItem = OpenAPI_mpx_media_info_parseFromJSON(mpx_media_dl_infos_local);
            if (!mpx_media_dl_infosItem) {
                ogs_error("No mpx_media_dl_infosItem");
                goto end;
            }
            OpenAPI_list_add(mpx_media_dl_infosList, mpx_media_dl_infosItem);
        }
    }

    flow_info_local_var = OpenAPI_flow_info_create (
        
        flow_id->valuedouble,
        flow_descriptions ? flow_descriptionsList : NULL,
        tos_tc && !cJSON_IsNull(tos_tc) ? ogs_strdup(tos_tc->valuestring) : NULL,
        mpx_media_ul_infos ? mpx_media_ul_infosList : NULL,
        mpx_media_dl_infos ? mpx_media_dl_infosList : NULL
    );

    return flow_info_local_var;
end:
    if (flow_descriptionsList) {
        OpenAPI_list_for_each(flow_descriptionsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(flow_descriptionsList);
        flow_descriptionsList = NULL;
    }
    if (mpx_media_ul_infosList) {
        OpenAPI_list_for_each(mpx_media_ul_infosList, node) {
            OpenAPI_mpx_media_info_free(node->data);
        }
        OpenAPI_list_free(mpx_media_ul_infosList);
        mpx_media_ul_infosList = NULL;
    }
    if (mpx_media_dl_infosList) {
        OpenAPI_list_for_each(mpx_media_dl_infosList, node) {
            OpenAPI_mpx_media_info_free(node->data);
        }
        OpenAPI_list_free(mpx_media_dl_infosList);
        mpx_media_dl_infosList = NULL;
    }
    return NULL;
}

OpenAPI_flow_info_t *OpenAPI_flow_info_copy(OpenAPI_flow_info_t *dst, OpenAPI_flow_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_flow_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_flow_info_convertToJSON() failed");
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

    OpenAPI_flow_info_free(dst);
    dst = OpenAPI_flow_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

