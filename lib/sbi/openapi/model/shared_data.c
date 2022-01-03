
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "shared_data.h"

OpenAPI_shared_data_t *OpenAPI_shared_data_create(
    char *shared_data_id,
    OpenAPI_access_and_mobility_subscription_data_1_t *shared_am_data,
    OpenAPI_sms_subscription_data_1_t *shared_sms_subs_data,
    OpenAPI_sms_management_subscription_data_1_t *shared_sms_mng_subs_data,
    OpenAPI_list_t* shared_dnn_configurations,
    OpenAPI_trace_data_t *shared_trace_data,
    OpenAPI_list_t* shared_snssai_infos,
    OpenAPI_list_t* shared_vn_group_datas
)
{
    OpenAPI_shared_data_t *shared_data_local_var = ogs_malloc(sizeof(OpenAPI_shared_data_t));
    ogs_assert(shared_data_local_var);

    shared_data_local_var->shared_data_id = shared_data_id;
    shared_data_local_var->shared_am_data = shared_am_data;
    shared_data_local_var->shared_sms_subs_data = shared_sms_subs_data;
    shared_data_local_var->shared_sms_mng_subs_data = shared_sms_mng_subs_data;
    shared_data_local_var->shared_dnn_configurations = shared_dnn_configurations;
    shared_data_local_var->shared_trace_data = shared_trace_data;
    shared_data_local_var->shared_snssai_infos = shared_snssai_infos;
    shared_data_local_var->shared_vn_group_datas = shared_vn_group_datas;

    return shared_data_local_var;
}

