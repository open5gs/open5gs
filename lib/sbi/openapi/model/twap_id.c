
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "twap_id.h"

OpenAPI_twap_id_t *OpenAPI_twap_id_create(
    char *ss_id,
    char *bss_id,
    char civic_address
)
{
    OpenAPI_twap_id_t *twap_id_local_var = ogs_malloc(sizeof(OpenAPI_twap_id_t));
    ogs_assert(twap_id_local_var);

    twap_id_local_var->ss_id = ss_id;
    twap_id_local_var->bss_id = bss_id;
    twap_id_local_var->civic_address = civic_address;

    return twap_id_local_var;
}

void OpenAPI_twap_id_free(OpenAPI_twap_id_t *twap_id)
{
    if (NULL == twap_id) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(twap_id->ss_id);
    ogs_free(twap_id->bss_id);
    ogs_free(twap_id);
}

cJSON *OpenAPI_twap_id_convertToJSON(OpenAPI_twap_id_t *twap_id)
{
    cJSON *item = NULL;

    if (twap_id == NULL) {
        ogs_error("OpenAPI_twap_id_convertToJSON() failed [TwapId]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddStringToObject(item, "ssId", twap_id->ss_id) == NULL) {
        ogs_error("OpenAPI_twap_id_convertToJSON() failed [ss_id]");
        goto end;
    }

    if (twap_id->bss_id) {
    if (cJSON_AddStringToObject(item, "bssId", twap_id->bss_id) == NULL) {
        ogs_error("OpenAPI_twap_id_convertToJSON() failed [bss_id]");
        goto end;
    }
    }

    if (twap_id->civic_address) {
    if (cJSON_AddNumberToObject(item, "civicAddress", twap_id->civic_address) == NULL) {
        ogs_error("OpenAPI_twap_id_convertToJSON() failed [civic_address]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_twap_id_t *OpenAPI_twap_id_parseFromJSON(cJSON *twap_idJSON)
{
    OpenAPI_twap_id_t *twap_id_local_var = NULL;
    cJSON *ss_id = cJSON_GetObjectItemCaseSensitive(twap_idJSON, "ssId");
    if (!ss_id) {
        ogs_error("OpenAPI_twap_id_parseFromJSON() failed [ss_id]");
        goto end;
    }

    if (!cJSON_IsString(ss_id)) {
        ogs_error("OpenAPI_twap_id_parseFromJSON() failed [ss_id]");
        goto end;
    }

    cJSON *bss_id = cJSON_GetObjectItemCaseSensitive(twap_idJSON, "bssId");

    if (bss_id) {
    if (!cJSON_IsString(bss_id)) {
        ogs_error("OpenAPI_twap_id_parseFromJSON() failed [bss_id]");
        goto end;
    }
    }

    cJSON *civic_address = cJSON_GetObjectItemCaseSensitive(twap_idJSON, "civicAddress");

    if (civic_address) {
    if (!cJSON_IsNumber(civic_address)) {
        ogs_error("OpenAPI_twap_id_parseFromJSON() failed [civic_address]");
        goto end;
    }
    }

    twap_id_local_var = OpenAPI_twap_id_create (
        ogs_strdup(ss_id->valuestring),
        bss_id ? ogs_strdup(bss_id->valuestring) : NULL,
        civic_address ? civic_address->valueint : 0
    );

    return twap_id_local_var;
end:
    return NULL;
}

OpenAPI_twap_id_t *OpenAPI_twap_id_copy(OpenAPI_twap_id_t *dst, OpenAPI_twap_id_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_twap_id_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_twap_id_convertToJSON() failed");
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

    OpenAPI_twap_id_free(dst);
    dst = OpenAPI_twap_id_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

