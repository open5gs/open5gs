
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "shared_data.h"

OpenAPI_shared_data_t *OpenAPI_shared_data_create(
    char *shared_data_id,
    bool is_shared_profile_data_null,
    OpenAPI_nf_profile_t *shared_profile_data,
    OpenAPI_nf_service_t *shared_service_data,
    OpenAPI_shared_scope_t *authorized_write_scope
)
{
    OpenAPI_shared_data_t *shared_data_local_var = ogs_malloc(sizeof(OpenAPI_shared_data_t));
    ogs_assert(shared_data_local_var);

    shared_data_local_var->shared_data_id = shared_data_id;
    shared_data_local_var->is_shared_profile_data_null = is_shared_profile_data_null;
    shared_data_local_var->shared_profile_data = shared_profile_data;
    shared_data_local_var->shared_service_data = shared_service_data;
    shared_data_local_var->authorized_write_scope = authorized_write_scope;

    return shared_data_local_var;
}

void OpenAPI_shared_data_free(OpenAPI_shared_data_t *shared_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == shared_data) {
        return;
    }
    if (shared_data->shared_data_id) {
        ogs_free(shared_data->shared_data_id);
        shared_data->shared_data_id = NULL;
    }
    if (shared_data->shared_profile_data) {
        OpenAPI_nf_profile_free(shared_data->shared_profile_data);
        shared_data->shared_profile_data = NULL;
    }
    if (shared_data->shared_service_data) {
        OpenAPI_nf_service_free(shared_data->shared_service_data);
        shared_data->shared_service_data = NULL;
    }
    if (shared_data->authorized_write_scope) {
        OpenAPI_shared_scope_free(shared_data->authorized_write_scope);
        shared_data->authorized_write_scope = NULL;
    }
    ogs_free(shared_data);
}

