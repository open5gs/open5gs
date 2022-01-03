
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_context_relocated_data.h"

OpenAPI_ue_context_relocated_data_t *OpenAPI_ue_context_relocated_data_create(
    OpenAPI_ue_context_t *ue_context
)
{
    OpenAPI_ue_context_relocated_data_t *ue_context_relocated_data_local_var = ogs_malloc(sizeof(OpenAPI_ue_context_relocated_data_t));
    ogs_assert(ue_context_relocated_data_local_var);

    ue_context_relocated_data_local_var->ue_context = ue_context;

    return ue_context_relocated_data_local_var;
}

void OpenAPI_ue_context_relocated_data_free(OpenAPI_ue_context_relocated_data_t *ue_context_relocated_data)
{
    if (NULL == ue_context_relocated_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_ue_context_free(ue_context_relocated_data->ue_context);
    ogs_free(ue_context_relocated_data);
}

cJSON *OpenAPI_ue_context_relocated_data_convertToJSON(OpenAPI_ue_context_relocated_data_t *ue_context_relocated_data)
{
    cJSON *item = NULL;

    if (ue_context_relocated_data == NULL) {
        ogs_error("OpenAPI_ue_context_relocated_data_convertToJSON() failed [UeContextRelocatedData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ue_context_relocated_data->ue_context) {
    cJSON *ue_context_local_JSON = OpenAPI_ue_context_convertToJSON(ue_context_relocated_data->ue_context);
    if (ue_context_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_context_relocated_data_convertToJSON() failed [ue_context]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ueContext", ue_context_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_context_relocated_data_convertToJSON() failed [ue_context]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ue_context_relocated_data_t *OpenAPI_ue_context_relocated_data_parseFromJSON(cJSON *ue_context_relocated_dataJSON)
{
    OpenAPI_ue_context_relocated_data_t *ue_context_relocated_data_local_var = NULL;
    cJSON *ue_context = cJSON_GetObjectItemCaseSensitive(ue_context_relocated_dataJSON, "ueContext");

    OpenAPI_ue_context_t *ue_context_local_nonprim = NULL;
    if (ue_context) {
    ue_context_local_nonprim = OpenAPI_ue_context_parseFromJSON(ue_context);
    }

    ue_context_relocated_data_local_var = OpenAPI_ue_context_relocated_data_create (
        ue_context ? ue_context_local_nonprim : NULL
    );

    return ue_context_relocated_data_local_var;
end:
    return NULL;
}

OpenAPI_ue_context_relocated_data_t *OpenAPI_ue_context_relocated_data_copy(OpenAPI_ue_context_relocated_data_t *dst, OpenAPI_ue_context_relocated_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ue_context_relocated_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ue_context_relocated_data_convertToJSON() failed");
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

    OpenAPI_ue_context_relocated_data_free(dst);
    dst = OpenAPI_ue_context_relocated_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

