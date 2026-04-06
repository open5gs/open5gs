
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "smsf_registration_modification.h"

OpenAPI_smsf_registration_modification_t *OpenAPI_smsf_registration_modification_create(
    char *smsf_instance_id,
    char *smsf_set_id,
    bool is_ue_memory_available_ind,
    int ue_memory_available_ind
)
{
    OpenAPI_smsf_registration_modification_t *smsf_registration_modification_local_var = ogs_malloc(sizeof(OpenAPI_smsf_registration_modification_t));
    ogs_assert(smsf_registration_modification_local_var);

    smsf_registration_modification_local_var->smsf_instance_id = smsf_instance_id;
    smsf_registration_modification_local_var->smsf_set_id = smsf_set_id;
    smsf_registration_modification_local_var->is_ue_memory_available_ind = is_ue_memory_available_ind;
    smsf_registration_modification_local_var->ue_memory_available_ind = ue_memory_available_ind;

    return smsf_registration_modification_local_var;
}

void OpenAPI_smsf_registration_modification_free(OpenAPI_smsf_registration_modification_t *smsf_registration_modification)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == smsf_registration_modification) {
        return;
    }
    if (smsf_registration_modification->smsf_instance_id) {
        ogs_free(smsf_registration_modification->smsf_instance_id);
        smsf_registration_modification->smsf_instance_id = NULL;
    }
    if (smsf_registration_modification->smsf_set_id) {
        ogs_free(smsf_registration_modification->smsf_set_id);
        smsf_registration_modification->smsf_set_id = NULL;
    }
    ogs_free(smsf_registration_modification);
}

cJSON *OpenAPI_smsf_registration_modification_convertToJSON(OpenAPI_smsf_registration_modification_t *smsf_registration_modification)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (smsf_registration_modification == NULL) {
        ogs_error("OpenAPI_smsf_registration_modification_convertToJSON() failed [SmsfRegistrationModification]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!smsf_registration_modification->smsf_instance_id) {
        ogs_error("OpenAPI_smsf_registration_modification_convertToJSON() failed [smsf_instance_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "smsfInstanceId", smsf_registration_modification->smsf_instance_id) == NULL) {
        ogs_error("OpenAPI_smsf_registration_modification_convertToJSON() failed [smsf_instance_id]");
        goto end;
    }

    if (smsf_registration_modification->smsf_set_id) {
    if (cJSON_AddStringToObject(item, "smsfSetId", smsf_registration_modification->smsf_set_id) == NULL) {
        ogs_error("OpenAPI_smsf_registration_modification_convertToJSON() failed [smsf_set_id]");
        goto end;
    }
    }

    if (smsf_registration_modification->is_ue_memory_available_ind) {
    if (cJSON_AddBoolToObject(item, "ueMemoryAvailableInd", smsf_registration_modification->ue_memory_available_ind) == NULL) {
        ogs_error("OpenAPI_smsf_registration_modification_convertToJSON() failed [ue_memory_available_ind]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_smsf_registration_modification_t *OpenAPI_smsf_registration_modification_parseFromJSON(cJSON *smsf_registration_modificationJSON)
{
    OpenAPI_smsf_registration_modification_t *smsf_registration_modification_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *smsf_instance_id = NULL;
    cJSON *smsf_set_id = NULL;
    cJSON *ue_memory_available_ind = NULL;
    smsf_instance_id = cJSON_GetObjectItemCaseSensitive(smsf_registration_modificationJSON, "smsfInstanceId");
    if (!smsf_instance_id) {
        ogs_error("OpenAPI_smsf_registration_modification_parseFromJSON() failed [smsf_instance_id]");
        goto end;
    }
    if (!cJSON_IsString(smsf_instance_id)) {
        ogs_error("OpenAPI_smsf_registration_modification_parseFromJSON() failed [smsf_instance_id]");
        goto end;
    }

    smsf_set_id = cJSON_GetObjectItemCaseSensitive(smsf_registration_modificationJSON, "smsfSetId");
    if (smsf_set_id) {
    if (!cJSON_IsString(smsf_set_id) && !cJSON_IsNull(smsf_set_id)) {
        ogs_error("OpenAPI_smsf_registration_modification_parseFromJSON() failed [smsf_set_id]");
        goto end;
    }
    }

    ue_memory_available_ind = cJSON_GetObjectItemCaseSensitive(smsf_registration_modificationJSON, "ueMemoryAvailableInd");
    if (ue_memory_available_ind) {
    if (!cJSON_IsBool(ue_memory_available_ind)) {
        ogs_error("OpenAPI_smsf_registration_modification_parseFromJSON() failed [ue_memory_available_ind]");
        goto end;
    }
    }

    smsf_registration_modification_local_var = OpenAPI_smsf_registration_modification_create (
        ogs_strdup(smsf_instance_id->valuestring),
        smsf_set_id && !cJSON_IsNull(smsf_set_id) ? ogs_strdup(smsf_set_id->valuestring) : NULL,
        ue_memory_available_ind ? true : false,
        ue_memory_available_ind ? ue_memory_available_ind->valueint : 0
    );

    return smsf_registration_modification_local_var;
end:
    return NULL;
}

OpenAPI_smsf_registration_modification_t *OpenAPI_smsf_registration_modification_copy(OpenAPI_smsf_registration_modification_t *dst, OpenAPI_smsf_registration_modification_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_smsf_registration_modification_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_smsf_registration_modification_convertToJSON() failed");
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

    OpenAPI_smsf_registration_modification_free(dst);
    dst = OpenAPI_smsf_registration_modification_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

