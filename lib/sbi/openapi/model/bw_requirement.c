
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "bw_requirement.h"

OpenAPI_bw_requirement_t *OpenAPI_bw_requirement_create(
    char *app_id,
    char *mar_bw_dl,
    char *mar_bw_ul,
    char *mir_bw_dl,
    char *mir_bw_ul
)
{
    OpenAPI_bw_requirement_t *bw_requirement_local_var = ogs_malloc(sizeof(OpenAPI_bw_requirement_t));
    ogs_assert(bw_requirement_local_var);

    bw_requirement_local_var->app_id = app_id;
    bw_requirement_local_var->mar_bw_dl = mar_bw_dl;
    bw_requirement_local_var->mar_bw_ul = mar_bw_ul;
    bw_requirement_local_var->mir_bw_dl = mir_bw_dl;
    bw_requirement_local_var->mir_bw_ul = mir_bw_ul;

    return bw_requirement_local_var;
}

void OpenAPI_bw_requirement_free(OpenAPI_bw_requirement_t *bw_requirement)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == bw_requirement) {
        return;
    }
    if (bw_requirement->app_id) {
        ogs_free(bw_requirement->app_id);
        bw_requirement->app_id = NULL;
    }
    if (bw_requirement->mar_bw_dl) {
        ogs_free(bw_requirement->mar_bw_dl);
        bw_requirement->mar_bw_dl = NULL;
    }
    if (bw_requirement->mar_bw_ul) {
        ogs_free(bw_requirement->mar_bw_ul);
        bw_requirement->mar_bw_ul = NULL;
    }
    if (bw_requirement->mir_bw_dl) {
        ogs_free(bw_requirement->mir_bw_dl);
        bw_requirement->mir_bw_dl = NULL;
    }
    if (bw_requirement->mir_bw_ul) {
        ogs_free(bw_requirement->mir_bw_ul);
        bw_requirement->mir_bw_ul = NULL;
    }
    ogs_free(bw_requirement);
}

cJSON *OpenAPI_bw_requirement_convertToJSON(OpenAPI_bw_requirement_t *bw_requirement)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (bw_requirement == NULL) {
        ogs_error("OpenAPI_bw_requirement_convertToJSON() failed [BwRequirement]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!bw_requirement->app_id) {
        ogs_error("OpenAPI_bw_requirement_convertToJSON() failed [app_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "appId", bw_requirement->app_id) == NULL) {
        ogs_error("OpenAPI_bw_requirement_convertToJSON() failed [app_id]");
        goto end;
    }

    if (bw_requirement->mar_bw_dl) {
    if (cJSON_AddStringToObject(item, "marBwDl", bw_requirement->mar_bw_dl) == NULL) {
        ogs_error("OpenAPI_bw_requirement_convertToJSON() failed [mar_bw_dl]");
        goto end;
    }
    }

    if (bw_requirement->mar_bw_ul) {
    if (cJSON_AddStringToObject(item, "marBwUl", bw_requirement->mar_bw_ul) == NULL) {
        ogs_error("OpenAPI_bw_requirement_convertToJSON() failed [mar_bw_ul]");
        goto end;
    }
    }

    if (bw_requirement->mir_bw_dl) {
    if (cJSON_AddStringToObject(item, "mirBwDl", bw_requirement->mir_bw_dl) == NULL) {
        ogs_error("OpenAPI_bw_requirement_convertToJSON() failed [mir_bw_dl]");
        goto end;
    }
    }

    if (bw_requirement->mir_bw_ul) {
    if (cJSON_AddStringToObject(item, "mirBwUl", bw_requirement->mir_bw_ul) == NULL) {
        ogs_error("OpenAPI_bw_requirement_convertToJSON() failed [mir_bw_ul]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_bw_requirement_t *OpenAPI_bw_requirement_parseFromJSON(cJSON *bw_requirementJSON)
{
    OpenAPI_bw_requirement_t *bw_requirement_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *app_id = NULL;
    cJSON *mar_bw_dl = NULL;
    cJSON *mar_bw_ul = NULL;
    cJSON *mir_bw_dl = NULL;
    cJSON *mir_bw_ul = NULL;
    app_id = cJSON_GetObjectItemCaseSensitive(bw_requirementJSON, "appId");
    if (!app_id) {
        ogs_error("OpenAPI_bw_requirement_parseFromJSON() failed [app_id]");
        goto end;
    }
    if (!cJSON_IsString(app_id)) {
        ogs_error("OpenAPI_bw_requirement_parseFromJSON() failed [app_id]");
        goto end;
    }

    mar_bw_dl = cJSON_GetObjectItemCaseSensitive(bw_requirementJSON, "marBwDl");
    if (mar_bw_dl) {
    if (!cJSON_IsString(mar_bw_dl) && !cJSON_IsNull(mar_bw_dl)) {
        ogs_error("OpenAPI_bw_requirement_parseFromJSON() failed [mar_bw_dl]");
        goto end;
    }
    }

    mar_bw_ul = cJSON_GetObjectItemCaseSensitive(bw_requirementJSON, "marBwUl");
    if (mar_bw_ul) {
    if (!cJSON_IsString(mar_bw_ul) && !cJSON_IsNull(mar_bw_ul)) {
        ogs_error("OpenAPI_bw_requirement_parseFromJSON() failed [mar_bw_ul]");
        goto end;
    }
    }

    mir_bw_dl = cJSON_GetObjectItemCaseSensitive(bw_requirementJSON, "mirBwDl");
    if (mir_bw_dl) {
    if (!cJSON_IsString(mir_bw_dl) && !cJSON_IsNull(mir_bw_dl)) {
        ogs_error("OpenAPI_bw_requirement_parseFromJSON() failed [mir_bw_dl]");
        goto end;
    }
    }

    mir_bw_ul = cJSON_GetObjectItemCaseSensitive(bw_requirementJSON, "mirBwUl");
    if (mir_bw_ul) {
    if (!cJSON_IsString(mir_bw_ul) && !cJSON_IsNull(mir_bw_ul)) {
        ogs_error("OpenAPI_bw_requirement_parseFromJSON() failed [mir_bw_ul]");
        goto end;
    }
    }

    bw_requirement_local_var = OpenAPI_bw_requirement_create (
        ogs_strdup(app_id->valuestring),
        mar_bw_dl && !cJSON_IsNull(mar_bw_dl) ? ogs_strdup(mar_bw_dl->valuestring) : NULL,
        mar_bw_ul && !cJSON_IsNull(mar_bw_ul) ? ogs_strdup(mar_bw_ul->valuestring) : NULL,
        mir_bw_dl && !cJSON_IsNull(mir_bw_dl) ? ogs_strdup(mir_bw_dl->valuestring) : NULL,
        mir_bw_ul && !cJSON_IsNull(mir_bw_ul) ? ogs_strdup(mir_bw_ul->valuestring) : NULL
    );

    return bw_requirement_local_var;
end:
    return NULL;
}

OpenAPI_bw_requirement_t *OpenAPI_bw_requirement_copy(OpenAPI_bw_requirement_t *dst, OpenAPI_bw_requirement_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_bw_requirement_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_bw_requirement_convertToJSON() failed");
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

    OpenAPI_bw_requirement_free(dst);
    dst = OpenAPI_bw_requirement_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

