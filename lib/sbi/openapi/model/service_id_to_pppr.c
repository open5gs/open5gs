
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "service_id_to_pppr.h"

OpenAPI_service_id_to_pppr_t *OpenAPI_service_id_to_pppr_create(
    OpenAPI_list_t *ser_ids,
    int pppr
    )
{
    OpenAPI_service_id_to_pppr_t *service_id_to_pppr_local_var = OpenAPI_malloc(sizeof(OpenAPI_service_id_to_pppr_t));
    if (!service_id_to_pppr_local_var) {
        return NULL;
    }
    service_id_to_pppr_local_var->ser_ids = ser_ids;
    service_id_to_pppr_local_var->pppr = pppr;

    return service_id_to_pppr_local_var;
}

void OpenAPI_service_id_to_pppr_free(OpenAPI_service_id_to_pppr_t *service_id_to_pppr)
{
    if (NULL == service_id_to_pppr) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(service_id_to_pppr->ser_ids, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(service_id_to_pppr->ser_ids);
    ogs_free(service_id_to_pppr);
}

cJSON *OpenAPI_service_id_to_pppr_convertToJSON(OpenAPI_service_id_to_pppr_t *service_id_to_pppr)
{
    cJSON *item = NULL;

    if (service_id_to_pppr == NULL) {
        ogs_error("OpenAPI_service_id_to_pppr_convertToJSON() failed [ServiceIdToPppr]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!service_id_to_pppr->ser_ids) {
        ogs_error("OpenAPI_service_id_to_pppr_convertToJSON() failed [ser_ids]");
        goto end;
    }
    cJSON *ser_ids = cJSON_AddArrayToObject(item, "serIds");
    if (ser_ids == NULL) {
        ogs_error("OpenAPI_service_id_to_pppr_convertToJSON() failed [ser_ids]");
        goto end;
    }

    OpenAPI_lnode_t *ser_ids_node;
    OpenAPI_list_for_each(service_id_to_pppr->ser_ids, ser_ids_node)  {
        if (cJSON_AddStringToObject(ser_ids, "", (char*)ser_ids_node->data) == NULL) {
            ogs_error("OpenAPI_service_id_to_pppr_convertToJSON() failed [ser_ids]");
            goto end;
        }
    }

    if (!service_id_to_pppr->pppr) {
        ogs_error("OpenAPI_service_id_to_pppr_convertToJSON() failed [pppr]");
        goto end;
    }
    if (cJSON_AddNumberToObject(item, "pppr", service_id_to_pppr->pppr) == NULL) {
        ogs_error("OpenAPI_service_id_to_pppr_convertToJSON() failed [pppr]");
        goto end;
    }

end:
    return item;
}

OpenAPI_service_id_to_pppr_t *OpenAPI_service_id_to_pppr_parseFromJSON(cJSON *service_id_to_ppprJSON)
{
    OpenAPI_service_id_to_pppr_t *service_id_to_pppr_local_var = NULL;
    cJSON *ser_ids = cJSON_GetObjectItemCaseSensitive(service_id_to_ppprJSON, "serIds");
    if (!ser_ids) {
        ogs_error("OpenAPI_service_id_to_pppr_parseFromJSON() failed [ser_ids]");
        goto end;
    }

    OpenAPI_list_t *ser_idsList;

    cJSON *ser_ids_local;
    if (!cJSON_IsArray(ser_ids)) {
        ogs_error("OpenAPI_service_id_to_pppr_parseFromJSON() failed [ser_ids]");
        goto end;
    }
    ser_idsList = OpenAPI_list_create();

    cJSON_ArrayForEach(ser_ids_local, ser_ids) {
        if (!cJSON_IsString(ser_ids_local)) {
            ogs_error("OpenAPI_service_id_to_pppr_parseFromJSON() failed [ser_ids]");
            goto end;
        }
        OpenAPI_list_add(ser_idsList, ogs_strdup(ser_ids_local->valuestring));
    }

    cJSON *pppr = cJSON_GetObjectItemCaseSensitive(service_id_to_ppprJSON, "pppr");
    if (!pppr) {
        ogs_error("OpenAPI_service_id_to_pppr_parseFromJSON() failed [pppr]");
        goto end;
    }


    if (!cJSON_IsNumber(pppr)) {
        ogs_error("OpenAPI_service_id_to_pppr_parseFromJSON() failed [pppr]");
        goto end;
    }

    service_id_to_pppr_local_var = OpenAPI_service_id_to_pppr_create (
        ser_idsList,
        pppr->valuedouble
        );

    return service_id_to_pppr_local_var;
end:
    return NULL;
}

OpenAPI_service_id_to_pppr_t *OpenAPI_service_id_to_pppr_copy(OpenAPI_service_id_to_pppr_t *dst, OpenAPI_service_id_to_pppr_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_service_id_to_pppr_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_service_id_to_pppr_convertToJSON() failed");
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

    OpenAPI_service_id_to_pppr_free(dst);
    dst = OpenAPI_service_id_to_pppr_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

