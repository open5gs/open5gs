
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "steering_info_1.h"

OpenAPI_steering_info_1_t *OpenAPI_steering_info_1_create(
    OpenAPI_plmn_id_t *plmn_id,
    OpenAPI_list_t *access_tech_list
)
{
    OpenAPI_steering_info_1_t *steering_info_1_local_var = ogs_malloc(sizeof(OpenAPI_steering_info_1_t));
    ogs_assert(steering_info_1_local_var);

    steering_info_1_local_var->plmn_id = plmn_id;
    steering_info_1_local_var->access_tech_list = access_tech_list;

    return steering_info_1_local_var;
}

void OpenAPI_steering_info_1_free(OpenAPI_steering_info_1_t *steering_info_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == steering_info_1) {
        return;
    }
    if (steering_info_1->plmn_id) {
        OpenAPI_plmn_id_free(steering_info_1->plmn_id);
        steering_info_1->plmn_id = NULL;
    }
    if (steering_info_1->access_tech_list) {
        OpenAPI_list_free(steering_info_1->access_tech_list);
        steering_info_1->access_tech_list = NULL;
    }
    ogs_free(steering_info_1);
}

cJSON *OpenAPI_steering_info_1_convertToJSON(OpenAPI_steering_info_1_t *steering_info_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (steering_info_1 == NULL) {
        ogs_error("OpenAPI_steering_info_1_convertToJSON() failed [SteeringInfo_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!steering_info_1->plmn_id) {
        ogs_error("OpenAPI_steering_info_1_convertToJSON() failed [plmn_id]");
        return NULL;
    }
    cJSON *plmn_id_local_JSON = OpenAPI_plmn_id_convertToJSON(steering_info_1->plmn_id);
    if (plmn_id_local_JSON == NULL) {
        ogs_error("OpenAPI_steering_info_1_convertToJSON() failed [plmn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "plmnId", plmn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_steering_info_1_convertToJSON() failed [plmn_id]");
        goto end;
    }

    if (steering_info_1->access_tech_list != OpenAPI_access_tech_NULL) {
    cJSON *access_tech_listList = cJSON_AddArrayToObject(item, "accessTechList");
    if (access_tech_listList == NULL) {
        ogs_error("OpenAPI_steering_info_1_convertToJSON() failed [access_tech_list]");
        goto end;
    }
    OpenAPI_list_for_each(steering_info_1->access_tech_list, node) {
        if (cJSON_AddStringToObject(access_tech_listList, "", OpenAPI_access_tech_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_steering_info_1_convertToJSON() failed [access_tech_list]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_steering_info_1_t *OpenAPI_steering_info_1_parseFromJSON(cJSON *steering_info_1JSON)
{
    OpenAPI_steering_info_1_t *steering_info_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *plmn_id = NULL;
    OpenAPI_plmn_id_t *plmn_id_local_nonprim = NULL;
    cJSON *access_tech_list = NULL;
    OpenAPI_list_t *access_tech_listList = NULL;
    plmn_id = cJSON_GetObjectItemCaseSensitive(steering_info_1JSON, "plmnId");
    if (!plmn_id) {
        ogs_error("OpenAPI_steering_info_1_parseFromJSON() failed [plmn_id]");
        goto end;
    }
    plmn_id_local_nonprim = OpenAPI_plmn_id_parseFromJSON(plmn_id);
    if (!plmn_id_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_parseFromJSON failed [plmn_id]");
        goto end;
    }

    access_tech_list = cJSON_GetObjectItemCaseSensitive(steering_info_1JSON, "accessTechList");
    if (access_tech_list) {
        cJSON *access_tech_list_local = NULL;
        if (!cJSON_IsArray(access_tech_list)) {
            ogs_error("OpenAPI_steering_info_1_parseFromJSON() failed [access_tech_list]");
            goto end;
        }

        access_tech_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(access_tech_list_local, access_tech_list) {
            OpenAPI_access_tech_e localEnum = OpenAPI_access_tech_NULL;
            if (!cJSON_IsString(access_tech_list_local)) {
                ogs_error("OpenAPI_steering_info_1_parseFromJSON() failed [access_tech_list]");
                goto end;
            }
            localEnum = OpenAPI_access_tech_FromString(access_tech_list_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"access_tech_list\" is not supported. Ignoring it ...",
                         access_tech_list_local->valuestring);
            } else {
                OpenAPI_list_add(access_tech_listList, (void *)localEnum);
            }
        }
        if (access_tech_listList->count == 0) {
            ogs_error("OpenAPI_steering_info_1_parseFromJSON() failed: Expected access_tech_listList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    steering_info_1_local_var = OpenAPI_steering_info_1_create (
        plmn_id_local_nonprim,
        access_tech_list ? access_tech_listList : NULL
    );

    return steering_info_1_local_var;
end:
    if (plmn_id_local_nonprim) {
        OpenAPI_plmn_id_free(plmn_id_local_nonprim);
        plmn_id_local_nonprim = NULL;
    }
    if (access_tech_listList) {
        OpenAPI_list_free(access_tech_listList);
        access_tech_listList = NULL;
    }
    return NULL;
}

OpenAPI_steering_info_1_t *OpenAPI_steering_info_1_copy(OpenAPI_steering_info_1_t *dst, OpenAPI_steering_info_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_steering_info_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_steering_info_1_convertToJSON() failed");
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

    OpenAPI_steering_info_1_free(dst);
    dst = OpenAPI_steering_info_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

