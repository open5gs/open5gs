
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "congestion_info.h"

OpenAPI_congestion_info_t *OpenAPI_congestion_info_create(
    OpenAPI_congestion_type_t *cong_type,
    OpenAPI_time_window_t *time_intev,
    OpenAPI_threshold_level_t *nsi,
    bool is_confidence,
    int confidence,
    OpenAPI_list_t *top_app_list_ul,
    OpenAPI_list_t *top_app_list_dl
)
{
    OpenAPI_congestion_info_t *congestion_info_local_var = ogs_malloc(sizeof(OpenAPI_congestion_info_t));
    ogs_assert(congestion_info_local_var);

    congestion_info_local_var->cong_type = cong_type;
    congestion_info_local_var->time_intev = time_intev;
    congestion_info_local_var->nsi = nsi;
    congestion_info_local_var->is_confidence = is_confidence;
    congestion_info_local_var->confidence = confidence;
    congestion_info_local_var->top_app_list_ul = top_app_list_ul;
    congestion_info_local_var->top_app_list_dl = top_app_list_dl;

    return congestion_info_local_var;
}

void OpenAPI_congestion_info_free(OpenAPI_congestion_info_t *congestion_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == congestion_info) {
        return;
    }
    if (congestion_info->cong_type) {
        OpenAPI_congestion_type_free(congestion_info->cong_type);
        congestion_info->cong_type = NULL;
    }
    if (congestion_info->time_intev) {
        OpenAPI_time_window_free(congestion_info->time_intev);
        congestion_info->time_intev = NULL;
    }
    if (congestion_info->nsi) {
        OpenAPI_threshold_level_free(congestion_info->nsi);
        congestion_info->nsi = NULL;
    }
    if (congestion_info->top_app_list_ul) {
        OpenAPI_list_for_each(congestion_info->top_app_list_ul, node) {
            OpenAPI_top_application_free(node->data);
        }
        OpenAPI_list_free(congestion_info->top_app_list_ul);
        congestion_info->top_app_list_ul = NULL;
    }
    if (congestion_info->top_app_list_dl) {
        OpenAPI_list_for_each(congestion_info->top_app_list_dl, node) {
            OpenAPI_top_application_free(node->data);
        }
        OpenAPI_list_free(congestion_info->top_app_list_dl);
        congestion_info->top_app_list_dl = NULL;
    }
    ogs_free(congestion_info);
}

