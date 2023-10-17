
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "redirect_response.h"

OpenAPI_redirect_response_t *OpenAPI_redirect_response_create(
    char *cause,
    char *target_scp,
    char *target_sepp
)
{
    OpenAPI_redirect_response_t *redirect_response_local_var = ogs_malloc(sizeof(OpenAPI_redirect_response_t));
    ogs_assert(redirect_response_local_var);

    redirect_response_local_var->cause = cause;
    redirect_response_local_var->target_scp = target_scp;
    redirect_response_local_var->target_sepp = target_sepp;

    return redirect_response_local_var;
}

void OpenAPI_redirect_response_free(OpenAPI_redirect_response_t *redirect_response)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == redirect_response) {
        return;
    }
    if (redirect_response->cause) {
        ogs_free(redirect_response->cause);
        redirect_response->cause = NULL;
    }
    if (redirect_response->target_scp) {
        ogs_free(redirect_response->target_scp);
        redirect_response->target_scp = NULL;
    }
    if (redirect_response->target_sepp) {
        ogs_free(redirect_response->target_sepp);
        redirect_response->target_sepp = NULL;
    }
    ogs_free(redirect_response);
}

cJSON *OpenAPI_redirect_response_convertToJSON(OpenAPI_redirect_response_t *redirect_response)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (redirect_response == NULL) {
        ogs_error("OpenAPI_redirect_response_convertToJSON() failed [RedirectResponse]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (redirect_response->cause) {
    if (cJSON_AddStringToObject(item, "cause", redirect_response->cause) == NULL) {
        ogs_error("OpenAPI_redirect_response_convertToJSON() failed [cause]");
        goto end;
    }
    }

    if (redirect_response->target_scp) {
    if (cJSON_AddStringToObject(item, "targetScp", redirect_response->target_scp) == NULL) {
        ogs_error("OpenAPI_redirect_response_convertToJSON() failed [target_scp]");
        goto end;
    }
    }

    if (redirect_response->target_sepp) {
    if (cJSON_AddStringToObject(item, "targetSepp", redirect_response->target_sepp) == NULL) {
        ogs_error("OpenAPI_redirect_response_convertToJSON() failed [target_sepp]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_redirect_response_t *OpenAPI_redirect_response_parseFromJSON(cJSON *redirect_responseJSON)
{
    OpenAPI_redirect_response_t *redirect_response_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *cause = NULL;
    cJSON *target_scp = NULL;
    cJSON *target_sepp = NULL;
    cause = cJSON_GetObjectItemCaseSensitive(redirect_responseJSON, "cause");
    if (cause) {
    if (!cJSON_IsString(cause) && !cJSON_IsNull(cause)) {
        ogs_error("OpenAPI_redirect_response_parseFromJSON() failed [cause]");
        goto end;
    }
    }

    target_scp = cJSON_GetObjectItemCaseSensitive(redirect_responseJSON, "targetScp");
    if (target_scp) {
    if (!cJSON_IsString(target_scp) && !cJSON_IsNull(target_scp)) {
        ogs_error("OpenAPI_redirect_response_parseFromJSON() failed [target_scp]");
        goto end;
    }
    }

    target_sepp = cJSON_GetObjectItemCaseSensitive(redirect_responseJSON, "targetSepp");
    if (target_sepp) {
    if (!cJSON_IsString(target_sepp) && !cJSON_IsNull(target_sepp)) {
        ogs_error("OpenAPI_redirect_response_parseFromJSON() failed [target_sepp]");
        goto end;
    }
    }

    redirect_response_local_var = OpenAPI_redirect_response_create (
        cause && !cJSON_IsNull(cause) ? ogs_strdup(cause->valuestring) : NULL,
        target_scp && !cJSON_IsNull(target_scp) ? ogs_strdup(target_scp->valuestring) : NULL,
        target_sepp && !cJSON_IsNull(target_sepp) ? ogs_strdup(target_sepp->valuestring) : NULL
    );

    return redirect_response_local_var;
end:
    return NULL;
}

OpenAPI_redirect_response_t *OpenAPI_redirect_response_copy(OpenAPI_redirect_response_t *dst, OpenAPI_redirect_response_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_redirect_response_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_redirect_response_convertToJSON() failed");
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

    OpenAPI_redirect_response_free(dst);
    dst = OpenAPI_redirect_response_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

