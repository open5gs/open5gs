
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pp_data_entry_list.h"

OpenAPI_pp_data_entry_list_t *OpenAPI_pp_data_entry_list_create(
    OpenAPI_list_t *pp_data_entry_list,
    char *supported_features
)
{
    OpenAPI_pp_data_entry_list_t *pp_data_entry_list_local_var = ogs_malloc(sizeof(OpenAPI_pp_data_entry_list_t));
    ogs_assert(pp_data_entry_list_local_var);

    pp_data_entry_list_local_var->pp_data_entry_list = pp_data_entry_list;
    pp_data_entry_list_local_var->supported_features = supported_features;

    return pp_data_entry_list_local_var;
}

void OpenAPI_pp_data_entry_list_free(OpenAPI_pp_data_entry_list_t *pp_data_entry_list)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pp_data_entry_list) {
        return;
    }
    if (pp_data_entry_list->pp_data_entry_list) {
        OpenAPI_list_for_each(pp_data_entry_list->pp_data_entry_list, node) {
            OpenAPI_pp_data_entry_free(node->data);
        }
        OpenAPI_list_free(pp_data_entry_list->pp_data_entry_list);
        pp_data_entry_list->pp_data_entry_list = NULL;
    }
    if (pp_data_entry_list->supported_features) {
        ogs_free(pp_data_entry_list->supported_features);
        pp_data_entry_list->supported_features = NULL;
    }
    ogs_free(pp_data_entry_list);
}

cJSON *OpenAPI_pp_data_entry_list_convertToJSON(OpenAPI_pp_data_entry_list_t *pp_data_entry_list)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pp_data_entry_list == NULL) {
        ogs_error("OpenAPI_pp_data_entry_list_convertToJSON() failed [PpDataEntryList]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (pp_data_entry_list->pp_data_entry_list) {
    cJSON *pp_data_entry_listList = cJSON_AddArrayToObject(item, "ppDataEntryList");
    if (pp_data_entry_listList == NULL) {
        ogs_error("OpenAPI_pp_data_entry_list_convertToJSON() failed [pp_data_entry_list]");
        goto end;
    }
    OpenAPI_list_for_each(pp_data_entry_list->pp_data_entry_list, node) {
        cJSON *itemLocal = OpenAPI_pp_data_entry_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_pp_data_entry_list_convertToJSON() failed [pp_data_entry_list]");
            goto end;
        }
        cJSON_AddItemToArray(pp_data_entry_listList, itemLocal);
    }
    }

    if (pp_data_entry_list->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", pp_data_entry_list->supported_features) == NULL) {
        ogs_error("OpenAPI_pp_data_entry_list_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_pp_data_entry_list_t *OpenAPI_pp_data_entry_list_parseFromJSON(cJSON *pp_data_entry_listJSON)
{
    OpenAPI_pp_data_entry_list_t *pp_data_entry_list_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *pp_data_entry_list = NULL;
    OpenAPI_list_t *pp_data_entry_listList = NULL;
    cJSON *supported_features = NULL;
    pp_data_entry_list = cJSON_GetObjectItemCaseSensitive(pp_data_entry_listJSON, "ppDataEntryList");
    if (pp_data_entry_list) {
        cJSON *pp_data_entry_list_local = NULL;
        if (!cJSON_IsArray(pp_data_entry_list)) {
            ogs_error("OpenAPI_pp_data_entry_list_parseFromJSON() failed [pp_data_entry_list]");
            goto end;
        }

        pp_data_entry_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(pp_data_entry_list_local, pp_data_entry_list) {
            if (!cJSON_IsObject(pp_data_entry_list_local)) {
                ogs_error("OpenAPI_pp_data_entry_list_parseFromJSON() failed [pp_data_entry_list]");
                goto end;
            }
            OpenAPI_pp_data_entry_t *pp_data_entry_listItem = OpenAPI_pp_data_entry_parseFromJSON(pp_data_entry_list_local);
            if (!pp_data_entry_listItem) {
                ogs_error("No pp_data_entry_listItem");
                goto end;
            }
            OpenAPI_list_add(pp_data_entry_listList, pp_data_entry_listItem);
        }
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(pp_data_entry_listJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_pp_data_entry_list_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    pp_data_entry_list_local_var = OpenAPI_pp_data_entry_list_create (
        pp_data_entry_list ? pp_data_entry_listList : NULL,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL
    );

    return pp_data_entry_list_local_var;
end:
    if (pp_data_entry_listList) {
        OpenAPI_list_for_each(pp_data_entry_listList, node) {
            OpenAPI_pp_data_entry_free(node->data);
        }
        OpenAPI_list_free(pp_data_entry_listList);
        pp_data_entry_listList = NULL;
    }
    return NULL;
}

OpenAPI_pp_data_entry_list_t *OpenAPI_pp_data_entry_list_copy(OpenAPI_pp_data_entry_list_t *dst, OpenAPI_pp_data_entry_list_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pp_data_entry_list_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pp_data_entry_list_convertToJSON() failed");
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

    OpenAPI_pp_data_entry_list_free(dst);
    dst = OpenAPI_pp_data_entry_list_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

