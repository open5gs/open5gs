
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "aiot_dev_profile_data.h"

OpenAPI_aiot_dev_profile_data_t *OpenAPI_aiot_dev_profile_data_create(
    char *aiot_dev_perm_id,
    OpenAPI_last_known_aiotf_info_t *last_known_aiotf_info,
    char *tid_current,
    char *tid_previous,
    OpenAPI_tid_handling_information_t *tid_handling_information
)
{
    OpenAPI_aiot_dev_profile_data_t *aiot_dev_profile_data_local_var = ogs_malloc(sizeof(OpenAPI_aiot_dev_profile_data_t));
    ogs_assert(aiot_dev_profile_data_local_var);

    aiot_dev_profile_data_local_var->aiot_dev_perm_id = aiot_dev_perm_id;
    aiot_dev_profile_data_local_var->last_known_aiotf_info = last_known_aiotf_info;
    aiot_dev_profile_data_local_var->tid_current = tid_current;
    aiot_dev_profile_data_local_var->tid_previous = tid_previous;
    aiot_dev_profile_data_local_var->tid_handling_information = tid_handling_information;

    return aiot_dev_profile_data_local_var;
}

void OpenAPI_aiot_dev_profile_data_free(OpenAPI_aiot_dev_profile_data_t *aiot_dev_profile_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == aiot_dev_profile_data) {
        return;
    }
    if (aiot_dev_profile_data->aiot_dev_perm_id) {
        ogs_free(aiot_dev_profile_data->aiot_dev_perm_id);
        aiot_dev_profile_data->aiot_dev_perm_id = NULL;
    }
    if (aiot_dev_profile_data->last_known_aiotf_info) {
        OpenAPI_last_known_aiotf_info_free(aiot_dev_profile_data->last_known_aiotf_info);
        aiot_dev_profile_data->last_known_aiotf_info = NULL;
    }
    if (aiot_dev_profile_data->tid_current) {
        ogs_free(aiot_dev_profile_data->tid_current);
        aiot_dev_profile_data->tid_current = NULL;
    }
    if (aiot_dev_profile_data->tid_previous) {
        ogs_free(aiot_dev_profile_data->tid_previous);
        aiot_dev_profile_data->tid_previous = NULL;
    }
    if (aiot_dev_profile_data->tid_handling_information) {
        OpenAPI_tid_handling_information_free(aiot_dev_profile_data->tid_handling_information);
        aiot_dev_profile_data->tid_handling_information = NULL;
    }
    ogs_free(aiot_dev_profile_data);
}

