
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "access_transfer_information.h"

OpenAPI_access_transfer_information_t *OpenAPI_access_transfer_information_create(
    OpenAPI_access_transfer_type_e access_transfer_type,
    OpenAPI_list_t *access_network_information,
    char *cellular_network_information,
    OpenAPI_ue_transfer_type_e inter_ue_transfer,
    char *user_equipment_info,
    char *instance_id,
    char *related_ims_charging_identifier,
    bool is_related_ims_charging_identifier_node_null,
    OpenAPI_ims_address_t *related_ims_charging_identifier_node,
    char *change_time
)
{
    OpenAPI_access_transfer_information_t *access_transfer_information_local_var = ogs_malloc(sizeof(OpenAPI_access_transfer_information_t));
    ogs_assert(access_transfer_information_local_var);

    access_transfer_information_local_var->access_transfer_type = access_transfer_type;
    access_transfer_information_local_var->access_network_information = access_network_information;
    access_transfer_information_local_var->cellular_network_information = cellular_network_information;
    access_transfer_information_local_var->inter_ue_transfer = inter_ue_transfer;
    access_transfer_information_local_var->user_equipment_info = user_equipment_info;
    access_transfer_information_local_var->instance_id = instance_id;
    access_transfer_information_local_var->related_ims_charging_identifier = related_ims_charging_identifier;
    access_transfer_information_local_var->is_related_ims_charging_identifier_node_null = is_related_ims_charging_identifier_node_null;
    access_transfer_information_local_var->related_ims_charging_identifier_node = related_ims_charging_identifier_node;
    access_transfer_information_local_var->change_time = change_time;

    return access_transfer_information_local_var;
}

void OpenAPI_access_transfer_information_free(OpenAPI_access_transfer_information_t *access_transfer_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == access_transfer_information) {
        return;
    }
    if (access_transfer_information->access_network_information) {
        OpenAPI_list_for_each(access_transfer_information->access_network_information, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(access_transfer_information->access_network_information);
        access_transfer_information->access_network_information = NULL;
    }
    if (access_transfer_information->cellular_network_information) {
        ogs_free(access_transfer_information->cellular_network_information);
        access_transfer_information->cellular_network_information = NULL;
    }
    if (access_transfer_information->user_equipment_info) {
        ogs_free(access_transfer_information->user_equipment_info);
        access_transfer_information->user_equipment_info = NULL;
    }
    if (access_transfer_information->instance_id) {
        ogs_free(access_transfer_information->instance_id);
        access_transfer_information->instance_id = NULL;
    }
    if (access_transfer_information->related_ims_charging_identifier) {
        ogs_free(access_transfer_information->related_ims_charging_identifier);
        access_transfer_information->related_ims_charging_identifier = NULL;
    }
    if (access_transfer_information->related_ims_charging_identifier_node) {
        OpenAPI_ims_address_free(access_transfer_information->related_ims_charging_identifier_node);
        access_transfer_information->related_ims_charging_identifier_node = NULL;
    }
    if (access_transfer_information->change_time) {
        ogs_free(access_transfer_information->change_time);
        access_transfer_information->change_time = NULL;
    }
    ogs_free(access_transfer_information);
}

