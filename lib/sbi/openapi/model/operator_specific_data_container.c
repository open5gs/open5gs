
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "operator_specific_data_container.h"

OpenAPI_operator_specific_data_container_t *OpenAPI_operator_specific_data_container_create(
    char *data_type,
    char *data_type_definition,
    char *value,
    char *supported_features
)
{
    OpenAPI_operator_specific_data_container_t *operator_specific_data_container_local_var = ogs_malloc(sizeof(OpenAPI_operator_specific_data_container_t));
    ogs_assert(operator_specific_data_container_local_var);

    operator_specific_data_container_local_var->data_type = data_type;
    operator_specific_data_container_local_var->data_type_definition = data_type_definition;
    operator_specific_data_container_local_var->value = value;
    operator_specific_data_container_local_var->supported_features = supported_features;

    return operator_specific_data_container_local_var;
}

void OpenAPI_operator_specific_data_container_free(OpenAPI_operator_specific_data_container_t *operator_specific_data_container)
{
    if (NULL == operator_specific_data_container) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(operator_specific_data_container->data_type);
    ogs_free(operator_specific_data_container->data_type_definition);
    ogs_free(operator_specific_data_container->value);
    ogs_free(operator_specific_data_container->supported_features);
    ogs_free(operator_specific_data_container);
}

cJSON *OpenAPI_operator_specific_data_container_convertToJSON(OpenAPI_operator_specific_data_container_t *operator_specific_data_container)
{
    cJSON *item = NULL;

    if (operator_specific_data_container == NULL) {
        ogs_error("OpenAPI_operator_specific_data_container_convertToJSON() failed [OperatorSpecificDataContainer]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddStringToObject(item, "dataType", operator_specific_data_container->data_type) == NULL) {
        ogs_error("OpenAPI_operator_specific_data_container_convertToJSON() failed [data_type]");
        goto end;
    }

    if (operator_specific_data_container->data_type_definition) {
    if (cJSON_AddStringToObject(item, "dataTypeDefinition", operator_specific_data_container->data_type_definition) == NULL) {
        ogs_error("OpenAPI_operator_specific_data_container_convertToJSON() failed [data_type_definition]");
        goto end;
    }
    }

    if (cJSON_AddStringToObject(item, "value", operator_specific_data_container->value) == NULL) {
        ogs_error("OpenAPI_operator_specific_data_container_convertToJSON() failed [value]");
        goto end;
    }

    if (operator_specific_data_container->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", operator_specific_data_container->supported_features) == NULL) {
        ogs_error("OpenAPI_operator_specific_data_container_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_operator_specific_data_container_t *OpenAPI_operator_specific_data_container_parseFromJSON(cJSON *operator_specific_data_containerJSON)
{
    OpenAPI_operator_specific_data_container_t *operator_specific_data_container_local_var = NULL;
    cJSON *data_type = cJSON_GetObjectItemCaseSensitive(operator_specific_data_containerJSON, "dataType");
    if (!data_type) {
        ogs_error("OpenAPI_operator_specific_data_container_parseFromJSON() failed [data_type]");
        goto end;
    }

    if (!cJSON_IsString(data_type)) {
        ogs_error("OpenAPI_operator_specific_data_container_parseFromJSON() failed [data_type]");
        goto end;
    }

    cJSON *data_type_definition = cJSON_GetObjectItemCaseSensitive(operator_specific_data_containerJSON, "dataTypeDefinition");

    if (data_type_definition) {
    if (!cJSON_IsString(data_type_definition)) {
        ogs_error("OpenAPI_operator_specific_data_container_parseFromJSON() failed [data_type_definition]");
        goto end;
    }
    }

    cJSON *value = cJSON_GetObjectItemCaseSensitive(operator_specific_data_containerJSON, "value");
    if (!value) {
        ogs_error("OpenAPI_operator_specific_data_container_parseFromJSON() failed [value]");
        goto end;
    }

    if (!cJSON_IsString(value)) {
        ogs_error("OpenAPI_operator_specific_data_container_parseFromJSON() failed [value]");
        goto end;
    }

    cJSON *supported_features = cJSON_GetObjectItemCaseSensitive(operator_specific_data_containerJSON, "supportedFeatures");

    if (supported_features) {
    if (!cJSON_IsString(supported_features)) {
        ogs_error("OpenAPI_operator_specific_data_container_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    operator_specific_data_container_local_var = OpenAPI_operator_specific_data_container_create (
        ogs_strdup(data_type->valuestring),
        data_type_definition ? ogs_strdup(data_type_definition->valuestring) : NULL,
        ogs_strdup(value->valuestring),
        supported_features ? ogs_strdup(supported_features->valuestring) : NULL
    );

    return operator_specific_data_container_local_var;
end:
    return NULL;
}

OpenAPI_operator_specific_data_container_t *OpenAPI_operator_specific_data_container_copy(OpenAPI_operator_specific_data_container_t *dst, OpenAPI_operator_specific_data_container_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_operator_specific_data_container_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_operator_specific_data_container_convertToJSON() failed");
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

    OpenAPI_operator_specific_data_container_free(dst);
    dst = OpenAPI_operator_specific_data_container_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