void OpenAPI_shared_data_free(OpenAPI_shared_data_t *shared_data)
{
    if (NULL == shared_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(shared_data->shared_data_id);
    OpenAPI_access_and_mobility_subscription_data_1_free(shared_data->shared_am_data);
    OpenAPI_sms_subscription_data_1_free(shared_data->shared_sms_subs_data);
    OpenAPI_sms_management_subscription_data_1_free(shared_data->shared_sms_mng_subs_data);
    OpenAPI_list_for_each(shared_data->shared_dnn_configurations, node) {
        OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
        ogs_free(localKeyValue->key);
        OpenAPI_dnn_configuration_1_free(localKeyValue->value);
        ogs_free(localKeyValue);
    }
    OpenAPI_list_free(shared_data->shared_dnn_configurations);
    OpenAPI_trace_data_free(shared_data->shared_trace_data);
    OpenAPI_list_for_each(shared_data->shared_snssai_infos, node) {
        OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
        ogs_free(localKeyValue->key);
        OpenAPI_snssai_info_free(localKeyValue->value);
        ogs_free(localKeyValue);
    }
    OpenAPI_list_free(shared_data->shared_snssai_infos);
    OpenAPI_list_for_each(shared_data->shared_vn_group_datas, node) {
        OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
        ogs_free(localKeyValue->key);
        OpenAPI_vn_group_data_free(localKeyValue->value);
        ogs_free(localKeyValue);
    }
    OpenAPI_list_free(shared_data->shared_vn_group_datas);
    ogs_free(shared_data);
}

cJSON *OpenAPI_shared_data_convertToJSON(OpenAPI_shared_data_t *shared_data)
{
    cJSON *item = NULL;

    if (shared_data == NULL) {
        ogs_error("OpenAPI_shared_data_convertToJSON() failed [SharedData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddStringToObject(item, "sharedDataId", shared_data->shared_data_id) == NULL) {
        ogs_error("OpenAPI_shared_data_convertToJSON() failed [shared_data_id]");
        goto end;
    }

    if (shared_data->shared_am_data) {
    cJSON *shared_am_data_local_JSON = OpenAPI_access_and_mobility_subscription_data_1_convertToJSON(shared_data->shared_am_data);
    if (shared_am_data_local_JSON == NULL) {
        ogs_error("OpenAPI_shared_data_convertToJSON() failed [shared_am_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sharedAmData", shared_am_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_shared_data_convertToJSON() failed [shared_am_data]");
        goto end;
    }
    }

    if (shared_data->shared_sms_subs_data) {
    cJSON *shared_sms_subs_data_local_JSON = OpenAPI_sms_subscription_data_1_convertToJSON(shared_data->shared_sms_subs_data);
    if (shared_sms_subs_data_local_JSON == NULL) {
        ogs_error("OpenAPI_shared_data_convertToJSON() failed [shared_sms_subs_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sharedSmsSubsData", shared_sms_subs_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_shared_data_convertToJSON() failed [shared_sms_subs_data]");
        goto end;
    }
    }

    if (shared_data->shared_sms_mng_subs_data) {
    cJSON *shared_sms_mng_subs_data_local_JSON = OpenAPI_sms_management_subscription_data_1_convertToJSON(shared_data->shared_sms_mng_subs_data);
    if (shared_sms_mng_subs_data_local_JSON == NULL) {
        ogs_error("OpenAPI_shared_data_convertToJSON() failed [shared_sms_mng_subs_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sharedSmsMngSubsData", shared_sms_mng_subs_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_shared_data_convertToJSON() failed [shared_sms_mng_subs_data]");
        goto end;
    }
    }

    if (shared_data->shared_dnn_configurations) {
    cJSON *shared_dnn_configurations = cJSON_AddObjectToObject(item, "sharedDnnConfigurations");
    if (shared_dnn_configurations == NULL) {
        ogs_error("OpenAPI_shared_data_convertToJSON() failed [shared_dnn_configurations]");
        goto end;
    }
    cJSON *localMapObject = shared_dnn_configurations;
    OpenAPI_lnode_t *shared_dnn_configurations_node;
    if (shared_data->shared_dnn_configurations) {
        OpenAPI_list_for_each(shared_data->shared_dnn_configurations, shared_dnn_configurations_node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)shared_dnn_configurations_node->data;
        cJSON *itemLocal = localKeyValue->value ?
            OpenAPI_dnn_configuration_1_convertToJSON(localKeyValue->value) :
            cJSON_CreateNull();
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_shared_data_convertToJSON() failed [shared_dnn_configurations]");
            goto end;
        }
        cJSON_AddItemToObject(shared_dnn_configurations, localKeyValue->key, itemLocal);
            }
        }
    }

    if (shared_data->shared_trace_data) {
    cJSON *shared_trace_data_local_JSON = OpenAPI_trace_data_convertToJSON(shared_data->shared_trace_data);
    if (shared_trace_data_local_JSON == NULL) {
        ogs_error("OpenAPI_shared_data_convertToJSON() failed [shared_trace_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sharedTraceData", shared_trace_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_shared_data_convertToJSON() failed [shared_trace_data]");
        goto end;
    }
    }

    if (shared_data->shared_snssai_infos) {
    cJSON *shared_snssai_infos = cJSON_AddObjectToObject(item, "sharedSnssaiInfos");
    if (shared_snssai_infos == NULL) {
        ogs_error("OpenAPI_shared_data_convertToJSON() failed [shared_snssai_infos]");
        goto end;
    }
    cJSON *localMapObject = shared_snssai_infos;
    OpenAPI_lnode_t *shared_snssai_infos_node;
    if (shared_data->shared_snssai_infos) {
        OpenAPI_list_for_each(shared_data->shared_snssai_infos, shared_snssai_infos_node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)shared_snssai_infos_node->data;
        cJSON *itemLocal = localKeyValue->value ?
            OpenAPI_snssai_info_convertToJSON(localKeyValue->value) :
            cJSON_CreateNull();
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_shared_data_convertToJSON() failed [shared_snssai_infos]");
            goto end;
        }
        cJSON_AddItemToObject(shared_snssai_infos, localKeyValue->key, itemLocal);
            }
        }
    }

    if (shared_data->shared_vn_group_datas) {
    cJSON *shared_vn_group_datas = cJSON_AddObjectToObject(item, "sharedVnGroupDatas");
    if (shared_vn_group_datas == NULL) {
        ogs_error("OpenAPI_shared_data_convertToJSON() failed [shared_vn_group_datas]");
        goto end;
    }
    cJSON *localMapObject = shared_vn_group_datas;
    OpenAPI_lnode_t *shared_vn_group_datas_node;
    if (shared_data->shared_vn_group_datas) {
        OpenAPI_list_for_each(shared_data->shared_vn_group_datas, shared_vn_group_datas_node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)shared_vn_group_datas_node->data;
        cJSON *itemLocal = localKeyValue->value ?
            OpenAPI_vn_group_data_convertToJSON(localKeyValue->value) :
            cJSON_CreateNull();
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_shared_data_convertToJSON() failed [shared_vn_group_datas]");
            goto end;
        }
        cJSON_AddItemToObject(shared_vn_group_datas, localKeyValue->key, itemLocal);
            }
        }
    }

end:
    return item;
}

