
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tnap_id.h"

OpenAPI_tnap_id_t *OpenAPI_tnap_id_create(
    char *ss_id,
    char *bss_id,
    char *civic_address
)
{
    OpenAPI_tnap_id_t *tnap_id_local_var = ogs_malloc(sizeof(OpenAPI_tnap_id_t));
    ogs_assert(tnap_id_local_var);

    tnap_id_local_var->ss_id = ss_id;
    tnap_id_local_var->bss_id = bss_id;
    tnap_id_local_var->civic_address = civic_address;

    return tnap_id_local_var;
}

void OpenAPI_tnap_id_free(OpenAPI_tnap_id_t *tnap_id)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == tnap_id) {
        return;
    }
    if (tnap_id->ss_id) {
        ogs_free(tnap_id->ss_id);
        tnap_id->ss_id = NULL;
    }
    if (tnap_id->bss_id) {
        ogs_free(tnap_id->bss_id);
        tnap_id->bss_id = NULL;
    }
    if (tnap_id->civic_address) {
        ogs_free(tnap_id->civic_address);
        tnap_id->civic_address = NULL;
    }
    ogs_free(tnap_id);
}

cJSON *OpenAPI_tnap_id_convertToJSON(OpenAPI_tnap_id_t *tnap_id)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (tnap_id == NULL) {
        ogs_error("OpenAPI_tnap_id_convertToJSON() failed [TnapId]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (tnap_id->ss_id) {
    if (cJSON_AddStringToObject(item, "ssId", tnap_id->ss_id) == NULL) {
        ogs_error("OpenAPI_tnap_id_convertToJSON() failed [ss_id]");
        goto end;
    }
    }

    if (tnap_id->bss_id) {
    if (cJSON_AddStringToObject(item, "bssId", tnap_id->bss_id) == NULL) {
        ogs_error("OpenAPI_tnap_id_convertToJSON() failed [bss_id]");
        goto end;
    }
    }

    if (tnap_id->civic_address) {
    if (cJSON_AddStringToObject(item, "civicAddress", tnap_id->civic_address) == NULL) {
        ogs_error("OpenAPI_tnap_id_convertToJSON() failed [civic_address]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_tnap_id_t *OpenAPI_tnap_id_parseFromJSON(cJSON *tnap_idJSON)
{
    OpenAPI_tnap_id_t *tnap_id_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ss_id = NULL;
    cJSON *bss_id = NULL;
    cJSON *civic_address = NULL;
    ss_id = cJSON_GetObjectItemCaseSensitive(tnap_idJSON, "ssId");
    if (ss_id) {
    if (!cJSON_IsString(ss_id) && !cJSON_IsNull(ss_id)) {
        ogs_error("OpenAPI_tnap_id_parseFromJSON() failed [ss_id]");
        goto end;
    }
    }

    bss_id = cJSON_GetObjectItemCaseSensitive(tnap_idJSON, "bssId");
    if (bss_id) {
    if (!cJSON_IsString(bss_id) && !cJSON_IsNull(bss_id)) {
        ogs_error("OpenAPI_tnap_id_parseFromJSON() failed [bss_id]");
        goto end;
    }
    }

    civic_address = cJSON_GetObjectItemCaseSensitive(tnap_idJSON, "civicAddress");
    if (civic_address) {
    if (!cJSON_IsString(civic_address) && !cJSON_IsNull(civic_address)) {
        ogs_error("OpenAPI_tnap_id_parseFromJSON() failed [civic_address]");
        goto end;
    }
    }

    tnap_id_local_var = OpenAPI_tnap_id_create (
        ss_id && !cJSON_IsNull(ss_id) ? ogs_strdup(ss_id->valuestring) : NULL,
        bss_id && !cJSON_IsNull(bss_id) ? ogs_strdup(bss_id->valuestring) : NULL,
        civic_address && !cJSON_IsNull(civic_address) ? ogs_strdup(civic_address->valuestring) : NULL
    );

    return tnap_id_local_var;
end:
    return NULL;
}

OpenAPI_tnap_id_t *OpenAPI_tnap_id_copy(OpenAPI_tnap_id_t *dst, OpenAPI_tnap_id_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_tnap_id_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_tnap_id_convertToJSON() failed");
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

    OpenAPI_tnap_id_free(dst);
    dst = OpenAPI_tnap_id_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

