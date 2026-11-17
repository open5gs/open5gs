
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nsm_charging_information.h"

OpenAPI_nsm_charging_information_t *OpenAPI_nsm_charging_information_create(
    OpenAPI_management_operation_e management_operation,
    char *id_network_slice_instance,
    OpenAPI_list_t *list_ofservice_profile_charging_information,
    OpenAPI_management_operation_status_e management_operation_status,
    OpenAPI_operational_state_e management_operational_state,
    OpenAPI_administrative_state_e management_administrative_state
)
{
    OpenAPI_nsm_charging_information_t *nsm_charging_information_local_var = ogs_malloc(sizeof(OpenAPI_nsm_charging_information_t));
    ogs_assert(nsm_charging_information_local_var);

    nsm_charging_information_local_var->management_operation = management_operation;
    nsm_charging_information_local_var->id_network_slice_instance = id_network_slice_instance;
    nsm_charging_information_local_var->list_ofservice_profile_charging_information = list_ofservice_profile_charging_information;
    nsm_charging_information_local_var->management_operation_status = management_operation_status;
    nsm_charging_information_local_var->management_operational_state = management_operational_state;
    nsm_charging_information_local_var->management_administrative_state = management_administrative_state;

    return nsm_charging_information_local_var;
}

void OpenAPI_nsm_charging_information_free(OpenAPI_nsm_charging_information_t *nsm_charging_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == nsm_charging_information) {
        return;
    }
    if (nsm_charging_information->id_network_slice_instance) {
        ogs_free(nsm_charging_information->id_network_slice_instance);
        nsm_charging_information->id_network_slice_instance = NULL;
    }
    if (nsm_charging_information->list_ofservice_profile_charging_information) {
        OpenAPI_list_for_each(nsm_charging_information->list_ofservice_profile_charging_information, node) {
            OpenAPI_service_profile_charging_information_free(node->data);
        }
        OpenAPI_list_free(nsm_charging_information->list_ofservice_profile_charging_information);
        nsm_charging_information->list_ofservice_profile_charging_information = NULL;
    }
    ogs_free(nsm_charging_information);
}