cJSON *OpenAPI_access_transfer_information_convertToJSON(OpenAPI_access_transfer_information_t *access_transfer_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (access_transfer_information == NULL) {
        ogs_error("OpenAPI_access_transfer_information_convertToJSON() failed [AccessTransferInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (access_transfer_information->access_transfer_type != OpenAPI_access_transfer_type_NULL) {
    if (cJSON_AddStringToObject(item, "accessTransferType", OpenAPI_access_transfer_type_ToString(access_transfer_information->access_transfer_type)) == NULL) {
        ogs_error("OpenAPI_access_transfer_information_convertToJSON() failed [access_transfer_type]");
        goto end;
    }
    }

    if (access_transfer_information->access_network_information) {
    cJSON *access_network_informationList = cJSON_AddArrayToObject(item, "accessNetworkInformation");
    if (access_network_informationList == NULL) {
        ogs_error("OpenAPI_access_transfer_information_convertToJSON() failed [access_network_information]");
        goto end;
    }
    OpenAPI_list_for_each(access_transfer_information->access_network_information, node) {
        if (cJSON_AddStringToObject(access_network_informationList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_access_transfer_information_convertToJSON() failed [access_network_information]");
            goto end;
        }
    }
    }

    if (access_transfer_information->cellular_network_information) {
    if (cJSON_AddStringToObject(item, "cellularNetworkInformation", access_transfer_information->cellular_network_information) == NULL) {
        ogs_error("OpenAPI_access_transfer_information_convertToJSON() failed [cellular_network_information]");
        goto end;
    }
    }

    if (access_transfer_information->inter_ue_transfer != OpenAPI_ue_transfer_type_NULL) {
    if (cJSON_AddStringToObject(item, "interUETransfer", OpenAPI_ue_transfer_type_ToString(access_transfer_information->inter_ue_transfer)) == NULL) {
        ogs_error("OpenAPI_access_transfer_information_convertToJSON() failed [inter_ue_transfer]");
        goto end;
    }
    }

    if (access_transfer_information->user_equipment_info) {
    if (cJSON_AddStringToObject(item, "userEquipmentInfo", access_transfer_information->user_equipment_info) == NULL) {
        ogs_error("OpenAPI_access_transfer_information_convertToJSON() failed [user_equipment_info]");
        goto end;
    }
    }

    if (access_transfer_information->instance_id) {
    if (cJSON_AddStringToObject(item, "instanceId", access_transfer_information->instance_id) == NULL) {
        ogs_error("OpenAPI_access_transfer_information_convertToJSON() failed [instance_id]");
        goto end;
    }
    }

    if (access_transfer_information->related_ims_charging_identifier) {
    if (cJSON_AddStringToObject(item, "relatedIMSChargingIdentifier", access_transfer_information->related_ims_charging_identifier) == NULL) {
        ogs_error("OpenAPI_access_transfer_information_convertToJSON() failed [related_ims_charging_identifier]");
        goto end;
    }
    }

    if (access_transfer_information->related_ims_charging_identifier_node) {
    cJSON *related_ims_charging_identifier_node_local_JSON = OpenAPI_ims_address_convertToJSON(access_transfer_information->related_ims_charging_identifier_node);
    if (related_ims_charging_identifier_node_local_JSON == NULL) {
        ogs_error("OpenAPI_access_transfer_information_convertToJSON() failed [related_ims_charging_identifier_node]");
        goto end;
    }
    cJSON_AddItemToObject(item, "relatedIMSChargingIdentifierNode", related_ims_charging_identifier_node_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_access_transfer_information_convertToJSON() failed [related_ims_charging_identifier_node]");
        goto end;
    }
    } else if (access_transfer_information->is_related_ims_charging_identifier_node_null) {
        if (cJSON_AddNullToObject(item, "relatedIMSChargingIdentifierNode") == NULL) {
            ogs_error("OpenAPI_access_transfer_information_convertToJSON() failed [related_ims_charging_identifier_node]");
            goto end;
        }
    }

    if (access_transfer_information->change_time) {
    if (cJSON_AddStringToObject(item, "changeTime", access_transfer_information->change_time) == NULL) {
        ogs_error("OpenAPI_access_transfer_information_convertToJSON() failed [change_time]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_access_transfer_information_t *OpenAPI_access_transfer_information_parseFromJSON(cJSON *access_transfer_informationJSON)
{
    OpenAPI_access_transfer_information_t *access_transfer_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *access_transfer_type = NULL;
    OpenAPI_access_transfer_type_e access_transfer_typeVariable = 0;
    cJSON *access_network_information = NULL;
    OpenAPI_list_t *access_network_informationList = NULL;
    cJSON *cellular_network_information = NULL;
    cJSON *inter_ue_transfer = NULL;
    OpenAPI_ue_transfer_type_e inter_ue_transferVariable = 0;
    cJSON *user_equipment_info = NULL;
    cJSON *instance_id = NULL;
    cJSON *related_ims_charging_identifier = NULL;
    cJSON *related_ims_charging_identifier_node = NULL;
    OpenAPI_ims_address_t *related_ims_charging_identifier_node_local_nonprim = NULL;
    cJSON *change_time = NULL;
    access_transfer_type = cJSON_GetObjectItemCaseSensitive(access_transfer_informationJSON, "accessTransferType");
    if (access_transfer_type) {
    if (!cJSON_IsString(access_transfer_type)) {
        ogs_error("OpenAPI_access_transfer_information_parseFromJSON() failed [access_transfer_type]");
        goto end;
    }
    access_transfer_typeVariable = OpenAPI_access_transfer_type_FromString(access_transfer_type->valuestring);
    }

    access_network_information = cJSON_GetObjectItemCaseSensitive(access_transfer_informationJSON, "accessNetworkInformation");
    if (access_network_information) {
        cJSON *access_network_information_local = NULL;
        if (!cJSON_IsArray(access_network_information)) {
            ogs_error("OpenAPI_access_transfer_information_parseFromJSON() failed [access_network_information]");
            goto end;
        }

        access_network_informationList = OpenAPI_list_create();

        cJSON_ArrayForEach(access_network_information_local, access_network_information) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(access_network_information_local)) {
                ogs_error("OpenAPI_access_transfer_information_parseFromJSON() failed [access_network_information]");
                goto end;
            }
            OpenAPI_list_add(access_network_informationList, ogs_strdup(access_network_information_local->valuestring));
        }
    }

    cellular_network_information = cJSON_GetObjectItemCaseSensitive(access_transfer_informationJSON, "cellularNetworkInformation");
    if (cellular_network_information) {
    if (!cJSON_IsString(cellular_network_information) && !cJSON_IsNull(cellular_network_information)) {
        ogs_error("OpenAPI_access_transfer_information_parseFromJSON() failed [cellular_network_information]");
        goto end;
    }
    }

    inter_ue_transfer = cJSON_GetObjectItemCaseSensitive(access_transfer_informationJSON, "interUETransfer");
    if (inter_ue_transfer) {
    if (!cJSON_IsString(inter_ue_transfer)) {
        ogs_error("OpenAPI_access_transfer_information_parseFromJSON() failed [inter_ue_transfer]");
        goto end;
    }
    inter_ue_transferVariable = OpenAPI_ue_transfer_type_FromString(inter_ue_transfer->valuestring);
    }

    user_equipment_info = cJSON_GetObjectItemCaseSensitive(access_transfer_informationJSON, "userEquipmentInfo");
    if (user_equipment_info) {
    if (!cJSON_IsString(user_equipment_info) && !cJSON_IsNull(user_equipment_info)) {
        ogs_error("OpenAPI_access_transfer_information_parseFromJSON() failed [user_equipment_info]");
        goto end;
    }
    }

    instance_id = cJSON_GetObjectItemCaseSensitive(access_transfer_informationJSON, "instanceId");
    if (instance_id) {
    if (!cJSON_IsString(instance_id) && !cJSON_IsNull(instance_id)) {
        ogs_error("OpenAPI_access_transfer_information_parseFromJSON() failed [instance_id]");
        goto end;
    }
    }

    related_ims_charging_identifier = cJSON_GetObjectItemCaseSensitive(access_transfer_informationJSON, "relatedIMSChargingIdentifier");
    if (related_ims_charging_identifier) {
    if (!cJSON_IsString(related_ims_charging_identifier) && !cJSON_IsNull(related_ims_charging_identifier)) {
        ogs_error("OpenAPI_access_transfer_information_parseFromJSON() failed [related_ims_charging_identifier]");
        goto end;
    }
    }

    related_ims_charging_identifier_node = cJSON_GetObjectItemCaseSensitive(access_transfer_informationJSON, "relatedIMSChargingIdentifierNode");
    if (related_ims_charging_identifier_node) {
    if (!cJSON_IsNull(related_ims_charging_identifier_node)) {
    related_ims_charging_identifier_node_local_nonprim = OpenAPI_ims_address_parseFromJSON(related_ims_charging_identifier_node);
    if (!related_ims_charging_identifier_node_local_nonprim) {
        ogs_error("OpenAPI_ims_address_parseFromJSON failed [related_ims_charging_identifier_node]");
        goto end;
    }
    }
    }

    change_time = cJSON_GetObjectItemCaseSensitive(access_transfer_informationJSON, "changeTime");
    if (change_time) {
    if (!cJSON_IsString(change_time) && !cJSON_IsNull(change_time)) {
        ogs_error("OpenAPI_access_transfer_information_parseFromJSON() failed [change_time]");
        goto end;
    }
    }

    access_transfer_information_local_var = OpenAPI_access_transfer_information_create (
        access_transfer_type ? access_transfer_typeVariable : 0,
        access_network_information ? access_network_informationList : NULL,
        cellular_network_information && !cJSON_IsNull(cellular_network_information) ? ogs_strdup(cellular_network_information->valuestring) : NULL,
        inter_ue_transfer ? inter_ue_transferVariable : 0,
        user_equipment_info && !cJSON_IsNull(user_equipment_info) ? ogs_strdup(user_equipment_info->valuestring) : NULL,
        instance_id && !cJSON_IsNull(instance_id) ? ogs_strdup(instance_id->valuestring) : NULL,
        related_ims_charging_identifier && !cJSON_IsNull(related_ims_charging_identifier) ? ogs_strdup(related_ims_charging_identifier->valuestring) : NULL,
        related_ims_charging_identifier_node && cJSON_IsNull(related_ims_charging_identifier_node) ? true : false,
        related_ims_charging_identifier_node ? related_ims_charging_identifier_node_local_nonprim : NULL,
        change_time && !cJSON_IsNull(change_time) ? ogs_strdup(change_time->valuestring) : NULL
    );

    return access_transfer_information_local_var;
end:
    if (access_network_informationList) {
        OpenAPI_list_for_each(access_network_informationList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(access_network_informationList);
        access_network_informationList = NULL;
    }
    if (related_ims_charging_identifier_node_local_nonprim) {
        OpenAPI_ims_address_free(related_ims_charging_identifier_node_local_nonprim);
        related_ims_charging_identifier_node_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_access_transfer_information_t *OpenAPI_access_transfer_information_copy(OpenAPI_access_transfer_information_t *dst, OpenAPI_access_transfer_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_access_transfer_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_access_transfer_information_convertToJSON() failed");
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

    OpenAPI_access_transfer_information_free(dst);
    dst = OpenAPI_access_transfer_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

