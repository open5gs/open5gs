
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "inter_operator_identifier.h"

OpenAPI_inter_operator_identifier_t *OpenAPI_inter_operator_identifier_create(
    char *originating_ioi,
    char *terminating_ioi
)
{
    OpenAPI_inter_operator_identifier_t *inter_operator_identifier_local_var = ogs_malloc(sizeof(OpenAPI_inter_operator_identifier_t));
    ogs_assert(inter_operator_identifier_local_var);

    inter_operator_identifier_local_var->originating_ioi = originating_ioi;
    inter_operator_identifier_local_var->terminating_ioi = terminating_ioi;

    return inter_operator_identifier_local_var;
}

void OpenAPI_inter_operator_identifier_free(OpenAPI_inter_operator_identifier_t *inter_operator_identifier)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == inter_operator_identifier) {
        return;
    }
    if (inter_operator_identifier->originating_ioi) {
        ogs_free(inter_operator_identifier->originating_ioi);
        inter_operator_identifier->originating_ioi = NULL;
    }
    if (inter_operator_identifier->terminating_ioi) {
        ogs_free(inter_operator_identifier->terminating_ioi);
        inter_operator_identifier->terminating_ioi = NULL;
    }
    ogs_free(inter_operator_identifier);
}

cJSON *OpenAPI_inter_operator_identifier_convertToJSON(OpenAPI_inter_operator_identifier_t *inter_operator_identifier)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (inter_operator_identifier == NULL) {
        ogs_error("OpenAPI_inter_operator_identifier_convertToJSON() failed [InterOperatorIdentifier]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (inter_operator_identifier->originating_ioi) {
    if (cJSON_AddStringToObject(item, "originatingIOI", inter_operator_identifier->originating_ioi) == NULL) {
        ogs_error("OpenAPI_inter_operator_identifier_convertToJSON() failed [originating_ioi]");
        goto end;
    }
    }

    if (inter_operator_identifier->terminating_ioi) {
    if (cJSON_AddStringToObject(item, "terminatingIOI", inter_operator_identifier->terminating_ioi) == NULL) {
        ogs_error("OpenAPI_inter_operator_identifier_convertToJSON() failed [terminating_ioi]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_inter_operator_identifier_t *OpenAPI_inter_operator_identifier_parseFromJSON(cJSON *inter_operator_identifierJSON)
{
    OpenAPI_inter_operator_identifier_t *inter_operator_identifier_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *originating_ioi = NULL;
    cJSON *terminating_ioi = NULL;
    originating_ioi = cJSON_GetObjectItemCaseSensitive(inter_operator_identifierJSON, "originatingIOI");
    if (originating_ioi) {
    if (!cJSON_IsString(originating_ioi) && !cJSON_IsNull(originating_ioi)) {
        ogs_error("OpenAPI_inter_operator_identifier_parseFromJSON() failed [originating_ioi]");
        goto end;
    }
    }

    terminating_ioi = cJSON_GetObjectItemCaseSensitive(inter_operator_identifierJSON, "terminatingIOI");
    if (terminating_ioi) {
    if (!cJSON_IsString(terminating_ioi) && !cJSON_IsNull(terminating_ioi)) {
        ogs_error("OpenAPI_inter_operator_identifier_parseFromJSON() failed [terminating_ioi]");
        goto end;
    }
    }

    inter_operator_identifier_local_var = OpenAPI_inter_operator_identifier_create (
        originating_ioi && !cJSON_IsNull(originating_ioi) ? ogs_strdup(originating_ioi->valuestring) : NULL,
        terminating_ioi && !cJSON_IsNull(terminating_ioi) ? ogs_strdup(terminating_ioi->valuestring) : NULL
    );

    return inter_operator_identifier_local_var;
end:
    return NULL;
}

OpenAPI_inter_operator_identifier_t *OpenAPI_inter_operator_identifier_copy(OpenAPI_inter_operator_identifier_t *dst, OpenAPI_inter_operator_identifier_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_inter_operator_identifier_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_inter_operator_identifier_convertToJSON() failed");
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

    OpenAPI_inter_operator_identifier_free(dst);
    dst = OpenAPI_inter_operator_identifier_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

