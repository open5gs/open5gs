
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "configured_snssai.h"

OpenAPI_configured_snssai_t *OpenAPI_configured_snssai_create(
    OpenAPI_snssai_t *configured_snssai,
    OpenAPI_snssai_t *mapped_home_snssai
)
{
    OpenAPI_configured_snssai_t *configured_snssai_local_var = ogs_malloc(sizeof(OpenAPI_configured_snssai_t));
    ogs_assert(configured_snssai_local_var);

    configured_snssai_local_var->configured_snssai = configured_snssai;
    configured_snssai_local_var->mapped_home_snssai = mapped_home_snssai;

    return configured_snssai_local_var;
}

void OpenAPI_configured_snssai_free(OpenAPI_configured_snssai_t *configured_snssai)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == configured_snssai) {
        return;
    }
    if (configured_snssai->configured_snssai) {
        OpenAPI_snssai_free(configured_snssai->configured_snssai);
        configured_snssai->configured_snssai = NULL;
    }
    if (configured_snssai->mapped_home_snssai) {
        OpenAPI_snssai_free(configured_snssai->mapped_home_snssai);
        configured_snssai->mapped_home_snssai = NULL;
    }
    ogs_free(configured_snssai);
}

cJSON *OpenAPI_configured_snssai_convertToJSON(OpenAPI_configured_snssai_t *configured_snssai)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (configured_snssai == NULL) {
        ogs_error("OpenAPI_configured_snssai_convertToJSON() failed [ConfiguredSnssai]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!configured_snssai->configured_snssai) {
        ogs_error("OpenAPI_configured_snssai_convertToJSON() failed [configured_snssai]");
        return NULL;
    }
    cJSON *configured_snssai_local_JSON = OpenAPI_snssai_convertToJSON(configured_snssai->configured_snssai);
    if (configured_snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_configured_snssai_convertToJSON() failed [configured_snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "configuredSnssai", configured_snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_configured_snssai_convertToJSON() failed [configured_snssai]");
        goto end;
    }

    if (configured_snssai->mapped_home_snssai) {
    cJSON *mapped_home_snssai_local_JSON = OpenAPI_snssai_convertToJSON(configured_snssai->mapped_home_snssai);
    if (mapped_home_snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_configured_snssai_convertToJSON() failed [mapped_home_snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "mappedHomeSnssai", mapped_home_snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_configured_snssai_convertToJSON() failed [mapped_home_snssai]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_configured_snssai_t *OpenAPI_configured_snssai_parseFromJSON(cJSON *configured_snssaiJSON)
{
    OpenAPI_configured_snssai_t *configured_snssai_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *configured_snssai = NULL;
    OpenAPI_snssai_t *configured_snssai_local_nonprim = NULL;
    cJSON *mapped_home_snssai = NULL;
    OpenAPI_snssai_t *mapped_home_snssai_local_nonprim = NULL;
    configured_snssai = cJSON_GetObjectItemCaseSensitive(configured_snssaiJSON, "configuredSnssai");
    if (!configured_snssai) {
        ogs_error("OpenAPI_configured_snssai_parseFromJSON() failed [configured_snssai]");
        goto end;
    }
    configured_snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(configured_snssai);
    if (!configured_snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [configured_snssai]");
        goto end;
    }

    mapped_home_snssai = cJSON_GetObjectItemCaseSensitive(configured_snssaiJSON, "mappedHomeSnssai");
    if (mapped_home_snssai) {
    mapped_home_snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(mapped_home_snssai);
    if (!mapped_home_snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [mapped_home_snssai]");
        goto end;
    }
    }

    configured_snssai_local_var = OpenAPI_configured_snssai_create (
        configured_snssai_local_nonprim,
        mapped_home_snssai ? mapped_home_snssai_local_nonprim : NULL
    );

    return configured_snssai_local_var;
end:
    if (configured_snssai_local_nonprim) {
        OpenAPI_snssai_free(configured_snssai_local_nonprim);
        configured_snssai_local_nonprim = NULL;
    }
    if (mapped_home_snssai_local_nonprim) {
        OpenAPI_snssai_free(mapped_home_snssai_local_nonprim);
        mapped_home_snssai_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_configured_snssai_t *OpenAPI_configured_snssai_copy(OpenAPI_configured_snssai_t *dst, OpenAPI_configured_snssai_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_configured_snssai_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_configured_snssai_convertToJSON() failed");
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

    OpenAPI_configured_snssai_free(dst);
    dst = OpenAPI_configured_snssai_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

