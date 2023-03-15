
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sor_info.h"

OpenAPI_sor_info_t *OpenAPI_sor_info_create(
    OpenAPI_steering_container_t *steering_container,
    int ack_ind,
    char *sor_mac_iausf,
    char *countersor,
    char *provisioning_time,
    char *sor_transparent_container,
    char *sor_cmci,
    bool is_store_sor_cmci_in_me,
    int store_sor_cmci_in_me,
    bool is_usim_support_of_sor_cmci,
    int usim_support_of_sor_cmci
)
{
    OpenAPI_sor_info_t *sor_info_local_var = ogs_malloc(sizeof(OpenAPI_sor_info_t));
    ogs_assert(sor_info_local_var);

    sor_info_local_var->steering_container = steering_container;
    sor_info_local_var->ack_ind = ack_ind;
    sor_info_local_var->sor_mac_iausf = sor_mac_iausf;
    sor_info_local_var->countersor = countersor;
    sor_info_local_var->provisioning_time = provisioning_time;
    sor_info_local_var->sor_transparent_container = sor_transparent_container;
    sor_info_local_var->sor_cmci = sor_cmci;
    sor_info_local_var->is_store_sor_cmci_in_me = is_store_sor_cmci_in_me;
    sor_info_local_var->store_sor_cmci_in_me = store_sor_cmci_in_me;
    sor_info_local_var->is_usim_support_of_sor_cmci = is_usim_support_of_sor_cmci;
    sor_info_local_var->usim_support_of_sor_cmci = usim_support_of_sor_cmci;

    return sor_info_local_var;
}

void OpenAPI_sor_info_free(OpenAPI_sor_info_t *sor_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == sor_info) {
        return;
    }
    if (sor_info->steering_container) {
        OpenAPI_steering_container_free(sor_info->steering_container);
        sor_info->steering_container = NULL;
    }
    if (sor_info->sor_mac_iausf) {
        ogs_free(sor_info->sor_mac_iausf);
        sor_info->sor_mac_iausf = NULL;
    }
    if (sor_info->countersor) {
        ogs_free(sor_info->countersor);
        sor_info->countersor = NULL;
    }
    if (sor_info->provisioning_time) {
        ogs_free(sor_info->provisioning_time);
        sor_info->provisioning_time = NULL;
    }
    if (sor_info->sor_transparent_container) {
        ogs_free(sor_info->sor_transparent_container);
        sor_info->sor_transparent_container = NULL;
    }
    if (sor_info->sor_cmci) {
        ogs_free(sor_info->sor_cmci);
        sor_info->sor_cmci = NULL;
    }
    ogs_free(sor_info);
}

