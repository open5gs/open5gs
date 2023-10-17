
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ml_analytics_info.h"

OpenAPI_ml_analytics_info_t *OpenAPI_ml_analytics_info_create(
    OpenAPI_list_t *ml_analytics_ids,
    OpenAPI_list_t *snssai_list,
    OpenAPI_list_t *tracking_area_list
)
{
    OpenAPI_ml_analytics_info_t *ml_analytics_info_local_var = ogs_malloc(sizeof(OpenAPI_ml_analytics_info_t));
    ogs_assert(ml_analytics_info_local_var);

    ml_analytics_info_local_var->ml_analytics_ids = ml_analytics_ids;
    ml_analytics_info_local_var->snssai_list = snssai_list;
    ml_analytics_info_local_var->tracking_area_list = tracking_area_list;

    return ml_analytics_info_local_var;
}

void OpenAPI_ml_analytics_info_free(OpenAPI_ml_analytics_info_t *ml_analytics_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ml_analytics_info) {
        return;
    }
    if (ml_analytics_info->ml_analytics_ids) {
        OpenAPI_list_for_each(ml_analytics_info->ml_analytics_ids, node) {
            OpenAPI_nwdaf_event_free(node->data);
        }
        OpenAPI_list_free(ml_analytics_info->ml_analytics_ids);
        ml_analytics_info->ml_analytics_ids = NULL;
    }
    if (ml_analytics_info->snssai_list) {
        OpenAPI_list_for_each(ml_analytics_info->snssai_list, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(ml_analytics_info->snssai_list);
        ml_analytics_info->snssai_list = NULL;
    }
    if (ml_analytics_info->tracking_area_list) {
        OpenAPI_list_for_each(ml_analytics_info->tracking_area_list, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(ml_analytics_info->tracking_area_list);
        ml_analytics_info->tracking_area_list = NULL;
    }
    ogs_free(ml_analytics_info);
}

cJSON *OpenAPI_ml_analytics_info_convertToJSON(OpenAPI_ml_analytics_info_t *ml_analytics_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ml_analytics_info == NULL) {
        ogs_error("OpenAPI_ml_analytics_info_convertToJSON() failed [MlAnalyticsInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ml_analytics_info->ml_analytics_ids) {
    cJSON *ml_analytics_idsList = cJSON_AddArrayToObject(item, "mlAnalyticsIds");
    if (ml_analytics_idsList == NULL) {
        ogs_error("OpenAPI_ml_analytics_info_convertToJSON() failed [ml_analytics_ids]");
        goto end;
    }
    OpenAPI_list_for_each(ml_analytics_info->ml_analytics_ids, node) {
        cJSON *itemLocal = OpenAPI_nwdaf_event_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_ml_analytics_info_convertToJSON() failed [ml_analytics_ids]");
            goto end;
        }
        cJSON_AddItemToArray(ml_analytics_idsList, itemLocal);
    }
    }

    if (ml_analytics_info->snssai_list) {
    cJSON *snssai_listList = cJSON_AddArrayToObject(item, "snssaiList");
    if (snssai_listList == NULL) {
        ogs_error("OpenAPI_ml_analytics_info_convertToJSON() failed [snssai_list]");
        goto end;
    }
    OpenAPI_list_for_each(ml_analytics_info->snssai_list, node) {
        cJSON *itemLocal = OpenAPI_snssai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_ml_analytics_info_convertToJSON() failed [snssai_list]");
            goto end;
        }
        cJSON_AddItemToArray(snssai_listList, itemLocal);
    }
    }

    if (ml_analytics_info->tracking_area_list) {
    cJSON *tracking_area_listList = cJSON_AddArrayToObject(item, "trackingAreaList");
    if (tracking_area_listList == NULL) {
        ogs_error("OpenAPI_ml_analytics_info_convertToJSON() failed [tracking_area_list]");
        goto end;
    }
    OpenAPI_list_for_each(ml_analytics_info->tracking_area_list, node) {
        cJSON *itemLocal = OpenAPI_tai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_ml_analytics_info_convertToJSON() failed [tracking_area_list]");
            goto end;
        }
        cJSON_AddItemToArray(tracking_area_listList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_ml_analytics_info_t *OpenAPI_ml_analytics_info_parseFromJSON(cJSON *ml_analytics_infoJSON)
{
    OpenAPI_ml_analytics_info_t *ml_analytics_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ml_analytics_ids = NULL;
    OpenAPI_list_t *ml_analytics_idsList = NULL;
    cJSON *snssai_list = NULL;
    OpenAPI_list_t *snssai_listList = NULL;
    cJSON *tracking_area_list = NULL;
    OpenAPI_list_t *tracking_area_listList = NULL;
    ml_analytics_ids = cJSON_GetObjectItemCaseSensitive(ml_analytics_infoJSON, "mlAnalyticsIds");
    if (ml_analytics_ids) {
        cJSON *ml_analytics_ids_local = NULL;
        if (!cJSON_IsArray(ml_analytics_ids)) {
            ogs_error("OpenAPI_ml_analytics_info_parseFromJSON() failed [ml_analytics_ids]");
            goto end;
        }

        ml_analytics_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(ml_analytics_ids_local, ml_analytics_ids) {
            if (!cJSON_IsObject(ml_analytics_ids_local)) {
                ogs_error("OpenAPI_ml_analytics_info_parseFromJSON() failed [ml_analytics_ids]");
                goto end;
            }
            OpenAPI_nwdaf_event_t *ml_analytics_idsItem = OpenAPI_nwdaf_event_parseFromJSON(ml_analytics_ids_local);
            if (!ml_analytics_idsItem) {
                ogs_error("No ml_analytics_idsItem");
                goto end;
            }
            OpenAPI_list_add(ml_analytics_idsList, ml_analytics_idsItem);
        }
    }

    snssai_list = cJSON_GetObjectItemCaseSensitive(ml_analytics_infoJSON, "snssaiList");
    if (snssai_list) {
        cJSON *snssai_list_local = NULL;
        if (!cJSON_IsArray(snssai_list)) {
            ogs_error("OpenAPI_ml_analytics_info_parseFromJSON() failed [snssai_list]");
            goto end;
        }

        snssai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(snssai_list_local, snssai_list) {
            if (!cJSON_IsObject(snssai_list_local)) {
                ogs_error("OpenAPI_ml_analytics_info_parseFromJSON() failed [snssai_list]");
                goto end;
            }
            OpenAPI_snssai_t *snssai_listItem = OpenAPI_snssai_parseFromJSON(snssai_list_local);
            if (!snssai_listItem) {
                ogs_error("No snssai_listItem");
                goto end;
            }
            OpenAPI_list_add(snssai_listList, snssai_listItem);
        }
    }

    tracking_area_list = cJSON_GetObjectItemCaseSensitive(ml_analytics_infoJSON, "trackingAreaList");
    if (tracking_area_list) {
        cJSON *tracking_area_list_local = NULL;
        if (!cJSON_IsArray(tracking_area_list)) {
            ogs_error("OpenAPI_ml_analytics_info_parseFromJSON() failed [tracking_area_list]");
            goto end;
        }

        tracking_area_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tracking_area_list_local, tracking_area_list) {
            if (!cJSON_IsObject(tracking_area_list_local)) {
                ogs_error("OpenAPI_ml_analytics_info_parseFromJSON() failed [tracking_area_list]");
                goto end;
            }
            OpenAPI_tai_t *tracking_area_listItem = OpenAPI_tai_parseFromJSON(tracking_area_list_local);
            if (!tracking_area_listItem) {
                ogs_error("No tracking_area_listItem");
                goto end;
            }
            OpenAPI_list_add(tracking_area_listList, tracking_area_listItem);
        }
    }

    ml_analytics_info_local_var = OpenAPI_ml_analytics_info_create (
        ml_analytics_ids ? ml_analytics_idsList : NULL,
        snssai_list ? snssai_listList : NULL,
        tracking_area_list ? tracking_area_listList : NULL
    );

    return ml_analytics_info_local_var;
end:
    if (ml_analytics_idsList) {
        OpenAPI_list_for_each(ml_analytics_idsList, node) {
            OpenAPI_nwdaf_event_free(node->data);
        }
        OpenAPI_list_free(ml_analytics_idsList);
        ml_analytics_idsList = NULL;
    }
    if (snssai_listList) {
        OpenAPI_list_for_each(snssai_listList, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(snssai_listList);
        snssai_listList = NULL;
    }
    if (tracking_area_listList) {
        OpenAPI_list_for_each(tracking_area_listList, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(tracking_area_listList);
        tracking_area_listList = NULL;
    }
    return NULL;
}

OpenAPI_ml_analytics_info_t *OpenAPI_ml_analytics_info_copy(OpenAPI_ml_analytics_info_t *dst, OpenAPI_ml_analytics_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ml_analytics_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ml_analytics_info_convertToJSON() failed");
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

    OpenAPI_ml_analytics_info_free(dst);
    dst = OpenAPI_ml_analytics_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

