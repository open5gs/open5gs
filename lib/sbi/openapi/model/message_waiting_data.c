
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "message_waiting_data.h"

OpenAPI_message_waiting_data_t *OpenAPI_message_waiting_data_create(
    OpenAPI_list_t *mwd_list
)
{
    OpenAPI_message_waiting_data_t *message_waiting_data_local_var = ogs_malloc(sizeof(OpenAPI_message_waiting_data_t));
    ogs_assert(message_waiting_data_local_var);

    message_waiting_data_local_var->mwd_list = mwd_list;

    return message_waiting_data_local_var;
}

void OpenAPI_message_waiting_data_free(OpenAPI_message_waiting_data_t *message_waiting_data)
{
    if (NULL == message_waiting_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(message_waiting_data->mwd_list, node) {
        OpenAPI_smsc_data_free(node->data);
    }
    OpenAPI_list_free(message_waiting_data->mwd_list);
    ogs_free(message_waiting_data);
}

cJSON *OpenAPI_message_waiting_data_convertToJSON(OpenAPI_message_waiting_data_t *message_waiting_data)
{
    cJSON *item = NULL;

    if (message_waiting_data == NULL) {
        ogs_error("OpenAPI_message_waiting_data_convertToJSON() failed [MessageWaitingData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (message_waiting_data->mwd_list) {
    cJSON *mwd_listList = cJSON_AddArrayToObject(item, "mwdList");
    if (mwd_listList == NULL) {
        ogs_error("OpenAPI_message_waiting_data_convertToJSON() failed [mwd_list]");
        goto end;
    }

    OpenAPI_lnode_t *mwd_list_node;
    if (message_waiting_data->mwd_list) {
        OpenAPI_list_for_each(message_waiting_data->mwd_list, mwd_list_node) {
            cJSON *itemLocal = OpenAPI_smsc_data_convertToJSON(mwd_list_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_message_waiting_data_convertToJSON() failed [mwd_list]");
                goto end;
            }
            cJSON_AddItemToArray(mwd_listList, itemLocal);
        }
    }
    }

end:
    return item;
}

OpenAPI_message_waiting_data_t *OpenAPI_message_waiting_data_parseFromJSON(cJSON *message_waiting_dataJSON)
{
    OpenAPI_message_waiting_data_t *message_waiting_data_local_var = NULL;
    cJSON *mwd_list = cJSON_GetObjectItemCaseSensitive(message_waiting_dataJSON, "mwdList");

    OpenAPI_list_t *mwd_listList;
    if (mwd_list) {
    cJSON *mwd_list_local_nonprimitive;
    if (!cJSON_IsArray(mwd_list)){
        ogs_error("OpenAPI_message_waiting_data_parseFromJSON() failed [mwd_list]");
        goto end;
    }

    mwd_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(mwd_list_local_nonprimitive, mwd_list ) {
        if (!cJSON_IsObject(mwd_list_local_nonprimitive)) {
            ogs_error("OpenAPI_message_waiting_data_parseFromJSON() failed [mwd_list]");
            goto end;
        }
        OpenAPI_smsc_data_t *mwd_listItem = OpenAPI_smsc_data_parseFromJSON(mwd_list_local_nonprimitive);

        if (!mwd_listItem) {
            ogs_error("No mwd_listItem");
            OpenAPI_list_free(mwd_listList);
            goto end;
        }

        OpenAPI_list_add(mwd_listList, mwd_listItem);
    }
    }

    message_waiting_data_local_var = OpenAPI_message_waiting_data_create (
        mwd_list ? mwd_listList : NULL
    );

    return message_waiting_data_local_var;
end:
    return NULL;
}

OpenAPI_message_waiting_data_t *OpenAPI_message_waiting_data_copy(OpenAPI_message_waiting_data_t *dst, OpenAPI_message_waiting_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_message_waiting_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_message_waiting_data_convertToJSON() failed");
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

    OpenAPI_message_waiting_data_free(dst);
    dst = OpenAPI_message_waiting_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

