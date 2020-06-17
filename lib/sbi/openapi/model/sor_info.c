
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sor_info.h"

OpenAPI_sor_info_t *OpenAPI_sor_info_create(
    OpenAPI_steering_container_t *steering_container,
    int ack_ind,
    char *sor_mac_iausf,
    char *countersor,
    char *provisioning_time
    )
{
    OpenAPI_sor_info_t *sor_info_local_var = OpenAPI_malloc(sizeof(OpenAPI_sor_info_t));
    if (!sor_info_local_var) {
        return NULL;
    }
    sor_info_local_var->steering_container = steering_container;
    sor_info_local_var->ack_ind = ack_ind;
    sor_info_local_var->sor_mac_iausf = sor_mac_iausf;
    sor_info_local_var->countersor = countersor;
    sor_info_local_var->provisioning_time = provisioning_time;

    return sor_info_local_var;
}

void OpenAPI_sor_info_free(OpenAPI_sor_info_t *sor_info)
{
    if (NULL == sor_info) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_steering_container_free(sor_info->steering_container);
    ogs_free(sor_info->sor_mac_iausf);
    ogs_free(sor_info->countersor);
    ogs_free(sor_info->provisioning_time);
    ogs_free(sor_info);
}

cJSON *OpenAPI_sor_info_convertToJSON(OpenAPI_sor_info_t *sor_info)
{
    cJSON *item = NULL;

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
        goto end;
    }
    if (cJSON_AddStringToObject(item, "provisioningTime", sor_info->provisioning_time) == NULL) {
        ogs_error("OpenAPI_sor_info_convertToJSON() failed [provisioning_time]");
        goto end;
    }

end:
    return item;
}

OpenAPI_sor_info_t *OpenAPI_sor_info_parseFromJSON(cJSON *sor_infoJSON)
{
    OpenAPI_sor_info_t *sor_info_local_var = NULL;
    cJSON *steering_container = cJSON_GetObjectItemCaseSensitive(sor_infoJSON, "steeringContainer");

    OpenAPI_steering_container_t *steering_container_local_nonprim = NULL;
    if (steering_container) {
        steering_container_local_nonprim = OpenAPI_steering_container_parseFromJSON(steering_container);
    }

    cJSON *ack_ind = cJSON_GetObjectItemCaseSensitive(sor_infoJSON, "ackInd");
    if (!ack_ind) {
        ogs_error("OpenAPI_sor_info_parseFromJSON() failed [ack_ind]");
        goto end;
    }


    if (!cJSON_IsBool(ack_ind)) {
        ogs_error("OpenAPI_sor_info_parseFromJSON() failed [ack_ind]");
        goto end;
    }

    cJSON *sor_mac_iausf = cJSON_GetObjectItemCaseSensitive(sor_infoJSON, "sorMacIausf");

    if (sor_mac_iausf) {
        if (!cJSON_IsString(sor_mac_iausf)) {
            ogs_error("OpenAPI_sor_info_parseFromJSON() failed [sor_mac_iausf]");
            goto end;
        }
    }

    cJSON *countersor = cJSON_GetObjectItemCaseSensitive(sor_infoJSON, "countersor");

    if (countersor) {
        if (!cJSON_IsString(countersor)) {
            ogs_error("OpenAPI_sor_info_parseFromJSON() failed [countersor]");
            goto end;
        }
    }

    cJSON *provisioning_time = cJSON_GetObjectItemCaseSensitive(sor_infoJSON, "provisioningTime");
    if (!provisioning_time) {
        ogs_error("OpenAPI_sor_info_parseFromJSON() failed [provisioning_time]");
        goto end;
    }


    if (!cJSON_IsString(provisioning_time)) {
        ogs_error("OpenAPI_sor_info_parseFromJSON() failed [provisioning_time]");
        goto end;
    }

    sor_info_local_var = OpenAPI_sor_info_create (
        steering_container ? steering_container_local_nonprim : NULL,
        ack_ind->valueint,
        sor_mac_iausf ? ogs_strdup(sor_mac_iausf->valuestring) : NULL,
        countersor ? ogs_strdup(countersor->valuestring) : NULL,
        ogs_strdup(provisioning_time->valuestring)
        );

    return sor_info_local_var;
end:
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