cJSON *OpenAPI_aiot_dev_profile_data_convertToJSON(OpenAPI_aiot_dev_profile_data_t *aiot_dev_profile_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (aiot_dev_profile_data == NULL) {
        ogs_error("OpenAPI_aiot_dev_profile_data_convertToJSON() failed [AiotDevProfileData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!aiot_dev_profile_data->aiot_dev_perm_id) {
        ogs_error("OpenAPI_aiot_dev_profile_data_convertToJSON() failed [aiot_dev_perm_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "aiotDevPermId", aiot_dev_profile_data->aiot_dev_perm_id) == NULL) {
        ogs_error("OpenAPI_aiot_dev_profile_data_convertToJSON() failed [aiot_dev_perm_id]");
        goto end;
    }

    if (!aiot_dev_profile_data->last_known_aiotf_info) {
        ogs_error("OpenAPI_aiot_dev_profile_data_convertToJSON() failed [last_known_aiotf_info]");
        return NULL;
    }
    cJSON *last_known_aiotf_info_local_JSON = OpenAPI_last_known_aiotf_info_convertToJSON(aiot_dev_profile_data->last_known_aiotf_info);
    if (last_known_aiotf_info_local_JSON == NULL) {
        ogs_error("OpenAPI_aiot_dev_profile_data_convertToJSON() failed [last_known_aiotf_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "lastKnownAiotfInfo", last_known_aiotf_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_aiot_dev_profile_data_convertToJSON() failed [last_known_aiotf_info]");
        goto end;
    }

    if (aiot_dev_profile_data->tid_current) {
    if (cJSON_AddStringToObject(item, "tidCurrent", aiot_dev_profile_data->tid_current) == NULL) {
        ogs_error("OpenAPI_aiot_dev_profile_data_convertToJSON() failed [tid_current]");
        goto end;
    }
    }

    if (aiot_dev_profile_data->tid_previous) {
    if (cJSON_AddStringToObject(item, "tidPrevious", aiot_dev_profile_data->tid_previous) == NULL) {
        ogs_error("OpenAPI_aiot_dev_profile_data_convertToJSON() failed [tid_previous]");
        goto end;
    }
    }

    if (aiot_dev_profile_data->tid_handling_information) {
    cJSON *tid_handling_information_local_JSON = OpenAPI_tid_handling_information_convertToJSON(aiot_dev_profile_data->tid_handling_information);
    if (tid_handling_information_local_JSON == NULL) {
        ogs_error("OpenAPI_aiot_dev_profile_data_convertToJSON() failed [tid_handling_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "tidHandlingInformation", tid_handling_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_aiot_dev_profile_data_convertToJSON() failed [tid_handling_information]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_aiot_dev_profile_data_t *OpenAPI_aiot_dev_profile_data_parseFromJSON(cJSON *aiot_dev_profile_dataJSON)
{
    OpenAPI_aiot_dev_profile_data_t *aiot_dev_profile_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *aiot_dev_perm_id = NULL;
    cJSON *last_known_aiotf_info = NULL;
    OpenAPI_last_known_aiotf_info_t *last_known_aiotf_info_local_nonprim = NULL;
    cJSON *tid_current = NULL;
    cJSON *tid_previous = NULL;
    cJSON *tid_handling_information = NULL;
    OpenAPI_tid_handling_information_t *tid_handling_information_local_nonprim = NULL;
    aiot_dev_perm_id = cJSON_GetObjectItemCaseSensitive(aiot_dev_profile_dataJSON, "aiotDevPermId");
    if (!aiot_dev_perm_id) {
        ogs_error("OpenAPI_aiot_dev_profile_data_parseFromJSON() failed [aiot_dev_perm_id]");
        goto end;
    }
    if (!cJSON_IsString(aiot_dev_perm_id)) {
        ogs_error("OpenAPI_aiot_dev_profile_data_parseFromJSON() failed [aiot_dev_perm_id]");
        goto end;
    }

    last_known_aiotf_info = cJSON_GetObjectItemCaseSensitive(aiot_dev_profile_dataJSON, "lastKnownAiotfInfo");
    if (!last_known_aiotf_info) {
        ogs_error("OpenAPI_aiot_dev_profile_data_parseFromJSON() failed [last_known_aiotf_info]");
        goto end;
    }
    last_known_aiotf_info_local_nonprim = OpenAPI_last_known_aiotf_info_parseFromJSON(last_known_aiotf_info);
    if (!last_known_aiotf_info_local_nonprim) {
        ogs_error("OpenAPI_last_known_aiotf_info_parseFromJSON failed [last_known_aiotf_info]");
        goto end;
    }

    tid_current = cJSON_GetObjectItemCaseSensitive(aiot_dev_profile_dataJSON, "tidCurrent");
    if (tid_current) {
    if (!cJSON_IsString(tid_current) && !cJSON_IsNull(tid_current)) {
        ogs_error("OpenAPI_aiot_dev_profile_data_parseFromJSON() failed [tid_current]");
        goto end;
    }
    }

    tid_previous = cJSON_GetObjectItemCaseSensitive(aiot_dev_profile_dataJSON, "tidPrevious");
    if (tid_previous) {
    if (!cJSON_IsString(tid_previous) && !cJSON_IsNull(tid_previous)) {
        ogs_error("OpenAPI_aiot_dev_profile_data_parseFromJSON() failed [tid_previous]");
        goto end;
    }
    }

    tid_handling_information = cJSON_GetObjectItemCaseSensitive(aiot_dev_profile_dataJSON, "tidHandlingInformation");
    if (tid_handling_information) {
    tid_handling_information_local_nonprim = OpenAPI_tid_handling_information_parseFromJSON(tid_handling_information);
    if (!tid_handling_information_local_nonprim) {
        ogs_error("OpenAPI_tid_handling_information_parseFromJSON failed [tid_handling_information]");
        goto end;
    }
    }

    aiot_dev_profile_data_local_var = OpenAPI_aiot_dev_profile_data_create (
        ogs_strdup(aiot_dev_perm_id->valuestring),
        last_known_aiotf_info_local_nonprim,
        tid_current && !cJSON_IsNull(tid_current) ? ogs_strdup(tid_current->valuestring) : NULL,
        tid_previous && !cJSON_IsNull(tid_previous) ? ogs_strdup(tid_previous->valuestring) : NULL,
        tid_handling_information ? tid_handling_information_local_nonprim : NULL
    );

    return aiot_dev_profile_data_local_var;
end:
    if (last_known_aiotf_info_local_nonprim) {
        OpenAPI_last_known_aiotf_info_free(last_known_aiotf_info_local_nonprim);
        last_known_aiotf_info_local_nonprim = NULL;
    }
    if (tid_handling_information_local_nonprim) {
        OpenAPI_tid_handling_information_free(tid_handling_information_local_nonprim);
        tid_handling_information_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_aiot_dev_profile_data_t *OpenAPI_aiot_dev_profile_data_copy(OpenAPI_aiot_dev_profile_data_t *dst, OpenAPI_aiot_dev_profile_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_aiot_dev_profile_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_aiot_dev_profile_data_convertToJSON() failed");
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

    OpenAPI_aiot_dev_profile_data_free(dst);
    dst = OpenAPI_aiot_dev_profile_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

