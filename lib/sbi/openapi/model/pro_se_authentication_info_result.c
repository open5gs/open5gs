
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pro_se_authentication_info_result.h"

OpenAPI_pro_se_authentication_info_result_t *OpenAPI_pro_se_authentication_info_result_create(
    OpenAPI_auth_type_e auth_type,
    OpenAPI_pro_se_authentication_vectors_t *prose_authentication_vectors,
    char *supi,
    char *supported_features
)
{
    OpenAPI_pro_se_authentication_info_result_t *pro_se_authentication_info_result_local_var = ogs_malloc(sizeof(OpenAPI_pro_se_authentication_info_result_t));
    ogs_assert(pro_se_authentication_info_result_local_var);

    pro_se_authentication_info_result_local_var->auth_type = auth_type;
    pro_se_authentication_info_result_local_var->prose_authentication_vectors = prose_authentication_vectors;
    pro_se_authentication_info_result_local_var->supi = supi;
    pro_se_authentication_info_result_local_var->supported_features = supported_features;

    return pro_se_authentication_info_result_local_var;
}

void OpenAPI_pro_se_authentication_info_result_free(OpenAPI_pro_se_authentication_info_result_t *pro_se_authentication_info_result)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pro_se_authentication_info_result) {
        return;
    }
    if (pro_se_authentication_info_result->prose_authentication_vectors) {
        OpenAPI_pro_se_authentication_vectors_free(pro_se_authentication_info_result->prose_authentication_vectors);
        pro_se_authentication_info_result->prose_authentication_vectors = NULL;
    }
    if (pro_se_authentication_info_result->supi) {
        ogs_free(pro_se_authentication_info_result->supi);
        pro_se_authentication_info_result->supi = NULL;
    }
    if (pro_se_authentication_info_result->supported_features) {
        ogs_free(pro_se_authentication_info_result->supported_features);
        pro_se_authentication_info_result->supported_features = NULL;
    }
    ogs_free(pro_se_authentication_info_result);
}

cJSON *OpenAPI_pro_se_authentication_info_result_convertToJSON(OpenAPI_pro_se_authentication_info_result_t *pro_se_authentication_info_result)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pro_se_authentication_info_result == NULL) {
        ogs_error("OpenAPI_pro_se_authentication_info_result_convertToJSON() failed [ProSeAuthenticationInfoResult]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (pro_se_authentication_info_result->auth_type == OpenAPI_auth_type_NULL) {
        ogs_error("OpenAPI_pro_se_authentication_info_result_convertToJSON() failed [auth_type]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "authType", OpenAPI_auth_type_ToString(pro_se_authentication_info_result->auth_type)) == NULL) {
        ogs_error("OpenAPI_pro_se_authentication_info_result_convertToJSON() failed [auth_type]");
        goto end;
    }

    if (pro_se_authentication_info_result->prose_authentication_vectors) {
    cJSON *prose_authentication_vectors_local_JSON = OpenAPI_pro_se_authentication_vectors_convertToJSON(pro_se_authentication_info_result->prose_authentication_vectors);
    if (prose_authentication_vectors_local_JSON == NULL) {
        ogs_error("OpenAPI_pro_se_authentication_info_result_convertToJSON() failed [prose_authentication_vectors]");
        goto end;
    }
    cJSON_AddItemToObject(item, "proseAuthenticationVectors", prose_authentication_vectors_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pro_se_authentication_info_result_convertToJSON() failed [prose_authentication_vectors]");
        goto end;
    }
    }

    if (pro_se_authentication_info_result->supi) {
    if (cJSON_AddStringToObject(item, "supi", pro_se_authentication_info_result->supi) == NULL) {
        ogs_error("OpenAPI_pro_se_authentication_info_result_convertToJSON() failed [supi]");
        goto end;
    }
    }

    if (pro_se_authentication_info_result->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", pro_se_authentication_info_result->supported_features) == NULL) {
        ogs_error("OpenAPI_pro_se_authentication_info_result_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_pro_se_authentication_info_result_t *OpenAPI_pro_se_authentication_info_result_parseFromJSON(cJSON *pro_se_authentication_info_resultJSON)
{
    OpenAPI_pro_se_authentication_info_result_t *pro_se_authentication_info_result_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *auth_type = NULL;
    OpenAPI_auth_type_e auth_typeVariable = 0;
    cJSON *prose_authentication_vectors = NULL;
    OpenAPI_pro_se_authentication_vectors_t *prose_authentication_vectors_local_nonprim = NULL;
    cJSON *supi = NULL;
    cJSON *supported_features = NULL;
    auth_type = cJSON_GetObjectItemCaseSensitive(pro_se_authentication_info_resultJSON, "authType");
    if (!auth_type) {
        ogs_error("OpenAPI_pro_se_authentication_info_result_parseFromJSON() failed [auth_type]");
        goto end;
    }
    if (!cJSON_IsString(auth_type)) {
        ogs_error("OpenAPI_pro_se_authentication_info_result_parseFromJSON() failed [auth_type]");
        goto end;
    }
    auth_typeVariable = OpenAPI_auth_type_FromString(auth_type->valuestring);

    prose_authentication_vectors = cJSON_GetObjectItemCaseSensitive(pro_se_authentication_info_resultJSON, "proseAuthenticationVectors");
    if (prose_authentication_vectors) {
    prose_authentication_vectors_local_nonprim = OpenAPI_pro_se_authentication_vectors_parseFromJSON(prose_authentication_vectors);
    if (!prose_authentication_vectors_local_nonprim) {
        ogs_error("OpenAPI_pro_se_authentication_vectors_parseFromJSON failed [prose_authentication_vectors]");
        goto end;
    }
    }

    supi = cJSON_GetObjectItemCaseSensitive(pro_se_authentication_info_resultJSON, "supi");
    if (supi) {
    if (!cJSON_IsString(supi) && !cJSON_IsNull(supi)) {
        ogs_error("OpenAPI_pro_se_authentication_info_result_parseFromJSON() failed [supi]");
        goto end;
    }
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(pro_se_authentication_info_resultJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_pro_se_authentication_info_result_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    pro_se_authentication_info_result_local_var = OpenAPI_pro_se_authentication_info_result_create (
        auth_typeVariable,
        prose_authentication_vectors ? prose_authentication_vectors_local_nonprim : NULL,
        supi && !cJSON_IsNull(supi) ? ogs_strdup(supi->valuestring) : NULL,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL
    );

    return pro_se_authentication_info_result_local_var;
end:
    if (prose_authentication_vectors_local_nonprim) {
        OpenAPI_pro_se_authentication_vectors_free(prose_authentication_vectors_local_nonprim);
        prose_authentication_vectors_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_pro_se_authentication_info_result_t *OpenAPI_pro_se_authentication_info_result_copy(OpenAPI_pro_se_authentication_info_result_t *dst, OpenAPI_pro_se_authentication_info_result_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pro_se_authentication_info_result_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pro_se_authentication_info_result_convertToJSON() failed");
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

    OpenAPI_pro_se_authentication_info_result_free(dst);
    dst = OpenAPI_pro_se_authentication_info_result_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

