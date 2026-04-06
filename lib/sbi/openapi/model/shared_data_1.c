
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "shared_data_1.h"

OpenAPI_shared_data_1_t *OpenAPI_shared_data_1_create(
    char *shared_data_id,
    OpenAPI_access_and_mobility_subscription_data_1_t *shared_am_data,
    OpenAPI_sms_subscription_data_1_t *shared_sms_subs_data,
    OpenAPI_sms_management_subscription_data_1_t *shared_sms_mng_subs_data,
    OpenAPI_list_t* shared_dnn_configurations,
    bool is_shared_trace_data_null,
    OpenAPI_trace_data_t *shared_trace_data,
    OpenAPI_list_t* shared_snssai_infos,
    OpenAPI_list_t* shared_vn_group_datas,
    OpenAPI_list_t* treatment_instructions,
    OpenAPI_session_management_subscription_data_1_t *shared_sm_subs_data,
    bool is_shared_ecs_addr_config_info_null,
    OpenAPI_ecs_addr_config_info_1_t *shared_ecs_addr_config_info,
    OpenAPI_monitoring_suspension_1_t *shared_monitoring_suspension
)
{
    OpenAPI_shared_data_1_t *shared_data_1_local_var = ogs_malloc(sizeof(OpenAPI_shared_data_1_t));
    ogs_assert(shared_data_1_local_var);

    shared_data_1_local_var->shared_data_id = shared_data_id;
    shared_data_1_local_var->shared_am_data = shared_am_data;
    shared_data_1_local_var->shared_sms_subs_data = shared_sms_subs_data;
    shared_data_1_local_var->shared_sms_mng_subs_data = shared_sms_mng_subs_data;
    shared_data_1_local_var->shared_dnn_configurations = shared_dnn_configurations;
    shared_data_1_local_var->is_shared_trace_data_null = is_shared_trace_data_null;
    shared_data_1_local_var->shared_trace_data = shared_trace_data;
    shared_data_1_local_var->shared_snssai_infos = shared_snssai_infos;
    shared_data_1_local_var->shared_vn_group_datas = shared_vn_group_datas;
    shared_data_1_local_var->treatment_instructions = treatment_instructions;
    shared_data_1_local_var->shared_sm_subs_data = shared_sm_subs_data;
    shared_data_1_local_var->is_shared_ecs_addr_config_info_null = is_shared_ecs_addr_config_info_null;
    shared_data_1_local_var->shared_ecs_addr_config_info = shared_ecs_addr_config_info;
    shared_data_1_local_var->shared_monitoring_suspension = shared_monitoring_suspension;

    return shared_data_1_local_var;
}

