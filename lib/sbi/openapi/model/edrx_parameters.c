
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "edrx_parameters.h"

OpenAPI_edrx_parameters_t *OpenAPI_edrx_parameters_create(
    OpenAPI_rat_type_t *rat_type,
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
    OpenAPI_rat_type_free(edrx_parameters->rat_type);
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
    cJSON *rat_type_local_JSON = OpenAPI_rat_type_convertToJSON(edrx_parameters->rat_type);
    if (rat_type_local_JSON == NULL) {
        ogs_error("OpenAPI_edrx_parameters_convertToJSON() failed [rat_type]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ratType", rat_type_local_JSON);
    if (item->child == NULL) {
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

    OpenAPI_rat_type_t *rat_type_local_nonprim = NULL;

    rat_type_local_nonprim = OpenAPI_rat_type_parseFromJSON(rat_type);

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
        rat_type_local_nonprim,
        ogs_strdup(edrx_value->valuestring)
        );

    return edrx_parameters_local_var;
end:
    return NULL;
}

