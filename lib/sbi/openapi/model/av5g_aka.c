
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "av5g_aka.h"

OpenAPI_av5g_aka_t *OpenAPI_av5g_aka_create(
    char *rand,
    char *hxres_star,
    char *autn
    )
{
    OpenAPI_av5g_aka_t *av5g_aka_local_var = OpenAPI_malloc(sizeof(OpenAPI_av5g_aka_t));
    if (!av5g_aka_local_var) {
        return NULL;
    }
    av5g_aka_local_var->rand = rand;
    av5g_aka_local_var->hxres_star = hxres_star;
    av5g_aka_local_var->autn = autn;

    return av5g_aka_local_var;
}

void OpenAPI_av5g_aka_free(OpenAPI_av5g_aka_t *av5g_aka)
{
    if (NULL == av5g_aka) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(av5g_aka->rand);
    ogs_free(av5g_aka->hxres_star);
    ogs_free(av5g_aka->autn);
    ogs_free(av5g_aka);
}

cJSON *OpenAPI_av5g_aka_convertToJSON(OpenAPI_av5g_aka_t *av5g_aka)
{
    cJSON *item = NULL;

    if (av5g_aka == NULL) {
        ogs_error("OpenAPI_av5g_aka_convertToJSON() failed [Av5gAka]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!av5g_aka->rand) {
        ogs_error("OpenAPI_av5g_aka_convertToJSON() failed [rand]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "rand", av5g_aka->rand) == NULL) {
        ogs_error("OpenAPI_av5g_aka_convertToJSON() failed [rand]");
        goto end;
    }

    if (!av5g_aka->hxres_star) {
        ogs_error("OpenAPI_av5g_aka_convertToJSON() failed [hxres_star]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "hxresStar", av5g_aka->hxres_star) == NULL) {
        ogs_error("OpenAPI_av5g_aka_convertToJSON() failed [hxres_star]");
        goto end;
    }

    if (!av5g_aka->autn) {
        ogs_error("OpenAPI_av5g_aka_convertToJSON() failed [autn]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "autn", av5g_aka->autn) == NULL) {
        ogs_error("OpenAPI_av5g_aka_convertToJSON() failed [autn]");
        goto end;
    }

end:
    return item;
}

OpenAPI_av5g_aka_t *OpenAPI_av5g_aka_parseFromJSON(cJSON *av5g_akaJSON)
{
    OpenAPI_av5g_aka_t *av5g_aka_local_var = NULL;
    cJSON *rand = cJSON_GetObjectItemCaseSensitive(av5g_akaJSON, "rand");
    if (!rand) {
        ogs_error("OpenAPI_av5g_aka_parseFromJSON() failed [rand]");
        goto end;
    }


    if (!cJSON_IsString(rand)) {
        ogs_error("OpenAPI_av5g_aka_parseFromJSON() failed [rand]");
        goto end;
    }

    cJSON *hxres_star = cJSON_GetObjectItemCaseSensitive(av5g_akaJSON, "hxresStar");
    if (!hxres_star) {
        ogs_error("OpenAPI_av5g_aka_parseFromJSON() failed [hxres_star]");
        goto end;
    }


    if (!cJSON_IsString(hxres_star)) {
        ogs_error("OpenAPI_av5g_aka_parseFromJSON() failed [hxres_star]");
        goto end;
    }

    cJSON *autn = cJSON_GetObjectItemCaseSensitive(av5g_akaJSON, "autn");
    if (!autn) {
        ogs_error("OpenAPI_av5g_aka_parseFromJSON() failed [autn]");
        goto end;
    }


    if (!cJSON_IsString(autn)) {
        ogs_error("OpenAPI_av5g_aka_parseFromJSON() failed [autn]");
        goto end;
    }

    av5g_aka_local_var = OpenAPI_av5g_aka_create (
        ogs_strdup(rand->valuestring),
        ogs_strdup(hxres_star->valuestring),
        ogs_strdup(autn->valuestring)
        );

    return av5g_aka_local_var;
end:
    return NULL;
}

OpenAPI_av5g_aka_t *OpenAPI_av5g_aka_copy(OpenAPI_av5g_aka_t *dst, OpenAPI_av5g_aka_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_av5g_aka_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_av5g_aka_convertToJSON() failed");
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

    OpenAPI_av5g_aka_free(dst);
    dst = OpenAPI_av5g_aka_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

