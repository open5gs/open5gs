
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "session_management_subscription_data_1.h"

OpenAPI_session_management_subscription_data_1_t *OpenAPI_session_management_subscription_data_1_create(
    OpenAPI_snssai_t *single_nssai,
    OpenAPI_list_t* dnn_configurations,
    OpenAPI_list_t *internal_group_ids,
    OpenAPI_list_t* shared_vn_group_data_ids,
    char *shared_dnn_configurations_id,
    OpenAPI_odb_packet_services_e odb_packet_services,
    OpenAPI_trace_data_t *trace_data,
    char *shared_trace_data_id,
    OpenAPI_list_t* expected_ue_behaviours_list,
    OpenAPI_list_t* suggested_packet_num_dl_list,
    char *_3gpp_charging_characteristics
)
{
    OpenAPI_session_management_subscription_data_1_t *session_management_subscription_data_1_local_var = ogs_malloc(sizeof(OpenAPI_session_management_subscription_data_1_t));
    ogs_assert(session_management_subscription_data_1_local_var);

    session_management_subscription_data_1_local_var->single_nssai = single_nssai;
    session_management_subscription_data_1_local_var->dnn_configurations = dnn_configurations;
    session_management_subscription_data_1_local_var->internal_group_ids = internal_group_ids;
    session_management_subscription_data_1_local_var->shared_vn_group_data_ids = shared_vn_group_data_ids;
    session_management_subscription_data_1_local_var->shared_dnn_configurations_id = shared_dnn_configurations_id;
    session_management_subscription_data_1_local_var->odb_packet_services = odb_packet_services;
    session_management_subscription_data_1_local_var->trace_data = trace_data;
    session_management_subscription_data_1_local_var->shared_trace_data_id = shared_trace_data_id;
    session_management_subscription_data_1_local_var->expected_ue_behaviours_list = expected_ue_behaviours_list;
    session_management_subscription_data_1_local_var->suggested_packet_num_dl_list = suggested_packet_num_dl_list;
    session_management_subscription_data_1_local_var->_3gpp_charging_characteristics = _3gpp_charging_characteristics;

    return session_management_subscription_data_1_local_var;
}

