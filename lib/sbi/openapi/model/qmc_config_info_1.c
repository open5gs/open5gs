
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "qmc_config_info_1.h"

OpenAPI_qmc_config_info_1_t *OpenAPI_qmc_config_info_1_create(
    char *qoe_reference,
    OpenAPI_qoe_service_type_e service_type,
    OpenAPI_list_t *slice_scope,
    OpenAPI_qmc_area_scope_1_t *area_scope,
    OpenAPI_ip_addr_1_t *qoe_collection_entity_address,
    OpenAPI_qoe_target_1_t *qoe_target,
    char *mdt_alignment_info,
    OpenAPI_list_t *available_ran_visible_qoe_metrics,
    char *container_for_app_layer_meas_config,
    OpenAPI_mbs_service_type_e mbs_communication_service_type
)
{
    OpenAPI_qmc_config_info_1_t *qmc_config_info_1_local_var = ogs_malloc(sizeof(OpenAPI_qmc_config_info_1_t));
    ogs_assert(qmc_config_info_1_local_var);

    qmc_config_info_1_local_var->qoe_reference = qoe_reference;
    qmc_config_info_1_local_var->service_type = service_type;
    qmc_config_info_1_local_var->slice_scope = slice_scope;
    qmc_config_info_1_local_var->area_scope = area_scope;
    qmc_config_info_1_local_var->qoe_collection_entity_address = qoe_collection_entity_address;
    qmc_config_info_1_local_var->qoe_target = qoe_target;
    qmc_config_info_1_local_var->mdt_alignment_info = mdt_alignment_info;
    qmc_config_info_1_local_var->available_ran_visible_qoe_metrics = available_ran_visible_qoe_metrics;
    qmc_config_info_1_local_var->container_for_app_layer_meas_config = container_for_app_layer_meas_config;
    qmc_config_info_1_local_var->mbs_communication_service_type = mbs_communication_service_type;

    return qmc_config_info_1_local_var;
}

void OpenAPI_qmc_config_info_1_free(OpenAPI_qmc_config_info_1_t *qmc_config_info_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == qmc_config_info_1) {
        return;
    }
    if (qmc_config_info_1->qoe_reference) {
        ogs_free(qmc_config_info_1->qoe_reference);
        qmc_config_info_1->qoe_reference = NULL;
    }
    if (qmc_config_info_1->slice_scope) {
        OpenAPI_list_for_each(qmc_config_info_1->slice_scope, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(qmc_config_info_1->slice_scope);
        qmc_config_info_1->slice_scope = NULL;
    }
    if (qmc_config_info_1->area_scope) {
        OpenAPI_qmc_area_scope_1_free(qmc_config_info_1->area_scope);
        qmc_config_info_1->area_scope = NULL;
    }
    if (qmc_config_info_1->qoe_collection_entity_address) {
        OpenAPI_ip_addr_1_free(qmc_config_info_1->qoe_collection_entity_address);
        qmc_config_info_1->qoe_collection_entity_address = NULL;
    }
    if (qmc_config_info_1->qoe_target) {
        OpenAPI_qoe_target_1_free(qmc_config_info_1->qoe_target);
        qmc_config_info_1->qoe_target = NULL;
    }
    if (qmc_config_info_1->mdt_alignment_info) {
        ogs_free(qmc_config_info_1->mdt_alignment_info);
        qmc_config_info_1->mdt_alignment_info = NULL;
    }
    if (qmc_config_info_1->available_ran_visible_qoe_metrics) {
        OpenAPI_list_free(qmc_config_info_1->available_ran_visible_qoe_metrics);
        qmc_config_info_1->available_ran_visible_qoe_metrics = NULL;
    }
    if (qmc_config_info_1->container_for_app_layer_meas_config) {
        ogs_free(qmc_config_info_1->container_for_app_layer_meas_config);
        qmc_config_info_1->container_for_app_layer_meas_config = NULL;
    }
    ogs_free(qmc_config_info_1);
}

