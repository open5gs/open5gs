
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "parameter_processing_instruction.h"

OpenAPI_parameter_processing_instruction_t *OpenAPI_parameter_processing_instruction_create(
    char *name,
    OpenAPI_list_t *values,
    OpenAPI_list_t *sum_attrs,
    OpenAPI_aggregation_level_e aggr_level,
    OpenAPI_list_t *supis,
    bool is_temporal_aggr_level,
    int temporal_aggr_level,
    OpenAPI_list_t *areas
)
{
    OpenAPI_parameter_processing_instruction_t *parameter_processing_instruction_local_var = ogs_malloc(sizeof(OpenAPI_parameter_processing_instruction_t));
    ogs_assert(parameter_processing_instruction_local_var);

    parameter_processing_instruction_local_var->name = name;
    parameter_processing_instruction_local_var->values = values;
    parameter_processing_instruction_local_var->sum_attrs = sum_attrs;
    parameter_processing_instruction_local_var->aggr_level = aggr_level;
    parameter_processing_instruction_local_var->supis = supis;
    parameter_processing_instruction_local_var->is_temporal_aggr_level = is_temporal_aggr_level;
    parameter_processing_instruction_local_var->temporal_aggr_level = temporal_aggr_level;
    parameter_processing_instruction_local_var->areas = areas;

    return parameter_processing_instruction_local_var;
}

void OpenAPI_parameter_processing_instruction_free(OpenAPI_parameter_processing_instruction_t *parameter_processing_instruction)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == parameter_processing_instruction) {
        return;
    }
    if (parameter_processing_instruction->name) {
        ogs_free(parameter_processing_instruction->name);
        parameter_processing_instruction->name = NULL;
    }
    if (parameter_processing_instruction->values) {
        OpenAPI_list_for_each(parameter_processing_instruction->values, node) {
            OpenAPI_any_type_free(node->data);
        }
        OpenAPI_list_free(parameter_processing_instruction->values);
        parameter_processing_instruction->values = NULL;
    }
    if (parameter_processing_instruction->sum_attrs) {
        OpenAPI_list_free(parameter_processing_instruction->sum_attrs);
        parameter_processing_instruction->sum_attrs = NULL;
    }
    if (parameter_processing_instruction->supis) {
        OpenAPI_list_for_each(parameter_processing_instruction->supis, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(parameter_processing_instruction->supis);
        parameter_processing_instruction->supis = NULL;
    }
    if (parameter_processing_instruction->areas) {
        OpenAPI_list_for_each(parameter_processing_instruction->areas, node) {
            OpenAPI_network_area_info_free(node->data);
        }
        OpenAPI_list_free(parameter_processing_instruction->areas);
        parameter_processing_instruction->areas = NULL;
    }
    ogs_free(parameter_processing_instruction);
}

