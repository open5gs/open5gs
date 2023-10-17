
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "termination_info.h"

OpenAPI_termination_info_t *OpenAPI_termination_info_create(
    OpenAPI_termination_cause_e term_cause,
    char *res_uri
)
{
    OpenAPI_termination_info_t *termination_info_local_var = ogs_malloc(sizeof(OpenAPI_termination_info_t));
    ogs_assert(termination_info_local_var);

    termination_info_local_var->term_cause = term_cause;
    termination_info_local_var->res_uri = res_uri;

    return termination_info_local_var;
}

void OpenAPI_termination_info_free(OpenAPI_termination_info_t *termination_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == termination_info) {
        return;
    }
    if (termination_info->res_uri) {
        ogs_free(termination_info->res_uri);
        termination_info->res_uri = NULL;
    }
    ogs_free(termination_info);
}

cJSON *OpenAPI_termination_info_convertToJSON(OpenAPI_termination_info_t *termination_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (termination_info == NULL) {
        ogs_error("OpenAPI_termination_info_convertToJSON() failed [TerminationInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (termination_info->term_cause == OpenAPI_termination_cause_NULL) {
        ogs_error("OpenAPI_termination_info_convertToJSON() failed [term_cause]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "termCause", OpenAPI_termination_cause_ToString(termination_info->term_cause)) == NULL) {
        ogs_error("OpenAPI_termination_info_convertToJSON() failed [term_cause]");
        goto end;
    }

    if (!termination_info->res_uri) {
        ogs_error("OpenAPI_termination_info_convertToJSON() failed [res_uri]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "resUri", termination_info->res_uri) == NULL) {
        ogs_error("OpenAPI_termination_info_convertToJSON() failed [res_uri]");
        goto end;
    }

end:
    return item;
}

OpenAPI_termination_info_t *OpenAPI_termination_info_parseFromJSON(cJSON *termination_infoJSON)
{
    OpenAPI_termination_info_t *termination_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *term_cause = NULL;
    OpenAPI_termination_cause_e term_causeVariable = 0;
    cJSON *res_uri = NULL;
    term_cause = cJSON_GetObjectItemCaseSensitive(termination_infoJSON, "termCause");
    if (!term_cause) {
        ogs_error("OpenAPI_termination_info_parseFromJSON() failed [term_cause]");
        goto end;
    }
    if (!cJSON_IsString(term_cause)) {
        ogs_error("OpenAPI_termination_info_parseFromJSON() failed [term_cause]");
        goto end;
    }
    term_causeVariable = OpenAPI_termination_cause_FromString(term_cause->valuestring);

    res_uri = cJSON_GetObjectItemCaseSensitive(termination_infoJSON, "resUri");
    if (!res_uri) {
        ogs_error("OpenAPI_termination_info_parseFromJSON() failed [res_uri]");
        goto end;
    }
    if (!cJSON_IsString(res_uri)) {
        ogs_error("OpenAPI_termination_info_parseFromJSON() failed [res_uri]");
        goto end;
    }

    termination_info_local_var = OpenAPI_termination_info_create (
        term_causeVariable,
        ogs_strdup(res_uri->valuestring)
    );

    return termination_info_local_var;
end:
    return NULL;
}

OpenAPI_termination_info_t *OpenAPI_termination_info_copy(OpenAPI_termination_info_t *dst, OpenAPI_termination_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_termination_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_termination_info_convertToJSON() failed");
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

    OpenAPI_termination_info_free(dst);
    dst = OpenAPI_termination_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

