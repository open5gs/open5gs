
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "model_5_glan_type_service.h"

OpenAPI_model_5_glan_type_service_t *OpenAPI_model_5_glan_type_service_create(
    char *internal_group_identifier
)
{
    OpenAPI_model_5_glan_type_service_t *model_5_glan_type_service_local_var = ogs_malloc(sizeof(OpenAPI_model_5_glan_type_service_t));
    ogs_assert(model_5_glan_type_service_local_var);

    model_5_glan_type_service_local_var->internal_group_identifier = internal_group_identifier;

    return model_5_glan_type_service_local_var;
}

void OpenAPI_model_5_glan_type_service_free(OpenAPI_model_5_glan_type_service_t *model_5_glan_type_service)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == model_5_glan_type_service) {
        return;
    }
    if (model_5_glan_type_service->internal_group_identifier) {
        ogs_free(model_5_glan_type_service->internal_group_identifier);
        model_5_glan_type_service->internal_group_identifier = NULL;
    }
    ogs_free(model_5_glan_type_service);
}

cJSON *OpenAPI_model_5_glan_type_service_convertToJSON(OpenAPI_model_5_glan_type_service_t *model_5_glan_type_service)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (model_5_glan_type_service == NULL) {
        ogs_error("OpenAPI_model_5_glan_type_service_convertToJSON() failed [5GLANTypeService]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (model_5_glan_type_service->internal_group_identifier) {
    if (cJSON_AddStringToObject(item, "internalGroupIdentifier", model_5_glan_type_service->internal_group_identifier) == NULL) {
        ogs_error("OpenAPI_model_5_glan_type_service_convertToJSON() failed [internal_group_identifier]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_model_5_glan_type_service_t *OpenAPI_model_5_glan_type_service_parseFromJSON(cJSON *model_5_glan_type_serviceJSON)
{
    OpenAPI_model_5_glan_type_service_t *model_5_glan_type_service_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *internal_group_identifier = NULL;
    internal_group_identifier = cJSON_GetObjectItemCaseSensitive(model_5_glan_type_serviceJSON, "internalGroupIdentifier");
    if (internal_group_identifier) {
    if (!cJSON_IsString(internal_group_identifier) && !cJSON_IsNull(internal_group_identifier)) {
        ogs_error("OpenAPI_model_5_glan_type_service_parseFromJSON() failed [internal_group_identifier]");
        goto end;
    }
    }

    model_5_glan_type_service_local_var = OpenAPI_model_5_glan_type_service_create (
        internal_group_identifier && !cJSON_IsNull(internal_group_identifier) ? ogs_strdup(internal_group_identifier->valuestring) : NULL
    );

    return model_5_glan_type_service_local_var;
end:
    return NULL;
}

OpenAPI_model_5_glan_type_service_t *OpenAPI_model_5_glan_type_service_copy(OpenAPI_model_5_glan_type_service_t *dst, OpenAPI_model_5_glan_type_service_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_model_5_glan_type_service_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_model_5_glan_type_service_convertToJSON() failed");
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

    OpenAPI_model_5_glan_type_service_free(dst);
    dst = OpenAPI_model_5_glan_type_service_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

