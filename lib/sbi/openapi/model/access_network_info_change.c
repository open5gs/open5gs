
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "access_network_info_change.h"

OpenAPI_access_network_info_change_t *OpenAPI_access_network_info_change_create(
    OpenAPI_list_t *access_network_information,
    char *cellular_network_information,
    char *change_time
)
{
    OpenAPI_access_network_info_change_t *access_network_info_change_local_var = ogs_malloc(sizeof(OpenAPI_access_network_info_change_t));
    ogs_assert(access_network_info_change_local_var);

    access_network_info_change_local_var->access_network_information = access_network_information;
    access_network_info_change_local_var->cellular_network_information = cellular_network_information;
    access_network_info_change_local_var->change_time = change_time;

    return access_network_info_change_local_var;
}

void OpenAPI_access_network_info_change_free(OpenAPI_access_network_info_change_t *access_network_info_change)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == access_network_info_change) {
        return;
    }
    if (access_network_info_change->access_network_information) {
        OpenAPI_list_for_each(access_network_info_change->access_network_information, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(access_network_info_change->access_network_information);
        access_network_info_change->access_network_information = NULL;
    }
    if (access_network_info_change->cellular_network_information) {
        ogs_free(access_network_info_change->cellular_network_information);
        access_network_info_change->cellular_network_information = NULL;
    }
    if (access_network_info_change->change_time) {
        ogs_free(access_network_info_change->change_time);
        access_network_info_change->change_time = NULL;
    }
    ogs_free(access_network_info_change);
}

cJSON *OpenAPI_access_network_info_change_convertToJSON(OpenAPI_access_network_info_change_t *access_network_info_change)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (access_network_info_change == NULL) {
        ogs_error("OpenAPI_access_network_info_change_convertToJSON() failed [AccessNetworkInfoChange]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (access_network_info_change->access_network_information) {
    cJSON *access_network_informationList = cJSON_AddArrayToObject(item, "accessNetworkInformation");
    if (access_network_informationList == NULL) {
        ogs_error("OpenAPI_access_network_info_change_convertToJSON() failed [access_network_information]");
        goto end;
    }
    OpenAPI_list_for_each(access_network_info_change->access_network_information, node) {
        if (cJSON_AddStringToObject(access_network_informationList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_access_network_info_change_convertToJSON() failed [access_network_information]");
            goto end;
        }
    }
    }

    if (access_network_info_change->cellular_network_information) {
    if (cJSON_AddStringToObject(item, "cellularNetworkInformation", access_network_info_change->cellular_network_information) == NULL) {
        ogs_error("OpenAPI_access_network_info_change_convertToJSON() failed [cellular_network_information]");
        goto end;
    }
    }

    if (access_network_info_change->change_time) {
    if (cJSON_AddStringToObject(item, "changeTime", access_network_info_change->change_time) == NULL) {
        ogs_error("OpenAPI_access_network_info_change_convertToJSON() failed [change_time]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_access_network_info_change_t *OpenAPI_access_network_info_change_parseFromJSON(cJSON *access_network_info_changeJSON)
{
    OpenAPI_access_network_info_change_t *access_network_info_change_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *access_network_information = NULL;
    OpenAPI_list_t *access_network_informationList = NULL;
    cJSON *cellular_network_information = NULL;
    cJSON *change_time = NULL;
    access_network_information = cJSON_GetObjectItemCaseSensitive(access_network_info_changeJSON, "accessNetworkInformation");
    if (access_network_information) {
        cJSON *access_network_information_local = NULL;
        if (!cJSON_IsArray(access_network_information)) {
            ogs_error("OpenAPI_access_network_info_change_parseFromJSON() failed [access_network_information]");
            goto end;
        }

        access_network_informationList = OpenAPI_list_create();

        cJSON_ArrayForEach(access_network_information_local, access_network_information) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(access_network_information_local)) {
                ogs_error("OpenAPI_access_network_info_change_parseFromJSON() failed [access_network_information]");
                goto end;
            }
            OpenAPI_list_add(access_network_informationList, ogs_strdup(access_network_information_local->valuestring));
        }
    }

    cellular_network_information = cJSON_GetObjectItemCaseSensitive(access_network_info_changeJSON, "cellularNetworkInformation");
    if (cellular_network_information) {
    if (!cJSON_IsString(cellular_network_information) && !cJSON_IsNull(cellular_network_information)) {
        ogs_error("OpenAPI_access_network_info_change_parseFromJSON() failed [cellular_network_information]");
        goto end;
    }
    }

    change_time = cJSON_GetObjectItemCaseSensitive(access_network_info_changeJSON, "changeTime");
    if (change_time) {
    if (!cJSON_IsString(change_time) && !cJSON_IsNull(change_time)) {
        ogs_error("OpenAPI_access_network_info_change_parseFromJSON() failed [change_time]");
        goto end;
    }
    }

    access_network_info_change_local_var = OpenAPI_access_network_info_change_create (
        access_network_information ? access_network_informationList : NULL,
        cellular_network_information && !cJSON_IsNull(cellular_network_information) ? ogs_strdup(cellular_network_information->valuestring) : NULL,
        change_time && !cJSON_IsNull(change_time) ? ogs_strdup(change_time->valuestring) : NULL
    );

    return access_network_info_change_local_var;
end:
    if (access_network_informationList) {
        OpenAPI_list_for_each(access_network_informationList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(access_network_informationList);
        access_network_informationList = NULL;
    }
    return NULL;
}

OpenAPI_access_network_info_change_t *OpenAPI_access_network_info_change_copy(OpenAPI_access_network_info_change_t *dst, OpenAPI_access_network_info_change_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_access_network_info_change_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_access_network_info_change_convertToJSON() failed");
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

    OpenAPI_access_network_info_change_free(dst);
    dst = OpenAPI_access_network_info_change_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

