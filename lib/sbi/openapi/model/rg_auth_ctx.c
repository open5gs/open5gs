
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "rg_auth_ctx.h"

OpenAPI_rg_auth_ctx_t *OpenAPI_rg_auth_ctx_create(
    OpenAPI_auth_result_e auth_result,
    char *supi,
    int auth_ind
    )
{
    OpenAPI_rg_auth_ctx_t *rg_auth_ctx_local_var = OpenAPI_malloc(sizeof(OpenAPI_rg_auth_ctx_t));
    if (!rg_auth_ctx_local_var) {
        return NULL;
    }
    rg_auth_ctx_local_var->auth_result = auth_result;
    rg_auth_ctx_local_var->supi = supi;
    rg_auth_ctx_local_var->auth_ind = auth_ind;

    return rg_auth_ctx_local_var;
}

void OpenAPI_rg_auth_ctx_free(OpenAPI_rg_auth_ctx_t *rg_auth_ctx)
{
    if (NULL == rg_auth_ctx) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(rg_auth_ctx->supi);
    ogs_free(rg_auth_ctx);
}

cJSON *OpenAPI_rg_auth_ctx_convertToJSON(OpenAPI_rg_auth_ctx_t *rg_auth_ctx)
{
    cJSON *item = NULL;

    if (rg_auth_ctx == NULL) {
        ogs_error("OpenAPI_rg_auth_ctx_convertToJSON() failed [RgAuthCtx]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!rg_auth_ctx->auth_result) {
        ogs_error("OpenAPI_rg_auth_ctx_convertToJSON() failed [auth_result]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "authResult", OpenAPI_auth_result_ToString(rg_auth_ctx->auth_result)) == NULL) {
        ogs_error("OpenAPI_rg_auth_ctx_convertToJSON() failed [auth_result]");
        goto end;
    }

    if (rg_auth_ctx->supi) {
        if (cJSON_AddStringToObject(item, "supi", rg_auth_ctx->supi) == NULL) {
            ogs_error("OpenAPI_rg_auth_ctx_convertToJSON() failed [supi]");
            goto end;
        }
    }

    if (rg_auth_ctx->auth_ind) {
        if (cJSON_AddBoolToObject(item, "authInd", rg_auth_ctx->auth_ind) == NULL) {
            ogs_error("OpenAPI_rg_auth_ctx_convertToJSON() failed [auth_ind]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_rg_auth_ctx_t *OpenAPI_rg_auth_ctx_parseFromJSON(cJSON *rg_auth_ctxJSON)
{
    OpenAPI_rg_auth_ctx_t *rg_auth_ctx_local_var = NULL;
    cJSON *auth_result = cJSON_GetObjectItemCaseSensitive(rg_auth_ctxJSON, "authResult");
    if (!auth_result) {
        ogs_error("OpenAPI_rg_auth_ctx_parseFromJSON() failed [auth_result]");
        goto end;
    }

    OpenAPI_auth_result_e auth_resultVariable;

    if (!cJSON_IsString(auth_result)) {
        ogs_error("OpenAPI_rg_auth_ctx_parseFromJSON() failed [auth_result]");
        goto end;
    }
    auth_resultVariable = OpenAPI_auth_result_FromString(auth_result->valuestring);

    cJSON *supi = cJSON_GetObjectItemCaseSensitive(rg_auth_ctxJSON, "supi");

    if (supi) {
        if (!cJSON_IsString(supi)) {
            ogs_error("OpenAPI_rg_auth_ctx_parseFromJSON() failed [supi]");
            goto end;
        }
    }

    cJSON *auth_ind = cJSON_GetObjectItemCaseSensitive(rg_auth_ctxJSON, "authInd");

    if (auth_ind) {
        if (!cJSON_IsBool(auth_ind)) {
            ogs_error("OpenAPI_rg_auth_ctx_parseFromJSON() failed [auth_ind]");
            goto end;
        }
    }

    rg_auth_ctx_local_var = OpenAPI_rg_auth_ctx_create (
        auth_resultVariable,
        supi ? ogs_strdup(supi->valuestring) : NULL,
        auth_ind ? auth_ind->valueint : 0
        );

    return rg_auth_ctx_local_var;
end:
    return NULL;
}

OpenAPI_rg_auth_ctx_t *OpenAPI_rg_auth_ctx_copy(OpenAPI_rg_auth_ctx_t *dst, OpenAPI_rg_auth_ctx_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_rg_auth_ctx_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_rg_auth_ctx_convertToJSON() failed");
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

    OpenAPI_rg_auth_ctx_free(dst);
    dst = OpenAPI_rg_auth_ctx_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