cJSON *OpenAPI_nsm_charging_information_convertToJSON(OpenAPI_nsm_charging_information_t *nsm_charging_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (nsm_charging_information == NULL) {
        ogs_error("OpenAPI_nsm_charging_information_convertToJSON() failed [NSMChargingInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (nsm_charging_information->management_operation == OpenAPI_management_operation_NULL) {
        ogs_error("OpenAPI_nsm_charging_information_convertToJSON() failed [management_operation]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "managementOperation", OpenAPI_management_operation_ToString(nsm_charging_information->management_operation)) == NULL) {
        ogs_error("OpenAPI_nsm_charging_information_convertToJSON() failed [management_operation]");
        goto end;
    }

    if (nsm_charging_information->id_network_slice_instance) {
    if (cJSON_AddStringToObject(item, "idNetworkSliceInstance", nsm_charging_information->id_network_slice_instance) == NULL) {
        ogs_error("OpenAPI_nsm_charging_information_convertToJSON() failed [id_network_slice_instance]");
        goto end;
    }
    }

    if (nsm_charging_information->list_ofservice_profile_charging_information) {
    cJSON *list_ofservice_profile_charging_informationList = cJSON_AddArrayToObject(item, "listOfserviceProfileChargingInformation");
    if (list_ofservice_profile_charging_informationList == NULL) {
        ogs_error("OpenAPI_nsm_charging_information_convertToJSON() failed [list_ofservice_profile_charging_information]");
        goto end;
    }
    OpenAPI_list_for_each(nsm_charging_information->list_ofservice_profile_charging_information, node) {
        cJSON *itemLocal = OpenAPI_service_profile_charging_information_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nsm_charging_information_convertToJSON() failed [list_ofservice_profile_charging_information]");
            goto end;
        }
        cJSON_AddItemToArray(list_ofservice_profile_charging_informationList, itemLocal);
    }
    }

    if (nsm_charging_information->management_operation_status != OpenAPI_management_operation_status_NULL) {
    if (cJSON_AddStringToObject(item, "managementOperationStatus", OpenAPI_management_operation_status_ToString(nsm_charging_information->management_operation_status)) == NULL) {
        ogs_error("OpenAPI_nsm_charging_information_convertToJSON() failed [management_operation_status]");
        goto end;
    }
    }

    if (nsm_charging_information->management_operational_state != OpenAPI_operational_state_NULL) {
    if (cJSON_AddStringToObject(item, "managementOperationalState", OpenAPI_operational_state_ToString(nsm_charging_information->management_operational_state)) == NULL) {
        ogs_error("OpenAPI_nsm_charging_information_convertToJSON() failed [management_operational_state]");
        goto end;
    }
    }

    if (nsm_charging_information->management_administrative_state != OpenAPI_administrative_state_NULL) {
    if (cJSON_AddStringToObject(item, "managementAdministrativeState", OpenAPI_administrative_state_ToString(nsm_charging_information->management_administrative_state)) == NULL) {
        ogs_error("OpenAPI_nsm_charging_information_convertToJSON() failed [management_administrative_state]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_nsm_charging_information_t *OpenAPI_nsm_charging_information_parseFromJSON(cJSON *nsm_charging_informationJSON)
{
    OpenAPI_nsm_charging_information_t *nsm_charging_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *management_operation = NULL;
    OpenAPI_management_operation_e management_operationVariable = 0;
    cJSON *id_network_slice_instance = NULL;
    cJSON *list_ofservice_profile_charging_information = NULL;
    OpenAPI_list_t *list_ofservice_profile_charging_informationList = NULL;
    cJSON *management_operation_status = NULL;
    OpenAPI_management_operation_status_e management_operation_statusVariable = 0;
    cJSON *management_operational_state = NULL;
    OpenAPI_operational_state_e management_operational_stateVariable = 0;
    cJSON *management_administrative_state = NULL;
    OpenAPI_administrative_state_e management_administrative_stateVariable = 0;
    management_operation = cJSON_GetObjectItemCaseSensitive(nsm_charging_informationJSON, "managementOperation");
    if (!management_operation) {
        ogs_error("OpenAPI_nsm_charging_information_parseFromJSON() failed [management_operation]");
        goto end;
    }
    if (!cJSON_IsString(management_operation)) {
        ogs_error("OpenAPI_nsm_charging_information_parseFromJSON() failed [management_operation]");
        goto end;
    }
    management_operationVariable = OpenAPI_management_operation_FromString(management_operation->valuestring);

    id_network_slice_instance = cJSON_GetObjectItemCaseSensitive(nsm_charging_informationJSON, "idNetworkSliceInstance");
    if (id_network_slice_instance) {
    if (!cJSON_IsString(id_network_slice_instance) && !cJSON_IsNull(id_network_slice_instance)) {
        ogs_error("OpenAPI_nsm_charging_information_parseFromJSON() failed [id_network_slice_instance]");
        goto end;
    }
    }

    list_ofservice_profile_charging_information = cJSON_GetObjectItemCaseSensitive(nsm_charging_informationJSON, "listOfserviceProfileChargingInformation");
    if (list_ofservice_profile_charging_information) {
        cJSON *list_ofservice_profile_charging_information_local = NULL;
        if (!cJSON_IsArray(list_ofservice_profile_charging_information)) {
            ogs_error("OpenAPI_nsm_charging_information_parseFromJSON() failed [list_ofservice_profile_charging_information]");
            goto end;
        }

        list_ofservice_profile_charging_informationList = OpenAPI_list_create();

        cJSON_ArrayForEach(list_ofservice_profile_charging_information_local, list_ofservice_profile_charging_information) {
            if (!cJSON_IsObject(list_ofservice_profile_charging_information_local)) {
                ogs_error("OpenAPI_nsm_charging_information_parseFromJSON() failed [list_ofservice_profile_charging_information]");
                goto end;
            }
            OpenAPI_service_profile_charging_information_t *list_ofservice_profile_charging_informationItem = OpenAPI_service_profile_charging_information_parseFromJSON(list_ofservice_profile_charging_information_local);
            if (!list_ofservice_profile_charging_informationItem) {
                ogs_error("No list_ofservice_profile_charging_informationItem");
                goto end;
            }
            OpenAPI_list_add(list_ofservice_profile_charging_informationList, list_ofservice_profile_charging_informationItem);
        }
    }

    management_operation_status = cJSON_GetObjectItemCaseSensitive(nsm_charging_informationJSON, "managementOperationStatus");
    if (management_operation_status) {
    if (!cJSON_IsString(management_operation_status)) {
        ogs_error("OpenAPI_nsm_charging_information_parseFromJSON() failed [management_operation_status]");
        goto end;
    }
    management_operation_statusVariable = OpenAPI_management_operation_status_FromString(management_operation_status->valuestring);
    }

    management_operational_state = cJSON_GetObjectItemCaseSensitive(nsm_charging_informationJSON, "managementOperationalState");
    if (management_operational_state) {
    if (!cJSON_IsString(management_operational_state)) {
        ogs_error("OpenAPI_nsm_charging_information_parseFromJSON() failed [management_operational_state]");
        goto end;
    }
    management_operational_stateVariable = OpenAPI_operational_state_FromString(management_operational_state->valuestring);
    }

    management_administrative_state = cJSON_GetObjectItemCaseSensitive(nsm_charging_informationJSON, "managementAdministrativeState");
    if (management_administrative_state) {
    if (!cJSON_IsString(management_administrative_state)) {
        ogs_error("OpenAPI_nsm_charging_information_parseFromJSON() failed [management_administrative_state]");
        goto end;
    }
    management_administrative_stateVariable = OpenAPI_administrative_state_FromString(management_administrative_state->valuestring);
    }

    nsm_charging_information_local_var = OpenAPI_nsm_charging_information_create (
        management_operationVariable,
        id_network_slice_instance && !cJSON_IsNull(id_network_slice_instance) ? ogs_strdup(id_network_slice_instance->valuestring) : NULL,
        list_ofservice_profile_charging_information ? list_ofservice_profile_charging_informationList : NULL,
        management_operation_status ? management_operation_statusVariable : 0,
        management_operational_state ? management_operational_stateVariable : 0,
        management_administrative_state ? management_administrative_stateVariable : 0
    );

    return nsm_charging_information_local_var;
end:
    if (list_ofservice_profile_charging_informationList) {
        OpenAPI_list_for_each(list_ofservice_profile_charging_informationList, node) {
            OpenAPI_service_profile_charging_information_free(node->data);
        }
        OpenAPI_list_free(list_ofservice_profile_charging_informationList);
        list_ofservice_profile_charging_informationList = NULL;
    }
    return NULL;
}

OpenAPI_nsm_charging_information_t *OpenAPI_nsm_charging_information_copy(OpenAPI_nsm_charging_information_t *dst, OpenAPI_nsm_charging_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nsm_charging_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nsm_charging_information_convertToJSON() failed");
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

    OpenAPI_nsm_charging_information_free(dst);
    dst = OpenAPI_nsm_charging_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

