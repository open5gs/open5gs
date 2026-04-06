
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "redirect_response_1.h"

OpenAPI_redirect_response_1_t *OpenAPI_redirect_response_1_create(
    char *cause,
    char *target_scp,
    char *target_sepp
)
{
    OpenAPI_redirect_response_1_t *redirect_response_1_local_var = ogs_malloc(sizeof(OpenAPI_redirect_response_1_t));
    ogs_assert(redirect_response_1_local_var);

    redirect_response_1_local_var->cause = cause;
    redirect_response_1_local_var->target_scp = target_scp;
    redirect_response_1_local_var->target_sepp = target_sepp;

    return redirect_response_1_local_var;
}

void OpenAPI_redirect_response_1_free(OpenAPI_redirect_response_1_t *redirect_response_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == redirect_response_1) {
        return;
    }
    if (redirect_response_1->cause) {
        ogs_free(redirect_response_1->cause);
        redirect_response_1->cause = NULL;
    }
    if (redirect_response_1->target_scp) {
        ogs_free(redirect_response_1->target_scp);
        redirect_response_1->target_scp = NULL;
    }
    if (redirect_response_1->target_sepp) {
        ogs_free(redirect_response_1->target_sepp);
        redirect_response_1->target_sepp = NULL;
    }
    ogs_free(redirect_response_1);
}

cJSON *OpenAPI_redirect_response_1_convertToJSON(OpenAPI_redirect_response_1_t *redirect_response_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (redirect_response_1 == NULL) {
        ogs_error("OpenAPI_redirect_response_1_convertToJSON() failed [RedirectResponse_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (redirect_response_1->cause) {
    if (cJSON_AddStringToObject(item, "cause", redirect_response_1->cause) == NULL) {
        ogs_error("OpenAPI_redirect_response_1_convertToJSON() failed [cause]");
        goto end;
    }
    }

    if (redirect_response_1->target_scp) {
    if (cJSON_AddStringToObject(item, "targetScp", redirect_response_1->target_scp) == NULL) {
        ogs_error("OpenAPI_redirect_response_1_convertToJSON() failed [target_scp]");
        goto end;
    }
    }

    if (redirect_response_1->target_sepp) {
    if (cJSON_AddStringToObject(item, "targetSepp", redirect_response_1->target_sepp) == NULL) {
        ogs_error("OpenAPI_redirect_response_1_convertToJSON() failed [target_sepp]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_redirect_response_1_t *OpenAPI_redirect_response_1_parseFromJSON(cJSON *redirect_response_1JSON)
{
    OpenAPI_redirect_response_1_t *redirect_response_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *cause = NULL;
    cJSON *target_scp = NULL;
    cJSON *target_sepp = NULL;
    cause = cJSON_GetObjectItemCaseSensitive(redirect_response_1JSON, "cause");
    if (cause) {
    if (!cJSON_IsString(cause) && !cJSON_IsNull(cause)) {
        ogs_error("OpenAPI_redirect_response_1_parseFromJSON() failed [cause]");
        goto end;
    }
    }

    target_scp = cJSON_GetObjectItemCaseSensitive(redirect_response_1JSON, "targetScp");
    if (target_scp) {
    if (!cJSON_IsString(target_scp) && !cJSON_IsNull(target_scp)) {
        ogs_error("OpenAPI_redirect_response_1_parseFromJSON() failed [target_scp]");
        goto end;
    }
    }

    target_sepp = cJSON_GetObjectItemCaseSensitive(redirect_response_1JSON, "targetSepp");
    if (target_sepp) {
    if (!cJSON_IsString(target_sepp) && !cJSON_IsNull(target_sepp)) {
        ogs_error("OpenAPI_redirect_response_1_parseFromJSON() failed [target_sepp]");
        goto end;
    }
    }

    redirect_response_1_local_var = OpenAPI_redirect_response_1_create (
        cause && !cJSON_IsNull(cause) ? ogs_strdup(cause->valuestring) : NULL,
        target_scp && !cJSON_IsNull(target_scp) ? ogs_strdup(target_scp->valuestring) : NULL,
        target_sepp && !cJSON_IsNull(target_sepp) ? ogs_strdup(target_sepp->valuestring) : NULL
    );

    return redirect_response_1_local_var;
end:
    return NULL;
}

OpenAPI_redirect_response_1_t *OpenAPI_redirect_response_1_copy(OpenAPI_redirect_response_1_t *dst, OpenAPI_redirect_response_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_redirect_response_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_redirect_response_1_convertToJSON() failed");
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

    OpenAPI_redirect_response_1_free(dst);
    dst = OpenAPI_redirect_response_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

