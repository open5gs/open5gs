
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "individual_af_authorization_data.h"

OpenAPI_individual_af_authorization_data_t *OpenAPI_individual_af_authorization_data_create(
    char *af_id,
    OpenAPI_aiot_area_t *allowed_area,
    OpenAPI_list_t *allowed_service_operations,
    OpenAPI_list_t *allowed_target_aiot_devices
)
{
    OpenAPI_individual_af_authorization_data_t *individual_af_authorization_data_local_var = ogs_malloc(sizeof(OpenAPI_individual_af_authorization_data_t));
    ogs_assert(individual_af_authorization_data_local_var);

    individual_af_authorization_data_local_var->af_id = af_id;
    individual_af_authorization_data_local_var->allowed_area = allowed_area;
    individual_af_authorization_data_local_var->allowed_service_operations = allowed_service_operations;
    individual_af_authorization_data_local_var->allowed_target_aiot_devices = allowed_target_aiot_devices;

    return individual_af_authorization_data_local_var;
}

void OpenAPI_individual_af_authorization_data_free(OpenAPI_individual_af_authorization_data_t *individual_af_authorization_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == individual_af_authorization_data) {
        return;
    }
    if (individual_af_authorization_data->af_id) {
        ogs_free(individual_af_authorization_data->af_id);
        individual_af_authorization_data->af_id = NULL;
    }
    if (individual_af_authorization_data->allowed_area) {
        OpenAPI_aiot_area_free(individual_af_authorization_data->allowed_area);
        individual_af_authorization_data->allowed_area = NULL;
    }
    if (individual_af_authorization_data->allowed_service_operations) {
        OpenAPI_list_free(individual_af_authorization_data->allowed_service_operations);
        individual_af_authorization_data->allowed_service_operations = NULL;
    }
    if (individual_af_authorization_data->allowed_target_aiot_devices) {
        OpenAPI_list_for_each(individual_af_authorization_data->allowed_target_aiot_devices, node) {
            OpenAPI_allowed_target_aiot_device_free(node->data);
        }
        OpenAPI_list_free(individual_af_authorization_data->allowed_target_aiot_devices);
        individual_af_authorization_data->allowed_target_aiot_devices = NULL;
    }
    ogs_free(individual_af_authorization_data);
}

