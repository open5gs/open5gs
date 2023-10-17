
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_authentication_ctx_5g_auth_data.h"

OpenAPI_ue_authentication_ctx_5g_auth_data_t *OpenAPI_ue_authentication_ctx_5g_auth_data_create(
    char *rand,
    char *hxres_star,
    char *autn
)
{
    OpenAPI_ue_authentication_ctx_5g_auth_data_t *ue_authentication_ctx_5g_auth_data_local_var = ogs_malloc(sizeof(OpenAPI_ue_authentication_ctx_5g_auth_data_t));
    ogs_assert(ue_authentication_ctx_5g_auth_data_local_var);

    ue_authentication_ctx_5g_auth_data_local_var->rand = rand;
    ue_authentication_ctx_5g_auth_data_local_var->hxres_star = hxres_star;
    ue_authentication_ctx_5g_auth_data_local_var->autn = autn;

    return ue_authentication_ctx_5g_auth_data_local_var;
}

void OpenAPI_ue_authentication_ctx_5g_auth_data_free(OpenAPI_ue_authentication_ctx_5g_auth_data_t *ue_authentication_ctx_5g_auth_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ue_authentication_ctx_5g_auth_data) {
        return;
    }
    if (ue_authentication_ctx_5g_auth_data->rand) {
        ogs_free(ue_authentication_ctx_5g_auth_data->rand);
        ue_authentication_ctx_5g_auth_data->rand = NULL;
    }
    if (ue_authentication_ctx_5g_auth_data->hxres_star) {
        ogs_free(ue_authentication_ctx_5g_auth_data->hxres_star);
        ue_authentication_ctx_5g_auth_data->hxres_star = NULL;
    }
    if (ue_authentication_ctx_5g_auth_data->autn) {
        ogs_free(ue_authentication_ctx_5g_auth_data->autn);
        ue_authentication_ctx_5g_auth_data->autn = NULL;
    }
    ogs_free(ue_authentication_ctx_5g_auth_data);
}

cJSON *OpenAPI_ue_authentication_ctx_5g_auth_data_convertToJSON(OpenAPI_ue_authentication_ctx_5g_auth_data_t *ue_authentication_ctx_5g_auth_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ue_authentication_ctx_5g_auth_data == NULL) {
        ogs_error("OpenAPI_ue_authentication_ctx_5g_auth_data_convertToJSON() failed [UEAuthenticationCtx_5gAuthData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!ue_authentication_ctx_5g_auth_data->rand) {
        ogs_error("OpenAPI_ue_authentication_ctx_5g_auth_data_convertToJSON() failed [rand]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "rand", ue_authentication_ctx_5g_auth_data->rand) == NULL) {
        ogs_error("OpenAPI_ue_authentication_ctx_5g_auth_data_convertToJSON() failed [rand]");
        goto end;
    }

    if (!ue_authentication_ctx_5g_auth_data->hxres_star) {
        ogs_error("OpenAPI_ue_authentication_ctx_5g_auth_data_convertToJSON() failed [hxres_star]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "hxresStar", ue_authentication_ctx_5g_auth_data->hxres_star) == NULL) {
        ogs_error("OpenAPI_ue_authentication_ctx_5g_auth_data_convertToJSON() failed [hxres_star]");
        goto end;
    }

    if (!ue_authentication_ctx_5g_auth_data->autn) {
        ogs_error("OpenAPI_ue_authentication_ctx_5g_auth_data_convertToJSON() failed [autn]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "autn", ue_authentication_ctx_5g_auth_data->autn) == NULL) {
        ogs_error("OpenAPI_ue_authentication_ctx_5g_auth_data_convertToJSON() failed [autn]");
        goto end;
    }

end:
    return item;
}

OpenAPI_ue_authentication_ctx_5g_auth_data_t *OpenAPI_ue_authentication_ctx_5g_auth_data_parseFromJSON(cJSON *ue_authentication_ctx_5g_auth_dataJSON)
{
    OpenAPI_ue_authentication_ctx_5g_auth_data_t *ue_authentication_ctx_5g_auth_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *rand = NULL;
    cJSON *hxres_star = NULL;
    cJSON *autn = NULL;
    rand = cJSON_GetObjectItemCaseSensitive(ue_authentication_ctx_5g_auth_dataJSON, "rand");
    if (!rand) {
        ogs_error("OpenAPI_ue_authentication_ctx_5g_auth_data_parseFromJSON() failed [rand]");
        goto end;
    }
    if (!cJSON_IsString(rand)) {
        ogs_error("OpenAPI_ue_authentication_ctx_5g_auth_data_parseFromJSON() failed [rand]");
        goto end;
    }

    hxres_star = cJSON_GetObjectItemCaseSensitive(ue_authentication_ctx_5g_auth_dataJSON, "hxresStar");
    if (!hxres_star) {
        ogs_error("OpenAPI_ue_authentication_ctx_5g_auth_data_parseFromJSON() failed [hxres_star]");
        goto end;
    }
    if (!cJSON_IsString(hxres_star)) {
        ogs_error("OpenAPI_ue_authentication_ctx_5g_auth_data_parseFromJSON() failed [hxres_star]");
        goto end;
    }

    autn = cJSON_GetObjectItemCaseSensitive(ue_authentication_ctx_5g_auth_dataJSON, "autn");
    if (!autn) {
        ogs_error("OpenAPI_ue_authentication_ctx_5g_auth_data_parseFromJSON() failed [autn]");
        goto end;
    }
    if (!cJSON_IsString(autn)) {
        ogs_error("OpenAPI_ue_authentication_ctx_5g_auth_data_parseFromJSON() failed [autn]");
        goto end;
    }

    ue_authentication_ctx_5g_auth_data_local_var = OpenAPI_ue_authentication_ctx_5g_auth_data_create (
        ogs_strdup(rand->valuestring),
        ogs_strdup(hxres_star->valuestring),
        ogs_strdup(autn->valuestring)
    );

    return ue_authentication_ctx_5g_auth_data_local_var;
end:
    return NULL;
}

OpenAPI_ue_authentication_ctx_5g_auth_data_t *OpenAPI_ue_authentication_ctx_5g_auth_data_copy(OpenAPI_ue_authentication_ctx_5g_auth_data_t *dst, OpenAPI_ue_authentication_ctx_5g_auth_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ue_authentication_ctx_5g_auth_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ue_authentication_ctx_5g_auth_data_convertToJSON() failed");
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

    OpenAPI_ue_authentication_ctx_5g_auth_data_free(dst);
    dst = OpenAPI_ue_authentication_ctx_5g_auth_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

