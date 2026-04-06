
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "traffic_influence_info.h"

OpenAPI_traffic_influence_info_t *OpenAPI_traffic_influence_info_create(
    OpenAPI_list_t *traff_influ_data,
    OpenAPI_list_t* traff_cont_decs
)
{
    OpenAPI_traffic_influence_info_t *traffic_influence_info_local_var = ogs_malloc(sizeof(OpenAPI_traffic_influence_info_t));
    ogs_assert(traffic_influence_info_local_var);

    traffic_influence_info_local_var->traff_influ_data = traff_influ_data;
    traffic_influence_info_local_var->traff_cont_decs = traff_cont_decs;

    return traffic_influence_info_local_var;
}

void OpenAPI_traffic_influence_info_free(OpenAPI_traffic_influence_info_t *traffic_influence_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == traffic_influence_info) {
        return;
    }
    if (traffic_influence_info->traff_influ_data) {
        OpenAPI_list_for_each(traffic_influence_info->traff_influ_data, node) {
            OpenAPI_traffic_influence_data_free(node->data);
        }
        OpenAPI_list_free(traffic_influence_info->traff_influ_data);
        traffic_influence_info->traff_influ_data = NULL;
    }
    if (traffic_influence_info->traff_cont_decs) {
        OpenAPI_list_for_each(traffic_influence_info->traff_cont_decs, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_traffic_control_data_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(traffic_influence_info->traff_cont_decs);
        traffic_influence_info->traff_cont_decs = NULL;
    }
    ogs_free(traffic_influence_info);
}

cJSON *OpenAPI_traffic_influence_info_convertToJSON(OpenAPI_traffic_influence_info_t *traffic_influence_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (traffic_influence_info == NULL) {
        ogs_error("OpenAPI_traffic_influence_info_convertToJSON() failed [TrafficInfluenceInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (traffic_influence_info->traff_influ_data) {
    cJSON *traff_influ_dataList = cJSON_AddArrayToObject(item, "traffInfluData");
    if (traff_influ_dataList == NULL) {
        ogs_error("OpenAPI_traffic_influence_info_convertToJSON() failed [traff_influ_data]");
        goto end;
    }
    OpenAPI_list_for_each(traffic_influence_info->traff_influ_data, node) {
        cJSON *itemLocal = OpenAPI_traffic_influence_data_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_traffic_influence_info_convertToJSON() failed [traff_influ_data]");
            goto end;
        }
        cJSON_AddItemToArray(traff_influ_dataList, itemLocal);
    }
    }

    if (traffic_influence_info->traff_cont_decs) {
    cJSON *traff_cont_decs = cJSON_AddObjectToObject(item, "traffContDecs");
    if (traff_cont_decs == NULL) {
        ogs_error("OpenAPI_traffic_influence_info_convertToJSON() failed [traff_cont_decs]");
        goto end;
    }
    cJSON *localMapObject = traff_cont_decs;
    if (traffic_influence_info->traff_cont_decs) {
        OpenAPI_list_for_each(traffic_influence_info->traff_cont_decs, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_traffic_influence_info_convertToJSON() failed [traff_cont_decs]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_traffic_influence_info_convertToJSON() failed [traff_cont_decs]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_traffic_control_data_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_traffic_influence_info_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

end:
    return item;
}

OpenAPI_traffic_influence_info_t *OpenAPI_traffic_influence_info_parseFromJSON(cJSON *traffic_influence_infoJSON)
{
    OpenAPI_traffic_influence_info_t *traffic_influence_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *traff_influ_data = NULL;
    OpenAPI_list_t *traff_influ_dataList = NULL;
    cJSON *traff_cont_decs = NULL;
    OpenAPI_list_t *traff_cont_decsList = NULL;
    traff_influ_data = cJSON_GetObjectItemCaseSensitive(traffic_influence_infoJSON, "traffInfluData");
    if (traff_influ_data) {
        cJSON *traff_influ_data_local = NULL;
        if (!cJSON_IsArray(traff_influ_data)) {
            ogs_error("OpenAPI_traffic_influence_info_parseFromJSON() failed [traff_influ_data]");
            goto end;
        }

        traff_influ_dataList = OpenAPI_list_create();

        cJSON_ArrayForEach(traff_influ_data_local, traff_influ_data) {
            if (!cJSON_IsObject(traff_influ_data_local)) {
                ogs_error("OpenAPI_traffic_influence_info_parseFromJSON() failed [traff_influ_data]");
                goto end;
            }
            OpenAPI_traffic_influence_data_t *traff_influ_dataItem = OpenAPI_traffic_influence_data_parseFromJSON(traff_influ_data_local);
            if (!traff_influ_dataItem) {
                ogs_error("No traff_influ_dataItem");
                goto end;
            }
            OpenAPI_list_add(traff_influ_dataList, traff_influ_dataItem);
        }
    }

    traff_cont_decs = cJSON_GetObjectItemCaseSensitive(traffic_influence_infoJSON, "traffContDecs");
    if (traff_cont_decs) {
        cJSON *traff_cont_decs_local_map = NULL;
        if (!cJSON_IsObject(traff_cont_decs) && !cJSON_IsNull(traff_cont_decs)) {
            ogs_error("OpenAPI_traffic_influence_info_parseFromJSON() failed [traff_cont_decs]");
            goto end;
        }
        if (cJSON_IsObject(traff_cont_decs)) {
            traff_cont_decsList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(traff_cont_decs_local_map, traff_cont_decs) {
                cJSON *localMapObject = traff_cont_decs_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_traffic_control_data_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_traffic_influence_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(traff_cont_decsList, localMapKeyPair);
            }
        }
    }

    traffic_influence_info_local_var = OpenAPI_traffic_influence_info_create (
        traff_influ_data ? traff_influ_dataList : NULL,
        traff_cont_decs ? traff_cont_decsList : NULL
    );

    return traffic_influence_info_local_var;
end:
    if (traff_influ_dataList) {
        OpenAPI_list_for_each(traff_influ_dataList, node) {
            OpenAPI_traffic_influence_data_free(node->data);
        }
        OpenAPI_list_free(traff_influ_dataList);
        traff_influ_dataList = NULL;
    }
    if (traff_cont_decsList) {
        OpenAPI_list_for_each(traff_cont_decsList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_traffic_control_data_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(traff_cont_decsList);
        traff_cont_decsList = NULL;
    }
    return NULL;
}

OpenAPI_traffic_influence_info_t *OpenAPI_traffic_influence_info_copy(OpenAPI_traffic_influence_info_t *dst, OpenAPI_traffic_influence_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_traffic_influence_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_traffic_influence_info_convertToJSON() failed");
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

    OpenAPI_traffic_influence_info_free(dst);
    dst = OpenAPI_traffic_influence_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