cJSON *OpenAPI_individual_af_authorization_data_convertToJSON(OpenAPI_individual_af_authorization_data_t *individual_af_authorization_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (individual_af_authorization_data == NULL) {
        ogs_error("OpenAPI_individual_af_authorization_data_convertToJSON() failed [IndividualAfAuthorizationData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!individual_af_authorization_data->af_id) {
        ogs_error("OpenAPI_individual_af_authorization_data_convertToJSON() failed [af_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "afId", individual_af_authorization_data->af_id) == NULL) {
        ogs_error("OpenAPI_individual_af_authorization_data_convertToJSON() failed [af_id]");
        goto end;
    }

    if (individual_af_authorization_data->allowed_area) {
    cJSON *allowed_area_local_JSON = OpenAPI_aiot_area_convertToJSON(individual_af_authorization_data->allowed_area);
    if (allowed_area_local_JSON == NULL) {
        ogs_error("OpenAPI_individual_af_authorization_data_convertToJSON() failed [allowed_area]");
        goto end;
    }
    cJSON_AddItemToObject(item, "allowedArea", allowed_area_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_individual_af_authorization_data_convertToJSON() failed [allowed_area]");
        goto end;
    }
    }

    if (individual_af_authorization_data->allowed_service_operations != OpenAPI_allowed_service_operation_NULL) {
    cJSON *allowed_service_operationsList = cJSON_AddArrayToObject(item, "allowedServiceOperations");
    if (allowed_service_operationsList == NULL) {
        ogs_error("OpenAPI_individual_af_authorization_data_convertToJSON() failed [allowed_service_operations]");
        goto end;
    }
    OpenAPI_list_for_each(individual_af_authorization_data->allowed_service_operations, node) {
        if (cJSON_AddStringToObject(allowed_service_operationsList, "", OpenAPI_allowed_service_operation_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_individual_af_authorization_data_convertToJSON() failed [allowed_service_operations]");
            goto end;
        }
    }
    }

    if (individual_af_authorization_data->allowed_target_aiot_devices) {
    cJSON *allowed_target_aiot_devicesList = cJSON_AddArrayToObject(item, "allowedTargetAiotDevices");
    if (allowed_target_aiot_devicesList == NULL) {
        ogs_error("OpenAPI_individual_af_authorization_data_convertToJSON() failed [allowed_target_aiot_devices]");
        goto end;
    }
    OpenAPI_list_for_each(individual_af_authorization_data->allowed_target_aiot_devices, node) {
        cJSON *itemLocal = OpenAPI_allowed_target_aiot_device_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_individual_af_authorization_data_convertToJSON() failed [allowed_target_aiot_devices]");
            goto end;
        }
        cJSON_AddItemToArray(allowed_target_aiot_devicesList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_individual_af_authorization_data_t *OpenAPI_individual_af_authorization_data_parseFromJSON(cJSON *individual_af_authorization_dataJSON)
{
    OpenAPI_individual_af_authorization_data_t *individual_af_authorization_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *af_id = NULL;
    cJSON *allowed_area = NULL;
    OpenAPI_aiot_area_t *allowed_area_local_nonprim = NULL;
    cJSON *allowed_service_operations = NULL;
    OpenAPI_list_t *allowed_service_operationsList = NULL;
    cJSON *allowed_target_aiot_devices = NULL;
    OpenAPI_list_t *allowed_target_aiot_devicesList = NULL;
    af_id = cJSON_GetObjectItemCaseSensitive(individual_af_authorization_dataJSON, "afId");
    if (!af_id) {
        ogs_error("OpenAPI_individual_af_authorization_data_parseFromJSON() failed [af_id]");
        goto end;
    }
    if (!cJSON_IsString(af_id)) {
        ogs_error("OpenAPI_individual_af_authorization_data_parseFromJSON() failed [af_id]");
        goto end;
    }

    allowed_area = cJSON_GetObjectItemCaseSensitive(individual_af_authorization_dataJSON, "allowedArea");
    if (allowed_area) {
    allowed_area_local_nonprim = OpenAPI_aiot_area_parseFromJSON(allowed_area);
    if (!allowed_area_local_nonprim) {
        ogs_error("OpenAPI_aiot_area_parseFromJSON failed [allowed_area]");
        goto end;
    }
    }

    allowed_service_operations = cJSON_GetObjectItemCaseSensitive(individual_af_authorization_dataJSON, "allowedServiceOperations");
    if (allowed_service_operations) {
        cJSON *allowed_service_operations_local = NULL;
        if (!cJSON_IsArray(allowed_service_operations)) {
            ogs_error("OpenAPI_individual_af_authorization_data_parseFromJSON() failed [allowed_service_operations]");
            goto end;
        }

        allowed_service_operationsList = OpenAPI_list_create();

        cJSON_ArrayForEach(allowed_service_operations_local, allowed_service_operations) {
            OpenAPI_allowed_service_operation_e localEnum = OpenAPI_allowed_service_operation_NULL;
            if (!cJSON_IsString(allowed_service_operations_local)) {
                ogs_error("OpenAPI_individual_af_authorization_data_parseFromJSON() failed [allowed_service_operations]");
                goto end;
            }
            localEnum = OpenAPI_allowed_service_operation_FromString(allowed_service_operations_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"allowed_service_operations\" is not supported. Ignoring it ...",
                         allowed_service_operations_local->valuestring);
            } else {
                OpenAPI_list_add(allowed_service_operationsList, (void *)localEnum);
            }
        }
        if (allowed_service_operationsList->count == 0) {
            ogs_error("OpenAPI_individual_af_authorization_data_parseFromJSON() failed: Expected allowed_service_operationsList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    allowed_target_aiot_devices = cJSON_GetObjectItemCaseSensitive(individual_af_authorization_dataJSON, "allowedTargetAiotDevices");
    if (allowed_target_aiot_devices) {
        cJSON *allowed_target_aiot_devices_local = NULL;
        if (!cJSON_IsArray(allowed_target_aiot_devices)) {
            ogs_error("OpenAPI_individual_af_authorization_data_parseFromJSON() failed [allowed_target_aiot_devices]");
            goto end;
        }

        allowed_target_aiot_devicesList = OpenAPI_list_create();

        cJSON_ArrayForEach(allowed_target_aiot_devices_local, allowed_target_aiot_devices) {
            if (!cJSON_IsObject(allowed_target_aiot_devices_local)) {
                ogs_error("OpenAPI_individual_af_authorization_data_parseFromJSON() failed [allowed_target_aiot_devices]");
                goto end;
            }
            OpenAPI_allowed_target_aiot_device_t *allowed_target_aiot_devicesItem = OpenAPI_allowed_target_aiot_device_parseFromJSON(allowed_target_aiot_devices_local);
            if (!allowed_target_aiot_devicesItem) {
                ogs_error("No allowed_target_aiot_devicesItem");
                goto end;
            }
            OpenAPI_list_add(allowed_target_aiot_devicesList, allowed_target_aiot_devicesItem);
        }
    }

    individual_af_authorization_data_local_var = OpenAPI_individual_af_authorization_data_create (
        ogs_strdup(af_id->valuestring),
        allowed_area ? allowed_area_local_nonprim : NULL,
        allowed_service_operations ? allowed_service_operationsList : NULL,
        allowed_target_aiot_devices ? allowed_target_aiot_devicesList : NULL
    );

    return individual_af_authorization_data_local_var;
end:
    if (allowed_area_local_nonprim) {
        OpenAPI_aiot_area_free(allowed_area_local_nonprim);
        allowed_area_local_nonprim = NULL;
    }
    if (allowed_service_operationsList) {
        OpenAPI_list_free(allowed_service_operationsList);
        allowed_service_operationsList = NULL;
    }
    if (allowed_target_aiot_devicesList) {
        OpenAPI_list_for_each(allowed_target_aiot_devicesList, node) {
            OpenAPI_allowed_target_aiot_device_free(node->data);
        }
        OpenAPI_list_free(allowed_target_aiot_devicesList);
        allowed_target_aiot_devicesList = NULL;
    }
    return NULL;
}

OpenAPI_individual_af_authorization_data_t *OpenAPI_individual_af_authorization_data_copy(OpenAPI_individual_af_authorization_data_t *dst, OpenAPI_individual_af_authorization_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_individual_af_authorization_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_individual_af_authorization_data_convertToJSON() failed");
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

    OpenAPI_individual_af_authorization_data_free(dst);
    dst = OpenAPI_individual_af_authorization_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

