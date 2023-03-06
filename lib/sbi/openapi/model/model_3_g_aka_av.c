
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "model_3_g_aka_av.h"

OpenAPI_model_3_g_aka_av_t *OpenAPI_model_3_g_aka_av_create(
    char *rand,
    char *xres,
    char *autn,
    char *ck,
    char *ik
)
{
    OpenAPI_model_3_g_aka_av_t *model_3_g_aka_av_local_var = ogs_malloc(sizeof(OpenAPI_model_3_g_aka_av_t));
    ogs_assert(model_3_g_aka_av_local_var);

    model_3_g_aka_av_local_var->rand = rand;
    model_3_g_aka_av_local_var->xres = xres;
    model_3_g_aka_av_local_var->autn = autn;
    model_3_g_aka_av_local_var->ck = ck;
    model_3_g_aka_av_local_var->ik = ik;

    return model_3_g_aka_av_local_var;
}

void OpenAPI_model_3_g_aka_av_free(OpenAPI_model_3_g_aka_av_t *model_3_g_aka_av)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == model_3_g_aka_av) {
        return;
    }
    if (model_3_g_aka_av->rand) {
        ogs_free(model_3_g_aka_av->rand);
        model_3_g_aka_av->rand = NULL;
    }
    if (model_3_g_aka_av->xres) {
        ogs_free(model_3_g_aka_av->xres);
        model_3_g_aka_av->xres = NULL;
    }
    if (model_3_g_aka_av->autn) {
        ogs_free(model_3_g_aka_av->autn);
        model_3_g_aka_av->autn = NULL;
    }
    if (model_3_g_aka_av->ck) {
        ogs_free(model_3_g_aka_av->ck);
        model_3_g_aka_av->ck = NULL;
    }
    if (model_3_g_aka_av->ik) {
        ogs_free(model_3_g_aka_av->ik);
        model_3_g_aka_av->ik = NULL;
    }
    ogs_free(model_3_g_aka_av);
}

cJSON *OpenAPI_model_3_g_aka_av_convertToJSON(OpenAPI_model_3_g_aka_av_t *model_3_g_aka_av)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (model_3_g_aka_av == NULL) {
        ogs_error("OpenAPI_model_3_g_aka_av_convertToJSON() failed [3GAkaAv]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!model_3_g_aka_av->rand) {
        ogs_error("OpenAPI_model_3_g_aka_av_convertToJSON() failed [rand]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "rand", model_3_g_aka_av->rand) == NULL) {
        ogs_error("OpenAPI_model_3_g_aka_av_convertToJSON() failed [rand]");
        goto end;
    }

    if (!model_3_g_aka_av->xres) {
        ogs_error("OpenAPI_model_3_g_aka_av_convertToJSON() failed [xres]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "xres", model_3_g_aka_av->xres) == NULL) {
        ogs_error("OpenAPI_model_3_g_aka_av_convertToJSON() failed [xres]");
        goto end;
    }

    if (!model_3_g_aka_av->autn) {
        ogs_error("OpenAPI_model_3_g_aka_av_convertToJSON() failed [autn]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "autn", model_3_g_aka_av->autn) == NULL) {
        ogs_error("OpenAPI_model_3_g_aka_av_convertToJSON() failed [autn]");
        goto end;
    }

    if (!model_3_g_aka_av->ck) {
        ogs_error("OpenAPI_model_3_g_aka_av_convertToJSON() failed [ck]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "ck", model_3_g_aka_av->ck) == NULL) {
        ogs_error("OpenAPI_model_3_g_aka_av_convertToJSON() failed [ck]");
        goto end;
    }

    if (!model_3_g_aka_av->ik) {
        ogs_error("OpenAPI_model_3_g_aka_av_convertToJSON() failed [ik]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "ik", model_3_g_aka_av->ik) == NULL) {
        ogs_error("OpenAPI_model_3_g_aka_av_convertToJSON() failed [ik]");
        goto end;
    }

end:
    return item;
}

OpenAPI_model_3_g_aka_av_t *OpenAPI_model_3_g_aka_av_parseFromJSON(cJSON *model_3_g_aka_avJSON)
{
    OpenAPI_model_3_g_aka_av_t *model_3_g_aka_av_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *rand = NULL;
    cJSON *xres = NULL;
    cJSON *autn = NULL;
    cJSON *ck = NULL;
    cJSON *ik = NULL;
    rand = cJSON_GetObjectItemCaseSensitive(model_3_g_aka_avJSON, "rand");
    if (!rand) {
        ogs_error("OpenAPI_model_3_g_aka_av_parseFromJSON() failed [rand]");
        goto end;
    }
    if (!cJSON_IsString(rand)) {
        ogs_error("OpenAPI_model_3_g_aka_av_parseFromJSON() failed [rand]");
        goto end;
    }

    xres = cJSON_GetObjectItemCaseSensitive(model_3_g_aka_avJSON, "xres");
    if (!xres) {
        ogs_error("OpenAPI_model_3_g_aka_av_parseFromJSON() failed [xres]");
        goto end;
    }
    if (!cJSON_IsString(xres)) {
        ogs_error("OpenAPI_model_3_g_aka_av_parseFromJSON() failed [xres]");
        goto end;
    }

    autn = cJSON_GetObjectItemCaseSensitive(model_3_g_aka_avJSON, "autn");
    if (!autn) {
        ogs_error("OpenAPI_model_3_g_aka_av_parseFromJSON() failed [autn]");
        goto end;
    }
    if (!cJSON_IsString(autn)) {
        ogs_error("OpenAPI_model_3_g_aka_av_parseFromJSON() failed [autn]");
        goto end;
    }

    ck = cJSON_GetObjectItemCaseSensitive(model_3_g_aka_avJSON, "ck");
    if (!ck) {
        ogs_error("OpenAPI_model_3_g_aka_av_parseFromJSON() failed [ck]");
        goto end;
    }
    if (!cJSON_IsString(ck)) {
        ogs_error("OpenAPI_model_3_g_aka_av_parseFromJSON() failed [ck]");
        goto end;
    }

    ik = cJSON_GetObjectItemCaseSensitive(model_3_g_aka_avJSON, "ik");
    if (!ik) {
        ogs_error("OpenAPI_model_3_g_aka_av_parseFromJSON() failed [ik]");
        goto end;
    }
    if (!cJSON_IsString(ik)) {
        ogs_error("OpenAPI_model_3_g_aka_av_parseFromJSON() failed [ik]");
        goto end;
    }

    model_3_g_aka_av_local_var = OpenAPI_model_3_g_aka_av_create (
        ogs_strdup(rand->valuestring),
        ogs_strdup(xres->valuestring),
        ogs_strdup(autn->valuestring),
        ogs_strdup(ck->valuestring),
        ogs_strdup(ik->valuestring)
    );

    return model_3_g_aka_av_local_var;
end:
    return NULL;
}

OpenAPI_model_3_g_aka_av_t *OpenAPI_model_3_g_aka_av_copy(OpenAPI_model_3_g_aka_av_t *dst, OpenAPI_model_3_g_aka_av_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_model_3_g_aka_av_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_model_3_g_aka_av_convertToJSON() failed");
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

    OpenAPI_model_3_g_aka_av_free(dst);
    dst = OpenAPI_model_3_g_aka_av_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

