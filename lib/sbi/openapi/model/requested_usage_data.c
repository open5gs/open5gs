
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "requested_usage_data.h"

OpenAPI_requested_usage_data_t *OpenAPI_requested_usage_data_create(
    OpenAPI_list_t *ref_um_ids,
    bool is_all_um_ids,
    int all_um_ids
)
{
    OpenAPI_requested_usage_data_t *requested_usage_data_local_var = ogs_malloc(sizeof(OpenAPI_requested_usage_data_t));
    ogs_assert(requested_usage_data_local_var);

    requested_usage_data_local_var->ref_um_ids = ref_um_ids;
    requested_usage_data_local_var->is_all_um_ids = is_all_um_ids;
    requested_usage_data_local_var->all_um_ids = all_um_ids;

    return requested_usage_data_local_var;
}

void OpenAPI_requested_usage_data_free(OpenAPI_requested_usage_data_t *requested_usage_data)
{
    if (NULL == requested_usage_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(requested_usage_data->ref_um_ids, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(requested_usage_data->ref_um_ids);
    ogs_free(requested_usage_data);
}

cJSON *OpenAPI_requested_usage_data_convertToJSON(OpenAPI_requested_usage_data_t *requested_usage_data)
{
    cJSON *item = NULL;

    if (requested_usage_data == NULL) {
        ogs_error("OpenAPI_requested_usage_data_convertToJSON() failed [RequestedUsageData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (requested_usage_data->ref_um_ids) {
    cJSON *ref_um_ids = cJSON_AddArrayToObject(item, "refUmIds");
    if (ref_um_ids == NULL) {
        ogs_error("OpenAPI_requested_usage_data_convertToJSON() failed [ref_um_ids]");
        goto end;
    }

    OpenAPI_lnode_t *ref_um_ids_node;
    OpenAPI_list_for_each(requested_usage_data->ref_um_ids, ref_um_ids_node)  {
    if (cJSON_AddStringToObject(ref_um_ids, "", (char*)ref_um_ids_node->data) == NULL) {
        ogs_error("OpenAPI_requested_usage_data_convertToJSON() failed [ref_um_ids]");
        goto end;
    }
                    }
    }

    if (requested_usage_data->is_all_um_ids) {
    if (cJSON_AddBoolToObject(item, "allUmIds", requested_usage_data->all_um_ids) == NULL) {
        ogs_error("OpenAPI_requested_usage_data_convertToJSON() failed [all_um_ids]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_requested_usage_data_t *OpenAPI_requested_usage_data_parseFromJSON(cJSON *requested_usage_dataJSON)
{
    OpenAPI_requested_usage_data_t *requested_usage_data_local_var = NULL;
    cJSON *ref_um_ids = cJSON_GetObjectItemCaseSensitive(requested_usage_dataJSON, "refUmIds");

    OpenAPI_list_t *ref_um_idsList;
    if (ref_um_ids) {
    cJSON *ref_um_ids_local;
    if (!cJSON_IsArray(ref_um_ids)) {
        ogs_error("OpenAPI_requested_usage_data_parseFromJSON() failed [ref_um_ids]");
        goto end;
    }
    ref_um_idsList = OpenAPI_list_create();

    cJSON_ArrayForEach(ref_um_ids_local, ref_um_ids) {
    if (!cJSON_IsString(ref_um_ids_local)) {
        ogs_error("OpenAPI_requested_usage_data_parseFromJSON() failed [ref_um_ids]");
        goto end;
    }
    OpenAPI_list_add(ref_um_idsList , ogs_strdup(ref_um_ids_local->valuestring));
    }
    }

    cJSON *all_um_ids = cJSON_GetObjectItemCaseSensitive(requested_usage_dataJSON, "allUmIds");

    if (all_um_ids) {
    if (!cJSON_IsBool(all_um_ids)) {
        ogs_error("OpenAPI_requested_usage_data_parseFromJSON() failed [all_um_ids]");
        goto end;
    }
    }

    requested_usage_data_local_var = OpenAPI_requested_usage_data_create (
        ref_um_ids ? ref_um_idsList : NULL,
        all_um_ids ? true : false,
        all_um_ids ? all_um_ids->valueint : 0
    );

    return requested_usage_data_local_var;
end:
    return NULL;
}

OpenAPI_requested_usage_data_t *OpenAPI_requested_usage_data_copy(OpenAPI_requested_usage_data_t *dst, OpenAPI_requested_usage_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_requested_usage_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_requested_usage_data_convertToJSON() failed");
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

    OpenAPI_requested_usage_data_free(dst);
    dst = OpenAPI_requested_usage_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