cJSON *OpenAPI_sor_info_convertToJSON(OpenAPI_sor_info_t *sor_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (sor_info == NULL) {
        ogs_error("OpenAPI_sor_info_convertToJSON() failed [SorInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (sor_info->steering_container) {
    cJSON *steering_container_local_JSON = OpenAPI_steering_container_convertToJSON(sor_info->steering_container);
    if (steering_container_local_JSON == NULL) {
        ogs_error("OpenAPI_sor_info_convertToJSON() failed [steering_container]");
        goto end;
    }
    cJSON_AddItemToObject(item, "steeringContainer", steering_container_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sor_info_convertToJSON() failed [steering_container]");
        goto end;
    }
    }

    if (cJSON_AddBoolToObject(item, "ackInd", sor_info->ack_ind) == NULL) {
        ogs_error("OpenAPI_sor_info_convertToJSON() failed [ack_ind]");
        goto end;
    }

    if (sor_info->sor_mac_iausf) {
    if (cJSON_AddStringToObject(item, "sorMacIausf", sor_info->sor_mac_iausf) == NULL) {
        ogs_error("OpenAPI_sor_info_convertToJSON() failed [sor_mac_iausf]");
        goto end;
    }
    }

    if (sor_info->countersor) {
    if (cJSON_AddStringToObject(item, "countersor", sor_info->countersor) == NULL) {
        ogs_error("OpenAPI_sor_info_convertToJSON() failed [countersor]");
        goto end;
    }
    }

    if (!sor_info->provisioning_time) {
        ogs_error("OpenAPI_sor_info_convertToJSON() failed [provisioning_time]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "provisioningTime", sor_info->provisioning_time) == NULL) {
        ogs_error("OpenAPI_sor_info_convertToJSON() failed [provisioning_time]");
        goto end;
    }

    if (sor_info->sor_transparent_container) {
    if (cJSON_AddStringToObject(item, "sorTransparentContainer", sor_info->sor_transparent_container) == NULL) {
        ogs_error("OpenAPI_sor_info_convertToJSON() failed [sor_transparent_container]");
        goto end;
    }
    }

    if (sor_info->sor_cmci) {
    if (cJSON_AddStringToObject(item, "sorCmci", sor_info->sor_cmci) == NULL) {
        ogs_error("OpenAPI_sor_info_convertToJSON() failed [sor_cmci]");
        goto end;
    }
    }

    if (sor_info->is_store_sor_cmci_in_me) {
    if (cJSON_AddBoolToObject(item, "storeSorCmciInMe", sor_info->store_sor_cmci_in_me) == NULL) {
        ogs_error("OpenAPI_sor_info_convertToJSON() failed [store_sor_cmci_in_me]");
        goto end;
    }
    }

    if (sor_info->is_usim_support_of_sor_cmci) {
    if (cJSON_AddBoolToObject(item, "usimSupportOfSorCmci", sor_info->usim_support_of_sor_cmci) == NULL) {
        ogs_error("OpenAPI_sor_info_convertToJSON() failed [usim_support_of_sor_cmci]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_sor_info_t *OpenAPI_sor_info_parseFromJSON(cJSON *sor_infoJSON)
{
    OpenAPI_sor_info_t *sor_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *steering_container = NULL;
    OpenAPI_steering_container_t *steering_container_local_nonprim = NULL;
    cJSON *ack_ind = NULL;
    cJSON *sor_mac_iausf = NULL;
    cJSON *countersor = NULL;
    cJSON *provisioning_time = NULL;
    cJSON *sor_transparent_container = NULL;
    cJSON *sor_cmci = NULL;
    cJSON *store_sor_cmci_in_me = NULL;
    cJSON *usim_support_of_sor_cmci = NULL;
    steering_container = cJSON_GetObjectItemCaseSensitive(sor_infoJSON, "steeringContainer");
    if (steering_container) {
    steering_container_local_nonprim = OpenAPI_steering_container_parseFromJSON(steering_container);
    if (!steering_container_local_nonprim) {
        ogs_error("OpenAPI_steering_container_parseFromJSON failed [steering_container]");
        goto end;
    }
    }

    ack_ind = cJSON_GetObjectItemCaseSensitive(sor_infoJSON, "ackInd");
    if (!ack_ind) {
        ogs_error("OpenAPI_sor_info_parseFromJSON() failed [ack_ind]");
        goto end;
    }
    if (!cJSON_IsBool(ack_ind)) {
        ogs_error("OpenAPI_sor_info_parseFromJSON() failed [ack_ind]");
        goto end;
    }

    sor_mac_iausf = cJSON_GetObjectItemCaseSensitive(sor_infoJSON, "sorMacIausf");
    if (sor_mac_iausf) {
    if (!cJSON_IsString(sor_mac_iausf) && !cJSON_IsNull(sor_mac_iausf)) {
        ogs_error("OpenAPI_sor_info_parseFromJSON() failed [sor_mac_iausf]");
        goto end;
    }
    }

    countersor = cJSON_GetObjectItemCaseSensitive(sor_infoJSON, "countersor");
    if (countersor) {
    if (!cJSON_IsString(countersor) && !cJSON_IsNull(countersor)) {
        ogs_error("OpenAPI_sor_info_parseFromJSON() failed [countersor]");
        goto end;
    }
    }

    provisioning_time = cJSON_GetObjectItemCaseSensitive(sor_infoJSON, "provisioningTime");
    if (!provisioning_time) {
        ogs_error("OpenAPI_sor_info_parseFromJSON() failed [provisioning_time]");
        goto end;
    }
    if (!cJSON_IsString(provisioning_time) && !cJSON_IsNull(provisioning_time)) {
        ogs_error("OpenAPI_sor_info_parseFromJSON() failed [provisioning_time]");
        goto end;
    }

    sor_transparent_container = cJSON_GetObjectItemCaseSensitive(sor_infoJSON, "sorTransparentContainer");
    if (sor_transparent_container) {
    if (!cJSON_IsString(sor_transparent_container) && !cJSON_IsNull(sor_transparent_container)) {
        ogs_error("OpenAPI_sor_info_parseFromJSON() failed [sor_transparent_container]");
        goto end;
    }
    }

    sor_cmci = cJSON_GetObjectItemCaseSensitive(sor_infoJSON, "sorCmci");
    if (sor_cmci) {
    if (!cJSON_IsString(sor_cmci) && !cJSON_IsNull(sor_cmci)) {
        ogs_error("OpenAPI_sor_info_parseFromJSON() failed [sor_cmci]");
        goto end;
    }
    }

    store_sor_cmci_in_me = cJSON_GetObjectItemCaseSensitive(sor_infoJSON, "storeSorCmciInMe");
    if (store_sor_cmci_in_me) {
    if (!cJSON_IsBool(store_sor_cmci_in_me)) {
        ogs_error("OpenAPI_sor_info_parseFromJSON() failed [store_sor_cmci_in_me]");
        goto end;
    }
    }

    usim_support_of_sor_cmci = cJSON_GetObjectItemCaseSensitive(sor_infoJSON, "usimSupportOfSorCmci");
    if (usim_support_of_sor_cmci) {
    if (!cJSON_IsBool(usim_support_of_sor_cmci)) {
        ogs_error("OpenAPI_sor_info_parseFromJSON() failed [usim_support_of_sor_cmci]");
        goto end;
    }
    }

    sor_info_local_var = OpenAPI_sor_info_create (
        steering_container ? steering_container_local_nonprim : NULL,
        
        ack_ind->valueint,
        sor_mac_iausf && !cJSON_IsNull(sor_mac_iausf) ? ogs_strdup(sor_mac_iausf->valuestring) : NULL,
        countersor && !cJSON_IsNull(countersor) ? ogs_strdup(countersor->valuestring) : NULL,
        ogs_strdup(provisioning_time->valuestring),
        sor_transparent_container && !cJSON_IsNull(sor_transparent_container) ? ogs_strdup(sor_transparent_container->valuestring) : NULL,
        sor_cmci && !cJSON_IsNull(sor_cmci) ? ogs_strdup(sor_cmci->valuestring) : NULL,
        store_sor_cmci_in_me ? true : false,
        store_sor_cmci_in_me ? store_sor_cmci_in_me->valueint : 0,
        usim_support_of_sor_cmci ? true : false,
        usim_support_of_sor_cmci ? usim_support_of_sor_cmci->valueint : 0
    );

    return sor_info_local_var;
end:
    if (steering_container_local_nonprim) {
        OpenAPI_steering_container_free(steering_container_local_nonprim);
        steering_container_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_sor_info_t *OpenAPI_sor_info_copy(OpenAPI_sor_info_t *dst, OpenAPI_sor_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sor_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sor_info_convertToJSON() failed");
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

    OpenAPI_sor_info_free(dst);
    dst = OpenAPI_sor_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

