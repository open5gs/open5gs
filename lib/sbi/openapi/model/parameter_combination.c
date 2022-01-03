
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "parameter_combination.h"

OpenAPI_parameter_combination_t *OpenAPI_parameter_combination_create(
    char *supi,
    char *dnn,
    OpenAPI_snssai_t *snssai
)
{
    OpenAPI_parameter_combination_t *parameter_combination_local_var = ogs_malloc(sizeof(OpenAPI_parameter_combination_t));
    ogs_assert(parameter_combination_local_var);

    parameter_combination_local_var->supi = supi;
    parameter_combination_local_var->dnn = dnn;
    parameter_combination_local_var->snssai = snssai;

    return parameter_combination_local_var;
}

void OpenAPI_parameter_combination_free(OpenAPI_parameter_combination_t *parameter_combination)
{
    if (NULL == parameter_combination) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(parameter_combination->supi);
    ogs_free(parameter_combination->dnn);
    OpenAPI_snssai_free(parameter_combination->snssai);
    ogs_free(parameter_combination);
}

cJSON *OpenAPI_parameter_combination_convertToJSON(OpenAPI_parameter_combination_t *parameter_combination)
{
    cJSON *item = NULL;

    if (parameter_combination == NULL) {
        ogs_error("OpenAPI_parameter_combination_convertToJSON() failed [ParameterCombination]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (parameter_combination->supi) {
    if (cJSON_AddStringToObject(item, "supi", parameter_combination->supi) == NULL) {
        ogs_error("OpenAPI_parameter_combination_convertToJSON() failed [supi]");
        goto end;
    }
    }

    if (parameter_combination->dnn) {
    if (cJSON_AddStringToObject(item, "dnn", parameter_combination->dnn) == NULL) {
        ogs_error("OpenAPI_parameter_combination_convertToJSON() failed [dnn]");
        goto end;
    }
    }

    if (parameter_combination->snssai) {
    cJSON *snssai_local_JSON = OpenAPI_snssai_convertToJSON(parameter_combination->snssai);
    if (snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_parameter_combination_convertToJSON() failed [snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "snssai", snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_parameter_combination_convertToJSON() failed [snssai]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_parameter_combination_t *OpenAPI_parameter_combination_parseFromJSON(cJSON *parameter_combinationJSON)
{
    OpenAPI_parameter_combination_t *parameter_combination_local_var = NULL;
    cJSON *supi = cJSON_GetObjectItemCaseSensitive(parameter_combinationJSON, "supi");

    if (supi) {
    if (!cJSON_IsString(supi)) {
        ogs_error("OpenAPI_parameter_combination_parseFromJSON() failed [supi]");
        goto end;
    }
    }

    cJSON *dnn = cJSON_GetObjectItemCaseSensitive(parameter_combinationJSON, "dnn");

    if (dnn) {
    if (!cJSON_IsString(dnn)) {
        ogs_error("OpenAPI_parameter_combination_parseFromJSON() failed [dnn]");
        goto end;
    }
    }

    cJSON *snssai = cJSON_GetObjectItemCaseSensitive(parameter_combinationJSON, "snssai");

    OpenAPI_snssai_t *snssai_local_nonprim = NULL;
    if (snssai) {
    snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(snssai);
    }

    parameter_combination_local_var = OpenAPI_parameter_combination_create (
        supi ? ogs_strdup(supi->valuestring) : NULL,
        dnn ? ogs_strdup(dnn->valuestring) : NULL,
        snssai ? snssai_local_nonprim : NULL
    );

    return parameter_combination_local_var;
end:
    return NULL;
}

OpenAPI_parameter_combination_t *OpenAPI_parameter_combination_copy(OpenAPI_parameter_combination_t *dst, OpenAPI_parameter_combination_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_parameter_combination_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_parameter_combination_convertToJSON() failed");
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

    OpenAPI_parameter_combination_free(dst);
    dst = OpenAPI_parameter_combination_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

