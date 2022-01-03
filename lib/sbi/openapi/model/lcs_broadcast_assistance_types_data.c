
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lcs_broadcast_assistance_types_data.h"

OpenAPI_lcs_broadcast_assistance_types_data_t *OpenAPI_lcs_broadcast_assistance_types_data_create(
    OpenAPI_list_t *location_assistance_type
)
{
    OpenAPI_lcs_broadcast_assistance_types_data_t *lcs_broadcast_assistance_types_data_local_var = ogs_malloc(sizeof(OpenAPI_lcs_broadcast_assistance_types_data_t));
    ogs_assert(lcs_broadcast_assistance_types_data_local_var);

    lcs_broadcast_assistance_types_data_local_var->location_assistance_type = location_assistance_type;

    return lcs_broadcast_assistance_types_data_local_var;
}

void OpenAPI_lcs_broadcast_assistance_types_data_free(OpenAPI_lcs_broadcast_assistance_types_data_t *lcs_broadcast_assistance_types_data)
{
    if (NULL == lcs_broadcast_assistance_types_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(lcs_broadcast_assistance_types_data->location_assistance_type, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(lcs_broadcast_assistance_types_data->location_assistance_type);
    ogs_free(lcs_broadcast_assistance_types_data);
}

cJSON *OpenAPI_lcs_broadcast_assistance_types_data_convertToJSON(OpenAPI_lcs_broadcast_assistance_types_data_t *lcs_broadcast_assistance_types_data)
{
    cJSON *item = NULL;

    if (lcs_broadcast_assistance_types_data == NULL) {
        ogs_error("OpenAPI_lcs_broadcast_assistance_types_data_convertToJSON() failed [LcsBroadcastAssistanceTypesData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    cJSON *location_assistance_type = cJSON_AddArrayToObject(item, "locationAssistanceType");
    if (location_assistance_type == NULL) {
        ogs_error("OpenAPI_lcs_broadcast_assistance_types_data_convertToJSON() failed [location_assistance_type]");
        goto end;
    }

    OpenAPI_lnode_t *location_assistance_type_node;
    OpenAPI_list_for_each(lcs_broadcast_assistance_types_data->location_assistance_type, location_assistance_type_node)  {
                    }

end:
    return item;
}

OpenAPI_lcs_broadcast_assistance_types_data_t *OpenAPI_lcs_broadcast_assistance_types_data_parseFromJSON(cJSON *lcs_broadcast_assistance_types_dataJSON)
{
    OpenAPI_lcs_broadcast_assistance_types_data_t *lcs_broadcast_assistance_types_data_local_var = NULL;
    cJSON *location_assistance_type = cJSON_GetObjectItemCaseSensitive(lcs_broadcast_assistance_types_dataJSON, "locationAssistanceType");
    if (!location_assistance_type) {
        ogs_error("OpenAPI_lcs_broadcast_assistance_types_data_parseFromJSON() failed [location_assistance_type]");
        goto end;
    }

    OpenAPI_list_t *location_assistance_typeList;
    cJSON *location_assistance_type_local;
    if (!cJSON_IsArray(location_assistance_type)) {
        ogs_error("OpenAPI_lcs_broadcast_assistance_types_data_parseFromJSON() failed [location_assistance_type]");
        goto end;
    }
    location_assistance_typeList = OpenAPI_list_create();

    cJSON_ArrayForEach(location_assistance_type_local, location_assistance_type) {
    }

    lcs_broadcast_assistance_types_data_local_var = OpenAPI_lcs_broadcast_assistance_types_data_create (
        location_assistance_typeList
    );

    return lcs_broadcast_assistance_types_data_local_var;
end:
    return NULL;
}

OpenAPI_lcs_broadcast_assistance_types_data_t *OpenAPI_lcs_broadcast_assistance_types_data_copy(OpenAPI_lcs_broadcast_assistance_types_data_t *dst, OpenAPI_lcs_broadcast_assistance_types_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_lcs_broadcast_assistance_types_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_lcs_broadcast_assistance_types_data_convertToJSON() failed");
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

    OpenAPI_lcs_broadcast_assistance_types_data_free(dst);
    dst = OpenAPI_lcs_broadcast_assistance_types_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

