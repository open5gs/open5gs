
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sm_subs_data.h"

OpenAPI_sm_subs_data_t *OpenAPI_sm_subs_data_create(
    OpenAPI_list_t *session_management_subscription_data_list,
    OpenAPI_extended_sm_subs_data_t *extended_sm_subs_data
)
{
    OpenAPI_sm_subs_data_t *sm_subs_data_local_var = ogs_malloc(sizeof(OpenAPI_sm_subs_data_t));
    ogs_assert(sm_subs_data_local_var);

    sm_subs_data_local_var->session_management_subscription_data_list = session_management_subscription_data_list;
    sm_subs_data_local_var->extended_sm_subs_data = extended_sm_subs_data;

    return sm_subs_data_local_var;
}

void OpenAPI_sm_subs_data_free(OpenAPI_sm_subs_data_t *sm_subs_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == sm_subs_data) {
        return;
    }
    if (sm_subs_data->session_management_subscription_data_list) {
        OpenAPI_list_for_each(sm_subs_data->session_management_subscription_data_list, node) {
            OpenAPI_session_management_subscription_data_free(node->data);
        }
        OpenAPI_list_free(sm_subs_data->session_management_subscription_data_list);
        sm_subs_data->session_management_subscription_data_list = NULL;
    }
    if (sm_subs_data->extended_sm_subs_data) {
        OpenAPI_extended_sm_subs_data_free(sm_subs_data->extended_sm_subs_data);
        sm_subs_data->extended_sm_subs_data = NULL;
    }
    ogs_free(sm_subs_data);
}

cJSON *OpenAPI_sm_subs_data_convertToJSON(OpenAPI_sm_subs_data_t *sm_subs_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (sm_subs_data == NULL) {
        ogs_error("OpenAPI_sm_subs_data_convertToJSON() failed [SmSubsData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (sm_subs_data->session_management_subscription_data_list) {
    cJSON *session_management_subscription_data_listList = cJSON_AddArrayToObject(item, "SessionManagementSubscriptionDataList");
    if (session_management_subscription_data_listList == NULL) {
        ogs_error("OpenAPI_sm_subs_data_convertToJSON() failed [session_management_subscription_data_list]");
        goto end;
    }
    OpenAPI_list_for_each(sm_subs_data->session_management_subscription_data_list, node) {
        cJSON *itemLocal = OpenAPI_session_management_subscription_data_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_sm_subs_data_convertToJSON() failed [session_management_subscription_data_list]");
            goto end;
        }
        cJSON_AddItemToArray(session_management_subscription_data_listList, itemLocal);
    }
    }

    if (sm_subs_data->extended_sm_subs_data) {
    cJSON *extended_sm_subs_data_local_JSON = OpenAPI_extended_sm_subs_data_convertToJSON(sm_subs_data->extended_sm_subs_data);
    if (extended_sm_subs_data_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_subs_data_convertToJSON() failed [extended_sm_subs_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ExtendedSmSubsData", extended_sm_subs_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_subs_data_convertToJSON() failed [extended_sm_subs_data]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_sm_subs_data_t *OpenAPI_sm_subs_data_parseFromJSON(cJSON *sm_subs_dataJSON)
{
    OpenAPI_sm_subs_data_t *sm_subs_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *session_management_subscription_data_list = NULL;
    OpenAPI_list_t *session_management_subscription_data_listList = NULL;
    cJSON *extended_sm_subs_data = NULL;
    OpenAPI_extended_sm_subs_data_t *extended_sm_subs_data_local_nonprim = NULL;
    session_management_subscription_data_list = cJSON_GetObjectItemCaseSensitive(sm_subs_dataJSON, "SessionManagementSubscriptionDataList");
    if (session_management_subscription_data_list) {
        cJSON *session_management_subscription_data_list_local = NULL;
        if (!cJSON_IsArray(session_management_subscription_data_list)) {
            ogs_error("OpenAPI_sm_subs_data_parseFromJSON() failed [session_management_subscription_data_list]");
            goto end;
        }

        session_management_subscription_data_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(session_management_subscription_data_list_local, session_management_subscription_data_list) {
            if (!cJSON_IsObject(session_management_subscription_data_list_local)) {
                ogs_error("OpenAPI_sm_subs_data_parseFromJSON() failed [session_management_subscription_data_list]");
                goto end;
            }
            OpenAPI_session_management_subscription_data_t *session_management_subscription_data_listItem = OpenAPI_session_management_subscription_data_parseFromJSON(session_management_subscription_data_list_local);
            if (!session_management_subscription_data_listItem) {
                ogs_error("No session_management_subscription_data_listItem");
                goto end;
            }
            OpenAPI_list_add(session_management_subscription_data_listList, session_management_subscription_data_listItem);
        }
    }

    extended_sm_subs_data = cJSON_GetObjectItemCaseSensitive(sm_subs_dataJSON, "ExtendedSmSubsData");
    if (extended_sm_subs_data) {
    extended_sm_subs_data_local_nonprim = OpenAPI_extended_sm_subs_data_parseFromJSON(extended_sm_subs_data);
    if (!extended_sm_subs_data_local_nonprim) {
        ogs_error("OpenAPI_extended_sm_subs_data_parseFromJSON failed [extended_sm_subs_data]");
        goto end;
    }
    }

    sm_subs_data_local_var = OpenAPI_sm_subs_data_create (
        session_management_subscription_data_list ? session_management_subscription_data_listList : NULL,
        extended_sm_subs_data ? extended_sm_subs_data_local_nonprim : NULL
    );

    return sm_subs_data_local_var;
end:
    if (session_management_subscription_data_listList) {
        OpenAPI_list_for_each(session_management_subscription_data_listList, node) {
            OpenAPI_session_management_subscription_data_free(node->data);
        }
        OpenAPI_list_free(session_management_subscription_data_listList);
        session_management_subscription_data_listList = NULL;
    }
    if (extended_sm_subs_data_local_nonprim) {
        OpenAPI_extended_sm_subs_data_free(extended_sm_subs_data_local_nonprim);
        extended_sm_subs_data_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_sm_subs_data_t *OpenAPI_sm_subs_data_copy(OpenAPI_sm_subs_data_t *dst, OpenAPI_sm_subs_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sm_subs_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sm_subs_data_convertToJSON() failed");
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

    OpenAPI_sm_subs_data_free(dst);
    dst = OpenAPI_sm_subs_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

