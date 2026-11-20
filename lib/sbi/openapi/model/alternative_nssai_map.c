
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "alternative_nssai_map.h"

OpenAPI_alternative_nssai_map_t *OpenAPI_alternative_nssai_map_create(
    OpenAPI_snssai_t *snssai,
    OpenAPI_snssai_t *alternative_snssai
)
{
    OpenAPI_alternative_nssai_map_t *alternative_nssai_map_local_var = ogs_malloc(sizeof(OpenAPI_alternative_nssai_map_t));
    ogs_assert(alternative_nssai_map_local_var);

    alternative_nssai_map_local_var->snssai = snssai;
    alternative_nssai_map_local_var->alternative_snssai = alternative_snssai;

    return alternative_nssai_map_local_var;
}

void OpenAPI_alternative_nssai_map_free(OpenAPI_alternative_nssai_map_t *alternative_nssai_map)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == alternative_nssai_map) {
        return;
    }
    if (alternative_nssai_map->snssai) {
        OpenAPI_snssai_free(alternative_nssai_map->snssai);
        alternative_nssai_map->snssai = NULL;
    }
    if (alternative_nssai_map->alternative_snssai) {
        OpenAPI_snssai_free(alternative_nssai_map->alternative_snssai);
        alternative_nssai_map->alternative_snssai = NULL;
    }
    ogs_free(alternative_nssai_map);
}

cJSON *OpenAPI_alternative_nssai_map_convertToJSON(OpenAPI_alternative_nssai_map_t *alternative_nssai_map)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (alternative_nssai_map == NULL) {
        ogs_error("OpenAPI_alternative_nssai_map_convertToJSON() failed [AlternativeNSSAIMap]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!alternative_nssai_map->snssai) {
        ogs_error("OpenAPI_alternative_nssai_map_convertToJSON() failed [snssai]");
        return NULL;
    }
    cJSON *snssai_local_JSON = OpenAPI_snssai_convertToJSON(alternative_nssai_map->snssai);
    if (snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_alternative_nssai_map_convertToJSON() failed [snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "snssai", snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_alternative_nssai_map_convertToJSON() failed [snssai]");
        goto end;
    }

    if (!alternative_nssai_map->alternative_snssai) {
        ogs_error("OpenAPI_alternative_nssai_map_convertToJSON() failed [alternative_snssai]");
        return NULL;
    }
    cJSON *alternative_snssai_local_JSON = OpenAPI_snssai_convertToJSON(alternative_nssai_map->alternative_snssai);
    if (alternative_snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_alternative_nssai_map_convertToJSON() failed [alternative_snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "alternativeSnssai", alternative_snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_alternative_nssai_map_convertToJSON() failed [alternative_snssai]");
        goto end;
    }

end:
    return item;
}

OpenAPI_alternative_nssai_map_t *OpenAPI_alternative_nssai_map_parseFromJSON(cJSON *alternative_nssai_mapJSON)
{
    OpenAPI_alternative_nssai_map_t *alternative_nssai_map_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *snssai = NULL;
    OpenAPI_snssai_t *snssai_local_nonprim = NULL;
    cJSON *alternative_snssai = NULL;
    OpenAPI_snssai_t *alternative_snssai_local_nonprim = NULL;
    snssai = cJSON_GetObjectItemCaseSensitive(alternative_nssai_mapJSON, "snssai");
    if (!snssai) {
        ogs_error("OpenAPI_alternative_nssai_map_parseFromJSON() failed [snssai]");
        goto end;
    }
    snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(snssai);
    if (!snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [snssai]");
        goto end;
    }

    alternative_snssai = cJSON_GetObjectItemCaseSensitive(alternative_nssai_mapJSON, "alternativeSnssai");
    if (!alternative_snssai) {
        ogs_error("OpenAPI_alternative_nssai_map_parseFromJSON() failed [alternative_snssai]");
        goto end;
    }
    alternative_snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(alternative_snssai);
    if (!alternative_snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [alternative_snssai]");
        goto end;
    }

    alternative_nssai_map_local_var = OpenAPI_alternative_nssai_map_create (
        snssai_local_nonprim,
        alternative_snssai_local_nonprim
    );

    return alternative_nssai_map_local_var;
end:
    if (snssai_local_nonprim) {
        OpenAPI_snssai_free(snssai_local_nonprim);
        snssai_local_nonprim = NULL;
    }
    if (alternative_snssai_local_nonprim) {
        OpenAPI_snssai_free(alternative_snssai_local_nonprim);
        alternative_snssai_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_alternative_nssai_map_t *OpenAPI_alternative_nssai_map_copy(OpenAPI_alternative_nssai_map_t *dst, OpenAPI_alternative_nssai_map_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_alternative_nssai_map_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_alternative_nssai_map_convertToJSON() failed");
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

    OpenAPI_alternative_nssai_map_free(dst);
    dst = OpenAPI_alternative_nssai_map_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

