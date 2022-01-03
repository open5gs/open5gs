
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "rg_authentication_info.h"

OpenAPI_rg_authentication_info_t *OpenAPI_rg_authentication_info_create(
    char *suci,
    int authenticated_ind,
    char *supported_features
)
{
    OpenAPI_rg_authentication_info_t *rg_authentication_info_local_var = ogs_malloc(sizeof(OpenAPI_rg_authentication_info_t));
    ogs_assert(rg_authentication_info_local_var);

    rg_authentication_info_local_var->suci = suci;
    rg_authentication_info_local_var->authenticated_ind = authenticated_ind;
    rg_authentication_info_local_var->supported_features = supported_features;

    return rg_authentication_info_local_var;
}

void OpenAPI_rg_authentication_info_free(OpenAPI_rg_authentication_info_t *rg_authentication_info)
{
    if (NULL == rg_authentication_info) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(rg_authentication_info->suci);
    ogs_free(rg_authentication_info->supported_features);
    ogs_free(rg_authentication_info);
}

cJSON *OpenAPI_rg_authentication_info_convertToJSON(OpenAPI_rg_authentication_info_t *rg_authentication_info)
{
    cJSON *item = NULL;

    if (rg_authentication_info == NULL) {
        ogs_error("OpenAPI_rg_authentication_info_convertToJSON() failed [RgAuthenticationInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddStringToObject(item, "suci", rg_authentication_info->suci) == NULL) {
        ogs_error("OpenAPI_rg_authentication_info_convertToJSON() failed [suci]");
        goto end;
    }

    if (cJSON_AddBoolToObject(item, "authenticatedInd", rg_authentication_info->authenticated_ind) == NULL) {
        ogs_error("OpenAPI_rg_authentication_info_convertToJSON() failed [authenticated_ind]");
        goto end;
    }

    if (rg_authentication_info->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", rg_authentication_info->supported_features) == NULL) {
        ogs_error("OpenAPI_rg_authentication_info_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_rg_authentication_info_t *OpenAPI_rg_authentication_info_parseFromJSON(cJSON *rg_authentication_infoJSON)
{
    OpenAPI_rg_authentication_info_t *rg_authentication_info_local_var = NULL;
    cJSON *suci = cJSON_GetObjectItemCaseSensitive(rg_authentication_infoJSON, "suci");
    if (!suci) {
        ogs_error("OpenAPI_rg_authentication_info_parseFromJSON() failed [suci]");
        goto end;
    }

    if (!cJSON_IsString(suci)) {
        ogs_error("OpenAPI_rg_authentication_info_parseFromJSON() failed [suci]");
        goto end;
    }

    cJSON *authenticated_ind = cJSON_GetObjectItemCaseSensitive(rg_authentication_infoJSON, "authenticatedInd");
    if (!authenticated_ind) {
        ogs_error("OpenAPI_rg_authentication_info_parseFromJSON() failed [authenticated_ind]");
        goto end;
    }

    if (!cJSON_IsBool(authenticated_ind)) {
        ogs_error("OpenAPI_rg_authentication_info_parseFromJSON() failed [authenticated_ind]");
        goto end;
    }

    cJSON *supported_features = cJSON_GetObjectItemCaseSensitive(rg_authentication_infoJSON, "supportedFeatures");

    if (supported_features) {
    if (!cJSON_IsString(supported_features)) {
        ogs_error("OpenAPI_rg_authentication_info_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    rg_authentication_info_local_var = OpenAPI_rg_authentication_info_create (
        ogs_strdup(suci->valuestring),
        
        authenticated_ind->valueint,
        supported_features ? ogs_strdup(supported_features->valuestring) : NULL
    );

    return rg_authentication_info_local_var;
end:
    return NULL;
}

OpenAPI_rg_authentication_info_t *OpenAPI_rg_authentication_info_copy(OpenAPI_rg_authentication_info_t *dst, OpenAPI_rg_authentication_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_rg_authentication_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_rg_authentication_info_convertToJSON() failed");
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

    OpenAPI_rg_authentication_info_free(dst);
    dst = OpenAPI_rg_authentication_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

