
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "authorization_data.h"

OpenAPI_authorization_data_t *OpenAPI_authorization_data_create(
    OpenAPI_list_t *authorization_data,
    char *validity_time
    )
{
    OpenAPI_authorization_data_t *authorization_data_local_var = OpenAPI_malloc(sizeof(OpenAPI_authorization_data_t));
    if (!authorization_data_local_var) {
        return NULL;
    }
    authorization_data_local_var->authorization_data = authorization_data;
    authorization_data_local_var->validity_time = validity_time;

    return authorization_data_local_var;
}

void OpenAPI_authorization_data_free(OpenAPI_authorization_data_t *authorization_data)
{
    if (NULL == authorization_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(authorization_data->authorization_data, node) {
        OpenAPI_user_identifier_free(node->data);
    }
    OpenAPI_list_free(authorization_data->authorization_data);
    ogs_free(authorization_data->validity_time);
    ogs_free(authorization_data);
}

cJSON *OpenAPI_authorization_data_convertToJSON(OpenAPI_authorization_data_t *authorization_data)
{
    cJSON *item = NULL;

    if (authorization_data == NULL) {
        ogs_error("OpenAPI_authorization_data_convertToJSON() failed [AuthorizationData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!authorization_data->authorization_data) {
        ogs_error("OpenAPI_authorization_data_convertToJSON() failed [authorization_data]");
        goto end;
    }
    cJSON *authorization_dataList = cJSON_AddArrayToObject(item, "authorizationData");
    if (authorization_dataList == NULL) {
        ogs_error("OpenAPI_authorization_data_convertToJSON() failed [authorization_data]");
        goto end;
    }

    OpenAPI_lnode_t *authorization_data_node;
    if (authorization_data->authorization_data) {
        OpenAPI_list_for_each(authorization_data->authorization_data, authorization_data_node) {
            cJSON *itemLocal = OpenAPI_user_identifier_convertToJSON(authorization_data_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_authorization_data_convertToJSON() failed [authorization_data]");
                goto end;
            }
            cJSON_AddItemToArray(authorization_dataList, itemLocal);
        }
    }

    if (authorization_data->validity_time) {
        if (cJSON_AddStringToObject(item, "validityTime", authorization_data->validity_time) == NULL) {
            ogs_error("OpenAPI_authorization_data_convertToJSON() failed [validity_time]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_authorization_data_t *OpenAPI_authorization_data_parseFromJSON(cJSON *authorization_dataJSON)
{
    OpenAPI_authorization_data_t *authorization_data_local_var = NULL;
    cJSON *authorization_data = cJSON_GetObjectItemCaseSensitive(authorization_dataJSON, "authorizationData");
    if (!authorization_data) {
        ogs_error("OpenAPI_authorization_data_parseFromJSON() failed [authorization_data]");
        goto end;
    }

    OpenAPI_list_t *authorization_dataList;

    cJSON *authorization_data_local_nonprimitive;
    if (!cJSON_IsArray(authorization_data)) {
        ogs_error("OpenAPI_authorization_data_parseFromJSON() failed [authorization_data]");
        goto end;
    }

    authorization_dataList = OpenAPI_list_create();

    cJSON_ArrayForEach(authorization_data_local_nonprimitive, authorization_data ) {
        if (!cJSON_IsObject(authorization_data_local_nonprimitive)) {
            ogs_error("OpenAPI_authorization_data_parseFromJSON() failed [authorization_data]");
            goto end;
        }
        OpenAPI_user_identifier_t *authorization_dataItem = OpenAPI_user_identifier_parseFromJSON(authorization_data_local_nonprimitive);

        OpenAPI_list_add(authorization_dataList, authorization_dataItem);
    }

    cJSON *validity_time = cJSON_GetObjectItemCaseSensitive(authorization_dataJSON, "validityTime");

    if (validity_time) {
        if (!cJSON_IsString(validity_time)) {
            ogs_error("OpenAPI_authorization_data_parseFromJSON() failed [validity_time]");
            goto end;
        }
    }

    authorization_data_local_var = OpenAPI_authorization_data_create (
        authorization_dataList,
        validity_time ? ogs_strdup(validity_time->valuestring) : NULL
        );

    return authorization_data_local_var;
end:
    return NULL;
}

OpenAPI_authorization_data_t *OpenAPI_authorization_data_copy(OpenAPI_authorization_data_t *dst, OpenAPI_authorization_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_authorization_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_authorization_data_convertToJSON() failed");
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

    OpenAPI_authorization_data_free(dst);
    dst = OpenAPI_authorization_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

