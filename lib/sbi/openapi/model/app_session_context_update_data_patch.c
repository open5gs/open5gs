
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "app_session_context_update_data_patch.h"

OpenAPI_app_session_context_update_data_patch_t *OpenAPI_app_session_context_update_data_patch_create(
    OpenAPI_app_session_context_update_data_t *asc_req_data
)
{
    OpenAPI_app_session_context_update_data_patch_t *app_session_context_update_data_patch_local_var = ogs_malloc(sizeof(OpenAPI_app_session_context_update_data_patch_t));
    ogs_assert(app_session_context_update_data_patch_local_var);

    app_session_context_update_data_patch_local_var->asc_req_data = asc_req_data;

    return app_session_context_update_data_patch_local_var;
}

void OpenAPI_app_session_context_update_data_patch_free(OpenAPI_app_session_context_update_data_patch_t *app_session_context_update_data_patch)
{
    if (NULL == app_session_context_update_data_patch) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_app_session_context_update_data_free(app_session_context_update_data_patch->asc_req_data);
    ogs_free(app_session_context_update_data_patch);
}

cJSON *OpenAPI_app_session_context_update_data_patch_convertToJSON(OpenAPI_app_session_context_update_data_patch_t *app_session_context_update_data_patch)
{
    cJSON *item = NULL;

    if (app_session_context_update_data_patch == NULL) {
        ogs_error("OpenAPI_app_session_context_update_data_patch_convertToJSON() failed [AppSessionContextUpdateDataPatch]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (app_session_context_update_data_patch->asc_req_data) {
    cJSON *asc_req_data_local_JSON = OpenAPI_app_session_context_update_data_convertToJSON(app_session_context_update_data_patch->asc_req_data);
    if (asc_req_data_local_JSON == NULL) {
        ogs_error("OpenAPI_app_session_context_update_data_patch_convertToJSON() failed [asc_req_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ascReqData", asc_req_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_app_session_context_update_data_patch_convertToJSON() failed [asc_req_data]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_app_session_context_update_data_patch_t *OpenAPI_app_session_context_update_data_patch_parseFromJSON(cJSON *app_session_context_update_data_patchJSON)
{
    OpenAPI_app_session_context_update_data_patch_t *app_session_context_update_data_patch_local_var = NULL;
    cJSON *asc_req_data = cJSON_GetObjectItemCaseSensitive(app_session_context_update_data_patchJSON, "ascReqData");

    OpenAPI_app_session_context_update_data_t *asc_req_data_local_nonprim = NULL;
    if (asc_req_data) {
    asc_req_data_local_nonprim = OpenAPI_app_session_context_update_data_parseFromJSON(asc_req_data);
    }

    app_session_context_update_data_patch_local_var = OpenAPI_app_session_context_update_data_patch_create (
        asc_req_data ? asc_req_data_local_nonprim : NULL
    );

    return app_session_context_update_data_patch_local_var;
end:
    return NULL;
}

OpenAPI_app_session_context_update_data_patch_t *OpenAPI_app_session_context_update_data_patch_copy(OpenAPI_app_session_context_update_data_patch_t *dst, OpenAPI_app_session_context_update_data_patch_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_app_session_context_update_data_patch_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_app_session_context_update_data_patch_convertToJSON() failed");
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

    OpenAPI_app_session_context_update_data_patch_free(dst);
    dst = OpenAPI_app_session_context_update_data_patch_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