void OpenAPI_shared_data_1_free(OpenAPI_shared_data_1_t *shared_data_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == shared_data_1) {
        return;
    }
    if (shared_data_1->shared_data_id) {
        ogs_free(shared_data_1->shared_data_id);
        shared_data_1->shared_data_id = NULL;
    }
    if (shared_data_1->shared_am_data) {
        OpenAPI_access_and_mobility_subscription_data_1_free(shared_data_1->shared_am_data);
        shared_data_1->shared_am_data = NULL;
    }
    if (shared_data_1->shared_sms_subs_data) {
        OpenAPI_sms_subscription_data_1_free(shared_data_1->shared_sms_subs_data);
        shared_data_1->shared_sms_subs_data = NULL;
    }
    if (shared_data_1->shared_sms_mng_subs_data) {
        OpenAPI_sms_management_subscription_data_1_free(shared_data_1->shared_sms_mng_subs_data);
        shared_data_1->shared_sms_mng_subs_data = NULL;
    }
    if (shared_data_1->shared_dnn_configurations) {
        OpenAPI_list_for_each(shared_data_1->shared_dnn_configurations, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_dnn_configuration_1_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(shared_data_1->shared_dnn_configurations);
        shared_data_1->shared_dnn_configurations = NULL;
    }
    if (shared_data_1->shared_trace_data) {
        OpenAPI_trace_data_free(shared_data_1->shared_trace_data);
        shared_data_1->shared_trace_data = NULL;
    }
    if (shared_data_1->shared_snssai_infos) {
        OpenAPI_list_for_each(shared_data_1->shared_snssai_infos, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_snssai_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(shared_data_1->shared_snssai_infos);
        shared_data_1->shared_snssai_infos = NULL;
    }
    if (shared_data_1->shared_vn_group_datas) {
        OpenAPI_list_for_each(shared_data_1->shared_vn_group_datas, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_vn_group_data_1_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(shared_data_1->shared_vn_group_datas);
        shared_data_1->shared_vn_group_datas = NULL;
    }
    if (shared_data_1->treatment_instructions) {
        OpenAPI_list_for_each(shared_data_1->treatment_instructions, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(shared_data_1->treatment_instructions);
        shared_data_1->treatment_instructions = NULL;
    }
    if (shared_data_1->shared_sm_subs_data) {
        OpenAPI_session_management_subscription_data_1_free(shared_data_1->shared_sm_subs_data);
        shared_data_1->shared_sm_subs_data = NULL;
    }
    if (shared_data_1->shared_ecs_addr_config_info) {
        OpenAPI_ecs_addr_config_info_1_free(shared_data_1->shared_ecs_addr_config_info);
        shared_data_1->shared_ecs_addr_config_info = NULL;
    }
    if (shared_data_1->shared_monitoring_suspension) {
        OpenAPI_monitoring_suspension_1_free(shared_data_1->shared_monitoring_suspension);
        shared_data_1->shared_monitoring_suspension = NULL;
    }
    ogs_free(shared_data_1);
}

cJSON *OpenAPI_shared_data_1_convertToJSON(OpenAPI_shared_data_1_t *shared_data_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (shared_data_1 == NULL) {
        ogs_error("OpenAPI_shared_data_1_convertToJSON() failed [SharedData_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!shared_data_1->shared_data_id) {
        ogs_error("OpenAPI_shared_data_1_convertToJSON() failed [shared_data_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "sharedDataId", shared_data_1->shared_data_id) == NULL) {
        ogs_error("OpenAPI_shared_data_1_convertToJSON() failed [shared_data_id]");
        goto end;
    }

    if (shared_data_1->shared_am_data) {
    cJSON *shared_am_data_local_JSON = OpenAPI_access_and_mobility_subscription_data_1_convertToJSON(shared_data_1->shared_am_data);
    if (shared_am_data_local_JSON == NULL) {
        ogs_error("OpenAPI_shared_data_1_convertToJSON() failed [shared_am_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sharedAmData", shared_am_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_shared_data_1_convertToJSON() failed [shared_am_data]");
        goto end;
    }
    }

    if (shared_data_1->shared_sms_subs_data) {
    cJSON *shared_sms_subs_data_local_JSON = OpenAPI_sms_subscription_data_1_convertToJSON(shared_data_1->shared_sms_subs_data);
    if (shared_sms_subs_data_local_JSON == NULL) {
        ogs_error("OpenAPI_shared_data_1_convertToJSON() failed [shared_sms_subs_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sharedSmsSubsData", shared_sms_subs_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_shared_data_1_convertToJSON() failed [shared_sms_subs_data]");
        goto end;
    }
    }

    if (shared_data_1->shared_sms_mng_subs_data) {
    cJSON *shared_sms_mng_subs_data_local_JSON = OpenAPI_sms_management_subscription_data_1_convertToJSON(shared_data_1->shared_sms_mng_subs_data);
    if (shared_sms_mng_subs_data_local_JSON == NULL) {
        ogs_error("OpenAPI_shared_data_1_convertToJSON() failed [shared_sms_mng_subs_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sharedSmsMngSubsData", shared_sms_mng_subs_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_shared_data_1_convertToJSON() failed [shared_sms_mng_subs_data]");
        goto end;
    }
    }

    if (shared_data_1->shared_dnn_configurations) {
    cJSON *shared_dnn_configurations = cJSON_AddObjectToObject(item, "sharedDnnConfigurations");
    if (shared_dnn_configurations == NULL) {
        ogs_error("OpenAPI_shared_data_1_convertToJSON() failed [shared_dnn_configurations]");
        goto end;
    }
    cJSON *localMapObject = shared_dnn_configurations;
    if (shared_data_1->shared_dnn_configurations) {
        OpenAPI_list_for_each(shared_data_1->shared_dnn_configurations, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_shared_data_1_convertToJSON() failed [shared_dnn_configurations]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_shared_data_1_convertToJSON() failed [shared_dnn_configurations]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_dnn_configuration_1_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_shared_data_1_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (shared_data_1->shared_trace_data) {
    cJSON *shared_trace_data_local_JSON = OpenAPI_trace_data_convertToJSON(shared_data_1->shared_trace_data);
    if (shared_trace_data_local_JSON == NULL) {
        ogs_error("OpenAPI_shared_data_1_convertToJSON() failed [shared_trace_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sharedTraceData", shared_trace_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_shared_data_1_convertToJSON() failed [shared_trace_data]");
        goto end;
    }
    } else if (shared_data_1->is_shared_trace_data_null) {
        if (cJSON_AddNullToObject(item, "sharedTraceData") == NULL) {
            ogs_error("OpenAPI_shared_data_1_convertToJSON() failed [shared_trace_data]");
            goto end;
        }
    }

    if (shared_data_1->shared_snssai_infos) {
    cJSON *shared_snssai_infos = cJSON_AddObjectToObject(item, "sharedSnssaiInfos");
    if (shared_snssai_infos == NULL) {
        ogs_error("OpenAPI_shared_data_1_convertToJSON() failed [shared_snssai_infos]");
        goto end;
    }
    cJSON *localMapObject = shared_snssai_infos;
    if (shared_data_1->shared_snssai_infos) {
        OpenAPI_list_for_each(shared_data_1->shared_snssai_infos, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_shared_data_1_convertToJSON() failed [shared_snssai_infos]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_shared_data_1_convertToJSON() failed [shared_snssai_infos]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_snssai_info_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_shared_data_1_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (shared_data_1->shared_vn_group_datas) {
    cJSON *shared_vn_group_datas = cJSON_AddObjectToObject(item, "sharedVnGroupDatas");
    if (shared_vn_group_datas == NULL) {
        ogs_error("OpenAPI_shared_data_1_convertToJSON() failed [shared_vn_group_datas]");
        goto end;
    }
    cJSON *localMapObject = shared_vn_group_datas;
    if (shared_data_1->shared_vn_group_datas) {
        OpenAPI_list_for_each(shared_data_1->shared_vn_group_datas, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_shared_data_1_convertToJSON() failed [shared_vn_group_datas]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_shared_data_1_convertToJSON() failed [shared_vn_group_datas]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_vn_group_data_1_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_shared_data_1_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (shared_data_1->treatment_instructions != OpenAPI_shared_data_treatment_instruction_NULL) {
    cJSON *treatment_instructions = cJSON_AddObjectToObject(item, "treatmentInstructions");
    if (treatment_instructions == NULL) {
        ogs_error("OpenAPI_shared_data_1_convertToJSON() failed [treatment_instructions]");
        goto end;
    }
    cJSON *localMapObject = treatment_instructions;
    if (shared_data_1->treatment_instructions) {
        OpenAPI_list_for_each(shared_data_1->treatment_instructions, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_shared_data_1_convertToJSON() failed [treatment_instructions]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_shared_data_1_convertToJSON() failed [treatment_instructions]");
                goto end;
            }
            if (cJSON_AddStringToObject(localMapObject, localKeyValue->key, OpenAPI_shared_data_treatment_instruction_ToString((intptr_t)localKeyValue->value)) == NULL) {
                ogs_error("OpenAPI_shared_data_1_convertToJSON() failed [treatment_instructions]");
                goto end;
            }
        }
    }
    }

    if (shared_data_1->shared_sm_subs_data) {
    cJSON *shared_sm_subs_data_local_JSON = OpenAPI_session_management_subscription_data_1_convertToJSON(shared_data_1->shared_sm_subs_data);
    if (shared_sm_subs_data_local_JSON == NULL) {
        ogs_error("OpenAPI_shared_data_1_convertToJSON() failed [shared_sm_subs_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sharedSmSubsData", shared_sm_subs_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_shared_data_1_convertToJSON() failed [shared_sm_subs_data]");
        goto end;
    }
    }

    if (shared_data_1->shared_ecs_addr_config_info) {
    cJSON *shared_ecs_addr_config_info_local_JSON = OpenAPI_ecs_addr_config_info_1_convertToJSON(shared_data_1->shared_ecs_addr_config_info);
    if (shared_ecs_addr_config_info_local_JSON == NULL) {
        ogs_error("OpenAPI_shared_data_1_convertToJSON() failed [shared_ecs_addr_config_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sharedEcsAddrConfigInfo", shared_ecs_addr_config_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_shared_data_1_convertToJSON() failed [shared_ecs_addr_config_info]");
        goto end;
    }
    } else if (shared_data_1->is_shared_ecs_addr_config_info_null) {
        if (cJSON_AddNullToObject(item, "sharedEcsAddrConfigInfo") == NULL) {
            ogs_error("OpenAPI_shared_data_1_convertToJSON() failed [shared_ecs_addr_config_info]");
            goto end;
        }
    }

    if (shared_data_1->shared_monitoring_suspension) {
    cJSON *shared_monitoring_suspension_local_JSON = OpenAPI_monitoring_suspension_1_convertToJSON(shared_data_1->shared_monitoring_suspension);
    if (shared_monitoring_suspension_local_JSON == NULL) {
        ogs_error("OpenAPI_shared_data_1_convertToJSON() failed [shared_monitoring_suspension]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sharedMonitoringSuspension", shared_monitoring_suspension_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_shared_data_1_convertToJSON() failed [shared_monitoring_suspension]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_shared_data_1_t *OpenAPI_shared_data_1_parseFromJSON(cJSON *shared_data_1JSON)
{
    OpenAPI_shared_data_1_t *shared_data_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *shared_data_id = NULL;
    cJSON *shared_am_data = NULL;
    OpenAPI_access_and_mobility_subscription_data_1_t *shared_am_data_local_nonprim = NULL;
    cJSON *shared_sms_subs_data = NULL;
    OpenAPI_sms_subscription_data_1_t *shared_sms_subs_data_local_nonprim = NULL;
    cJSON *shared_sms_mng_subs_data = NULL;
    OpenAPI_sms_management_subscription_data_1_t *shared_sms_mng_subs_data_local_nonprim = NULL;
    cJSON *shared_dnn_configurations = NULL;
    OpenAPI_list_t *shared_dnn_configurationsList = NULL;
    cJSON *shared_trace_data = NULL;
    OpenAPI_trace_data_t *shared_trace_data_local_nonprim = NULL;
    cJSON *shared_snssai_infos = NULL;
    OpenAPI_list_t *shared_snssai_infosList = NULL;
    cJSON *shared_vn_group_datas = NULL;
    OpenAPI_list_t *shared_vn_group_datasList = NULL;
    cJSON *treatment_instructions = NULL;
    OpenAPI_list_t *treatment_instructionsList = NULL;
    cJSON *shared_sm_subs_data = NULL;
    OpenAPI_session_management_subscription_data_1_t *shared_sm_subs_data_local_nonprim = NULL;
    cJSON *shared_ecs_addr_config_info = NULL;
    OpenAPI_ecs_addr_config_info_1_t *shared_ecs_addr_config_info_local_nonprim = NULL;
    cJSON *shared_monitoring_suspension = NULL;
    OpenAPI_monitoring_suspension_1_t *shared_monitoring_suspension_local_nonprim = NULL;
    shared_data_id = cJSON_GetObjectItemCaseSensitive(shared_data_1JSON, "sharedDataId");
    if (!shared_data_id) {
        ogs_error("OpenAPI_shared_data_1_parseFromJSON() failed [shared_data_id]");
        goto end;
    }
    if (!cJSON_IsString(shared_data_id)) {
        ogs_error("OpenAPI_shared_data_1_parseFromJSON() failed [shared_data_id]");
        goto end;
    }

    shared_am_data = cJSON_GetObjectItemCaseSensitive(shared_data_1JSON, "sharedAmData");
    if (shared_am_data) {
    shared_am_data_local_nonprim = OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON(shared_am_data);
    if (!shared_am_data_local_nonprim) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON failed [shared_am_data]");
        goto end;
    }
    }

    shared_sms_subs_data = cJSON_GetObjectItemCaseSensitive(shared_data_1JSON, "sharedSmsSubsData");
    if (shared_sms_subs_data) {
    shared_sms_subs_data_local_nonprim = OpenAPI_sms_subscription_data_1_parseFromJSON(shared_sms_subs_data);
    if (!shared_sms_subs_data_local_nonprim) {
        ogs_error("OpenAPI_sms_subscription_data_1_parseFromJSON failed [shared_sms_subs_data]");
        goto end;
    }
    }

    shared_sms_mng_subs_data = cJSON_GetObjectItemCaseSensitive(shared_data_1JSON, "sharedSmsMngSubsData");
    if (shared_sms_mng_subs_data) {
    shared_sms_mng_subs_data_local_nonprim = OpenAPI_sms_management_subscription_data_1_parseFromJSON(shared_sms_mng_subs_data);
    if (!shared_sms_mng_subs_data_local_nonprim) {
        ogs_error("OpenAPI_sms_management_subscription_data_1_parseFromJSON failed [shared_sms_mng_subs_data]");
        goto end;
    }
    }

    shared_dnn_configurations = cJSON_GetObjectItemCaseSensitive(shared_data_1JSON, "sharedDnnConfigurations");
    if (shared_dnn_configurations) {
        cJSON *shared_dnn_configurations_local_map = NULL;
        if (!cJSON_IsObject(shared_dnn_configurations) && !cJSON_IsNull(shared_dnn_configurations)) {
            ogs_error("OpenAPI_shared_data_1_parseFromJSON() failed [shared_dnn_configurations]");
            goto end;
        }
        if (cJSON_IsObject(shared_dnn_configurations)) {
            shared_dnn_configurationsList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(shared_dnn_configurations_local_map, shared_dnn_configurations) {
                cJSON *localMapObject = shared_dnn_configurations_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_dnn_configuration_1_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_shared_data_1_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(shared_dnn_configurationsList, localMapKeyPair);
            }
        }
    }

    shared_trace_data = cJSON_GetObjectItemCaseSensitive(shared_data_1JSON, "sharedTraceData");
    if (shared_trace_data) {
    if (!cJSON_IsNull(shared_trace_data)) {
    shared_trace_data_local_nonprim = OpenAPI_trace_data_parseFromJSON(shared_trace_data);
    if (!shared_trace_data_local_nonprim) {
        ogs_error("OpenAPI_trace_data_parseFromJSON failed [shared_trace_data]");
        goto end;
    }
    }
    }

    shared_snssai_infos = cJSON_GetObjectItemCaseSensitive(shared_data_1JSON, "sharedSnssaiInfos");
    if (shared_snssai_infos) {
        cJSON *shared_snssai_infos_local_map = NULL;
        if (!cJSON_IsObject(shared_snssai_infos) && !cJSON_IsNull(shared_snssai_infos)) {
            ogs_error("OpenAPI_shared_data_1_parseFromJSON() failed [shared_snssai_infos]");
            goto end;
        }
        if (cJSON_IsObject(shared_snssai_infos)) {
            shared_snssai_infosList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(shared_snssai_infos_local_map, shared_snssai_infos) {
                cJSON *localMapObject = shared_snssai_infos_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_snssai_info_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_shared_data_1_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(shared_snssai_infosList, localMapKeyPair);
            }
        }
    }

    shared_vn_group_datas = cJSON_GetObjectItemCaseSensitive(shared_data_1JSON, "sharedVnGroupDatas");
    if (shared_vn_group_datas) {
        cJSON *shared_vn_group_datas_local_map = NULL;
        if (!cJSON_IsObject(shared_vn_group_datas) && !cJSON_IsNull(shared_vn_group_datas)) {
            ogs_error("OpenAPI_shared_data_1_parseFromJSON() failed [shared_vn_group_datas]");
            goto end;
        }
        if (cJSON_IsObject(shared_vn_group_datas)) {
            shared_vn_group_datasList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(shared_vn_group_datas_local_map, shared_vn_group_datas) {
                cJSON *localMapObject = shared_vn_group_datas_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_vn_group_data_1_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_shared_data_1_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(shared_vn_group_datasList, localMapKeyPair);
            }
        }
    }

    treatment_instructions = cJSON_GetObjectItemCaseSensitive(shared_data_1JSON, "treatmentInstructions");
    if (treatment_instructions) {
        cJSON *treatment_instructions_local_map = NULL;
        if (!cJSON_IsObject(treatment_instructions) && !cJSON_IsNull(treatment_instructions)) {
            ogs_error("OpenAPI_shared_data_1_parseFromJSON() failed [treatment_instructions]");
            goto end;
        }
        if (cJSON_IsObject(treatment_instructions)) {
            treatment_instructionsList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(treatment_instructions_local_map, treatment_instructions) {
                cJSON *localMapObject = treatment_instructions_local_map;
                if (!cJSON_IsString(localMapObject)) {
                    ogs_error("OpenAPI_shared_data_1_parseFromJSON() failed [treatment_instructions]");
                    goto end;
                }
                localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), (void *)OpenAPI_shared_data_treatment_instruction_FromString(localMapObject->string));
                OpenAPI_list_add(treatment_instructionsList, localMapKeyPair);
            }
        }
    }

    shared_sm_subs_data = cJSON_GetObjectItemCaseSensitive(shared_data_1JSON, "sharedSmSubsData");
    if (shared_sm_subs_data) {
    shared_sm_subs_data_local_nonprim = OpenAPI_session_management_subscription_data_1_parseFromJSON(shared_sm_subs_data);
    if (!shared_sm_subs_data_local_nonprim) {
        ogs_error("OpenAPI_session_management_subscription_data_1_parseFromJSON failed [shared_sm_subs_data]");
        goto end;
    }
    }

    shared_ecs_addr_config_info = cJSON_GetObjectItemCaseSensitive(shared_data_1JSON, "sharedEcsAddrConfigInfo");
    if (shared_ecs_addr_config_info) {
    if (!cJSON_IsNull(shared_ecs_addr_config_info)) {
    shared_ecs_addr_config_info_local_nonprim = OpenAPI_ecs_addr_config_info_1_parseFromJSON(shared_ecs_addr_config_info);
    if (!shared_ecs_addr_config_info_local_nonprim) {
        ogs_error("OpenAPI_ecs_addr_config_info_1_parseFromJSON failed [shared_ecs_addr_config_info]");
        goto end;
    }
    }
    }

    shared_monitoring_suspension = cJSON_GetObjectItemCaseSensitive(shared_data_1JSON, "sharedMonitoringSuspension");
    if (shared_monitoring_suspension) {
    shared_monitoring_suspension_local_nonprim = OpenAPI_monitoring_suspension_1_parseFromJSON(shared_monitoring_suspension);
    if (!shared_monitoring_suspension_local_nonprim) {
        ogs_error("OpenAPI_monitoring_suspension_1_parseFromJSON failed [shared_monitoring_suspension]");
        goto end;
    }
    }

    shared_data_1_local_var = OpenAPI_shared_data_1_create (
        ogs_strdup(shared_data_id->valuestring),
        shared_am_data ? shared_am_data_local_nonprim : NULL,
        shared_sms_subs_data ? shared_sms_subs_data_local_nonprim : NULL,
        shared_sms_mng_subs_data ? shared_sms_mng_subs_data_local_nonprim : NULL,
        shared_dnn_configurations ? shared_dnn_configurationsList : NULL,
        shared_trace_data && cJSON_IsNull(shared_trace_data) ? true : false,
        shared_trace_data ? shared_trace_data_local_nonprim : NULL,
        shared_snssai_infos ? shared_snssai_infosList : NULL,
        shared_vn_group_datas ? shared_vn_group_datasList : NULL,
        treatment_instructions ? treatment_instructionsList : NULL,
        shared_sm_subs_data ? shared_sm_subs_data_local_nonprim : NULL,
        shared_ecs_addr_config_info && cJSON_IsNull(shared_ecs_addr_config_info) ? true : false,
        shared_ecs_addr_config_info ? shared_ecs_addr_config_info_local_nonprim : NULL,
        shared_monitoring_suspension ? shared_monitoring_suspension_local_nonprim : NULL
    );

    return shared_data_1_local_var;
end:
    if (shared_am_data_local_nonprim) {
        OpenAPI_access_and_mobility_subscription_data_1_free(shared_am_data_local_nonprim);
        shared_am_data_local_nonprim = NULL;
    }
    if (shared_sms_subs_data_local_nonprim) {
        OpenAPI_sms_subscription_data_1_free(shared_sms_subs_data_local_nonprim);
        shared_sms_subs_data_local_nonprim = NULL;
    }
    if (shared_sms_mng_subs_data_local_nonprim) {
        OpenAPI_sms_management_subscription_data_1_free(shared_sms_mng_subs_data_local_nonprim);
        shared_sms_mng_subs_data_local_nonprim = NULL;
    }
    if (shared_dnn_configurationsList) {
        OpenAPI_list_for_each(shared_dnn_configurationsList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_dnn_configuration_1_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(shared_dnn_configurationsList);
        shared_dnn_configurationsList = NULL;
    }
    if (shared_trace_data_local_nonprim) {
        OpenAPI_trace_data_free(shared_trace_data_local_nonprim);
        shared_trace_data_local_nonprim = NULL;
    }
    if (shared_snssai_infosList) {
        OpenAPI_list_for_each(shared_snssai_infosList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_snssai_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(shared_snssai_infosList);
        shared_snssai_infosList = NULL;
    }
    if (shared_vn_group_datasList) {
        OpenAPI_list_for_each(shared_vn_group_datasList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_vn_group_data_1_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(shared_vn_group_datasList);
        shared_vn_group_datasList = NULL;
    }
    if (treatment_instructionsList) {
        OpenAPI_list_for_each(treatment_instructionsList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(treatment_instructionsList);
        treatment_instructionsList = NULL;
    }
    if (shared_sm_subs_data_local_nonprim) {
        OpenAPI_session_management_subscription_data_1_free(shared_sm_subs_data_local_nonprim);
        shared_sm_subs_data_local_nonprim = NULL;
    }
    if (shared_ecs_addr_config_info_local_nonprim) {
        OpenAPI_ecs_addr_config_info_1_free(shared_ecs_addr_config_info_local_nonprim);
        shared_ecs_addr_config_info_local_nonprim = NULL;
    }
    if (shared_monitoring_suspension_local_nonprim) {
        OpenAPI_monitoring_suspension_1_free(shared_monitoring_suspension_local_nonprim);
        shared_monitoring_suspension_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_shared_data_1_t *OpenAPI_shared_data_1_copy(OpenAPI_shared_data_1_t *dst, OpenAPI_shared_data_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_shared_data_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_shared_data_1_convertToJSON() failed");
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

    OpenAPI_shared_data_1_free(dst);
    dst = OpenAPI_shared_data_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

