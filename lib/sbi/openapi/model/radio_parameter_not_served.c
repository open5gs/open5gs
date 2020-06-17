
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "radio_parameter_not_served.h"

OpenAPI_radio_parameter_not_served_t *OpenAPI_radio_parameter_not_served_create(
    char *radio_params,
    OpenAPI_v2x_rat_type_t *rat,
    char *geographical_area,
    OpenAPI_operator_managed_t *oper_managed
    )
{
    OpenAPI_radio_parameter_not_served_t *radio_parameter_not_served_local_var = OpenAPI_malloc(sizeof(OpenAPI_radio_parameter_not_served_t));
    if (!radio_parameter_not_served_local_var) {
        return NULL;
    }
    radio_parameter_not_served_local_var->radio_params = radio_params;
    radio_parameter_not_served_local_var->rat = rat;
    radio_parameter_not_served_local_var->geographical_area = geographical_area;
    radio_parameter_not_served_local_var->oper_managed = oper_managed;

    return radio_parameter_not_served_local_var;
}

void OpenAPI_radio_parameter_not_served_free(OpenAPI_radio_parameter_not_served_t *radio_parameter_not_served)
{
    if (NULL == radio_parameter_not_served) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(radio_parameter_not_served->radio_params);
    OpenAPI_v2x_rat_type_free(radio_parameter_not_served->rat);
    ogs_free(radio_parameter_not_served->geographical_area);
    OpenAPI_operator_managed_free(radio_parameter_not_served->oper_managed);
    ogs_free(radio_parameter_not_served);
}

cJSON *OpenAPI_radio_parameter_not_served_convertToJSON(OpenAPI_radio_parameter_not_served_t *radio_parameter_not_served)
{
    cJSON *item = NULL;

    if (radio_parameter_not_served == NULL) {
        ogs_error("OpenAPI_radio_parameter_not_served_convertToJSON() failed [RadioParameterNotServed]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!radio_parameter_not_served->radio_params) {
        ogs_error("OpenAPI_radio_parameter_not_served_convertToJSON() failed [radio_params]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "radioParams", radio_parameter_not_served->radio_params) == NULL) {
        ogs_error("OpenAPI_radio_parameter_not_served_convertToJSON() failed [radio_params]");
        goto end;
    }

    if (!radio_parameter_not_served->rat) {
        ogs_error("OpenAPI_radio_parameter_not_served_convertToJSON() failed [rat]");
        goto end;
    }
    cJSON *rat_local_JSON = OpenAPI_v2x_rat_type_convertToJSON(radio_parameter_not_served->rat);
    if (rat_local_JSON == NULL) {
        ogs_error("OpenAPI_radio_parameter_not_served_convertToJSON() failed [rat]");
        goto end;
    }
    cJSON_AddItemToObject(item, "rat", rat_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_radio_parameter_not_served_convertToJSON() failed [rat]");
        goto end;
    }

    if (!radio_parameter_not_served->geographical_area) {
        ogs_error("OpenAPI_radio_parameter_not_served_convertToJSON() failed [geographical_area]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "geographicalArea", radio_parameter_not_served->geographical_area) == NULL) {
        ogs_error("OpenAPI_radio_parameter_not_served_convertToJSON() failed [geographical_area]");
        goto end;
    }

    if (!radio_parameter_not_served->oper_managed) {
        ogs_error("OpenAPI_radio_parameter_not_served_convertToJSON() failed [oper_managed]");
        goto end;
    }
    cJSON *oper_managed_local_JSON = OpenAPI_operator_managed_convertToJSON(radio_parameter_not_served->oper_managed);
    if (oper_managed_local_JSON == NULL) {
        ogs_error("OpenAPI_radio_parameter_not_served_convertToJSON() failed [oper_managed]");
        goto end;
    }
    cJSON_AddItemToObject(item, "operManaged", oper_managed_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_radio_parameter_not_served_convertToJSON() failed [oper_managed]");
        goto end;
    }

end:
    return item;
}

OpenAPI_radio_parameter_not_served_t *OpenAPI_radio_parameter_not_served_parseFromJSON(cJSON *radio_parameter_not_servedJSON)
{
    OpenAPI_radio_parameter_not_served_t *radio_parameter_not_served_local_var = NULL;
    cJSON *radio_params = cJSON_GetObjectItemCaseSensitive(radio_parameter_not_servedJSON, "radioParams");
    if (!radio_params) {
        ogs_error("OpenAPI_radio_parameter_not_served_parseFromJSON() failed [radio_params]");
        goto end;
    }


    if (!cJSON_IsString(radio_params)) {
        ogs_error("OpenAPI_radio_parameter_not_served_parseFromJSON() failed [radio_params]");
        goto end;
    }

    cJSON *rat = cJSON_GetObjectItemCaseSensitive(radio_parameter_not_servedJSON, "rat");
    if (!rat) {
        ogs_error("OpenAPI_radio_parameter_not_served_parseFromJSON() failed [rat]");
        goto end;
    }

    OpenAPI_v2x_rat_type_t *rat_local_nonprim = NULL;

    rat_local_nonprim = OpenAPI_v2x_rat_type_parseFromJSON(rat);

    cJSON *geographical_area = cJSON_GetObjectItemCaseSensitive(radio_parameter_not_servedJSON, "geographicalArea");
    if (!geographical_area) {
        ogs_error("OpenAPI_radio_parameter_not_served_parseFromJSON() failed [geographical_area]");
        goto end;
    }


    if (!cJSON_IsString(geographical_area)) {
        ogs_error("OpenAPI_radio_parameter_not_served_parseFromJSON() failed [geographical_area]");
        goto end;
    }

    cJSON *oper_managed = cJSON_GetObjectItemCaseSensitive(radio_parameter_not_servedJSON, "operManaged");
    if (!oper_managed) {
        ogs_error("OpenAPI_radio_parameter_not_served_parseFromJSON() failed [oper_managed]");
        goto end;
    }

    OpenAPI_operator_managed_t *oper_managed_local_nonprim = NULL;

    oper_managed_local_nonprim = OpenAPI_operator_managed_parseFromJSON(oper_managed);

    radio_parameter_not_served_local_var = OpenAPI_radio_parameter_not_served_create (
        ogs_strdup(radio_params->valuestring),
        rat_local_nonprim,
        ogs_strdup(geographical_area->valuestring),
        oper_managed_local_nonprim
        );

    return radio_parameter_not_served_local_var;
end:
    return NULL;
}

OpenAPI_radio_parameter_not_served_t *OpenAPI_radio_parameter_not_served_copy(OpenAPI_radio_parameter_not_served_t *dst, OpenAPI_radio_parameter_not_served_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_radio_parameter_not_served_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_radio_parameter_not_served_convertToJSON() failed");
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

    OpenAPI_radio_parameter_not_served_free(dst);
    dst = OpenAPI_radio_parameter_not_served_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

