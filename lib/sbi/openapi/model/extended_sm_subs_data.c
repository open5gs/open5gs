
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "extended_sm_subs_data.h"

OpenAPI_extended_sm_subs_data_t *OpenAPI_extended_sm_subs_data_create(
    OpenAPI_list_t *shared_sm_subs_data_ids,
    OpenAPI_list_t *individual_sm_subs_data
)
{
    OpenAPI_extended_sm_subs_data_t *extended_sm_subs_data_local_var = ogs_malloc(sizeof(OpenAPI_extended_sm_subs_data_t));
    ogs_assert(extended_sm_subs_data_local_var);

    extended_sm_subs_data_local_var->shared_sm_subs_data_ids = shared_sm_subs_data_ids;
    extended_sm_subs_data_local_var->individual_sm_subs_data = individual_sm_subs_data;

    return extended_sm_subs_data_local_var;
}

void OpenAPI_extended_sm_subs_data_free(OpenAPI_extended_sm_subs_data_t *extended_sm_subs_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == extended_sm_subs_data) {
        return;
    }
    if (extended_sm_subs_data->shared_sm_subs_data_ids) {
        OpenAPI_list_for_each(extended_sm_subs_data->shared_sm_subs_data_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(extended_sm_subs_data->shared_sm_subs_data_ids);
        extended_sm_subs_data->shared_sm_subs_data_ids = NULL;
    }
    if (extended_sm_subs_data->individual_sm_subs_data) {
        OpenAPI_list_for_each(extended_sm_subs_data->individual_sm_subs_data, node) {
            OpenAPI_session_management_subscription_data_free(node->data);
        }
        OpenAPI_list_free(extended_sm_subs_data->individual_sm_subs_data);
        extended_sm_subs_data->individual_sm_subs_data = NULL;
    }
    ogs_free(extended_sm_subs_data);
}

