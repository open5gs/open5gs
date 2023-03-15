
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mbs_subscription_data.h"

OpenAPI_mbs_subscription_data_t *OpenAPI_mbs_subscription_data_create(
    bool is_mbs_allowed,
    int mbs_allowed,
    OpenAPI_list_t *mbs_session_id_list
)
{
    OpenAPI_mbs_subscription_data_t *mbs_subscription_data_local_var = ogs_malloc(sizeof(OpenAPI_mbs_subscription_data_t));
    ogs_assert(mbs_subscription_data_local_var);

    mbs_subscription_data_local_var->is_mbs_allowed = is_mbs_allowed;
    mbs_subscription_data_local_var->mbs_allowed = mbs_allowed;
    mbs_subscription_data_local_var->mbs_session_id_list = mbs_session_id_list;

    return mbs_subscription_data_local_var;
}

void OpenAPI_mbs_subscription_data_free(OpenAPI_mbs_subscription_data_t *mbs_subscription_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == mbs_subscription_data) {
        return;
    }
    if (mbs_subscription_data->mbs_session_id_list) {
        OpenAPI_list_for_each(mbs_subscription_data->mbs_session_id_list, node) {
            OpenAPI_mbs_session_id_free(node->data);
        }
        OpenAPI_list_free(mbs_subscription_data->mbs_session_id_list);
        mbs_subscription_data->mbs_session_id_list = NULL;
    }
    ogs_free(mbs_subscription_data);
}

cJSON *OpenAPI_mbs_subscription_data_convertToJSON(OpenAPI_mbs_subscription_data_t *mbs_subscription_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (mbs_subscription_data == NULL) {
        ogs_error("OpenAPI_mbs_subscription_data_convertToJSON() failed [MbsSubscriptionData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (mbs_subscription_data->is_mbs_allowed) {
    if (cJSON_AddBoolToObject(item, "mbsAllowed", mbs_subscription_data->mbs_allowed) == NULL) {
        ogs_error("OpenAPI_mbs_subscription_data_convertToJSON() failed [mbs_allowed]");
        goto end;
    }
    }

    if (mbs_subscription_data->mbs_session_id_list) {
    cJSON *mbs_session_id_listList = cJSON_AddArrayToObject(item, "mbsSessionIdList");
    if (mbs_session_id_listList == NULL) {
        ogs_error("OpenAPI_mbs_subscription_data_convertToJSON() failed [mbs_session_id_list]");
        goto end;
    }
    OpenAPI_list_for_each(mbs_subscription_data->mbs_session_id_list, node) {
        cJSON *itemLocal = OpenAPI_mbs_session_id_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_mbs_subscription_data_convertToJSON() failed [mbs_session_id_list]");
            goto end;
        }
        cJSON_AddItemToArray(mbs_session_id_listList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_mbs_subscription_data_t *OpenAPI_mbs_subscription_data_parseFromJSON(cJSON *mbs_subscription_dataJSON)
{
    OpenAPI_mbs_subscription_data_t *mbs_subscription_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *mbs_allowed = NULL;
    cJSON *mbs_session_id_list = NULL;
    OpenAPI_list_t *mbs_session_id_listList = NULL;
    mbs_allowed = cJSON_GetObjectItemCaseSensitive(mbs_subscription_dataJSON, "mbsAllowed");
    if (mbs_allowed) {
    if (!cJSON_IsBool(mbs_allowed)) {
        ogs_error("OpenAPI_mbs_subscription_data_parseFromJSON() failed [mbs_allowed]");
        goto end;
    }
    }

    mbs_session_id_list = cJSON_GetObjectItemCaseSensitive(mbs_subscription_dataJSON, "mbsSessionIdList");
    if (mbs_session_id_list) {
        cJSON *mbs_session_id_list_local = NULL;
        if (!cJSON_IsArray(mbs_session_id_list)) {
            ogs_error("OpenAPI_mbs_subscription_data_parseFromJSON() failed [mbs_session_id_list]");
            goto end;
        }

        mbs_session_id_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(mbs_session_id_list_local, mbs_session_id_list) {
            if (!cJSON_IsObject(mbs_session_id_list_local)) {
                ogs_error("OpenAPI_mbs_subscription_data_parseFromJSON() failed [mbs_session_id_list]");
                goto end;
            }
            OpenAPI_mbs_session_id_t *mbs_session_id_listItem = OpenAPI_mbs_session_id_parseFromJSON(mbs_session_id_list_local);
            if (!mbs_session_id_listItem) {
                ogs_error("No mbs_session_id_listItem");
                goto end;
            }
            OpenAPI_list_add(mbs_session_id_listList, mbs_session_id_listItem);
        }
    }

    mbs_subscription_data_local_var = OpenAPI_mbs_subscription_data_create (
        mbs_allowed ? true : false,
        mbs_allowed ? mbs_allowed->valueint : 0,
        mbs_session_id_list ? mbs_session_id_listList : NULL
    );

    return mbs_subscription_data_local_var;
end:
    if (mbs_session_id_listList) {
        OpenAPI_list_for_each(mbs_session_id_listList, node) {
            OpenAPI_mbs_session_id_free(node->data);
        }
        OpenAPI_list_free(mbs_session_id_listList);
        mbs_session_id_listList = NULL;
    }
    return NULL;
}

OpenAPI_mbs_subscription_data_t *OpenAPI_mbs_subscription_data_copy(OpenAPI_mbs_subscription_data_t *dst, OpenAPI_mbs_subscription_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_mbs_subscription_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_mbs_subscription_data_convertToJSON() failed");
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

    OpenAPI_mbs_subscription_data_free(dst);
    dst = OpenAPI_mbs_subscription_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