OpenAPI_shared_data_t *OpenAPI_shared_data_parseFromJSON(cJSON *shared_dataJSON)
{
    OpenAPI_shared_data_t *shared_data_local_var = NULL;
    cJSON *shared_data_id = cJSON_GetObjectItemCaseSensitive(shared_dataJSON, "sharedDataId");
    if (!shared_data_id) {
        ogs_error("OpenAPI_shared_data_parseFromJSON() failed [shared_data_id]");
        goto end;
    }

    if (!cJSON_IsString(shared_data_id)) {
        ogs_error("OpenAPI_shared_data_parseFromJSON() failed [shared_data_id]");
        goto end;
    }

    cJSON *shared_am_data = cJSON_GetObjectItemCaseSensitive(shared_dataJSON, "sharedAmData");

    OpenAPI_access_and_mobility_subscription_data_1_t *shared_am_data_local_nonprim = NULL;
    if (shared_am_data) {
    shared_am_data_local_nonprim = OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON(shared_am_data);
    }

    cJSON *shared_sms_subs_data = cJSON_GetObjectItemCaseSensitive(shared_dataJSON, "sharedSmsSubsData");

    OpenAPI_sms_subscription_data_1_t *shared_sms_subs_data_local_nonprim = NULL;
    if (shared_sms_subs_data) {
    shared_sms_subs_data_local_nonprim = OpenAPI_sms_subscription_data_1_parseFromJSON(shared_sms_subs_data);
    }

    cJSON *shared_sms_mng_subs_data = cJSON_GetObjectItemCaseSensitive(shared_dataJSON, "sharedSmsMngSubsData");

    OpenAPI_sms_management_subscription_data_1_t *shared_sms_mng_subs_data_local_nonprim = NULL;
    if (shared_sms_mng_subs_data) {
    shared_sms_mng_subs_data_local_nonprim = OpenAPI_sms_management_subscription_data_1_parseFromJSON(shared_sms_mng_subs_data);
    }

    cJSON *shared_dnn_configurations = cJSON_GetObjectItemCaseSensitive(shared_dataJSON, "sharedDnnConfigurations");

    OpenAPI_list_t *shared_dnn_configurationsList;
    if (shared_dnn_configurations) {
    cJSON *shared_dnn_configurations_local_map;
    if (!cJSON_IsObject(shared_dnn_configurations)) {
        ogs_error("OpenAPI_shared_data_parseFromJSON() failed [shared_dnn_configurations]");
        goto end;
    }
    shared_dnn_configurationsList = OpenAPI_list_create();
    OpenAPI_map_t *localMapKeyPair = NULL;
    cJSON_ArrayForEach(shared_dnn_configurations_local_map, shared_dnn_configurations) {
        cJSON *localMapObject = shared_dnn_configurations_local_map;
        if (cJSON_IsObject(shared_dnn_configurations_local_map)) {
            localMapKeyPair = OpenAPI_map_create(
                ogs_strdup(localMapObject->string), OpenAPI_dnn_configuration_1_parseFromJSON(localMapObject));
        } else if (cJSON_IsNull(shared_dnn_configurations_local_map)) {
            localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
        } else {
            ogs_error("OpenAPI_shared_data_parseFromJSON() failed [shared_dnn_configurations]");
            goto end;
        }
        OpenAPI_list_add(shared_dnn_configurationsList , localMapKeyPair);
    }
    }

    cJSON *shared_trace_data = cJSON_GetObjectItemCaseSensitive(shared_dataJSON, "sharedTraceData");

    OpenAPI_trace_data_t *shared_trace_data_local_nonprim = NULL;
    if (shared_trace_data) {
    shared_trace_data_local_nonprim = OpenAPI_trace_data_parseFromJSON(shared_trace_data);
    }

    cJSON *shared_snssai_infos = cJSON_GetObjectItemCaseSensitive(shared_dataJSON, "sharedSnssaiInfos");

    OpenAPI_list_t *shared_snssai_infosList;
    if (shared_snssai_infos) {
    cJSON *shared_snssai_infos_local_map;
    if (!cJSON_IsObject(shared_snssai_infos)) {
        ogs_error("OpenAPI_shared_data_parseFromJSON() failed [shared_snssai_infos]");
        goto end;
    }
    shared_snssai_infosList = OpenAPI_list_create();
    OpenAPI_map_t *localMapKeyPair = NULL;
    cJSON_ArrayForEach(shared_snssai_infos_local_map, shared_snssai_infos) {
        cJSON *localMapObject = shared_snssai_infos_local_map;
        if (cJSON_IsObject(shared_snssai_infos_local_map)) {
            localMapKeyPair = OpenAPI_map_create(
                ogs_strdup(localMapObject->string), OpenAPI_snssai_info_parseFromJSON(localMapObject));
        } else if (cJSON_IsNull(shared_snssai_infos_local_map)) {
            localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
        } else {
            ogs_error("OpenAPI_shared_data_parseFromJSON() failed [shared_snssai_infos]");
            goto end;
        }
        OpenAPI_list_add(shared_snssai_infosList , localMapKeyPair);
    }
    }

    cJSON *shared_vn_group_datas = cJSON_GetObjectItemCaseSensitive(shared_dataJSON, "sharedVnGroupDatas");

    OpenAPI_list_t *shared_vn_group_datasList;
    if (shared_vn_group_datas) {
    cJSON *shared_vn_group_datas_local_map;
    if (!cJSON_IsObject(shared_vn_group_datas)) {
        ogs_error("OpenAPI_shared_data_parseFromJSON() failed [shared_vn_group_datas]");
        goto end;
    }
    shared_vn_group_datasList = OpenAPI_list_create();
    OpenAPI_map_t *localMapKeyPair = NULL;
    cJSON_ArrayForEach(shared_vn_group_datas_local_map, shared_vn_group_datas) {
        cJSON *localMapObject = shared_vn_group_datas_local_map;
        if (cJSON_IsObject(shared_vn_group_datas_local_map)) {
            localMapKeyPair = OpenAPI_map_create(
                ogs_strdup(localMapObject->string), OpenAPI_vn_group_data_parseFromJSON(localMapObject));
        } else if (cJSON_IsNull(shared_vn_group_datas_local_map)) {
            localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
        } else {
            ogs_error("OpenAPI_shared_data_parseFromJSON() failed [shared_vn_group_datas]");
            goto end;
        }
        OpenAPI_list_add(shared_vn_group_datasList , localMapKeyPair);
    }
    }

    shared_data_local_var = OpenAPI_shared_data_create (
        ogs_strdup(shared_data_id->valuestring),
        shared_am_data ? shared_am_data_local_nonprim : NULL,
        shared_sms_subs_data ? shared_sms_subs_data_local_nonprim : NULL,
        shared_sms_mng_subs_data ? shared_sms_mng_subs_data_local_nonprim : NULL,
        shared_dnn_configurations ? shared_dnn_configurationsList : NULL,
        shared_trace_data ? shared_trace_data_local_nonprim : NULL,
        shared_snssai_infos ? shared_snssai_infosList : NULL,
        shared_vn_group_datas ? shared_vn_group_datasList : NULL
    );

    return shared_data_local_var;
end:
    return NULL;
}

OpenAPI_shared_data_t *OpenAPI_shared_data_copy(OpenAPI_shared_data_t *dst, OpenAPI_shared_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_shared_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_shared_data_convertToJSON() failed");
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

    OpenAPI_shared_data_free(dst);
    dst = OpenAPI_shared_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