cJSON *OpenAPI_extended_sm_subs_data_convertToJSON(OpenAPI_extended_sm_subs_data_t *extended_sm_subs_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (extended_sm_subs_data == NULL) {
        ogs_error("OpenAPI_extended_sm_subs_data_convertToJSON() failed [ExtendedSmSubsData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!extended_sm_subs_data->shared_sm_subs_data_ids) {
        ogs_error("OpenAPI_extended_sm_subs_data_convertToJSON() failed [shared_sm_subs_data_ids]");
        return NULL;
    }
    cJSON *shared_sm_subs_data_idsList = cJSON_AddArrayToObject(item, "sharedSmSubsDataIds");
    if (shared_sm_subs_data_idsList == NULL) {
        ogs_error("OpenAPI_extended_sm_subs_data_convertToJSON() failed [shared_sm_subs_data_ids]");
        goto end;
    }
    OpenAPI_list_for_each(extended_sm_subs_data->shared_sm_subs_data_ids, node) {
        if (cJSON_AddStringToObject(shared_sm_subs_data_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_extended_sm_subs_data_convertToJSON() failed [shared_sm_subs_data_ids]");
            goto end;
        }
    }

    if (extended_sm_subs_data->individual_sm_subs_data) {
    cJSON *individual_sm_subs_dataList = cJSON_AddArrayToObject(item, "individualSmSubsData");
    if (individual_sm_subs_dataList == NULL) {
        ogs_error("OpenAPI_extended_sm_subs_data_convertToJSON() failed [individual_sm_subs_data]");
        goto end;
    }
    OpenAPI_list_for_each(extended_sm_subs_data->individual_sm_subs_data, node) {
        cJSON *itemLocal = OpenAPI_session_management_subscription_data_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_extended_sm_subs_data_convertToJSON() failed [individual_sm_subs_data]");
            goto end;
        }
        cJSON_AddItemToArray(individual_sm_subs_dataList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_extended_sm_subs_data_t *OpenAPI_extended_sm_subs_data_parseFromJSON(cJSON *extended_sm_subs_dataJSON)
{
    OpenAPI_extended_sm_subs_data_t *extended_sm_subs_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *shared_sm_subs_data_ids = NULL;
    OpenAPI_list_t *shared_sm_subs_data_idsList = NULL;
    cJSON *individual_sm_subs_data = NULL;
    OpenAPI_list_t *individual_sm_subs_dataList = NULL;
    shared_sm_subs_data_ids = cJSON_GetObjectItemCaseSensitive(extended_sm_subs_dataJSON, "sharedSmSubsDataIds");
    if (!shared_sm_subs_data_ids) {
        ogs_error("OpenAPI_extended_sm_subs_data_parseFromJSON() failed [shared_sm_subs_data_ids]");
        goto end;
    }
        cJSON *shared_sm_subs_data_ids_local = NULL;
        if (!cJSON_IsArray(shared_sm_subs_data_ids)) {
            ogs_error("OpenAPI_extended_sm_subs_data_parseFromJSON() failed [shared_sm_subs_data_ids]");
            goto end;
        }

        shared_sm_subs_data_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(shared_sm_subs_data_ids_local, shared_sm_subs_data_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(shared_sm_subs_data_ids_local)) {
                ogs_error("OpenAPI_extended_sm_subs_data_parseFromJSON() failed [shared_sm_subs_data_ids]");
                goto end;
            }
            OpenAPI_list_add(shared_sm_subs_data_idsList, ogs_strdup(shared_sm_subs_data_ids_local->valuestring));
        }

    individual_sm_subs_data = cJSON_GetObjectItemCaseSensitive(extended_sm_subs_dataJSON, "individualSmSubsData");
    if (individual_sm_subs_data) {
        cJSON *individual_sm_subs_data_local = NULL;
        if (!cJSON_IsArray(individual_sm_subs_data)) {
            ogs_error("OpenAPI_extended_sm_subs_data_parseFromJSON() failed [individual_sm_subs_data]");
            goto end;
        }

        individual_sm_subs_dataList = OpenAPI_list_create();

        cJSON_ArrayForEach(individual_sm_subs_data_local, individual_sm_subs_data) {
            if (!cJSON_IsObject(individual_sm_subs_data_local)) {
                ogs_error("OpenAPI_extended_sm_subs_data_parseFromJSON() failed [individual_sm_subs_data]");
                goto end;
            }
            OpenAPI_session_management_subscription_data_t *individual_sm_subs_dataItem = OpenAPI_session_management_subscription_data_parseFromJSON(individual_sm_subs_data_local);
            if (!individual_sm_subs_dataItem) {
                ogs_error("No individual_sm_subs_dataItem");
                goto end;
            }
            OpenAPI_list_add(individual_sm_subs_dataList, individual_sm_subs_dataItem);
        }
    }

    extended_sm_subs_data_local_var = OpenAPI_extended_sm_subs_data_create (
        shared_sm_subs_data_idsList,
        individual_sm_subs_data ? individual_sm_subs_dataList : NULL
    );

    return extended_sm_subs_data_local_var;
end:
    if (shared_sm_subs_data_idsList) {
        OpenAPI_list_for_each(shared_sm_subs_data_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(shared_sm_subs_data_idsList);
        shared_sm_subs_data_idsList = NULL;
    }
    if (individual_sm_subs_dataList) {
        OpenAPI_list_for_each(individual_sm_subs_dataList, node) {
            OpenAPI_session_management_subscription_data_free(node->data);
        }
        OpenAPI_list_free(individual_sm_subs_dataList);
        individual_sm_subs_dataList = NULL;
    }
    return NULL;
}

OpenAPI_extended_sm_subs_data_t *OpenAPI_extended_sm_subs_data_copy(OpenAPI_extended_sm_subs_data_t *dst, OpenAPI_extended_sm_subs_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_extended_sm_subs_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_extended_sm_subs_data_convertToJSON() failed");
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

    OpenAPI_extended_sm_subs_data_free(dst);
    dst = OpenAPI_extended_sm_subs_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