cJSON *OpenAPI_shared_data_convertToJSON(OpenAPI_shared_data_t *shared_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (shared_data == NULL) {
        ogs_error("OpenAPI_shared_data_convertToJSON() failed [SharedData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!shared_data->shared_data_id) {
        ogs_error("OpenAPI_shared_data_convertToJSON() failed [shared_data_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "sharedDataId", shared_data->shared_data_id) == NULL) {
        ogs_error("OpenAPI_shared_data_convertToJSON() failed [shared_data_id]");
        goto end;
    }

    if (shared_data->shared_profile_data) {
    cJSON *shared_profile_data_local_JSON = OpenAPI_nf_profile_convertToJSON(shared_data->shared_profile_data);
    if (shared_profile_data_local_JSON == NULL) {
        ogs_error("OpenAPI_shared_data_convertToJSON() failed [shared_profile_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sharedProfileData", shared_profile_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_shared_data_convertToJSON() failed [shared_profile_data]");
        goto end;
    }
    } else if (shared_data->is_shared_profile_data_null) {
        if (cJSON_AddNullToObject(item, "sharedProfileData") == NULL) {
            ogs_error("OpenAPI_shared_data_convertToJSON() failed [shared_profile_data]");
            goto end;
        }
    }

    if (shared_data->shared_service_data) {
    cJSON *shared_service_data_local_JSON = OpenAPI_nf_service_convertToJSON(shared_data->shared_service_data);
    if (shared_service_data_local_JSON == NULL) {
        ogs_error("OpenAPI_shared_data_convertToJSON() failed [shared_service_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sharedServiceData", shared_service_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_shared_data_convertToJSON() failed [shared_service_data]");
        goto end;
    }
    }

    if (shared_data->authorized_write_scope) {
    cJSON *authorized_write_scope_local_JSON = OpenAPI_shared_scope_convertToJSON(shared_data->authorized_write_scope);
    if (authorized_write_scope_local_JSON == NULL) {
        ogs_error("OpenAPI_shared_data_convertToJSON() failed [authorized_write_scope]");
        goto end;
    }
    cJSON_AddItemToObject(item, "authorizedWriteScope", authorized_write_scope_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_shared_data_convertToJSON() failed [authorized_write_scope]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_shared_data_t *OpenAPI_shared_data_parseFromJSON(cJSON *shared_dataJSON)
{
    OpenAPI_shared_data_t *shared_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *shared_data_id = NULL;
    cJSON *shared_profile_data = NULL;
    OpenAPI_nf_profile_t *shared_profile_data_local_nonprim = NULL;
    cJSON *shared_service_data = NULL;
    OpenAPI_nf_service_t *shared_service_data_local_nonprim = NULL;
    cJSON *authorized_write_scope = NULL;
    OpenAPI_shared_scope_t *authorized_write_scope_local_nonprim = NULL;
    shared_data_id = cJSON_GetObjectItemCaseSensitive(shared_dataJSON, "sharedDataId");
    if (!shared_data_id) {
        ogs_error("OpenAPI_shared_data_parseFromJSON() failed [shared_data_id]");
        goto end;
    }
    if (!cJSON_IsString(shared_data_id)) {
        ogs_error("OpenAPI_shared_data_parseFromJSON() failed [shared_data_id]");
        goto end;
    }

    shared_profile_data = cJSON_GetObjectItemCaseSensitive(shared_dataJSON, "sharedProfileData");
    if (shared_profile_data) {
    if (!cJSON_IsNull(shared_profile_data)) {
    shared_profile_data_local_nonprim = OpenAPI_nf_profile_parseFromJSON(shared_profile_data);
    if (!shared_profile_data_local_nonprim) {
        ogs_error("OpenAPI_nf_profile_parseFromJSON failed [shared_profile_data]");
        goto end;
    }
    }
    }

    shared_service_data = cJSON_GetObjectItemCaseSensitive(shared_dataJSON, "sharedServiceData");
    if (shared_service_data) {
    shared_service_data_local_nonprim = OpenAPI_nf_service_parseFromJSON(shared_service_data);
    if (!shared_service_data_local_nonprim) {
        ogs_error("OpenAPI_nf_service_parseFromJSON failed [shared_service_data]");
        goto end;
    }
    }

    authorized_write_scope = cJSON_GetObjectItemCaseSensitive(shared_dataJSON, "authorizedWriteScope");
    if (authorized_write_scope) {
    authorized_write_scope_local_nonprim = OpenAPI_shared_scope_parseFromJSON(authorized_write_scope);
    if (!authorized_write_scope_local_nonprim) {
        ogs_error("OpenAPI_shared_scope_parseFromJSON failed [authorized_write_scope]");
        goto end;
    }
    }

    shared_data_local_var = OpenAPI_shared_data_create (
        ogs_strdup(shared_data_id->valuestring),
        shared_profile_data && cJSON_IsNull(shared_profile_data) ? true : false,
        shared_profile_data ? shared_profile_data_local_nonprim : NULL,
        shared_service_data ? shared_service_data_local_nonprim : NULL,
        authorized_write_scope ? authorized_write_scope_local_nonprim : NULL
    );

    return shared_data_local_var;
end:
    if (shared_profile_data_local_nonprim) {
        OpenAPI_nf_profile_free(shared_profile_data_local_nonprim);
        shared_profile_data_local_nonprim = NULL;
    }
    if (shared_service_data_local_nonprim) {
        OpenAPI_nf_service_free(shared_service_data_local_nonprim);
        shared_service_data_local_nonprim = NULL;
    }
    if (authorized_write_scope_local_nonprim) {
        OpenAPI_shared_scope_free(authorized_write_scope_local_nonprim);
        authorized_write_scope_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_shared_data_t *OpenAPI_shared_data_copy(OpenAPI_shared_data_t *dst, OpenAPI_shared_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_shared_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_shared_data_convertToJSON() failed");
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

    OpenAPI_shared_data_free(dst);
    dst = OpenAPI_shared_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

