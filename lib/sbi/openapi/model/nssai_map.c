
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nssai_map.h"

OpenAPI_nssai_map_t *OpenAPI_nssai_map_create(
    OpenAPI_snssai_t *serving_snssai,
    OpenAPI_snssai_t *home_snssai
)
{
    OpenAPI_nssai_map_t *nssai_map_local_var = ogs_malloc(sizeof(OpenAPI_nssai_map_t));
    ogs_assert(nssai_map_local_var);

    nssai_map_local_var->serving_snssai = serving_snssai;
    nssai_map_local_var->home_snssai = home_snssai;

    return nssai_map_local_var;
}

void OpenAPI_nssai_map_free(OpenAPI_nssai_map_t *nssai_map)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == nssai_map) {
        return;
    }
    if (nssai_map->serving_snssai) {
        OpenAPI_snssai_free(nssai_map->serving_snssai);
        nssai_map->serving_snssai = NULL;
    }
    if (nssai_map->home_snssai) {
        OpenAPI_snssai_free(nssai_map->home_snssai);
        nssai_map->home_snssai = NULL;
    }
    ogs_free(nssai_map);
}

cJSON *OpenAPI_nssai_map_convertToJSON(OpenAPI_nssai_map_t *nssai_map)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (nssai_map == NULL) {
        ogs_error("OpenAPI_nssai_map_convertToJSON() failed [NSSAIMap]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!nssai_map->serving_snssai) {
        ogs_error("OpenAPI_nssai_map_convertToJSON() failed [serving_snssai]");
        return NULL;
    }
    cJSON *serving_snssai_local_JSON = OpenAPI_snssai_convertToJSON(nssai_map->serving_snssai);
    if (serving_snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_nssai_map_convertToJSON() failed [serving_snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "servingSnssai", serving_snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_nssai_map_convertToJSON() failed [serving_snssai]");
        goto end;
    }

    if (!nssai_map->home_snssai) {
        ogs_error("OpenAPI_nssai_map_convertToJSON() failed [home_snssai]");
        return NULL;
    }
    cJSON *home_snssai_local_JSON = OpenAPI_snssai_convertToJSON(nssai_map->home_snssai);
    if (home_snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_nssai_map_convertToJSON() failed [home_snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "homeSnssai", home_snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_nssai_map_convertToJSON() failed [home_snssai]");
        goto end;
    }

end:
    return item;
}

OpenAPI_nssai_map_t *OpenAPI_nssai_map_parseFromJSON(cJSON *nssai_mapJSON)
{
    OpenAPI_nssai_map_t *nssai_map_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *serving_snssai = NULL;
    OpenAPI_snssai_t *serving_snssai_local_nonprim = NULL;
    cJSON *home_snssai = NULL;
    OpenAPI_snssai_t *home_snssai_local_nonprim = NULL;
    serving_snssai = cJSON_GetObjectItemCaseSensitive(nssai_mapJSON, "servingSnssai");
    if (!serving_snssai) {
        ogs_error("OpenAPI_nssai_map_parseFromJSON() failed [serving_snssai]");
        goto end;
    }
    serving_snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(serving_snssai);
    if (!serving_snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [serving_snssai]");
        goto end;
    }

    home_snssai = cJSON_GetObjectItemCaseSensitive(nssai_mapJSON, "homeSnssai");
    if (!home_snssai) {
        ogs_error("OpenAPI_nssai_map_parseFromJSON() failed [home_snssai]");
        goto end;
    }
    home_snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(home_snssai);
    if (!home_snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [home_snssai]");
        goto end;
    }

    nssai_map_local_var = OpenAPI_nssai_map_create (
        serving_snssai_local_nonprim,
        home_snssai_local_nonprim
    );

    return nssai_map_local_var;
end:
    if (serving_snssai_local_nonprim) {
        OpenAPI_snssai_free(serving_snssai_local_nonprim);
        serving_snssai_local_nonprim = NULL;
    }
    if (home_snssai_local_nonprim) {
        OpenAPI_snssai_free(home_snssai_local_nonprim);
        home_snssai_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_nssai_map_t *OpenAPI_nssai_map_copy(OpenAPI_nssai_map_t *dst, OpenAPI_nssai_map_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nssai_map_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nssai_map_convertToJSON() failed");
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

    OpenAPI_nssai_map_free(dst);
    dst = OpenAPI_nssai_map_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

