
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "operator_specific_data_container.h"

char *OpenAPI_data_typeoperator_specific_data_container_ToString(OpenAPI_operator_specific_data_container_data_type_e data_type)
{
    const char *data_typeArray[] =  { "NULL", "string", "integer", "number", "boolean", "object", "array" };
    size_t sizeofArray = sizeof(data_typeArray) / sizeof(data_typeArray[0]);
    if (data_type < sizeofArray)
        return (char *)data_typeArray[data_type];
    else
        return (char *)"Unknown";
}

OpenAPI_operator_specific_data_container_data_type_e OpenAPI_data_typeoperator_specific_data_container_FromString(char* data_type)
{
    int stringToReturn = 0;
    const char *data_typeArray[] =  { "NULL", "string", "integer", "number", "boolean", "object", "array" };
    size_t sizeofArray = sizeof(data_typeArray) / sizeof(data_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(data_type, data_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}
OpenAPI_operator_specific_data_container_t *OpenAPI_operator_specific_data_container_create(
    OpenAPI_operator_specific_data_container_data_type_e data_type,
    char *data_type_definition,
    OpenAPI_operator_specific_data_container_value_t *value,
    char *supported_features,
    OpenAPI_list_t *reset_ids
)
{
    OpenAPI_operator_specific_data_container_t *operator_specific_data_container_local_var = ogs_malloc(sizeof(OpenAPI_operator_specific_data_container_t));
    ogs_assert(operator_specific_data_container_local_var);

    operator_specific_data_container_local_var->data_type = data_type;
    operator_specific_data_container_local_var->data_type_definition = data_type_definition;
    operator_specific_data_container_local_var->value = value;
    operator_specific_data_container_local_var->supported_features = supported_features;
    operator_specific_data_container_local_var->reset_ids = reset_ids;

    return operator_specific_data_container_local_var;
}

void OpenAPI_operator_specific_data_container_free(OpenAPI_operator_specific_data_container_t *operator_specific_data_container)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == operator_specific_data_container) {
        return;
    }
    if (operator_specific_data_container->data_type_definition) {
        ogs_free(operator_specific_data_container->data_type_definition);
        operator_specific_data_container->data_type_definition = NULL;
    }
    if (operator_specific_data_container->value) {
        OpenAPI_operator_specific_data_container_value_free(operator_specific_data_container->value);
        operator_specific_data_container->value = NULL;
    }
    if (operator_specific_data_container->supported_features) {
        ogs_free(operator_specific_data_container->supported_features);
        operator_specific_data_container->supported_features = NULL;
    }
    if (operator_specific_data_container->reset_ids) {
        OpenAPI_list_for_each(operator_specific_data_container->reset_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(operator_specific_data_container->reset_ids);
        operator_specific_data_container->reset_ids = NULL;
    }
    ogs_free(operator_specific_data_container);
}

cJSON *OpenAPI_operator_specific_data_container_convertToJSON(OpenAPI_operator_specific_data_container_t *operator_specific_data_container)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (operator_specific_data_container == NULL) {
        ogs_error("OpenAPI_operator_specific_data_container_convertToJSON() failed [OperatorSpecificDataContainer]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (operator_specific_data_container->data_type == OpenAPI_operator_specific_data_container_DATATYPE_NULL) {
        ogs_error("OpenAPI_operator_specific_data_container_convertToJSON() failed [data_type]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "dataType", OpenAPI_data_typeoperator_specific_data_container_ToString(operator_specific_data_container->data_type)) == NULL) {
        ogs_error("OpenAPI_operator_specific_data_container_convertToJSON() failed [data_type]");
        goto end;
    }

    if (operator_specific_data_container->data_type_definition) {
    if (cJSON_AddStringToObject(item, "dataTypeDefinition", operator_specific_data_container->data_type_definition) == NULL) {
        ogs_error("OpenAPI_operator_specific_data_container_convertToJSON() failed [data_type_definition]");
        goto end;
    }
    }

    if (!operator_specific_data_container->value) {
        ogs_error("OpenAPI_operator_specific_data_container_convertToJSON() failed [value]");
        return NULL;
    }
    cJSON *value_local_JSON = OpenAPI_operator_specific_data_container_value_convertToJSON(operator_specific_data_container->value);
    if (value_local_JSON == NULL) {
        ogs_error("OpenAPI_operator_specific_data_container_convertToJSON() failed [value]");
        goto end;
    }
    cJSON_AddItemToObject(item, "value", value_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_operator_specific_data_container_convertToJSON() failed [value]");
        goto end;
    }

    if (operator_specific_data_container->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", operator_specific_data_container->supported_features) == NULL) {
        ogs_error("OpenAPI_operator_specific_data_container_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

    if (operator_specific_data_container->reset_ids) {
    cJSON *reset_idsList = cJSON_AddArrayToObject(item, "resetIds");
    if (reset_idsList == NULL) {
        ogs_error("OpenAPI_operator_specific_data_container_convertToJSON() failed [reset_ids]");
        goto end;
    }
    OpenAPI_list_for_each(operator_specific_data_container->reset_ids, node) {
        if (cJSON_AddStringToObject(reset_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_operator_specific_data_container_convertToJSON() failed [reset_ids]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_operator_specific_data_container_t *OpenAPI_operator_specific_data_container_parseFromJSON(cJSON *operator_specific_data_containerJSON)
{
    OpenAPI_operator_specific_data_container_t *operator_specific_data_container_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *data_type = NULL;
    OpenAPI_operator_specific_data_container_data_type_e data_typeVariable = 0;
    cJSON *data_type_definition = NULL;
    cJSON *value = NULL;
    OpenAPI_operator_specific_data_container_value_t *value_local_nonprim = NULL;
    cJSON *supported_features = NULL;
    cJSON *reset_ids = NULL;
    OpenAPI_list_t *reset_idsList = NULL;
    data_type = cJSON_GetObjectItemCaseSensitive(operator_specific_data_containerJSON, "dataType");
    if (!data_type) {
        ogs_error("OpenAPI_operator_specific_data_container_parseFromJSON() failed [data_type]");
        goto end;
    }
    if (!cJSON_IsString(data_type)) {
        ogs_error("OpenAPI_operator_specific_data_container_parseFromJSON() failed [data_type]");
        goto end;
    }
    data_typeVariable = OpenAPI_data_typeoperator_specific_data_container_FromString(data_type->valuestring);

    data_type_definition = cJSON_GetObjectItemCaseSensitive(operator_specific_data_containerJSON, "dataTypeDefinition");
    if (data_type_definition) {
    if (!cJSON_IsString(data_type_definition) && !cJSON_IsNull(data_type_definition)) {
        ogs_error("OpenAPI_operator_specific_data_container_parseFromJSON() failed [data_type_definition]");
        goto end;
    }
    }

    value = cJSON_GetObjectItemCaseSensitive(operator_specific_data_containerJSON, "value");
    if (!value) {
        ogs_error("OpenAPI_operator_specific_data_container_parseFromJSON() failed [value]");
        goto end;
    }
    value_local_nonprim = OpenAPI_operator_specific_data_container_value_parseFromJSON(value);
    if (!value_local_nonprim) {
        ogs_error("OpenAPI_operator_specific_data_container_value_parseFromJSON failed [value]");
        goto end;
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(operator_specific_data_containerJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_operator_specific_data_container_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    reset_ids = cJSON_GetObjectItemCaseSensitive(operator_specific_data_containerJSON, "resetIds");
    if (reset_ids) {
        cJSON *reset_ids_local = NULL;
        if (!cJSON_IsArray(reset_ids)) {
            ogs_error("OpenAPI_operator_specific_data_container_parseFromJSON() failed [reset_ids]");
            goto end;
        }

        reset_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(reset_ids_local, reset_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(reset_ids_local)) {
                ogs_error("OpenAPI_operator_specific_data_container_parseFromJSON() failed [reset_ids]");
                goto end;
            }
            OpenAPI_list_add(reset_idsList, ogs_strdup(reset_ids_local->valuestring));
        }
    }

    operator_specific_data_container_local_var = OpenAPI_operator_specific_data_container_create (
        data_typeVariable,
        data_type_definition && !cJSON_IsNull(data_type_definition) ? ogs_strdup(data_type_definition->valuestring) : NULL,
        value_local_nonprim,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL,
        reset_ids ? reset_idsList : NULL
    );

    return operator_specific_data_container_local_var;
end:
    if (value_local_nonprim) {
        OpenAPI_operator_specific_data_container_value_free(value_local_nonprim);
        value_local_nonprim = NULL;
    }
    if (reset_idsList) {
        OpenAPI_list_for_each(reset_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(reset_idsList);
        reset_idsList = NULL;
    }
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

