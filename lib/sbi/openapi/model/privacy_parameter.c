
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "privacy_parameter.h"

OpenAPI_privacy_parameter_t *OpenAPI_privacy_parameter_create(
    OpenAPI_list_t *ser_ids,
    char *geographical_area,
    int timer
    )
{
    OpenAPI_privacy_parameter_t *privacy_parameter_local_var = OpenAPI_malloc(sizeof(OpenAPI_privacy_parameter_t));
    if (!privacy_parameter_local_var) {
        return NULL;
    }
    privacy_parameter_local_var->ser_ids = ser_ids;
    privacy_parameter_local_var->geographical_area = geographical_area;
    privacy_parameter_local_var->timer = timer;

    return privacy_parameter_local_var;
}

void OpenAPI_privacy_parameter_free(OpenAPI_privacy_parameter_t *privacy_parameter)
{
    if (NULL == privacy_parameter) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(privacy_parameter->ser_ids, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(privacy_parameter->ser_ids);
    ogs_free(privacy_parameter->geographical_area);
    ogs_free(privacy_parameter);
}

cJSON *OpenAPI_privacy_parameter_convertToJSON(OpenAPI_privacy_parameter_t *privacy_parameter)
{
    cJSON *item = NULL;

    if (privacy_parameter == NULL) {
        ogs_error("OpenAPI_privacy_parameter_convertToJSON() failed [PrivacyParameter]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!privacy_parameter->ser_ids) {
        ogs_error("OpenAPI_privacy_parameter_convertToJSON() failed [ser_ids]");
        goto end;
    }
    cJSON *ser_ids = cJSON_AddArrayToObject(item, "serIds");
    if (ser_ids == NULL) {
        ogs_error("OpenAPI_privacy_parameter_convertToJSON() failed [ser_ids]");
        goto end;
    }

    OpenAPI_lnode_t *ser_ids_node;
    OpenAPI_list_for_each(privacy_parameter->ser_ids, ser_ids_node)  {
        if (cJSON_AddStringToObject(ser_ids, "", (char*)ser_ids_node->data) == NULL) {
            ogs_error("OpenAPI_privacy_parameter_convertToJSON() failed [ser_ids]");
            goto end;
        }
    }

    if (!privacy_parameter->geographical_area) {
        ogs_error("OpenAPI_privacy_parameter_convertToJSON() failed [geographical_area]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "geographicalArea", privacy_parameter->geographical_area) == NULL) {
        ogs_error("OpenAPI_privacy_parameter_convertToJSON() failed [geographical_area]");
        goto end;
    }

    if (!privacy_parameter->timer) {
        ogs_error("OpenAPI_privacy_parameter_convertToJSON() failed [timer]");
        goto end;
    }
    if (cJSON_AddNumberToObject(item, "timer", privacy_parameter->timer) == NULL) {
        ogs_error("OpenAPI_privacy_parameter_convertToJSON() failed [timer]");
        goto end;
    }

end:
    return item;
}

OpenAPI_privacy_parameter_t *OpenAPI_privacy_parameter_parseFromJSON(cJSON *privacy_parameterJSON)
{
    OpenAPI_privacy_parameter_t *privacy_parameter_local_var = NULL;
    cJSON *ser_ids = cJSON_GetObjectItemCaseSensitive(privacy_parameterJSON, "serIds");
    if (!ser_ids) {
        ogs_error("OpenAPI_privacy_parameter_parseFromJSON() failed [ser_ids]");
        goto end;
    }

    OpenAPI_list_t *ser_idsList;

    cJSON *ser_ids_local;
    if (!cJSON_IsArray(ser_ids)) {
        ogs_error("OpenAPI_privacy_parameter_parseFromJSON() failed [ser_ids]");
        goto end;
    }
    ser_idsList = OpenAPI_list_create();

    cJSON_ArrayForEach(ser_ids_local, ser_ids) {
        if (!cJSON_IsString(ser_ids_local)) {
            ogs_error("OpenAPI_privacy_parameter_parseFromJSON() failed [ser_ids]");
            goto end;
        }
        OpenAPI_list_add(ser_idsList, ogs_strdup(ser_ids_local->valuestring));
    }

    cJSON *geographical_area = cJSON_GetObjectItemCaseSensitive(privacy_parameterJSON, "geographicalArea");
    if (!geographical_area) {
        ogs_error("OpenAPI_privacy_parameter_parseFromJSON() failed [geographical_area]");
        goto end;
    }


    if (!cJSON_IsString(geographical_area)) {
        ogs_error("OpenAPI_privacy_parameter_parseFromJSON() failed [geographical_area]");
        goto end;
    }

    cJSON *timer = cJSON_GetObjectItemCaseSensitive(privacy_parameterJSON, "timer");
    if (!timer) {
        ogs_error("OpenAPI_privacy_parameter_parseFromJSON() failed [timer]");
        goto end;
    }


    if (!cJSON_IsNumber(timer)) {
        ogs_error("OpenAPI_privacy_parameter_parseFromJSON() failed [timer]");
        goto end;
    }

    privacy_parameter_local_var = OpenAPI_privacy_parameter_create (
        ser_idsList,
        ogs_strdup(geographical_area->valuestring),
        timer->valuedouble
        );

    return privacy_parameter_local_var;
end:
    return NULL;
}

OpenAPI_privacy_parameter_t *OpenAPI_privacy_parameter_copy(OpenAPI_privacy_parameter_t *dst, OpenAPI_privacy_parameter_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_privacy_parameter_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_privacy_parameter_convertToJSON() failed");
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

    OpenAPI_privacy_parameter_free(dst);
    dst = OpenAPI_privacy_parameter_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

