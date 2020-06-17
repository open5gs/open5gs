
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "service_id_to_layer2_id.h"

OpenAPI_service_id_to_layer2_id_t *OpenAPI_service_id_to_layer2_id_create(
    OpenAPI_list_t *ser_ids,
    char *des_layer2_id
    )
{
    OpenAPI_service_id_to_layer2_id_t *service_id_to_layer2_id_local_var = OpenAPI_malloc(sizeof(OpenAPI_service_id_to_layer2_id_t));
    if (!service_id_to_layer2_id_local_var) {
        return NULL;
    }
    service_id_to_layer2_id_local_var->ser_ids = ser_ids;
    service_id_to_layer2_id_local_var->des_layer2_id = des_layer2_id;

    return service_id_to_layer2_id_local_var;
}

void OpenAPI_service_id_to_layer2_id_free(OpenAPI_service_id_to_layer2_id_t *service_id_to_layer2_id)
{
    if (NULL == service_id_to_layer2_id) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(service_id_to_layer2_id->ser_ids, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(service_id_to_layer2_id->ser_ids);
    ogs_free(service_id_to_layer2_id->des_layer2_id);
    ogs_free(service_id_to_layer2_id);
}

cJSON *OpenAPI_service_id_to_layer2_id_convertToJSON(OpenAPI_service_id_to_layer2_id_t *service_id_to_layer2_id)
{
    cJSON *item = NULL;

    if (service_id_to_layer2_id == NULL) {
        ogs_error("OpenAPI_service_id_to_layer2_id_convertToJSON() failed [ServiceIdToLayer2Id]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!service_id_to_layer2_id->ser_ids) {
        ogs_error("OpenAPI_service_id_to_layer2_id_convertToJSON() failed [ser_ids]");
        goto end;
    }
    cJSON *ser_ids = cJSON_AddArrayToObject(item, "serIds");
    if (ser_ids == NULL) {
        ogs_error("OpenAPI_service_id_to_layer2_id_convertToJSON() failed [ser_ids]");
        goto end;
    }

    OpenAPI_lnode_t *ser_ids_node;
    OpenAPI_list_for_each(service_id_to_layer2_id->ser_ids, ser_ids_node)  {
        if (cJSON_AddStringToObject(ser_ids, "", (char*)ser_ids_node->data) == NULL) {
            ogs_error("OpenAPI_service_id_to_layer2_id_convertToJSON() failed [ser_ids]");
            goto end;
        }
    }

    if (!service_id_to_layer2_id->des_layer2_id) {
        ogs_error("OpenAPI_service_id_to_layer2_id_convertToJSON() failed [des_layer2_id]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "desLayer2Id", service_id_to_layer2_id->des_layer2_id) == NULL) {
        ogs_error("OpenAPI_service_id_to_layer2_id_convertToJSON() failed [des_layer2_id]");
        goto end;
    }

end:
    return item;
}

OpenAPI_service_id_to_layer2_id_t *OpenAPI_service_id_to_layer2_id_parseFromJSON(cJSON *service_id_to_layer2_idJSON)
{
    OpenAPI_service_id_to_layer2_id_t *service_id_to_layer2_id_local_var = NULL;
    cJSON *ser_ids = cJSON_GetObjectItemCaseSensitive(service_id_to_layer2_idJSON, "serIds");
    if (!ser_ids) {
        ogs_error("OpenAPI_service_id_to_layer2_id_parseFromJSON() failed [ser_ids]");
        goto end;
    }

    OpenAPI_list_t *ser_idsList;

    cJSON *ser_ids_local;
    if (!cJSON_IsArray(ser_ids)) {
        ogs_error("OpenAPI_service_id_to_layer2_id_parseFromJSON() failed [ser_ids]");
        goto end;
    }
    ser_idsList = OpenAPI_list_create();

    cJSON_ArrayForEach(ser_ids_local, ser_ids) {
        if (!cJSON_IsString(ser_ids_local)) {
            ogs_error("OpenAPI_service_id_to_layer2_id_parseFromJSON() failed [ser_ids]");
            goto end;
        }
        OpenAPI_list_add(ser_idsList, ogs_strdup(ser_ids_local->valuestring));
    }

    cJSON *des_layer2_id = cJSON_GetObjectItemCaseSensitive(service_id_to_layer2_idJSON, "desLayer2Id");
    if (!des_layer2_id) {
        ogs_error("OpenAPI_service_id_to_layer2_id_parseFromJSON() failed [des_layer2_id]");
        goto end;
    }


    if (!cJSON_IsString(des_layer2_id)) {
        ogs_error("OpenAPI_service_id_to_layer2_id_parseFromJSON() failed [des_layer2_id]");
        goto end;
    }

    service_id_to_layer2_id_local_var = OpenAPI_service_id_to_layer2_id_create (
        ser_idsList,
        ogs_strdup(des_layer2_id->valuestring)
        );

    return service_id_to_layer2_id_local_var;
end:
    return NULL;
}

OpenAPI_service_id_to_layer2_id_t *OpenAPI_service_id_to_layer2_id_copy(OpenAPI_service_id_to_layer2_id_t *dst, OpenAPI_service_id_to_layer2_id_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_service_id_to_layer2_id_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_service_id_to_layer2_id_convertToJSON() failed");
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

    OpenAPI_service_id_to_layer2_id_free(dst);
    dst = OpenAPI_service_id_to_layer2_id_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

