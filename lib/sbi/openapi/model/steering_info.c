
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "steering_info.h"

OpenAPI_steering_info_t *OpenAPI_steering_info_create(
    OpenAPI_plmn_id_t *plmn_id,
    OpenAPI_list_t *access_tech_list
)
{
    OpenAPI_steering_info_t *steering_info_local_var = ogs_malloc(sizeof(OpenAPI_steering_info_t));
    ogs_assert(steering_info_local_var);

    steering_info_local_var->plmn_id = plmn_id;
    steering_info_local_var->access_tech_list = access_tech_list;

    return steering_info_local_var;
}

void OpenAPI_steering_info_free(OpenAPI_steering_info_t *steering_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == steering_info) {
        return;
    }
    if (steering_info->plmn_id) {
        OpenAPI_plmn_id_free(steering_info->plmn_id);
        steering_info->plmn_id = NULL;
    }
    if (steering_info->access_tech_list) {
        OpenAPI_list_for_each(steering_info->access_tech_list, node) {
            OpenAPI_access_tech_free(node->data);
        }
        OpenAPI_list_free(steering_info->access_tech_list);
        steering_info->access_tech_list = NULL;
    }
    ogs_free(steering_info);
}

cJSON *OpenAPI_steering_info_convertToJSON(OpenAPI_steering_info_t *steering_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (steering_info == NULL) {
        ogs_error("OpenAPI_steering_info_convertToJSON() failed [SteeringInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!steering_info->plmn_id) {
        ogs_error("OpenAPI_steering_info_convertToJSON() failed [plmn_id]");
        return NULL;
    }
    cJSON *plmn_id_local_JSON = OpenAPI_plmn_id_convertToJSON(steering_info->plmn_id);
    if (plmn_id_local_JSON == NULL) {
        ogs_error("OpenAPI_steering_info_convertToJSON() failed [plmn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "plmnId", plmn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_steering_info_convertToJSON() failed [plmn_id]");
        goto end;
    }

    if (steering_info->access_tech_list) {
    cJSON *access_tech_listList = cJSON_AddArrayToObject(item, "accessTechList");
    if (access_tech_listList == NULL) {
        ogs_error("OpenAPI_steering_info_convertToJSON() failed [access_tech_list]");
        goto end;
    }
    OpenAPI_list_for_each(steering_info->access_tech_list, node) {
        cJSON *itemLocal = OpenAPI_access_tech_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_steering_info_convertToJSON() failed [access_tech_list]");
            goto end;
        }
        cJSON_AddItemToArray(access_tech_listList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_steering_info_t *OpenAPI_steering_info_parseFromJSON(cJSON *steering_infoJSON)
{
    OpenAPI_steering_info_t *steering_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *plmn_id = NULL;
    OpenAPI_plmn_id_t *plmn_id_local_nonprim = NULL;
    cJSON *access_tech_list = NULL;
    OpenAPI_list_t *access_tech_listList = NULL;
    plmn_id = cJSON_GetObjectItemCaseSensitive(steering_infoJSON, "plmnId");
    if (!plmn_id) {
        ogs_error("OpenAPI_steering_info_parseFromJSON() failed [plmn_id]");
        goto end;
    }
    plmn_id_local_nonprim = OpenAPI_plmn_id_parseFromJSON(plmn_id);
    if (!plmn_id_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_parseFromJSON failed [plmn_id]");
        goto end;
    }

    access_tech_list = cJSON_GetObjectItemCaseSensitive(steering_infoJSON, "accessTechList");
    if (access_tech_list) {
        cJSON *access_tech_list_local = NULL;
        if (!cJSON_IsArray(access_tech_list)) {
            ogs_error("OpenAPI_steering_info_parseFromJSON() failed [access_tech_list]");
            goto end;
        }

        access_tech_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(access_tech_list_local, access_tech_list) {
            if (!cJSON_IsObject(access_tech_list_local)) {
                ogs_error("OpenAPI_steering_info_parseFromJSON() failed [access_tech_list]");
                goto end;
            }
            OpenAPI_access_tech_t *access_tech_listItem = OpenAPI_access_tech_parseFromJSON(access_tech_list_local);
            if (!access_tech_listItem) {
                ogs_error("No access_tech_listItem");
                goto end;
            }
            OpenAPI_list_add(access_tech_listList, access_tech_listItem);
        }
    }

    steering_info_local_var = OpenAPI_steering_info_create (
        plmn_id_local_nonprim,
        access_tech_list ? access_tech_listList : NULL
    );

    return steering_info_local_var;
end:
    if (plmn_id_local_nonprim) {
        OpenAPI_plmn_id_free(plmn_id_local_nonprim);
        plmn_id_local_nonprim = NULL;
    }
    if (access_tech_listList) {
        OpenAPI_list_for_each(access_tech_listList, node) {
            OpenAPI_access_tech_free(node->data);
        }
        OpenAPI_list_free(access_tech_listList);
        access_tech_listList = NULL;
    }
    return NULL;
}

OpenAPI_steering_info_t *OpenAPI_steering_info_copy(OpenAPI_steering_info_t *dst, OpenAPI_steering_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_steering_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_steering_info_convertToJSON() failed");
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

    OpenAPI_steering_info_free(dst);
    dst = OpenAPI_steering_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

