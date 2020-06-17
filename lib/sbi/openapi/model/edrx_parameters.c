
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "edrx_parameters.h"

OpenAPI_edrx_parameters_t *OpenAPI_edrx_parameters_create(
    OpenAPI_rat_type_e rat_type,
    char *edrx_value
    )
{
    OpenAPI_edrx_parameters_t *edrx_parameters_local_var = OpenAPI_malloc(sizeof(OpenAPI_edrx_parameters_t));
    if (!edrx_parameters_local_var) {
        return NULL;
    }
    edrx_parameters_local_var->rat_type = rat_type;
    edrx_parameters_local_var->edrx_value = edrx_value;

    return edrx_parameters_local_var;
}

void OpenAPI_edrx_parameters_free(OpenAPI_edrx_parameters_t *edrx_parameters)
{
    if (NULL == edrx_parameters) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(edrx_parameters->edrx_value);
    ogs_free(edrx_parameters);
}

cJSON *OpenAPI_edrx_parameters_convertToJSON(OpenAPI_edrx_parameters_t *edrx_parameters)
{
    cJSON *item = NULL;

    if (edrx_parameters == NULL) {
        ogs_error("OpenAPI_edrx_parameters_convertToJSON() failed [EdrxParameters]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!edrx_parameters->rat_type) {
        ogs_error("OpenAPI_edrx_parameters_convertToJSON() failed [rat_type]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "ratType", OpenAPI_rat_type_ToString(edrx_parameters->rat_type)) == NULL) {
        ogs_error("OpenAPI_edrx_parameters_convertToJSON() failed [rat_type]");
        goto end;
    }

    if (!edrx_parameters->edrx_value) {
        ogs_error("OpenAPI_edrx_parameters_convertToJSON() failed [edrx_value]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "edrxValue", edrx_parameters->edrx_value) == NULL) {
        ogs_error("OpenAPI_edrx_parameters_convertToJSON() failed [edrx_value]");
        goto end;
    }

end:
    return item;
}

OpenAPI_edrx_parameters_t *OpenAPI_edrx_parameters_parseFromJSON(cJSON *edrx_parametersJSON)
{
    OpenAPI_edrx_parameters_t *edrx_parameters_local_var = NULL;
    cJSON *rat_type = cJSON_GetObjectItemCaseSensitive(edrx_parametersJSON, "ratType");
    if (!rat_type) {
        ogs_error("OpenAPI_edrx_parameters_parseFromJSON() failed [rat_type]");
        goto end;
    }

    OpenAPI_rat_type_e rat_typeVariable;

    if (!cJSON_IsString(rat_type)) {
        ogs_error("OpenAPI_edrx_parameters_parseFromJSON() failed [rat_type]");
        goto end;
    }
    rat_typeVariable = OpenAPI_rat_type_FromString(rat_type->valuestring);

    cJSON *edrx_value = cJSON_GetObjectItemCaseSensitive(edrx_parametersJSON, "edrxValue");
    if (!edrx_value) {
        ogs_error("OpenAPI_edrx_parameters_parseFromJSON() failed [edrx_value]");
        goto end;
    }


    if (!cJSON_IsString(edrx_value)) {
        ogs_error("OpenAPI_edrx_parameters_parseFromJSON() failed [edrx_value]");
        goto end;
    }

    edrx_parameters_local_var = OpenAPI_edrx_parameters_create (
        rat_typeVariable,
        ogs_strdup(edrx_value->valuestring)
        );

    return edrx_parameters_local_var;
end:
    return NULL;
}

OpenAPI_edrx_parameters_t *OpenAPI_edrx_parameters_copy(OpenAPI_edrx_parameters_t *dst, OpenAPI_edrx_parameters_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_edrx_parameters_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_edrx_parameters_convertToJSON() failed");
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

    OpenAPI_edrx_parameters_free(dst);
    dst = OpenAPI_edrx_parameters_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

