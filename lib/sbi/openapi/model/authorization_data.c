
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "authorization_data.h"

OpenAPI_authorization_data_t *OpenAPI_authorization_data_create(
    OpenAPI_set_t *authorization_data,
    OpenAPI_list_t *allowed_dnn_list,
    OpenAPI_list_t *allowed_snssai_list,
    OpenAPI_list_t *allowed_mtc_providers,
    char *validity_time
)
{
    OpenAPI_authorization_data_t *authorization_data_local_var = ogs_malloc(sizeof(OpenAPI_authorization_data_t));
    ogs_assert(authorization_data_local_var);

    authorization_data_local_var->authorization_data = authorization_data;
    authorization_data_local_var->allowed_dnn_list = allowed_dnn_list;
    authorization_data_local_var->allowed_snssai_list = allowed_snssai_list;
    authorization_data_local_var->allowed_mtc_providers = allowed_mtc_providers;
    authorization_data_local_var->validity_time = validity_time;

    return authorization_data_local_var;
}

void OpenAPI_authorization_data_free(OpenAPI_authorization_data_t *authorization_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == authorization_data) {
        return;
    }
    if (authorization_data->authorization_data) {
        OpenAPI_list_for_each(authorization_data->authorization_data, node) {
            OpenAPI_user_identifier_free(node->data);
        }
        OpenAPI_list_free(authorization_data->authorization_data);
        authorization_data->authorization_data = NULL;
    }
    if (authorization_data->allowed_dnn_list) {
        OpenAPI_list_for_each(authorization_data->allowed_dnn_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(authorization_data->allowed_dnn_list);
        authorization_data->allowed_dnn_list = NULL;
    }
    if (authorization_data->allowed_snssai_list) {
        OpenAPI_list_for_each(authorization_data->allowed_snssai_list, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(authorization_data->allowed_snssai_list);
        authorization_data->allowed_snssai_list = NULL;
    }
    if (authorization_data->allowed_mtc_providers) {
        OpenAPI_list_for_each(authorization_data->allowed_mtc_providers, node) {
            OpenAPI_mtc_provider_free(node->data);
        }
        OpenAPI_list_free(authorization_data->allowed_mtc_providers);
        authorization_data->allowed_mtc_providers = NULL;
    }
    if (authorization_data->validity_time) {
        ogs_free(authorization_data->validity_time);
        authorization_data->validity_time = NULL;
    }
    ogs_free(authorization_data);
}

cJSON *OpenAPI_authorization_data_convertToJSON(OpenAPI_authorization_data_t *authorization_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (authorization_data == NULL) {
        ogs_error("OpenAPI_authorization_data_convertToJSON() failed [AuthorizationData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!authorization_data->authorization_data) {
        ogs_error("OpenAPI_authorization_data_convertToJSON() failed [authorization_data]");
        return NULL;
    }
    cJSON *authorization_dataList = cJSON_AddArrayToObject(item, "authorizationData");
    if (authorization_dataList == NULL) {
        ogs_error("OpenAPI_authorization_data_convertToJSON() failed [authorization_data]");
        goto end;
    }
    OpenAPI_list_for_each(authorization_data->authorization_data, node) {
        cJSON *itemLocal = OpenAPI_user_identifier_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_authorization_data_convertToJSON() failed [authorization_data]");
            goto end;
        }
        cJSON_AddItemToArray(authorization_dataList, itemLocal);
    }

    if (authorization_data->allowed_dnn_list) {
    cJSON *allowed_dnn_listList = cJSON_AddArrayToObject(item, "allowedDnnList");
    if (allowed_dnn_listList == NULL) {
        ogs_error("OpenAPI_authorization_data_convertToJSON() failed [allowed_dnn_list]");
        goto end;
    }
    OpenAPI_list_for_each(authorization_data->allowed_dnn_list, node) {
        if (cJSON_AddStringToObject(allowed_dnn_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_authorization_data_convertToJSON() failed [allowed_dnn_list]");
            goto end;
        }
    }
    }

    if (authorization_data->allowed_snssai_list) {
    cJSON *allowed_snssai_listList = cJSON_AddArrayToObject(item, "allowedSnssaiList");
    if (allowed_snssai_listList == NULL) {
        ogs_error("OpenAPI_authorization_data_convertToJSON() failed [allowed_snssai_list]");
        goto end;
    }
    OpenAPI_list_for_each(authorization_data->allowed_snssai_list, node) {
        cJSON *itemLocal = OpenAPI_snssai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_authorization_data_convertToJSON() failed [allowed_snssai_list]");
            goto end;
        }
        cJSON_AddItemToArray(allowed_snssai_listList, itemLocal);
    }
    }

    if (authorization_data->allowed_mtc_providers) {
    cJSON *allowed_mtc_providersList = cJSON_AddArrayToObject(item, "allowedMtcProviders");
    if (allowed_mtc_providersList == NULL) {
        ogs_error("OpenAPI_authorization_data_convertToJSON() failed [allowed_mtc_providers]");
        goto end;
    }
    OpenAPI_list_for_each(authorization_data->allowed_mtc_providers, node) {
        cJSON *itemLocal = OpenAPI_mtc_provider_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_authorization_data_convertToJSON() failed [allowed_mtc_providers]");
            goto end;
        }
        cJSON_AddItemToArray(allowed_mtc_providersList, itemLocal);
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
    OpenAPI_lnode_t *node = NULL;
    cJSON *authorization_data = NULL;
    OpenAPI_list_t *authorization_dataList = NULL;
    cJSON *allowed_dnn_list = NULL;
    OpenAPI_list_t *allowed_dnn_listList = NULL;
    cJSON *allowed_snssai_list = NULL;
    OpenAPI_list_t *allowed_snssai_listList = NULL;
    cJSON *allowed_mtc_providers = NULL;
    OpenAPI_list_t *allowed_mtc_providersList = NULL;
    cJSON *validity_time = NULL;
    authorization_data = cJSON_GetObjectItemCaseSensitive(authorization_dataJSON, "authorizationData");
    if (!authorization_data) {
        ogs_error("OpenAPI_authorization_data_parseFromJSON() failed [authorization_data]");
        goto end;
    }
        cJSON *authorization_data_local = NULL;
        if (!cJSON_IsArray(authorization_data)) {
            ogs_error("OpenAPI_authorization_data_parseFromJSON() failed [authorization_data]");
            goto end;
        }

        authorization_dataList = OpenAPI_list_create();

        cJSON_ArrayForEach(authorization_data_local, authorization_data) {
            if (!cJSON_IsObject(authorization_data_local)) {
                ogs_error("OpenAPI_authorization_data_parseFromJSON() failed [authorization_data]");
                goto end;
            }
            OpenAPI_user_identifier_t *authorization_dataItem = OpenAPI_user_identifier_parseFromJSON(authorization_data_local);
            if (!authorization_dataItem) {
                ogs_error("No authorization_dataItem");
                goto end;
            }
            OpenAPI_list_add(authorization_dataList, authorization_dataItem);
        }

    allowed_dnn_list = cJSON_GetObjectItemCaseSensitive(authorization_dataJSON, "allowedDnnList");
    if (allowed_dnn_list) {
        cJSON *allowed_dnn_list_local = NULL;
        if (!cJSON_IsArray(allowed_dnn_list)) {
            ogs_error("OpenAPI_authorization_data_parseFromJSON() failed [allowed_dnn_list]");
            goto end;
        }

        allowed_dnn_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(allowed_dnn_list_local, allowed_dnn_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(allowed_dnn_list_local)) {
                ogs_error("OpenAPI_authorization_data_parseFromJSON() failed [allowed_dnn_list]");
                goto end;
            }
            OpenAPI_list_add(allowed_dnn_listList, ogs_strdup(allowed_dnn_list_local->valuestring));
        }
    }

    allowed_snssai_list = cJSON_GetObjectItemCaseSensitive(authorization_dataJSON, "allowedSnssaiList");
    if (allowed_snssai_list) {
        cJSON *allowed_snssai_list_local = NULL;
        if (!cJSON_IsArray(allowed_snssai_list)) {
            ogs_error("OpenAPI_authorization_data_parseFromJSON() failed [allowed_snssai_list]");
            goto end;
        }

        allowed_snssai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(allowed_snssai_list_local, allowed_snssai_list) {
            if (!cJSON_IsObject(allowed_snssai_list_local)) {
                ogs_error("OpenAPI_authorization_data_parseFromJSON() failed [allowed_snssai_list]");
                goto end;
            }
            OpenAPI_snssai_t *allowed_snssai_listItem = OpenAPI_snssai_parseFromJSON(allowed_snssai_list_local);
            if (!allowed_snssai_listItem) {
                ogs_error("No allowed_snssai_listItem");
                goto end;
            }
            OpenAPI_list_add(allowed_snssai_listList, allowed_snssai_listItem);
        }
    }

    allowed_mtc_providers = cJSON_GetObjectItemCaseSensitive(authorization_dataJSON, "allowedMtcProviders");
    if (allowed_mtc_providers) {
        cJSON *allowed_mtc_providers_local = NULL;
        if (!cJSON_IsArray(allowed_mtc_providers)) {
            ogs_error("OpenAPI_authorization_data_parseFromJSON() failed [allowed_mtc_providers]");
            goto end;
        }

        allowed_mtc_providersList = OpenAPI_list_create();

        cJSON_ArrayForEach(allowed_mtc_providers_local, allowed_mtc_providers) {
            if (!cJSON_IsObject(allowed_mtc_providers_local)) {
                ogs_error("OpenAPI_authorization_data_parseFromJSON() failed [allowed_mtc_providers]");
                goto end;
            }
            OpenAPI_mtc_provider_t *allowed_mtc_providersItem = OpenAPI_mtc_provider_parseFromJSON(allowed_mtc_providers_local);
            if (!allowed_mtc_providersItem) {
                ogs_error("No allowed_mtc_providersItem");
                goto end;
            }
            OpenAPI_list_add(allowed_mtc_providersList, allowed_mtc_providersItem);
        }
    }

    validity_time = cJSON_GetObjectItemCaseSensitive(authorization_dataJSON, "validityTime");
    if (validity_time) {
    if (!cJSON_IsString(validity_time) && !cJSON_IsNull(validity_time)) {
        ogs_error("OpenAPI_authorization_data_parseFromJSON() failed [validity_time]");
        goto end;
    }
    }

    authorization_data_local_var = OpenAPI_authorization_data_create (
        authorization_dataList,
        allowed_dnn_list ? allowed_dnn_listList : NULL,
        allowed_snssai_list ? allowed_snssai_listList : NULL,
        allowed_mtc_providers ? allowed_mtc_providersList : NULL,
        validity_time && !cJSON_IsNull(validity_time) ? ogs_strdup(validity_time->valuestring) : NULL
    );

    return authorization_data_local_var;
end:
    if (authorization_dataList) {
        OpenAPI_list_for_each(authorization_dataList, node) {
            OpenAPI_user_identifier_free(node->data);
        }
        OpenAPI_list_free(authorization_dataList);
        authorization_dataList = NULL;
    }
    if (allowed_dnn_listList) {
        OpenAPI_list_for_each(allowed_dnn_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(allowed_dnn_listList);
        allowed_dnn_listList = NULL;
    }
    if (allowed_snssai_listList) {
        OpenAPI_list_for_each(allowed_snssai_listList, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(allowed_snssai_listList);
        allowed_snssai_listList = NULL;
    }
    if (allowed_mtc_providersList) {
        OpenAPI_list_for_each(allowed_mtc_providersList, node) {
            OpenAPI_mtc_provider_free(node->data);
        }
        OpenAPI_list_free(allowed_mtc_providersList);
        allowed_mtc_providersList = NULL;
    }
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

