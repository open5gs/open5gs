
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
    OpenAPI_lnode_t *node = NULL;

    if (NULL == parameter_combination) {
        return;
    }
    if (parameter_combination->supi) {
        ogs_free(parameter_combination->supi);
        parameter_combination->supi = NULL;
    }
    if (parameter_combination->dnn) {
        ogs_free(parameter_combination->dnn);
        parameter_combination->dnn = NULL;
    }
    if (parameter_combination->snssai) {
        OpenAPI_snssai_free(parameter_combination->snssai);
        parameter_combination->snssai = NULL;
    }
    ogs_free(parameter_combination);
}

cJSON *OpenAPI_parameter_combination_convertToJSON(OpenAPI_parameter_combination_t *parameter_combination)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

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
    OpenAPI_lnode_t *node = NULL;
    cJSON *supi = NULL;
    cJSON *dnn = NULL;
    cJSON *snssai = NULL;
    OpenAPI_snssai_t *snssai_local_nonprim = NULL;
    supi = cJSON_GetObjectItemCaseSensitive(parameter_combinationJSON, "supi");
    if (supi) {
    if (!cJSON_IsString(supi) && !cJSON_IsNull(supi)) {
        ogs_error("OpenAPI_parameter_combination_parseFromJSON() failed [supi]");
        goto end;
    }
    }

    dnn = cJSON_GetObjectItemCaseSensitive(parameter_combinationJSON, "dnn");
    if (dnn) {
    if (!cJSON_IsString(dnn) && !cJSON_IsNull(dnn)) {
        ogs_error("OpenAPI_parameter_combination_parseFromJSON() failed [dnn]");
        goto end;
    }
    }

    snssai = cJSON_GetObjectItemCaseSensitive(parameter_combinationJSON, "snssai");
    if (snssai) {
    snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(snssai);
    if (!snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [snssai]");
        goto end;
    }
    }

    parameter_combination_local_var = OpenAPI_parameter_combination_create (
        supi && !cJSON_IsNull(supi) ? ogs_strdup(supi->valuestring) : NULL,
        dnn && !cJSON_IsNull(dnn) ? ogs_strdup(dnn->valuestring) : NULL,
        snssai ? snssai_local_nonprim : NULL
    );

    return parameter_combination_local_var;
end:
    if (snssai_local_nonprim) {
        OpenAPI_snssai_free(snssai_local_nonprim);
        snssai_local_nonprim = NULL;
    }
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

