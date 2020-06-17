
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "service_id_to_frequency.h"

OpenAPI_service_id_to_frequency_t *OpenAPI_service_id_to_frequency_create(
    OpenAPI_list_t *ser_ids,
    double frequency,
    char *geographical_area
    )
{
    OpenAPI_service_id_to_frequency_t *service_id_to_frequency_local_var = OpenAPI_malloc(sizeof(OpenAPI_service_id_to_frequency_t));
    if (!service_id_to_frequency_local_var) {
        return NULL;
    }
    service_id_to_frequency_local_var->ser_ids = ser_ids;
    service_id_to_frequency_local_var->frequency = frequency;
    service_id_to_frequency_local_var->geographical_area = geographical_area;

    return service_id_to_frequency_local_var;
}

void OpenAPI_service_id_to_frequency_free(OpenAPI_service_id_to_frequency_t *service_id_to_frequency)
{
    if (NULL == service_id_to_frequency) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(service_id_to_frequency->ser_ids, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(service_id_to_frequency->ser_ids);
    ogs_free(service_id_to_frequency->geographical_area);
    ogs_free(service_id_to_frequency);
}

cJSON *OpenAPI_service_id_to_frequency_convertToJSON(OpenAPI_service_id_to_frequency_t *service_id_to_frequency)
{
    cJSON *item = NULL;

    if (service_id_to_frequency == NULL) {
        ogs_error("OpenAPI_service_id_to_frequency_convertToJSON() failed [ServiceIdToFrequency]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!service_id_to_frequency->ser_ids) {
        ogs_error("OpenAPI_service_id_to_frequency_convertToJSON() failed [ser_ids]");
        goto end;
    }
    cJSON *ser_ids = cJSON_AddArrayToObject(item, "serIds");
    if (ser_ids == NULL) {
        ogs_error("OpenAPI_service_id_to_frequency_convertToJSON() failed [ser_ids]");
        goto end;
    }

    OpenAPI_lnode_t *ser_ids_node;
    OpenAPI_list_for_each(service_id_to_frequency->ser_ids, ser_ids_node)  {
        if (cJSON_AddStringToObject(ser_ids, "", (char*)ser_ids_node->data) == NULL) {
            ogs_error("OpenAPI_service_id_to_frequency_convertToJSON() failed [ser_ids]");
            goto end;
        }
    }

    if (!service_id_to_frequency->frequency) {
        ogs_error("OpenAPI_service_id_to_frequency_convertToJSON() failed [frequency]");
        goto end;
    }
    if (cJSON_AddNumberToObject(item, "frequency", service_id_to_frequency->frequency) == NULL) {
        ogs_error("OpenAPI_service_id_to_frequency_convertToJSON() failed [frequency]");
        goto end;
    }

    if (service_id_to_frequency->geographical_area) {
        if (cJSON_AddStringToObject(item, "geographicalArea", service_id_to_frequency->geographical_area) == NULL) {
            ogs_error("OpenAPI_service_id_to_frequency_convertToJSON() failed [geographical_area]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_service_id_to_frequency_t *OpenAPI_service_id_to_frequency_parseFromJSON(cJSON *service_id_to_frequencyJSON)
{
    OpenAPI_service_id_to_frequency_t *service_id_to_frequency_local_var = NULL;
    cJSON *ser_ids = cJSON_GetObjectItemCaseSensitive(service_id_to_frequencyJSON, "serIds");
    if (!ser_ids) {
        ogs_error("OpenAPI_service_id_to_frequency_parseFromJSON() failed [ser_ids]");
        goto end;
    }

    OpenAPI_list_t *ser_idsList;

    cJSON *ser_ids_local;
    if (!cJSON_IsArray(ser_ids)) {
        ogs_error("OpenAPI_service_id_to_frequency_parseFromJSON() failed [ser_ids]");
        goto end;
    }
    ser_idsList = OpenAPI_list_create();

    cJSON_ArrayForEach(ser_ids_local, ser_ids) {
        if (!cJSON_IsString(ser_ids_local)) {
            ogs_error("OpenAPI_service_id_to_frequency_parseFromJSON() failed [ser_ids]");
            goto end;
        }
        OpenAPI_list_add(ser_idsList, ogs_strdup(ser_ids_local->valuestring));
    }

    cJSON *frequency = cJSON_GetObjectItemCaseSensitive(service_id_to_frequencyJSON, "frequency");
    if (!frequency) {
        ogs_error("OpenAPI_service_id_to_frequency_parseFromJSON() failed [frequency]");
        goto end;
    }


    if (!cJSON_IsNumber(frequency)) {
        ogs_error("OpenAPI_service_id_to_frequency_parseFromJSON() failed [frequency]");
        goto end;
    }

    cJSON *geographical_area = cJSON_GetObjectItemCaseSensitive(service_id_to_frequencyJSON, "geographicalArea");

    if (geographical_area) {
        if (!cJSON_IsString(geographical_area)) {
            ogs_error("OpenAPI_service_id_to_frequency_parseFromJSON() failed [geographical_area]");
            goto end;
        }
    }

    service_id_to_frequency_local_var = OpenAPI_service_id_to_frequency_create (
        ser_idsList,
        frequency->valuedouble,
        geographical_area ? ogs_strdup(geographical_area->valuestring) : NULL
        );

    return service_id_to_frequency_local_var;
end:
    return NULL;
}

OpenAPI_service_id_to_frequency_t *OpenAPI_service_id_to_frequency_copy(OpenAPI_service_id_to_frequency_t *dst, OpenAPI_service_id_to_frequency_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_service_id_to_frequency_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_service_id_to_frequency_convertToJSON() failed");
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

    OpenAPI_service_id_to_frequency_free(dst);
    dst = OpenAPI_service_id_to_frequency_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

