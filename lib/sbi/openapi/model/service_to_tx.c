
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "service_to_tx.h"

OpenAPI_service_to_tx_t *OpenAPI_service_to_tx_create(
    OpenAPI_list_t *ser_ids,
    char *tx_profile,
    OpenAPI_v2x_rat_type_t *rat
    )
{
    OpenAPI_service_to_tx_t *service_to_tx_local_var = OpenAPI_malloc(sizeof(OpenAPI_service_to_tx_t));
    if (!service_to_tx_local_var) {
        return NULL;
    }
    service_to_tx_local_var->ser_ids = ser_ids;
    service_to_tx_local_var->tx_profile = tx_profile;
    service_to_tx_local_var->rat = rat;

    return service_to_tx_local_var;
}

void OpenAPI_service_to_tx_free(OpenAPI_service_to_tx_t *service_to_tx)
{
    if (NULL == service_to_tx) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(service_to_tx->ser_ids, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(service_to_tx->ser_ids);
    ogs_free(service_to_tx->tx_profile);
    OpenAPI_v2x_rat_type_free(service_to_tx->rat);
    ogs_free(service_to_tx);
}

cJSON *OpenAPI_service_to_tx_convertToJSON(OpenAPI_service_to_tx_t *service_to_tx)
{
    cJSON *item = NULL;

    if (service_to_tx == NULL) {
        ogs_error("OpenAPI_service_to_tx_convertToJSON() failed [ServiceToTx]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!service_to_tx->ser_ids) {
        ogs_error("OpenAPI_service_to_tx_convertToJSON() failed [ser_ids]");
        goto end;
    }
    cJSON *ser_ids = cJSON_AddArrayToObject(item, "serIds");
    if (ser_ids == NULL) {
        ogs_error("OpenAPI_service_to_tx_convertToJSON() failed [ser_ids]");
        goto end;
    }

    OpenAPI_lnode_t *ser_ids_node;
    OpenAPI_list_for_each(service_to_tx->ser_ids, ser_ids_node)  {
        if (cJSON_AddStringToObject(ser_ids, "", (char*)ser_ids_node->data) == NULL) {
            ogs_error("OpenAPI_service_to_tx_convertToJSON() failed [ser_ids]");
            goto end;
        }
    }

    if (!service_to_tx->tx_profile) {
        ogs_error("OpenAPI_service_to_tx_convertToJSON() failed [tx_profile]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "txProfile", service_to_tx->tx_profile) == NULL) {
        ogs_error("OpenAPI_service_to_tx_convertToJSON() failed [tx_profile]");
        goto end;
    }

    if (!service_to_tx->rat) {
        ogs_error("OpenAPI_service_to_tx_convertToJSON() failed [rat]");
        goto end;
    }
    cJSON *rat_local_JSON = OpenAPI_v2x_rat_type_convertToJSON(service_to_tx->rat);
    if (rat_local_JSON == NULL) {
        ogs_error("OpenAPI_service_to_tx_convertToJSON() failed [rat]");
        goto end;
    }
    cJSON_AddItemToObject(item, "rat", rat_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_service_to_tx_convertToJSON() failed [rat]");
        goto end;
    }

end:
    return item;
}

OpenAPI_service_to_tx_t *OpenAPI_service_to_tx_parseFromJSON(cJSON *service_to_txJSON)
{
    OpenAPI_service_to_tx_t *service_to_tx_local_var = NULL;
    cJSON *ser_ids = cJSON_GetObjectItemCaseSensitive(service_to_txJSON, "serIds");
    if (!ser_ids) {
        ogs_error("OpenAPI_service_to_tx_parseFromJSON() failed [ser_ids]");
        goto end;
    }

    OpenAPI_list_t *ser_idsList;

    cJSON *ser_ids_local;
    if (!cJSON_IsArray(ser_ids)) {
        ogs_error("OpenAPI_service_to_tx_parseFromJSON() failed [ser_ids]");
        goto end;
    }
    ser_idsList = OpenAPI_list_create();

    cJSON_ArrayForEach(ser_ids_local, ser_ids) {
        if (!cJSON_IsString(ser_ids_local)) {
            ogs_error("OpenAPI_service_to_tx_parseFromJSON() failed [ser_ids]");
            goto end;
        }
        OpenAPI_list_add(ser_idsList, ogs_strdup(ser_ids_local->valuestring));
    }

    cJSON *tx_profile = cJSON_GetObjectItemCaseSensitive(service_to_txJSON, "txProfile");
    if (!tx_profile) {
        ogs_error("OpenAPI_service_to_tx_parseFromJSON() failed [tx_profile]");
        goto end;
    }


    if (!cJSON_IsString(tx_profile)) {
        ogs_error("OpenAPI_service_to_tx_parseFromJSON() failed [tx_profile]");
        goto end;
    }

    cJSON *rat = cJSON_GetObjectItemCaseSensitive(service_to_txJSON, "rat");
    if (!rat) {
        ogs_error("OpenAPI_service_to_tx_parseFromJSON() failed [rat]");
        goto end;
    }

    OpenAPI_v2x_rat_type_t *rat_local_nonprim = NULL;

    rat_local_nonprim = OpenAPI_v2x_rat_type_parseFromJSON(rat);

    service_to_tx_local_var = OpenAPI_service_to_tx_create (
        ser_idsList,
        ogs_strdup(tx_profile->valuestring),
        rat_local_nonprim
        );

    return service_to_tx_local_var;
end:
    return NULL;
}

OpenAPI_service_to_tx_t *OpenAPI_service_to_tx_copy(OpenAPI_service_to_tx_t *dst, OpenAPI_service_to_tx_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_service_to_tx_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_service_to_tx_convertToJSON() failed");
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

    OpenAPI_service_to_tx_free(dst);
    dst = OpenAPI_service_to_tx_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

