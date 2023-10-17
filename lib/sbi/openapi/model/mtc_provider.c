
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mtc_provider.h"

OpenAPI_mtc_provider_t *OpenAPI_mtc_provider_create(
    char *mtc_provider_information,
    char *af_id
)
{
    OpenAPI_mtc_provider_t *mtc_provider_local_var = ogs_malloc(sizeof(OpenAPI_mtc_provider_t));
    ogs_assert(mtc_provider_local_var);

    mtc_provider_local_var->mtc_provider_information = mtc_provider_information;
    mtc_provider_local_var->af_id = af_id;

    return mtc_provider_local_var;
}

void OpenAPI_mtc_provider_free(OpenAPI_mtc_provider_t *mtc_provider)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == mtc_provider) {
        return;
    }
    if (mtc_provider->mtc_provider_information) {
        ogs_free(mtc_provider->mtc_provider_information);
        mtc_provider->mtc_provider_information = NULL;
    }
    if (mtc_provider->af_id) {
        ogs_free(mtc_provider->af_id);
        mtc_provider->af_id = NULL;
    }
    ogs_free(mtc_provider);
}

cJSON *OpenAPI_mtc_provider_convertToJSON(OpenAPI_mtc_provider_t *mtc_provider)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (mtc_provider == NULL) {
        ogs_error("OpenAPI_mtc_provider_convertToJSON() failed [MtcProvider]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (mtc_provider->mtc_provider_information) {
    if (cJSON_AddStringToObject(item, "mtcProviderInformation", mtc_provider->mtc_provider_information) == NULL) {
        ogs_error("OpenAPI_mtc_provider_convertToJSON() failed [mtc_provider_information]");
        goto end;
    }
    }

    if (mtc_provider->af_id) {
    if (cJSON_AddStringToObject(item, "afId", mtc_provider->af_id) == NULL) {
        ogs_error("OpenAPI_mtc_provider_convertToJSON() failed [af_id]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_mtc_provider_t *OpenAPI_mtc_provider_parseFromJSON(cJSON *mtc_providerJSON)
{
    OpenAPI_mtc_provider_t *mtc_provider_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *mtc_provider_information = NULL;
    cJSON *af_id = NULL;
    mtc_provider_information = cJSON_GetObjectItemCaseSensitive(mtc_providerJSON, "mtcProviderInformation");
    if (mtc_provider_information) {
    if (!cJSON_IsString(mtc_provider_information) && !cJSON_IsNull(mtc_provider_information)) {
        ogs_error("OpenAPI_mtc_provider_parseFromJSON() failed [mtc_provider_information]");
        goto end;
    }
    }

    af_id = cJSON_GetObjectItemCaseSensitive(mtc_providerJSON, "afId");
    if (af_id) {
    if (!cJSON_IsString(af_id) && !cJSON_IsNull(af_id)) {
        ogs_error("OpenAPI_mtc_provider_parseFromJSON() failed [af_id]");
        goto end;
    }
    }

    mtc_provider_local_var = OpenAPI_mtc_provider_create (
        mtc_provider_information && !cJSON_IsNull(mtc_provider_information) ? ogs_strdup(mtc_provider_information->valuestring) : NULL,
        af_id && !cJSON_IsNull(af_id) ? ogs_strdup(af_id->valuestring) : NULL
    );

    return mtc_provider_local_var;
end:
    return NULL;
}

OpenAPI_mtc_provider_t *OpenAPI_mtc_provider_copy(OpenAPI_mtc_provider_t *dst, OpenAPI_mtc_provider_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_mtc_provider_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_mtc_provider_convertToJSON() failed");
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

    OpenAPI_mtc_provider_free(dst);
    dst = OpenAPI_mtc_provider_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

