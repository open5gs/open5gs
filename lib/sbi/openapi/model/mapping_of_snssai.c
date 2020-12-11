
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mapping_of_snssai.h"

OpenAPI_mapping_of_snssai_t *OpenAPI_mapping_of_snssai_create(
    OpenAPI_snssai_t *serving_snssai,
    OpenAPI_snssai_t *home_snssai
    )
{
    OpenAPI_mapping_of_snssai_t *mapping_of_snssai_local_var = OpenAPI_malloc(sizeof(OpenAPI_mapping_of_snssai_t));
    if (!mapping_of_snssai_local_var) {
        return NULL;
    }
    mapping_of_snssai_local_var->serving_snssai = serving_snssai;
    mapping_of_snssai_local_var->home_snssai = home_snssai;

    return mapping_of_snssai_local_var;
}

void OpenAPI_mapping_of_snssai_free(OpenAPI_mapping_of_snssai_t *mapping_of_snssai)
{
    if (NULL == mapping_of_snssai) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_snssai_free(mapping_of_snssai->serving_snssai);
    OpenAPI_snssai_free(mapping_of_snssai->home_snssai);
    ogs_free(mapping_of_snssai);
}

cJSON *OpenAPI_mapping_of_snssai_convertToJSON(OpenAPI_mapping_of_snssai_t *mapping_of_snssai)
{
    cJSON *item = NULL;

    if (mapping_of_snssai == NULL) {
        ogs_error("OpenAPI_mapping_of_snssai_convertToJSON() failed [MappingOfSnssai]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!mapping_of_snssai->serving_snssai) {
        ogs_error("OpenAPI_mapping_of_snssai_convertToJSON() failed [serving_snssai]");
        goto end;
    }
    cJSON *serving_snssai_local_JSON = OpenAPI_snssai_convertToJSON(mapping_of_snssai->serving_snssai);
    if (serving_snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_mapping_of_snssai_convertToJSON() failed [serving_snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "servingSnssai", serving_snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_mapping_of_snssai_convertToJSON() failed [serving_snssai]");
        goto end;
    }

    if (!mapping_of_snssai->home_snssai) {
        ogs_error("OpenAPI_mapping_of_snssai_convertToJSON() failed [home_snssai]");
        goto end;
    }
    cJSON *home_snssai_local_JSON = OpenAPI_snssai_convertToJSON(mapping_of_snssai->home_snssai);
    if (home_snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_mapping_of_snssai_convertToJSON() failed [home_snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "homeSnssai", home_snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_mapping_of_snssai_convertToJSON() failed [home_snssai]");
        goto end;
    }

end:
    return item;
}

OpenAPI_mapping_of_snssai_t *OpenAPI_mapping_of_snssai_parseFromJSON(cJSON *mapping_of_snssaiJSON)
{
    OpenAPI_mapping_of_snssai_t *mapping_of_snssai_local_var = NULL;
    cJSON *serving_snssai = cJSON_GetObjectItemCaseSensitive(mapping_of_snssaiJSON, "servingSnssai");
    if (!serving_snssai) {
        ogs_error("OpenAPI_mapping_of_snssai_parseFromJSON() failed [serving_snssai]");
        goto end;
    }

    OpenAPI_snssai_t *serving_snssai_local_nonprim = NULL;

    serving_snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(serving_snssai);

    cJSON *home_snssai = cJSON_GetObjectItemCaseSensitive(mapping_of_snssaiJSON, "homeSnssai");
    if (!home_snssai) {
        ogs_error("OpenAPI_mapping_of_snssai_parseFromJSON() failed [home_snssai]");
        goto end;
    }

    OpenAPI_snssai_t *home_snssai_local_nonprim = NULL;

    home_snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(home_snssai);

    mapping_of_snssai_local_var = OpenAPI_mapping_of_snssai_create (
        serving_snssai_local_nonprim,
        home_snssai_local_nonprim
        );

    return mapping_of_snssai_local_var;
end:
    return NULL;
}

OpenAPI_mapping_of_snssai_t *OpenAPI_mapping_of_snssai_copy(OpenAPI_mapping_of_snssai_t *dst, OpenAPI_mapping_of_snssai_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_mapping_of_snssai_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_mapping_of_snssai_convertToJSON() failed");
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

    OpenAPI_mapping_of_snssai_free(dst);
    dst = OpenAPI_mapping_of_snssai_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

