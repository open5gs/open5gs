
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_context_cancel_relocate_data.h"

OpenAPI_ue_context_cancel_relocate_data_t *OpenAPI_ue_context_cancel_relocate_data_create(
    char *supi,
    OpenAPI_ref_to_binary_data_t *relocation_cancel_request
)
{
    OpenAPI_ue_context_cancel_relocate_data_t *ue_context_cancel_relocate_data_local_var = ogs_malloc(sizeof(OpenAPI_ue_context_cancel_relocate_data_t));
    ogs_assert(ue_context_cancel_relocate_data_local_var);

    ue_context_cancel_relocate_data_local_var->supi = supi;
    ue_context_cancel_relocate_data_local_var->relocation_cancel_request = relocation_cancel_request;

    return ue_context_cancel_relocate_data_local_var;
}

void OpenAPI_ue_context_cancel_relocate_data_free(OpenAPI_ue_context_cancel_relocate_data_t *ue_context_cancel_relocate_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ue_context_cancel_relocate_data) {
        return;
    }
    if (ue_context_cancel_relocate_data->supi) {
        ogs_free(ue_context_cancel_relocate_data->supi);
        ue_context_cancel_relocate_data->supi = NULL;
    }
    if (ue_context_cancel_relocate_data->relocation_cancel_request) {
        OpenAPI_ref_to_binary_data_free(ue_context_cancel_relocate_data->relocation_cancel_request);
        ue_context_cancel_relocate_data->relocation_cancel_request = NULL;
    }
    ogs_free(ue_context_cancel_relocate_data);
}

cJSON *OpenAPI_ue_context_cancel_relocate_data_convertToJSON(OpenAPI_ue_context_cancel_relocate_data_t *ue_context_cancel_relocate_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ue_context_cancel_relocate_data == NULL) {
        ogs_error("OpenAPI_ue_context_cancel_relocate_data_convertToJSON() failed [UeContextCancelRelocateData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ue_context_cancel_relocate_data->supi) {
    if (cJSON_AddStringToObject(item, "supi", ue_context_cancel_relocate_data->supi) == NULL) {
        ogs_error("OpenAPI_ue_context_cancel_relocate_data_convertToJSON() failed [supi]");
        goto end;
    }
    }

    if (!ue_context_cancel_relocate_data->relocation_cancel_request) {
        ogs_error("OpenAPI_ue_context_cancel_relocate_data_convertToJSON() failed [relocation_cancel_request]");
        return NULL;
    }
    cJSON *relocation_cancel_request_local_JSON = OpenAPI_ref_to_binary_data_convertToJSON(ue_context_cancel_relocate_data->relocation_cancel_request);
    if (relocation_cancel_request_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_context_cancel_relocate_data_convertToJSON() failed [relocation_cancel_request]");
        goto end;
    }
    cJSON_AddItemToObject(item, "relocationCancelRequest", relocation_cancel_request_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_context_cancel_relocate_data_convertToJSON() failed [relocation_cancel_request]");
        goto end;
    }

end:
    return item;
}

OpenAPI_ue_context_cancel_relocate_data_t *OpenAPI_ue_context_cancel_relocate_data_parseFromJSON(cJSON *ue_context_cancel_relocate_dataJSON)
{
    OpenAPI_ue_context_cancel_relocate_data_t *ue_context_cancel_relocate_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *supi = NULL;
    cJSON *relocation_cancel_request = NULL;
    OpenAPI_ref_to_binary_data_t *relocation_cancel_request_local_nonprim = NULL;
    supi = cJSON_GetObjectItemCaseSensitive(ue_context_cancel_relocate_dataJSON, "supi");
    if (supi) {
    if (!cJSON_IsString(supi) && !cJSON_IsNull(supi)) {
        ogs_error("OpenAPI_ue_context_cancel_relocate_data_parseFromJSON() failed [supi]");
        goto end;
    }
    }

    relocation_cancel_request = cJSON_GetObjectItemCaseSensitive(ue_context_cancel_relocate_dataJSON, "relocationCancelRequest");
    if (!relocation_cancel_request) {
        ogs_error("OpenAPI_ue_context_cancel_relocate_data_parseFromJSON() failed [relocation_cancel_request]");
        goto end;
    }
    relocation_cancel_request_local_nonprim = OpenAPI_ref_to_binary_data_parseFromJSON(relocation_cancel_request);
    if (!relocation_cancel_request_local_nonprim) {
        ogs_error("OpenAPI_ref_to_binary_data_parseFromJSON failed [relocation_cancel_request]");
        goto end;
    }

    ue_context_cancel_relocate_data_local_var = OpenAPI_ue_context_cancel_relocate_data_create (
        supi && !cJSON_IsNull(supi) ? ogs_strdup(supi->valuestring) : NULL,
        relocation_cancel_request_local_nonprim
    );

    return ue_context_cancel_relocate_data_local_var;
end:
    if (relocation_cancel_request_local_nonprim) {
        OpenAPI_ref_to_binary_data_free(relocation_cancel_request_local_nonprim);
        relocation_cancel_request_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_ue_context_cancel_relocate_data_t *OpenAPI_ue_context_cancel_relocate_data_copy(OpenAPI_ue_context_cancel_relocate_data_t *dst, OpenAPI_ue_context_cancel_relocate_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ue_context_cancel_relocate_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ue_context_cancel_relocate_data_convertToJSON() failed");
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

    OpenAPI_ue_context_cancel_relocate_data_free(dst);
    dst = OpenAPI_ue_context_cancel_relocate_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

