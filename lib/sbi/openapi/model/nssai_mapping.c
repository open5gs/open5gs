
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nssai_mapping.h"

OpenAPI_nssai_mapping_t *OpenAPI_nssai_mapping_create(
    OpenAPI_snssai_t *mapped_snssai,
    OpenAPI_snssai_t *h_snssai
)
{
    OpenAPI_nssai_mapping_t *nssai_mapping_local_var = ogs_malloc(sizeof(OpenAPI_nssai_mapping_t));
    ogs_assert(nssai_mapping_local_var);

    nssai_mapping_local_var->mapped_snssai = mapped_snssai;
    nssai_mapping_local_var->h_snssai = h_snssai;

    return nssai_mapping_local_var;
}

void OpenAPI_nssai_mapping_free(OpenAPI_nssai_mapping_t *nssai_mapping)
{
    if (NULL == nssai_mapping) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_snssai_free(nssai_mapping->mapped_snssai);
    OpenAPI_snssai_free(nssai_mapping->h_snssai);
    ogs_free(nssai_mapping);
}

cJSON *OpenAPI_nssai_mapping_convertToJSON(OpenAPI_nssai_mapping_t *nssai_mapping)
{
    cJSON *item = NULL;

    if (nssai_mapping == NULL) {
        ogs_error("OpenAPI_nssai_mapping_convertToJSON() failed [NssaiMapping]");
        return NULL;
    }

    item = cJSON_CreateObject();
    cJSON *mapped_snssai_local_JSON = OpenAPI_snssai_convertToJSON(nssai_mapping->mapped_snssai);
    if (mapped_snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_nssai_mapping_convertToJSON() failed [mapped_snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "mappedSnssai", mapped_snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_nssai_mapping_convertToJSON() failed [mapped_snssai]");
        goto end;
    }

    cJSON *h_snssai_local_JSON = OpenAPI_snssai_convertToJSON(nssai_mapping->h_snssai);
    if (h_snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_nssai_mapping_convertToJSON() failed [h_snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "hSnssai", h_snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_nssai_mapping_convertToJSON() failed [h_snssai]");
        goto end;
    }

end:
    return item;
}

OpenAPI_nssai_mapping_t *OpenAPI_nssai_mapping_parseFromJSON(cJSON *nssai_mappingJSON)
{
    OpenAPI_nssai_mapping_t *nssai_mapping_local_var = NULL;
    cJSON *mapped_snssai = cJSON_GetObjectItemCaseSensitive(nssai_mappingJSON, "mappedSnssai");
    if (!mapped_snssai) {
        ogs_error("OpenAPI_nssai_mapping_parseFromJSON() failed [mapped_snssai]");
        goto end;
    }

    OpenAPI_snssai_t *mapped_snssai_local_nonprim = NULL;
    mapped_snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(mapped_snssai);

    cJSON *h_snssai = cJSON_GetObjectItemCaseSensitive(nssai_mappingJSON, "hSnssai");
    if (!h_snssai) {
        ogs_error("OpenAPI_nssai_mapping_parseFromJSON() failed [h_snssai]");
        goto end;
    }

    OpenAPI_snssai_t *h_snssai_local_nonprim = NULL;
    h_snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(h_snssai);

    nssai_mapping_local_var = OpenAPI_nssai_mapping_create (
        mapped_snssai_local_nonprim,
        h_snssai_local_nonprim
    );

    return nssai_mapping_local_var;
end:
    return NULL;
}

OpenAPI_nssai_mapping_t *OpenAPI_nssai_mapping_copy(OpenAPI_nssai_mapping_t *dst, OpenAPI_nssai_mapping_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nssai_mapping_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nssai_mapping_convertToJSON() failed");
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

    OpenAPI_nssai_mapping_free(dst);
    dst = OpenAPI_nssai_mapping_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