void OpenAPI_session_management_subscription_data_1_free(OpenAPI_session_management_subscription_data_1_t *session_management_subscription_data_1)
{
    if (NULL == session_management_subscription_data_1) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_snssai_free(session_management_subscription_data_1->single_nssai);
    OpenAPI_list_for_each(session_management_subscription_data_1->dnn_configurations, node) {
        OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
        ogs_free(localKeyValue->key);
        OpenAPI_dnn_configuration_1_free(localKeyValue->value);
        ogs_free(localKeyValue);
    }
    OpenAPI_list_free(session_management_subscription_data_1->dnn_configurations);
    OpenAPI_list_for_each(session_management_subscription_data_1->internal_group_ids, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(session_management_subscription_data_1->internal_group_ids);
    OpenAPI_list_for_each(session_management_subscription_data_1->shared_vn_group_data_ids, node) {
        OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
        ogs_free(localKeyValue->key);
        ogs_free(localKeyValue->value);
        ogs_free(localKeyValue);
    }
    OpenAPI_list_free(session_management_subscription_data_1->shared_vn_group_data_ids);
    ogs_free(session_management_subscription_data_1->shared_dnn_configurations_id);
    OpenAPI_trace_data_free(session_management_subscription_data_1->trace_data);
    ogs_free(session_management_subscription_data_1->shared_trace_data_id);
    OpenAPI_list_for_each(session_management_subscription_data_1->expected_ue_behaviours_list, node) {
        OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
        ogs_free(localKeyValue->key);
        OpenAPI_expected_ue_behaviour_data_1_free(localKeyValue->value);
        ogs_free(localKeyValue);
    }
    OpenAPI_list_free(session_management_subscription_data_1->expected_ue_behaviours_list);
    OpenAPI_list_for_each(session_management_subscription_data_1->suggested_packet_num_dl_list, node) {
        OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
        ogs_free(localKeyValue->key);
        OpenAPI_suggested_packet_num_dl_1_free(localKeyValue->value);
        ogs_free(localKeyValue);
    }
    OpenAPI_list_free(session_management_subscription_data_1->suggested_packet_num_dl_list);
    ogs_free(session_management_subscription_data_1->_3gpp_charging_characteristics);
    ogs_free(session_management_subscription_data_1);
}

cJSON *OpenAPI_session_management_subscription_data_1_convertToJSON(OpenAPI_session_management_subscription_data_1_t *session_management_subscription_data_1)
{
    cJSON *item = NULL;

    if (session_management_subscription_data_1 == NULL) {
        ogs_error("OpenAPI_session_management_subscription_data_1_convertToJSON() failed [SessionManagementSubscriptionData_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    cJSON *single_nssai_local_JSON = OpenAPI_snssai_convertToJSON(session_management_subscription_data_1->single_nssai);
    if (single_nssai_local_JSON == NULL) {
        ogs_error("OpenAPI_session_management_subscription_data_1_convertToJSON() failed [single_nssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "singleNssai", single_nssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_session_management_subscription_data_1_convertToJSON() failed [single_nssai]");
        goto end;
    }

    if (session_management_subscription_data_1->dnn_configurations) {
    cJSON *dnn_configurations = cJSON_AddObjectToObject(item, "dnnConfigurations");
    if (dnn_configurations == NULL) {
        ogs_error("OpenAPI_session_management_subscription_data_1_convertToJSON() failed [dnn_configurations]");
        goto end;
    }
    cJSON *localMapObject = dnn_configurations;
    OpenAPI_lnode_t *dnn_configurations_node;
    if (session_management_subscription_data_1->dnn_configurations) {
        OpenAPI_list_for_each(session_management_subscription_data_1->dnn_configurations, dnn_configurations_node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)dnn_configurations_node->data;
        cJSON *itemLocal = localKeyValue->value ?
            OpenAPI_dnn_configuration_1_convertToJSON(localKeyValue->value) :
            cJSON_CreateNull();
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_session_management_subscription_data_1_convertToJSON() failed [dnn_configurations]");
            goto end;
        }
        cJSON_AddItemToObject(dnn_configurations, localKeyValue->key, itemLocal);
            }
        }
    }

    if (session_management_subscription_data_1->internal_group_ids) {
    cJSON *internal_group_ids = cJSON_AddArrayToObject(item, "internalGroupIds");
    if (internal_group_ids == NULL) {
        ogs_error("OpenAPI_session_management_subscription_data_1_convertToJSON() failed [internal_group_ids]");
        goto end;
    }

    OpenAPI_lnode_t *internal_group_ids_node;
    OpenAPI_list_for_each(session_management_subscription_data_1->internal_group_ids, internal_group_ids_node)  {
    if (cJSON_AddStringToObject(internal_group_ids, "", (char*)internal_group_ids_node->data) == NULL) {
        ogs_error("OpenAPI_session_management_subscription_data_1_convertToJSON() failed [internal_group_ids]");
        goto end;
    }
                    }
    }

    if (session_management_subscription_data_1->shared_vn_group_data_ids) {
    cJSON *shared_vn_group_data_ids = cJSON_AddObjectToObject(item, "sharedVnGroupDataIds");
    if (shared_vn_group_data_ids == NULL) {
        ogs_error("OpenAPI_session_management_subscription_data_1_convertToJSON() failed [shared_vn_group_data_ids]");
        goto end;
    }
    cJSON *localMapObject = shared_vn_group_data_ids;
    OpenAPI_lnode_t *shared_vn_group_data_ids_node;
    if (session_management_subscription_data_1->shared_vn_group_data_ids) {
        OpenAPI_list_for_each(session_management_subscription_data_1->shared_vn_group_data_ids, shared_vn_group_data_ids_node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)shared_vn_group_data_ids_node->data;
            }
        }
    }

    if (session_management_subscription_data_1->shared_dnn_configurations_id) {
    if (cJSON_AddStringToObject(item, "sharedDnnConfigurationsId", session_management_subscription_data_1->shared_dnn_configurations_id) == NULL) {
        ogs_error("OpenAPI_session_management_subscription_data_1_convertToJSON() failed [shared_dnn_configurations_id]");
        goto end;
    }
    }

    if (session_management_subscription_data_1->odb_packet_services) {
    if (cJSON_AddStringToObject(item, "odbPacketServices", OpenAPI_odb_packet_services_ToString(session_management_subscription_data_1->odb_packet_services)) == NULL) {
        ogs_error("OpenAPI_session_management_subscription_data_1_convertToJSON() failed [odb_packet_services]");
        goto end;
    }
    }

    if (session_management_subscription_data_1->trace_data) {
    cJSON *trace_data_local_JSON = OpenAPI_trace_data_convertToJSON(session_management_subscription_data_1->trace_data);
    if (trace_data_local_JSON == NULL) {
        ogs_error("OpenAPI_session_management_subscription_data_1_convertToJSON() failed [trace_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "traceData", trace_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_session_management_subscription_data_1_convertToJSON() failed [trace_data]");
        goto end;
    }
    }

    if (session_management_subscription_data_1->shared_trace_data_id) {
    if (cJSON_AddStringToObject(item, "sharedTraceDataId", session_management_subscription_data_1->shared_trace_data_id) == NULL) {
        ogs_error("OpenAPI_session_management_subscription_data_1_convertToJSON() failed [shared_trace_data_id]");
        goto end;
    }
    }

    if (session_management_subscription_data_1->expected_ue_behaviours_list) {
    cJSON *expected_ue_behaviours_list = cJSON_AddObjectToObject(item, "expectedUeBehavioursList");
    if (expected_ue_behaviours_list == NULL) {
        ogs_error("OpenAPI_session_management_subscription_data_1_convertToJSON() failed [expected_ue_behaviours_list]");
        goto end;
    }
    cJSON *localMapObject = expected_ue_behaviours_list;
    OpenAPI_lnode_t *expected_ue_behaviours_list_node;
    if (session_management_subscription_data_1->expected_ue_behaviours_list) {
        OpenAPI_list_for_each(session_management_subscription_data_1->expected_ue_behaviours_list, expected_ue_behaviours_list_node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)expected_ue_behaviours_list_node->data;
        cJSON *itemLocal = localKeyValue->value ?
            OpenAPI_expected_ue_behaviour_data_1_convertToJSON(localKeyValue->value) :
            cJSON_CreateNull();
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_session_management_subscription_data_1_convertToJSON() failed [expected_ue_behaviours_list]");
            goto end;
        }
        cJSON_AddItemToObject(expected_ue_behaviours_list, localKeyValue->key, itemLocal);
            }
        }
    }

    if (session_management_subscription_data_1->suggested_packet_num_dl_list) {
    cJSON *suggested_packet_num_dl_list = cJSON_AddObjectToObject(item, "suggestedPacketNumDlList");
    if (suggested_packet_num_dl_list == NULL) {
        ogs_error("OpenAPI_session_management_subscription_data_1_convertToJSON() failed [suggested_packet_num_dl_list]");
        goto end;
    }
    cJSON *localMapObject = suggested_packet_num_dl_list;
    OpenAPI_lnode_t *suggested_packet_num_dl_list_node;
    if (session_management_subscription_data_1->suggested_packet_num_dl_list) {
        OpenAPI_list_for_each(session_management_subscription_data_1->suggested_packet_num_dl_list, suggested_packet_num_dl_list_node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)suggested_packet_num_dl_list_node->data;
        cJSON *itemLocal = localKeyValue->value ?
            OpenAPI_suggested_packet_num_dl_1_convertToJSON(localKeyValue->value) :
            cJSON_CreateNull();
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_session_management_subscription_data_1_convertToJSON() failed [suggested_packet_num_dl_list]");
            goto end;
        }
        cJSON_AddItemToObject(suggested_packet_num_dl_list, localKeyValue->key, itemLocal);
            }
        }
    }

    if (session_management_subscription_data_1->_3gpp_charging_characteristics) {
    if (cJSON_AddStringToObject(item, "3gppChargingCharacteristics", session_management_subscription_data_1->_3gpp_charging_characteristics) == NULL) {
        ogs_error("OpenAPI_session_management_subscription_data_1_convertToJSON() failed [_3gpp_charging_characteristics]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_session_management_subscription_data_1_t *OpenAPI_session_management_subscription_data_1_parseFromJSON(cJSON *session_management_subscription_data_1JSON)
{
    OpenAPI_session_management_subscription_data_1_t *session_management_subscription_data_1_local_var = NULL;
    cJSON *single_nssai = cJSON_GetObjectItemCaseSensitive(session_management_subscription_data_1JSON, "singleNssai");
    if (!single_nssai) {
        ogs_error("OpenAPI_session_management_subscription_data_1_parseFromJSON() failed [single_nssai]");
        goto end;
    }

    OpenAPI_snssai_t *single_nssai_local_nonprim = NULL;
    single_nssai_local_nonprim = OpenAPI_snssai_parseFromJSON(single_nssai);

    cJSON *dnn_configurations = cJSON_GetObjectItemCaseSensitive(session_management_subscription_data_1JSON, "dnnConfigurations");

    OpenAPI_list_t *dnn_configurationsList;
    if (dnn_configurations) {
    cJSON *dnn_configurations_local_map;
    if (!cJSON_IsObject(dnn_configurations)) {
        ogs_error("OpenAPI_session_management_subscription_data_1_parseFromJSON() failed [dnn_configurations]");
        goto end;
    }
    dnn_configurationsList = OpenAPI_list_create();
    OpenAPI_map_t *localMapKeyPair = NULL;
    cJSON_ArrayForEach(dnn_configurations_local_map, dnn_configurations) {
        cJSON *localMapObject = dnn_configurations_local_map;
        if (cJSON_IsObject(dnn_configurations_local_map)) {
            localMapKeyPair = OpenAPI_map_create(
                ogs_strdup(localMapObject->string), OpenAPI_dnn_configuration_1_parseFromJSON(localMapObject));
        } else if (cJSON_IsNull(dnn_configurations_local_map)) {
            localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
        } else {
            ogs_error("OpenAPI_session_management_subscription_data_1_parseFromJSON() failed [dnn_configurations]");
            goto end;
        }
        OpenAPI_list_add(dnn_configurationsList , localMapKeyPair);
    }
    }

    cJSON *internal_group_ids = cJSON_GetObjectItemCaseSensitive(session_management_subscription_data_1JSON, "internalGroupIds");

    OpenAPI_list_t *internal_group_idsList;
    if (internal_group_ids) {
    cJSON *internal_group_ids_local;
    if (!cJSON_IsArray(internal_group_ids)) {
        ogs_error("OpenAPI_session_management_subscription_data_1_parseFromJSON() failed [internal_group_ids]");
        goto end;
    }
    internal_group_idsList = OpenAPI_list_create();

    cJSON_ArrayForEach(internal_group_ids_local, internal_group_ids) {
    if (!cJSON_IsString(internal_group_ids_local)) {
        ogs_error("OpenAPI_session_management_subscription_data_1_parseFromJSON() failed [internal_group_ids]");
        goto end;
    }
    OpenAPI_list_add(internal_group_idsList , ogs_strdup(internal_group_ids_local->valuestring));
    }
    }

    cJSON *shared_vn_group_data_ids = cJSON_GetObjectItemCaseSensitive(session_management_subscription_data_1JSON, "sharedVnGroupDataIds");

    OpenAPI_list_t *shared_vn_group_data_idsList;
    if (shared_vn_group_data_ids) {
    cJSON *shared_vn_group_data_ids_local_map;
    if (!cJSON_IsObject(shared_vn_group_data_ids)) {
        ogs_error("OpenAPI_session_management_subscription_data_1_parseFromJSON() failed [shared_vn_group_data_ids]");
        goto end;
    }
    shared_vn_group_data_idsList = OpenAPI_list_create();
    OpenAPI_map_t *localMapKeyPair = NULL;
    cJSON_ArrayForEach(shared_vn_group_data_ids_local_map, shared_vn_group_data_ids) {
        cJSON *localMapObject = shared_vn_group_data_ids_local_map;
        OpenAPI_list_add(shared_vn_group_data_idsList , localMapKeyPair);
    }
    }

    cJSON *shared_dnn_configurations_id = cJSON_GetObjectItemCaseSensitive(session_management_subscription_data_1JSON, "sharedDnnConfigurationsId");

    if (shared_dnn_configurations_id) {
    if (!cJSON_IsString(shared_dnn_configurations_id)) {
        ogs_error("OpenAPI_session_management_subscription_data_1_parseFromJSON() failed [shared_dnn_configurations_id]");
        goto end;
    }
    }

    cJSON *odb_packet_services = cJSON_GetObjectItemCaseSensitive(session_management_subscription_data_1JSON, "odbPacketServices");

    OpenAPI_odb_packet_services_e odb_packet_servicesVariable;
    if (odb_packet_services) {
    if (!cJSON_IsString(odb_packet_services)) {
        ogs_error("OpenAPI_session_management_subscription_data_1_parseFromJSON() failed [odb_packet_services]");
        goto end;
    }
    odb_packet_servicesVariable = OpenAPI_odb_packet_services_FromString(odb_packet_services->valuestring);
    }

    cJSON *trace_data = cJSON_GetObjectItemCaseSensitive(session_management_subscription_data_1JSON, "traceData");

    OpenAPI_trace_data_t *trace_data_local_nonprim = NULL;
    if (trace_data) {
    trace_data_local_nonprim = OpenAPI_trace_data_parseFromJSON(trace_data);
    }

    cJSON *shared_trace_data_id = cJSON_GetObjectItemCaseSensitive(session_management_subscription_data_1JSON, "sharedTraceDataId");

    if (shared_trace_data_id) {
    if (!cJSON_IsString(shared_trace_data_id)) {
        ogs_error("OpenAPI_session_management_subscription_data_1_parseFromJSON() failed [shared_trace_data_id]");
        goto end;
    }
    }

    cJSON *expected_ue_behaviours_list = cJSON_GetObjectItemCaseSensitive(session_management_subscription_data_1JSON, "expectedUeBehavioursList");

    OpenAPI_list_t *expected_ue_behaviours_listList;
    if (expected_ue_behaviours_list) {
    cJSON *expected_ue_behaviours_list_local_map;
    if (!cJSON_IsObject(expected_ue_behaviours_list)) {
        ogs_error("OpenAPI_session_management_subscription_data_1_parseFromJSON() failed [expected_ue_behaviours_list]");
        goto end;
    }
    expected_ue_behaviours_listList = OpenAPI_list_create();
    OpenAPI_map_t *localMapKeyPair = NULL;
    cJSON_ArrayForEach(expected_ue_behaviours_list_local_map, expected_ue_behaviours_list) {
        cJSON *localMapObject = expected_ue_behaviours_list_local_map;
        if (cJSON_IsObject(expected_ue_behaviours_list_local_map)) {
            localMapKeyPair = OpenAPI_map_create(
                ogs_strdup(localMapObject->string), OpenAPI_expected_ue_behaviour_data_1_parseFromJSON(localMapObject));
        } else if (cJSON_IsNull(expected_ue_behaviours_list_local_map)) {
            localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
        } else {
            ogs_error("OpenAPI_session_management_subscription_data_1_parseFromJSON() failed [expected_ue_behaviours_list]");
            goto end;
        }
        OpenAPI_list_add(expected_ue_behaviours_listList , localMapKeyPair);
    }
    }

    cJSON *suggested_packet_num_dl_list = cJSON_GetObjectItemCaseSensitive(session_management_subscription_data_1JSON, "suggestedPacketNumDlList");

    OpenAPI_list_t *suggested_packet_num_dl_listList;
    if (suggested_packet_num_dl_list) {
    cJSON *suggested_packet_num_dl_list_local_map;
    if (!cJSON_IsObject(suggested_packet_num_dl_list)) {
        ogs_error("OpenAPI_session_management_subscription_data_1_parseFromJSON() failed [suggested_packet_num_dl_list]");
        goto end;
    }
    suggested_packet_num_dl_listList = OpenAPI_list_create();
    OpenAPI_map_t *localMapKeyPair = NULL;
    cJSON_ArrayForEach(suggested_packet_num_dl_list_local_map, suggested_packet_num_dl_list) {
        cJSON *localMapObject = suggested_packet_num_dl_list_local_map;
        if (cJSON_IsObject(suggested_packet_num_dl_list_local_map)) {
            localMapKeyPair = OpenAPI_map_create(
                ogs_strdup(localMapObject->string), OpenAPI_suggested_packet_num_dl_1_parseFromJSON(localMapObject));
        } else if (cJSON_IsNull(suggested_packet_num_dl_list_local_map)) {
            localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
        } else {
            ogs_error("OpenAPI_session_management_subscription_data_1_parseFromJSON() failed [suggested_packet_num_dl_list]");
            goto end;
        }
        OpenAPI_list_add(suggested_packet_num_dl_listList , localMapKeyPair);
    }
    }

    cJSON *_3gpp_charging_characteristics = cJSON_GetObjectItemCaseSensitive(session_management_subscription_data_1JSON, "3gppChargingCharacteristics");

    if (_3gpp_charging_characteristics) {
    if (!cJSON_IsString(_3gpp_charging_characteristics)) {
        ogs_error("OpenAPI_session_management_subscription_data_1_parseFromJSON() failed [_3gpp_charging_characteristics]");
        goto end;
    }
    }

    session_management_subscription_data_1_local_var = OpenAPI_session_management_subscription_data_1_create (
        single_nssai_local_nonprim,
        dnn_configurations ? dnn_configurationsList : NULL,
        internal_group_ids ? internal_group_idsList : NULL,
        shared_vn_group_data_ids ? shared_vn_group_data_idsList : NULL,
        shared_dnn_configurations_id ? ogs_strdup(shared_dnn_configurations_id->valuestring) : NULL,
        odb_packet_services ? odb_packet_servicesVariable : 0,
        trace_data ? trace_data_local_nonprim : NULL,
        shared_trace_data_id ? ogs_strdup(shared_trace_data_id->valuestring) : NULL,
        expected_ue_behaviours_list ? expected_ue_behaviours_listList : NULL,
        suggested_packet_num_dl_list ? suggested_packet_num_dl_listList : NULL,
        _3gpp_charging_characteristics ? ogs_strdup(_3gpp_charging_characteristics->valuestring) : NULL
    );

    return session_management_subscription_data_1_local_var;
end:
    return NULL;
}

OpenAPI_session_management_subscription_data_1_t *OpenAPI_session_management_subscription_data_1_copy(OpenAPI_session_management_subscription_data_1_t *dst, OpenAPI_session_management_subscription_data_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_session_management_subscription_data_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_session_management_subscription_data_1_convertToJSON() failed");
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

    OpenAPI_session_management_subscription_data_1_free(dst);
    dst = OpenAPI_session_management_subscription_data_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