cJSON *OpenAPI_parameter_processing_instruction_convertToJSON(OpenAPI_parameter_processing_instruction_t *parameter_processing_instruction)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (parameter_processing_instruction == NULL) {
        ogs_error("OpenAPI_parameter_processing_instruction_convertToJSON() failed [ParameterProcessingInstruction]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!parameter_processing_instruction->name) {
        ogs_error("OpenAPI_parameter_processing_instruction_convertToJSON() failed [name]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "name", parameter_processing_instruction->name) == NULL) {
        ogs_error("OpenAPI_parameter_processing_instruction_convertToJSON() failed [name]");
        goto end;
    }

    if (!parameter_processing_instruction->values) {
        ogs_error("OpenAPI_parameter_processing_instruction_convertToJSON() failed [values]");
        return NULL;
    }
    cJSON *valuesList = cJSON_AddArrayToObject(item, "values");
    if (valuesList == NULL) {
        ogs_error("OpenAPI_parameter_processing_instruction_convertToJSON() failed [values]");
        goto end;
    }
    OpenAPI_list_for_each(parameter_processing_instruction->values, node) {
        cJSON *itemLocal = node->data ? cJSON_Duplicate(node->data, 1) : cJSON_CreateNull();
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_parameter_processing_instruction_convertToJSON() failed [values]");
            goto end;
        }
        cJSON_AddItemToArray(valuesList, itemLocal);
    }

    if (parameter_processing_instruction->sum_attrs == OpenAPI_summarization_attribute_NULL) {
        ogs_error("OpenAPI_parameter_processing_instruction_convertToJSON() failed [sum_attrs]");
        return NULL;
    }
    cJSON *sum_attrsList = cJSON_AddArrayToObject(item, "sumAttrs");
    if (sum_attrsList == NULL) {
        ogs_error("OpenAPI_parameter_processing_instruction_convertToJSON() failed [sum_attrs]");
        goto end;
    }
    OpenAPI_list_for_each(parameter_processing_instruction->sum_attrs, node) {
        if (cJSON_AddStringToObject(sum_attrsList, "", OpenAPI_summarization_attribute_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_parameter_processing_instruction_convertToJSON() failed [sum_attrs]");
            goto end;
        }
    }

    if (parameter_processing_instruction->aggr_level != OpenAPI_aggregation_level_NULL) {
    if (cJSON_AddStringToObject(item, "aggrLevel", OpenAPI_aggregation_level_ToString(parameter_processing_instruction->aggr_level)) == NULL) {
        ogs_error("OpenAPI_parameter_processing_instruction_convertToJSON() failed [aggr_level]");
        goto end;
    }
    }

    if (parameter_processing_instruction->supis) {
    cJSON *supisList = cJSON_AddArrayToObject(item, "supis");
    if (supisList == NULL) {
        ogs_error("OpenAPI_parameter_processing_instruction_convertToJSON() failed [supis]");
        goto end;
    }
    OpenAPI_list_for_each(parameter_processing_instruction->supis, node) {
        if (cJSON_AddStringToObject(supisList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_parameter_processing_instruction_convertToJSON() failed [supis]");
            goto end;
        }
    }
    }

    if (parameter_processing_instruction->is_temporal_aggr_level) {
    if (cJSON_AddNumberToObject(item, "temporalAggrLevel", parameter_processing_instruction->temporal_aggr_level) == NULL) {
        ogs_error("OpenAPI_parameter_processing_instruction_convertToJSON() failed [temporal_aggr_level]");
        goto end;
    }
    }

    if (parameter_processing_instruction->areas) {
    cJSON *areasList = cJSON_AddArrayToObject(item, "areas");
    if (areasList == NULL) {
        ogs_error("OpenAPI_parameter_processing_instruction_convertToJSON() failed [areas]");
        goto end;
    }
    OpenAPI_list_for_each(parameter_processing_instruction->areas, node) {
        cJSON *itemLocal = OpenAPI_network_area_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_parameter_processing_instruction_convertToJSON() failed [areas]");
            goto end;
        }
        cJSON_AddItemToArray(areasList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_parameter_processing_instruction_t *OpenAPI_parameter_processing_instruction_parseFromJSON(cJSON *parameter_processing_instructionJSON)
{
    OpenAPI_parameter_processing_instruction_t *parameter_processing_instruction_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *name = NULL;
    cJSON *values = NULL;
    OpenAPI_list_t *valuesList = NULL;
    cJSON *sum_attrs = NULL;
    OpenAPI_list_t *sum_attrsList = NULL;
    cJSON *aggr_level = NULL;
    OpenAPI_aggregation_level_e aggr_levelVariable = 0;
    cJSON *supis = NULL;
    OpenAPI_list_t *supisList = NULL;
    cJSON *temporal_aggr_level = NULL;
    cJSON *areas = NULL;
    OpenAPI_list_t *areasList = NULL;
    name = cJSON_GetObjectItemCaseSensitive(parameter_processing_instructionJSON, "name");
    if (!name) {
        ogs_error("OpenAPI_parameter_processing_instruction_parseFromJSON() failed [name]");
        goto end;
    }
    if (!cJSON_IsString(name)) {
        ogs_error("OpenAPI_parameter_processing_instruction_parseFromJSON() failed [name]");
        goto end;
    }

    values = cJSON_GetObjectItemCaseSensitive(parameter_processing_instructionJSON, "values");
    if (!values) {
        ogs_error("OpenAPI_parameter_processing_instruction_parseFromJSON() failed [values]");
        goto end;
    }
        cJSON *values_local = NULL;
        if (!cJSON_IsArray(values)) {
            ogs_error("OpenAPI_parameter_processing_instruction_parseFromJSON() failed [values]");
            goto end;
        }

        valuesList = OpenAPI_list_create();

        cJSON_ArrayForEach(values_local, values) {
            if (!cJSON_IsObject(values_local)) {
                ogs_error("OpenAPI_parameter_processing_instruction_parseFromJSON() failed [values]");
                goto end;
            }
            cJSON *valuesItem = cJSON_Duplicate(values_local, 1);
            if (!valuesItem) {
                ogs_error("No valuesItem");
                goto end;
            }
            OpenAPI_list_add(valuesList, valuesItem);
        }

    sum_attrs = cJSON_GetObjectItemCaseSensitive(parameter_processing_instructionJSON, "sumAttrs");
    if (!sum_attrs) {
        ogs_error("OpenAPI_parameter_processing_instruction_parseFromJSON() failed [sum_attrs]");
        goto end;
    }
        cJSON *sum_attrs_local = NULL;
        if (!cJSON_IsArray(sum_attrs)) {
            ogs_error("OpenAPI_parameter_processing_instruction_parseFromJSON() failed [sum_attrs]");
            goto end;
        }

        sum_attrsList = OpenAPI_list_create();

        cJSON_ArrayForEach(sum_attrs_local, sum_attrs) {
            OpenAPI_summarization_attribute_e localEnum = OpenAPI_summarization_attribute_NULL;
            if (!cJSON_IsString(sum_attrs_local)) {
                ogs_error("OpenAPI_parameter_processing_instruction_parseFromJSON() failed [sum_attrs]");
                goto end;
            }
            localEnum = OpenAPI_summarization_attribute_FromString(sum_attrs_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"sum_attrs\" is not supported. Ignoring it ...",
                         sum_attrs_local->valuestring);
            } else {
                OpenAPI_list_add(sum_attrsList, (void *)localEnum);
            }
        }
        if (sum_attrsList->count == 0) {
            ogs_error("OpenAPI_parameter_processing_instruction_parseFromJSON() failed: Expected sum_attrsList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }

    aggr_level = cJSON_GetObjectItemCaseSensitive(parameter_processing_instructionJSON, "aggrLevel");
    if (aggr_level) {
    if (!cJSON_IsString(aggr_level)) {
        ogs_error("OpenAPI_parameter_processing_instruction_parseFromJSON() failed [aggr_level]");
        goto end;
    }
    aggr_levelVariable = OpenAPI_aggregation_level_FromString(aggr_level->valuestring);
    }

    supis = cJSON_GetObjectItemCaseSensitive(parameter_processing_instructionJSON, "supis");
    if (supis) {
        cJSON *supis_local = NULL;
        if (!cJSON_IsArray(supis)) {
            ogs_error("OpenAPI_parameter_processing_instruction_parseFromJSON() failed [supis]");
            goto end;
        }

        supisList = OpenAPI_list_create();

        cJSON_ArrayForEach(supis_local, supis) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(supis_local)) {
                ogs_error("OpenAPI_parameter_processing_instruction_parseFromJSON() failed [supis]");
                goto end;
            }
            OpenAPI_list_add(supisList, ogs_strdup(supis_local->valuestring));
        }
    }

    temporal_aggr_level = cJSON_GetObjectItemCaseSensitive(parameter_processing_instructionJSON, "temporalAggrLevel");
    if (temporal_aggr_level) {
    if (!cJSON_IsNumber(temporal_aggr_level)) {
        ogs_error("OpenAPI_parameter_processing_instruction_parseFromJSON() failed [temporal_aggr_level]");
        goto end;
    }
    }

    areas = cJSON_GetObjectItemCaseSensitive(parameter_processing_instructionJSON, "areas");
    if (areas) {
        cJSON *areas_local = NULL;
        if (!cJSON_IsArray(areas)) {
            ogs_error("OpenAPI_parameter_processing_instruction_parseFromJSON() failed [areas]");
            goto end;
        }

        areasList = OpenAPI_list_create();

        cJSON_ArrayForEach(areas_local, areas) {
            if (!cJSON_IsObject(areas_local)) {
                ogs_error("OpenAPI_parameter_processing_instruction_parseFromJSON() failed [areas]");
                goto end;
            }
            OpenAPI_network_area_info_t *areasItem = OpenAPI_network_area_info_parseFromJSON(areas_local);
            if (!areasItem) {
                ogs_error("No areasItem");
                goto end;
            }
            OpenAPI_list_add(areasList, areasItem);
        }
    }

    parameter_processing_instruction_local_var = OpenAPI_parameter_processing_instruction_create (
        ogs_strdup(name->valuestring),
        valuesList,
        sum_attrsList,
        aggr_level ? aggr_levelVariable : 0,
        supis ? supisList : NULL,
        temporal_aggr_level ? true : false,
        temporal_aggr_level ? temporal_aggr_level->valuedouble : 0,
        areas ? areasList : NULL
    );

    return parameter_processing_instruction_local_var;
end:
    if (valuesList) {
        OpenAPI_list_for_each(valuesList, node) {
            OpenAPI_any_type_free(node->data);
        }
        OpenAPI_list_free(valuesList);
        valuesList = NULL;
    }
    if (sum_attrsList) {
        OpenAPI_list_free(sum_attrsList);
        sum_attrsList = NULL;
    }
    if (supisList) {
        OpenAPI_list_for_each(supisList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(supisList);
        supisList = NULL;
    }
    if (areasList) {
        OpenAPI_list_for_each(areasList, node) {
            OpenAPI_network_area_info_free(node->data);
        }
        OpenAPI_list_free(areasList);
        areasList = NULL;
    }
    return NULL;
}

OpenAPI_parameter_processing_instruction_t *OpenAPI_parameter_processing_instruction_copy(OpenAPI_parameter_processing_instruction_t *dst, OpenAPI_parameter_processing_instruction_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_parameter_processing_instruction_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_parameter_processing_instruction_convertToJSON() failed");
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

    OpenAPI_parameter_processing_instruction_free(dst);
    dst = OpenAPI_parameter_processing_instruction_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