cJSON *OpenAPI_congestion_info_convertToJSON(OpenAPI_congestion_info_t *congestion_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (congestion_info == NULL) {
        ogs_error("OpenAPI_congestion_info_convertToJSON() failed [CongestionInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!congestion_info->cong_type) {
        ogs_error("OpenAPI_congestion_info_convertToJSON() failed [cong_type]");
        return NULL;
    }
    cJSON *cong_type_local_JSON = OpenAPI_congestion_type_convertToJSON(congestion_info->cong_type);
    if (cong_type_local_JSON == NULL) {
        ogs_error("OpenAPI_congestion_info_convertToJSON() failed [cong_type]");
        goto end;
    }
    cJSON_AddItemToObject(item, "congType", cong_type_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_congestion_info_convertToJSON() failed [cong_type]");
        goto end;
    }

    if (!congestion_info->time_intev) {
        ogs_error("OpenAPI_congestion_info_convertToJSON() failed [time_intev]");
        return NULL;
    }
    cJSON *time_intev_local_JSON = OpenAPI_time_window_convertToJSON(congestion_info->time_intev);
    if (time_intev_local_JSON == NULL) {
        ogs_error("OpenAPI_congestion_info_convertToJSON() failed [time_intev]");
        goto end;
    }
    cJSON_AddItemToObject(item, "timeIntev", time_intev_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_congestion_info_convertToJSON() failed [time_intev]");
        goto end;
    }

    if (!congestion_info->nsi) {
        ogs_error("OpenAPI_congestion_info_convertToJSON() failed [nsi]");
        return NULL;
    }
    cJSON *nsi_local_JSON = OpenAPI_threshold_level_convertToJSON(congestion_info->nsi);
    if (nsi_local_JSON == NULL) {
        ogs_error("OpenAPI_congestion_info_convertToJSON() failed [nsi]");
        goto end;
    }
    cJSON_AddItemToObject(item, "nsi", nsi_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_congestion_info_convertToJSON() failed [nsi]");
        goto end;
    }

    if (congestion_info->is_confidence) {
    if (cJSON_AddNumberToObject(item, "confidence", congestion_info->confidence) == NULL) {
        ogs_error("OpenAPI_congestion_info_convertToJSON() failed [confidence]");
        goto end;
    }
    }

    if (congestion_info->top_app_list_ul) {
    cJSON *top_app_list_ulList = cJSON_AddArrayToObject(item, "topAppListUl");
    if (top_app_list_ulList == NULL) {
        ogs_error("OpenAPI_congestion_info_convertToJSON() failed [top_app_list_ul]");
        goto end;
    }
    OpenAPI_list_for_each(congestion_info->top_app_list_ul, node) {
        cJSON *itemLocal = OpenAPI_top_application_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_congestion_info_convertToJSON() failed [top_app_list_ul]");
            goto end;
        }
        cJSON_AddItemToArray(top_app_list_ulList, itemLocal);
    }
    }

    if (congestion_info->top_app_list_dl) {
    cJSON *top_app_list_dlList = cJSON_AddArrayToObject(item, "topAppListDl");
    if (top_app_list_dlList == NULL) {
        ogs_error("OpenAPI_congestion_info_convertToJSON() failed [top_app_list_dl]");
        goto end;
    }
    OpenAPI_list_for_each(congestion_info->top_app_list_dl, node) {
        cJSON *itemLocal = OpenAPI_top_application_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_congestion_info_convertToJSON() failed [top_app_list_dl]");
            goto end;
        }
        cJSON_AddItemToArray(top_app_list_dlList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_congestion_info_t *OpenAPI_congestion_info_parseFromJSON(cJSON *congestion_infoJSON)
{
    OpenAPI_congestion_info_t *congestion_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *cong_type = NULL;
    OpenAPI_congestion_type_t *cong_type_local_nonprim = NULL;
    cJSON *time_intev = NULL;
    OpenAPI_time_window_t *time_intev_local_nonprim = NULL;
    cJSON *nsi = NULL;
    OpenAPI_threshold_level_t *nsi_local_nonprim = NULL;
    cJSON *confidence = NULL;
    cJSON *top_app_list_ul = NULL;
    OpenAPI_list_t *top_app_list_ulList = NULL;
    cJSON *top_app_list_dl = NULL;
    OpenAPI_list_t *top_app_list_dlList = NULL;
    cong_type = cJSON_GetObjectItemCaseSensitive(congestion_infoJSON, "congType");
    if (!cong_type) {
        ogs_error("OpenAPI_congestion_info_parseFromJSON() failed [cong_type]");
        goto end;
    }
    cong_type_local_nonprim = OpenAPI_congestion_type_parseFromJSON(cong_type);
    if (!cong_type_local_nonprim) {
        ogs_error("OpenAPI_congestion_type_parseFromJSON failed [cong_type]");
        goto end;
    }

    time_intev = cJSON_GetObjectItemCaseSensitive(congestion_infoJSON, "timeIntev");
    if (!time_intev) {
        ogs_error("OpenAPI_congestion_info_parseFromJSON() failed [time_intev]");
        goto end;
    }
    time_intev_local_nonprim = OpenAPI_time_window_parseFromJSON(time_intev);
    if (!time_intev_local_nonprim) {
        ogs_error("OpenAPI_time_window_parseFromJSON failed [time_intev]");
        goto end;
    }

    nsi = cJSON_GetObjectItemCaseSensitive(congestion_infoJSON, "nsi");
    if (!nsi) {
        ogs_error("OpenAPI_congestion_info_parseFromJSON() failed [nsi]");
        goto end;
    }
    nsi_local_nonprim = OpenAPI_threshold_level_parseFromJSON(nsi);
    if (!nsi_local_nonprim) {
        ogs_error("OpenAPI_threshold_level_parseFromJSON failed [nsi]");
        goto end;
    }

    confidence = cJSON_GetObjectItemCaseSensitive(congestion_infoJSON, "confidence");
    if (confidence) {
    if (!cJSON_IsNumber(confidence)) {
        ogs_error("OpenAPI_congestion_info_parseFromJSON() failed [confidence]");
        goto end;
    }
    }

    top_app_list_ul = cJSON_GetObjectItemCaseSensitive(congestion_infoJSON, "topAppListUl");
    if (top_app_list_ul) {
        cJSON *top_app_list_ul_local = NULL;
        if (!cJSON_IsArray(top_app_list_ul)) {
            ogs_error("OpenAPI_congestion_info_parseFromJSON() failed [top_app_list_ul]");
            goto end;
        }

        top_app_list_ulList = OpenAPI_list_create();

        cJSON_ArrayForEach(top_app_list_ul_local, top_app_list_ul) {
            if (!cJSON_IsObject(top_app_list_ul_local)) {
                ogs_error("OpenAPI_congestion_info_parseFromJSON() failed [top_app_list_ul]");
                goto end;
            }
            OpenAPI_top_application_t *top_app_list_ulItem = OpenAPI_top_application_parseFromJSON(top_app_list_ul_local);
            if (!top_app_list_ulItem) {
                ogs_error("No top_app_list_ulItem");
                goto end;
            }
            OpenAPI_list_add(top_app_list_ulList, top_app_list_ulItem);
        }
    }

    top_app_list_dl = cJSON_GetObjectItemCaseSensitive(congestion_infoJSON, "topAppListDl");
    if (top_app_list_dl) {
        cJSON *top_app_list_dl_local = NULL;
        if (!cJSON_IsArray(top_app_list_dl)) {
            ogs_error("OpenAPI_congestion_info_parseFromJSON() failed [top_app_list_dl]");
            goto end;
        }

        top_app_list_dlList = OpenAPI_list_create();

        cJSON_ArrayForEach(top_app_list_dl_local, top_app_list_dl) {
            if (!cJSON_IsObject(top_app_list_dl_local)) {
                ogs_error("OpenAPI_congestion_info_parseFromJSON() failed [top_app_list_dl]");
                goto end;
            }
            OpenAPI_top_application_t *top_app_list_dlItem = OpenAPI_top_application_parseFromJSON(top_app_list_dl_local);
            if (!top_app_list_dlItem) {
                ogs_error("No top_app_list_dlItem");
                goto end;
            }
            OpenAPI_list_add(top_app_list_dlList, top_app_list_dlItem);
        }
    }

    congestion_info_local_var = OpenAPI_congestion_info_create (
        cong_type_local_nonprim,
        time_intev_local_nonprim,
        nsi_local_nonprim,
        confidence ? true : false,
        confidence ? confidence->valuedouble : 0,
        top_app_list_ul ? top_app_list_ulList : NULL,
        top_app_list_dl ? top_app_list_dlList : NULL
    );

    return congestion_info_local_var;
end:
    if (cong_type_local_nonprim) {
        OpenAPI_congestion_type_free(cong_type_local_nonprim);
        cong_type_local_nonprim = NULL;
    }
    if (time_intev_local_nonprim) {
        OpenAPI_time_window_free(time_intev_local_nonprim);
        time_intev_local_nonprim = NULL;
    }
    if (nsi_local_nonprim) {
        OpenAPI_threshold_level_free(nsi_local_nonprim);
        nsi_local_nonprim = NULL;
    }
    if (top_app_list_ulList) {
        OpenAPI_list_for_each(top_app_list_ulList, node) {
            OpenAPI_top_application_free(node->data);
        }
        OpenAPI_list_free(top_app_list_ulList);
        top_app_list_ulList = NULL;
    }
    if (top_app_list_dlList) {
        OpenAPI_list_for_each(top_app_list_dlList, node) {
            OpenAPI_top_application_free(node->data);
        }
        OpenAPI_list_free(top_app_list_dlList);
        top_app_list_dlList = NULL;
    }
    return NULL;
}

OpenAPI_congestion_info_t *OpenAPI_congestion_info_copy(OpenAPI_congestion_info_t *dst, OpenAPI_congestion_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_congestion_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_congestion_info_convertToJSON() failed");
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

    OpenAPI_congestion_info_free(dst);
    dst = OpenAPI_congestion_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

