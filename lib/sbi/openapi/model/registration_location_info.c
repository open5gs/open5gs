
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "registration_location_info.h"

OpenAPI_registration_location_info_t *OpenAPI_registration_location_info_create(
    char *amf_instance_id,
    OpenAPI_plmn_id_t *plmn_id,
    OpenAPI_vgmlc_address_t *vgmlc_address,
    OpenAPI_list_t *access_type_list
)
{
    OpenAPI_registration_location_info_t *registration_location_info_local_var = ogs_malloc(sizeof(OpenAPI_registration_location_info_t));
    ogs_assert(registration_location_info_local_var);

    registration_location_info_local_var->amf_instance_id = amf_instance_id;
    registration_location_info_local_var->plmn_id = plmn_id;
    registration_location_info_local_var->vgmlc_address = vgmlc_address;
    registration_location_info_local_var->access_type_list = access_type_list;

    return registration_location_info_local_var;
}

void OpenAPI_registration_location_info_free(OpenAPI_registration_location_info_t *registration_location_info)
{
    if (NULL == registration_location_info) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(registration_location_info->amf_instance_id);
    OpenAPI_plmn_id_free(registration_location_info->plmn_id);
    OpenAPI_vgmlc_address_free(registration_location_info->vgmlc_address);
    OpenAPI_list_free(registration_location_info->access_type_list);
    ogs_free(registration_location_info);
}

cJSON *OpenAPI_registration_location_info_convertToJSON(OpenAPI_registration_location_info_t *registration_location_info)
{
    cJSON *item = NULL;

    if (registration_location_info == NULL) {
        ogs_error("OpenAPI_registration_location_info_convertToJSON() failed [RegistrationLocationInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddStringToObject(item, "amfInstanceId", registration_location_info->amf_instance_id) == NULL) {
        ogs_error("OpenAPI_registration_location_info_convertToJSON() failed [amf_instance_id]");
        goto end;
    }

    if (registration_location_info->plmn_id) {
    cJSON *plmn_id_local_JSON = OpenAPI_plmn_id_convertToJSON(registration_location_info->plmn_id);
    if (plmn_id_local_JSON == NULL) {
        ogs_error("OpenAPI_registration_location_info_convertToJSON() failed [plmn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "plmnId", plmn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_registration_location_info_convertToJSON() failed [plmn_id]");
        goto end;
    }
    }

    if (registration_location_info->vgmlc_address) {
    cJSON *vgmlc_address_local_JSON = OpenAPI_vgmlc_address_convertToJSON(registration_location_info->vgmlc_address);
    if (vgmlc_address_local_JSON == NULL) {
        ogs_error("OpenAPI_registration_location_info_convertToJSON() failed [vgmlc_address]");
        goto end;
    }
    cJSON_AddItemToObject(item, "vgmlcAddress", vgmlc_address_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_registration_location_info_convertToJSON() failed [vgmlc_address]");
        goto end;
    }
    }

    cJSON *access_type_list = cJSON_AddArrayToObject(item, "accessTypeList");
    if (access_type_list == NULL) {
        ogs_error("OpenAPI_registration_location_info_convertToJSON() failed [access_type_list]");
        goto end;
    }
    OpenAPI_lnode_t *access_type_list_node;
    OpenAPI_list_for_each(registration_location_info->access_type_list, access_type_list_node) {
        if (cJSON_AddStringToObject(access_type_list, "", OpenAPI_access_type_ToString((intptr_t)access_type_list_node->data)) == NULL) {
            ogs_error("OpenAPI_registration_location_info_convertToJSON() failed [access_type_list]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_registration_location_info_t *OpenAPI_registration_location_info_parseFromJSON(cJSON *registration_location_infoJSON)
{
    OpenAPI_registration_location_info_t *registration_location_info_local_var = NULL;
    cJSON *amf_instance_id = cJSON_GetObjectItemCaseSensitive(registration_location_infoJSON, "amfInstanceId");
    if (!amf_instance_id) {
        ogs_error("OpenAPI_registration_location_info_parseFromJSON() failed [amf_instance_id]");
        goto end;
    }

    if (!cJSON_IsString(amf_instance_id)) {
        ogs_error("OpenAPI_registration_location_info_parseFromJSON() failed [amf_instance_id]");
        goto end;
    }

    cJSON *plmn_id = cJSON_GetObjectItemCaseSensitive(registration_location_infoJSON, "plmnId");

    OpenAPI_plmn_id_t *plmn_id_local_nonprim = NULL;
    if (plmn_id) {
    plmn_id_local_nonprim = OpenAPI_plmn_id_parseFromJSON(plmn_id);
    }

    cJSON *vgmlc_address = cJSON_GetObjectItemCaseSensitive(registration_location_infoJSON, "vgmlcAddress");

    OpenAPI_vgmlc_address_t *vgmlc_address_local_nonprim = NULL;
    if (vgmlc_address) {
    vgmlc_address_local_nonprim = OpenAPI_vgmlc_address_parseFromJSON(vgmlc_address);
    }

    cJSON *access_type_list = cJSON_GetObjectItemCaseSensitive(registration_location_infoJSON, "accessTypeList");
    if (!access_type_list) {
        ogs_error("OpenAPI_registration_location_info_parseFromJSON() failed [access_type_list]");
        goto end;
    }

    OpenAPI_list_t *access_type_listList;
    cJSON *access_type_list_local_nonprimitive;
    if (!cJSON_IsArray(access_type_list)) {
        ogs_error("OpenAPI_registration_location_info_parseFromJSON() failed [access_type_list]");
        goto end;
    }

    access_type_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(access_type_list_local_nonprimitive, access_type_list ) {
        if (!cJSON_IsString(access_type_list_local_nonprimitive)){
            ogs_error("OpenAPI_registration_location_info_parseFromJSON() failed [access_type_list]");
            goto end;
        }

        OpenAPI_list_add(access_type_listList, (void *)OpenAPI_access_type_FromString(access_type_list_local_nonprimitive->valuestring));
    }

    registration_location_info_local_var = OpenAPI_registration_location_info_create (
        ogs_strdup(amf_instance_id->valuestring),
        plmn_id ? plmn_id_local_nonprim : NULL,
        vgmlc_address ? vgmlc_address_local_nonprim : NULL,
        access_type_listList
    );

    return registration_location_info_local_var;
end:
    return NULL;
}

OpenAPI_registration_location_info_t *OpenAPI_registration_location_info_copy(OpenAPI_registration_location_info_t *dst, OpenAPI_registration_location_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_registration_location_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_registration_location_info_convertToJSON() failed");
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

    OpenAPI_registration_location_info_free(dst);
    dst = OpenAPI_registration_location_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

