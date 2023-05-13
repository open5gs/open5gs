
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "uc_subscription_data.h"

OpenAPI_uc_subscription_data_t *OpenAPI_uc_subscription_data_create(
    OpenAPI_list_t* user_consent_per_purpose_list
)
{
    OpenAPI_uc_subscription_data_t *uc_subscription_data_local_var = ogs_malloc(sizeof(OpenAPI_uc_subscription_data_t));
    ogs_assert(uc_subscription_data_local_var);

    uc_subscription_data_local_var->user_consent_per_purpose_list = user_consent_per_purpose_list;

    return uc_subscription_data_local_var;
}

void OpenAPI_uc_subscription_data_free(OpenAPI_uc_subscription_data_t *uc_subscription_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == uc_subscription_data) {
        return;
    }
    if (uc_subscription_data->user_consent_per_purpose_list) {
        OpenAPI_list_for_each(uc_subscription_data->user_consent_per_purpose_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(uc_subscription_data->user_consent_per_purpose_list);
        uc_subscription_data->user_consent_per_purpose_list = NULL;
    }
    ogs_free(uc_subscription_data);
}

cJSON *OpenAPI_uc_subscription_data_convertToJSON(OpenAPI_uc_subscription_data_t *uc_subscription_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (uc_subscription_data == NULL) {
        ogs_error("OpenAPI_uc_subscription_data_convertToJSON() failed [UcSubscriptionData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (uc_subscription_data->user_consent_per_purpose_list != OpenAPI_user_consent_NULL) {
    cJSON *user_consent_per_purpose_list = cJSON_AddObjectToObject(item, "userConsentPerPurposeList");
    if (user_consent_per_purpose_list == NULL) {
        ogs_error("OpenAPI_uc_subscription_data_convertToJSON() failed [user_consent_per_purpose_list]");
        goto end;
    }
    cJSON *localMapObject = user_consent_per_purpose_list;
    if (uc_subscription_data->user_consent_per_purpose_list) {
        OpenAPI_list_for_each(uc_subscription_data->user_consent_per_purpose_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_uc_subscription_data_convertToJSON() failed [user_consent_per_purpose_list]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_uc_subscription_data_convertToJSON() failed [user_consent_per_purpose_list]");
                goto end;
            }
            if (cJSON_AddStringToObject(localMapObject, localKeyValue->key, OpenAPI_user_consent_ToString((intptr_t)localKeyValue->value)) == NULL) {
                ogs_error("OpenAPI_uc_subscription_data_convertToJSON() failed [user_consent_per_purpose_list]");
                goto end;
            }
        }
    }
    }

end:
    return item;
}

OpenAPI_uc_subscription_data_t *OpenAPI_uc_subscription_data_parseFromJSON(cJSON *uc_subscription_dataJSON)
{
    OpenAPI_uc_subscription_data_t *uc_subscription_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *user_consent_per_purpose_list = NULL;
    OpenAPI_list_t *user_consent_per_purpose_listList = NULL;
    user_consent_per_purpose_list = cJSON_GetObjectItemCaseSensitive(uc_subscription_dataJSON, "userConsentPerPurposeList");
    if (user_consent_per_purpose_list) {
        cJSON *user_consent_per_purpose_list_local_map = NULL;
        if (!cJSON_IsObject(user_consent_per_purpose_list) && !cJSON_IsNull(user_consent_per_purpose_list)) {
            ogs_error("OpenAPI_uc_subscription_data_parseFromJSON() failed [user_consent_per_purpose_list]");
            goto end;
        }
        if (cJSON_IsObject(user_consent_per_purpose_list)) {
            user_consent_per_purpose_listList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(user_consent_per_purpose_list_local_map, user_consent_per_purpose_list) {
                cJSON *localMapObject = user_consent_per_purpose_list_local_map;
                if (!cJSON_IsString(localMapObject)) {
                    ogs_error("OpenAPI_uc_subscription_data_parseFromJSON() failed [user_consent_per_purpose_list]");
                    goto end;
                }
                localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), (void *)OpenAPI_user_consent_FromString(localMapObject->string));
                OpenAPI_list_add(user_consent_per_purpose_listList, localMapKeyPair);
            }
        }
    }

    uc_subscription_data_local_var = OpenAPI_uc_subscription_data_create (
        user_consent_per_purpose_list ? user_consent_per_purpose_listList : NULL
    );

    return uc_subscription_data_local_var;
end:
    if (user_consent_per_purpose_listList) {
        OpenAPI_list_for_each(user_consent_per_purpose_listList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(user_consent_per_purpose_listList);
        user_consent_per_purpose_listList = NULL;
    }
    return NULL;
}

OpenAPI_uc_subscription_data_t *OpenAPI_uc_subscription_data_copy(OpenAPI_uc_subscription_data_t *dst, OpenAPI_uc_subscription_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_uc_subscription_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_uc_subscription_data_convertToJSON() failed");
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

    OpenAPI_uc_subscription_data_free(dst);
    dst = OpenAPI_uc_subscription_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