cJSON *OpenAPI_qmc_config_info_1_convertToJSON(OpenAPI_qmc_config_info_1_t *qmc_config_info_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (qmc_config_info_1 == NULL) {
        ogs_error("OpenAPI_qmc_config_info_1_convertToJSON() failed [QmcConfigInfo_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!qmc_config_info_1->qoe_reference) {
        ogs_error("OpenAPI_qmc_config_info_1_convertToJSON() failed [qoe_reference]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "qoeReference", qmc_config_info_1->qoe_reference) == NULL) {
        ogs_error("OpenAPI_qmc_config_info_1_convertToJSON() failed [qoe_reference]");
        goto end;
    }

    if (qmc_config_info_1->service_type != OpenAPI_qoe_service_type_NULL) {
    if (cJSON_AddStringToObject(item, "serviceType", OpenAPI_qoe_service_type_ToString(qmc_config_info_1->service_type)) == NULL) {
        ogs_error("OpenAPI_qmc_config_info_1_convertToJSON() failed [service_type]");
        goto end;
    }
    }

    if (qmc_config_info_1->slice_scope) {
    cJSON *slice_scopeList = cJSON_AddArrayToObject(item, "sliceScope");
    if (slice_scopeList == NULL) {
        ogs_error("OpenAPI_qmc_config_info_1_convertToJSON() failed [slice_scope]");
        goto end;
    }
    OpenAPI_list_for_each(qmc_config_info_1->slice_scope, node) {
        cJSON *itemLocal = OpenAPI_snssai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_qmc_config_info_1_convertToJSON() failed [slice_scope]");
            goto end;
        }
        cJSON_AddItemToArray(slice_scopeList, itemLocal);
    }
    }

    if (qmc_config_info_1->area_scope) {
    cJSON *area_scope_local_JSON = OpenAPI_qmc_area_scope_1_convertToJSON(qmc_config_info_1->area_scope);
    if (area_scope_local_JSON == NULL) {
        ogs_error("OpenAPI_qmc_config_info_1_convertToJSON() failed [area_scope]");
        goto end;
    }
    cJSON_AddItemToObject(item, "areaScope", area_scope_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_qmc_config_info_1_convertToJSON() failed [area_scope]");
        goto end;
    }
    }

    if (qmc_config_info_1->qoe_collection_entity_address) {
    cJSON *qoe_collection_entity_address_local_JSON = OpenAPI_ip_addr_1_convertToJSON(qmc_config_info_1->qoe_collection_entity_address);
    if (qoe_collection_entity_address_local_JSON == NULL) {
        ogs_error("OpenAPI_qmc_config_info_1_convertToJSON() failed [qoe_collection_entity_address]");
        goto end;
    }
    cJSON_AddItemToObject(item, "qoeCollectionEntityAddress", qoe_collection_entity_address_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_qmc_config_info_1_convertToJSON() failed [qoe_collection_entity_address]");
        goto end;
    }
    }

    if (qmc_config_info_1->qoe_target) {
    cJSON *qoe_target_local_JSON = OpenAPI_qoe_target_1_convertToJSON(qmc_config_info_1->qoe_target);
    if (qoe_target_local_JSON == NULL) {
        ogs_error("OpenAPI_qmc_config_info_1_convertToJSON() failed [qoe_target]");
        goto end;
    }
    cJSON_AddItemToObject(item, "qoeTarget", qoe_target_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_qmc_config_info_1_convertToJSON() failed [qoe_target]");
        goto end;
    }
    }

    if (qmc_config_info_1->mdt_alignment_info) {
    if (cJSON_AddStringToObject(item, "mdtAlignmentInfo", qmc_config_info_1->mdt_alignment_info) == NULL) {
        ogs_error("OpenAPI_qmc_config_info_1_convertToJSON() failed [mdt_alignment_info]");
        goto end;
    }
    }

    if (qmc_config_info_1->available_ran_visible_qoe_metrics != OpenAPI_available_ran_visible_qoe_metric_NULL) {
    cJSON *available_ran_visible_qoe_metricsList = cJSON_AddArrayToObject(item, "availableRanVisibleQoeMetrics");
    if (available_ran_visible_qoe_metricsList == NULL) {
        ogs_error("OpenAPI_qmc_config_info_1_convertToJSON() failed [available_ran_visible_qoe_metrics]");
        goto end;
    }
    OpenAPI_list_for_each(qmc_config_info_1->available_ran_visible_qoe_metrics, node) {
        if (cJSON_AddStringToObject(available_ran_visible_qoe_metricsList, "", OpenAPI_available_ran_visible_qoe_metric_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_qmc_config_info_1_convertToJSON() failed [available_ran_visible_qoe_metrics]");
            goto end;
        }
    }
    }

    if (qmc_config_info_1->container_for_app_layer_meas_config) {
    if (cJSON_AddStringToObject(item, "containerForAppLayerMeasConfig", qmc_config_info_1->container_for_app_layer_meas_config) == NULL) {
        ogs_error("OpenAPI_qmc_config_info_1_convertToJSON() failed [container_for_app_layer_meas_config]");
        goto end;
    }
    }

    if (qmc_config_info_1->mbs_communication_service_type != OpenAPI_mbs_service_type_NULL) {
    if (cJSON_AddStringToObject(item, "mbsCommunicationServiceType", OpenAPI_mbs_service_type_ToString(qmc_config_info_1->mbs_communication_service_type)) == NULL) {
        ogs_error("OpenAPI_qmc_config_info_1_convertToJSON() failed [mbs_communication_service_type]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_qmc_config_info_1_t *OpenAPI_qmc_config_info_1_parseFromJSON(cJSON *qmc_config_info_1JSON)
{
    OpenAPI_qmc_config_info_1_t *qmc_config_info_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *qoe_reference = NULL;
    cJSON *service_type = NULL;
    OpenAPI_qoe_service_type_e service_typeVariable = 0;
    cJSON *slice_scope = NULL;
    OpenAPI_list_t *slice_scopeList = NULL;
    cJSON *area_scope = NULL;
    OpenAPI_qmc_area_scope_1_t *area_scope_local_nonprim = NULL;
    cJSON *qoe_collection_entity_address = NULL;
    OpenAPI_ip_addr_1_t *qoe_collection_entity_address_local_nonprim = NULL;
    cJSON *qoe_target = NULL;
    OpenAPI_qoe_target_1_t *qoe_target_local_nonprim = NULL;
    cJSON *mdt_alignment_info = NULL;
    cJSON *available_ran_visible_qoe_metrics = NULL;
    OpenAPI_list_t *available_ran_visible_qoe_metricsList = NULL;
    cJSON *container_for_app_layer_meas_config = NULL;
    cJSON *mbs_communication_service_type = NULL;
    OpenAPI_mbs_service_type_e mbs_communication_service_typeVariable = 0;
    qoe_reference = cJSON_GetObjectItemCaseSensitive(qmc_config_info_1JSON, "qoeReference");
    if (!qoe_reference) {
        ogs_error("OpenAPI_qmc_config_info_1_parseFromJSON() failed [qoe_reference]");
        goto end;
    }
    if (!cJSON_IsString(qoe_reference)) {
        ogs_error("OpenAPI_qmc_config_info_1_parseFromJSON() failed [qoe_reference]");
        goto end;
    }

    service_type = cJSON_GetObjectItemCaseSensitive(qmc_config_info_1JSON, "serviceType");
    if (service_type) {
    if (!cJSON_IsString(service_type)) {
        ogs_error("OpenAPI_qmc_config_info_1_parseFromJSON() failed [service_type]");
        goto end;
    }
    service_typeVariable = OpenAPI_qoe_service_type_FromString(service_type->valuestring);
    }

    slice_scope = cJSON_GetObjectItemCaseSensitive(qmc_config_info_1JSON, "sliceScope");
    if (slice_scope) {
        cJSON *slice_scope_local = NULL;
        if (!cJSON_IsArray(slice_scope)) {
            ogs_error("OpenAPI_qmc_config_info_1_parseFromJSON() failed [slice_scope]");
            goto end;
        }

        slice_scopeList = OpenAPI_list_create();

        cJSON_ArrayForEach(slice_scope_local, slice_scope) {
            if (!cJSON_IsObject(slice_scope_local)) {
                ogs_error("OpenAPI_qmc_config_info_1_parseFromJSON() failed [slice_scope]");
                goto end;
            }
            OpenAPI_snssai_t *slice_scopeItem = OpenAPI_snssai_parseFromJSON(slice_scope_local);
            if (!slice_scopeItem) {
                ogs_error("No slice_scopeItem");
                goto end;
            }
            OpenAPI_list_add(slice_scopeList, slice_scopeItem);
        }
    }

    area_scope = cJSON_GetObjectItemCaseSensitive(qmc_config_info_1JSON, "areaScope");
    if (area_scope) {
    area_scope_local_nonprim = OpenAPI_qmc_area_scope_1_parseFromJSON(area_scope);
    if (!area_scope_local_nonprim) {
        ogs_error("OpenAPI_qmc_area_scope_1_parseFromJSON failed [area_scope]");
        goto end;
    }
    }

    qoe_collection_entity_address = cJSON_GetObjectItemCaseSensitive(qmc_config_info_1JSON, "qoeCollectionEntityAddress");
    if (qoe_collection_entity_address) {
    qoe_collection_entity_address_local_nonprim = OpenAPI_ip_addr_1_parseFromJSON(qoe_collection_entity_address);
    if (!qoe_collection_entity_address_local_nonprim) {
        ogs_error("OpenAPI_ip_addr_1_parseFromJSON failed [qoe_collection_entity_address]");
        goto end;
    }
    }

    qoe_target = cJSON_GetObjectItemCaseSensitive(qmc_config_info_1JSON, "qoeTarget");
    if (qoe_target) {
    qoe_target_local_nonprim = OpenAPI_qoe_target_1_parseFromJSON(qoe_target);
    if (!qoe_target_local_nonprim) {
        ogs_error("OpenAPI_qoe_target_1_parseFromJSON failed [qoe_target]");
        goto end;
    }
    }

    mdt_alignment_info = cJSON_GetObjectItemCaseSensitive(qmc_config_info_1JSON, "mdtAlignmentInfo");
    if (mdt_alignment_info) {
    if (!cJSON_IsString(mdt_alignment_info) && !cJSON_IsNull(mdt_alignment_info)) {
        ogs_error("OpenAPI_qmc_config_info_1_parseFromJSON() failed [mdt_alignment_info]");
        goto end;
    }
    }

    available_ran_visible_qoe_metrics = cJSON_GetObjectItemCaseSensitive(qmc_config_info_1JSON, "availableRanVisibleQoeMetrics");
    if (available_ran_visible_qoe_metrics) {
        cJSON *available_ran_visible_qoe_metrics_local = NULL;
        if (!cJSON_IsArray(available_ran_visible_qoe_metrics)) {
            ogs_error("OpenAPI_qmc_config_info_1_parseFromJSON() failed [available_ran_visible_qoe_metrics]");
            goto end;
        }

        available_ran_visible_qoe_metricsList = OpenAPI_list_create();

        cJSON_ArrayForEach(available_ran_visible_qoe_metrics_local, available_ran_visible_qoe_metrics) {
            OpenAPI_available_ran_visible_qoe_metric_e localEnum = OpenAPI_available_ran_visible_qoe_metric_NULL;
            if (!cJSON_IsString(available_ran_visible_qoe_metrics_local)) {
                ogs_error("OpenAPI_qmc_config_info_1_parseFromJSON() failed [available_ran_visible_qoe_metrics]");
                goto end;
            }
            localEnum = OpenAPI_available_ran_visible_qoe_metric_FromString(available_ran_visible_qoe_metrics_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"available_ran_visible_qoe_metrics\" is not supported. Ignoring it ...",
                         available_ran_visible_qoe_metrics_local->valuestring);
            } else {
                OpenAPI_list_add(available_ran_visible_qoe_metricsList, (void *)localEnum);
            }
        }
        if (available_ran_visible_qoe_metricsList->count == 0) {
            ogs_error("OpenAPI_qmc_config_info_1_parseFromJSON() failed: Expected available_ran_visible_qoe_metricsList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    container_for_app_layer_meas_config = cJSON_GetObjectItemCaseSensitive(qmc_config_info_1JSON, "containerForAppLayerMeasConfig");
    if (container_for_app_layer_meas_config) {
    if (!cJSON_IsString(container_for_app_layer_meas_config) && !cJSON_IsNull(container_for_app_layer_meas_config)) {
        ogs_error("OpenAPI_qmc_config_info_1_parseFromJSON() failed [container_for_app_layer_meas_config]");
        goto end;
    }
    }

    mbs_communication_service_type = cJSON_GetObjectItemCaseSensitive(qmc_config_info_1JSON, "mbsCommunicationServiceType");
    if (mbs_communication_service_type) {
    if (!cJSON_IsString(mbs_communication_service_type)) {
        ogs_error("OpenAPI_qmc_config_info_1_parseFromJSON() failed [mbs_communication_service_type]");
        goto end;
    }
    mbs_communication_service_typeVariable = OpenAPI_mbs_service_type_FromString(mbs_communication_service_type->valuestring);
    }

    qmc_config_info_1_local_var = OpenAPI_qmc_config_info_1_create (
        ogs_strdup(qoe_reference->valuestring),
        service_type ? service_typeVariable : 0,
        slice_scope ? slice_scopeList : NULL,
        area_scope ? area_scope_local_nonprim : NULL,
        qoe_collection_entity_address ? qoe_collection_entity_address_local_nonprim : NULL,
        qoe_target ? qoe_target_local_nonprim : NULL,
        mdt_alignment_info && !cJSON_IsNull(mdt_alignment_info) ? ogs_strdup(mdt_alignment_info->valuestring) : NULL,
        available_ran_visible_qoe_metrics ? available_ran_visible_qoe_metricsList : NULL,
        container_for_app_layer_meas_config && !cJSON_IsNull(container_for_app_layer_meas_config) ? ogs_strdup(container_for_app_layer_meas_config->valuestring) : NULL,
        mbs_communication_service_type ? mbs_communication_service_typeVariable : 0
    );

    return qmc_config_info_1_local_var;
end:
    if (slice_scopeList) {
        OpenAPI_list_for_each(slice_scopeList, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(slice_scopeList);
        slice_scopeList = NULL;
    }
    if (area_scope_local_nonprim) {
        OpenAPI_qmc_area_scope_1_free(area_scope_local_nonprim);
        area_scope_local_nonprim = NULL;
    }
    if (qoe_collection_entity_address_local_nonprim) {
        OpenAPI_ip_addr_1_free(qoe_collection_entity_address_local_nonprim);
        qoe_collection_entity_address_local_nonprim = NULL;
    }
    if (qoe_target_local_nonprim) {
        OpenAPI_qoe_target_1_free(qoe_target_local_nonprim);
        qoe_target_local_nonprim = NULL;
    }
    if (available_ran_visible_qoe_metricsList) {
        OpenAPI_list_free(available_ran_visible_qoe_metricsList);
        available_ran_visible_qoe_metricsList = NULL;
    }
    return NULL;
}

OpenAPI_qmc_config_info_1_t *OpenAPI_qmc_config_info_1_copy(OpenAPI_qmc_config_info_1_t *dst, OpenAPI_qmc_config_info_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_qmc_config_info_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_qmc_config_info_1_convertToJSON() failed");
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

    OpenAPI_qmc_config_info_1_free(dst);
    dst = OpenAPI_qmc_config_info_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

