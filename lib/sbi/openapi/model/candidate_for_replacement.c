
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "candidate_for_replacement.h"

OpenAPI_candidate_for_replacement_t *OpenAPI_candidate_for_replacement_create(
    OpenAPI_snssai_t *snssai,
    OpenAPI_list_t *dnns
)
{
    OpenAPI_candidate_for_replacement_t *candidate_for_replacement_local_var = ogs_malloc(sizeof(OpenAPI_candidate_for_replacement_t));
    ogs_assert(candidate_for_replacement_local_var);

    candidate_for_replacement_local_var->snssai = snssai;
    candidate_for_replacement_local_var->dnns = dnns;

    return candidate_for_replacement_local_var;
}

void OpenAPI_candidate_for_replacement_free(OpenAPI_candidate_for_replacement_t *candidate_for_replacement)
{
    if (NULL == candidate_for_replacement) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_snssai_free(candidate_for_replacement->snssai);
    OpenAPI_list_for_each(candidate_for_replacement->dnns, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(candidate_for_replacement->dnns);
    ogs_free(candidate_for_replacement);
}

cJSON *OpenAPI_candidate_for_replacement_convertToJSON(OpenAPI_candidate_for_replacement_t *candidate_for_replacement)
{
    cJSON *item = NULL;

    if (candidate_for_replacement == NULL) {
        ogs_error("OpenAPI_candidate_for_replacement_convertToJSON() failed [CandidateForReplacement]");
        return NULL;
    }

    item = cJSON_CreateObject();
    cJSON *snssai_local_JSON = OpenAPI_snssai_convertToJSON(candidate_for_replacement->snssai);
    if (snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_candidate_for_replacement_convertToJSON() failed [snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "snssai", snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_candidate_for_replacement_convertToJSON() failed [snssai]");
        goto end;
    }

    if (candidate_for_replacement->dnns) {
    cJSON *dnns = cJSON_AddArrayToObject(item, "dnns");
    if (dnns == NULL) {
        ogs_error("OpenAPI_candidate_for_replacement_convertToJSON() failed [dnns]");
        goto end;
    }

    OpenAPI_lnode_t *dnns_node;
    OpenAPI_list_for_each(candidate_for_replacement->dnns, dnns_node)  {
    if (cJSON_AddStringToObject(dnns, "", (char*)dnns_node->data) == NULL) {
        ogs_error("OpenAPI_candidate_for_replacement_convertToJSON() failed [dnns]");
        goto end;
    }
                    }
    }

end:
    return item;
}

OpenAPI_candidate_for_replacement_t *OpenAPI_candidate_for_replacement_parseFromJSON(cJSON *candidate_for_replacementJSON)
{
    OpenAPI_candidate_for_replacement_t *candidate_for_replacement_local_var = NULL;
    cJSON *snssai = cJSON_GetObjectItemCaseSensitive(candidate_for_replacementJSON, "snssai");
    if (!snssai) {
        ogs_error("OpenAPI_candidate_for_replacement_parseFromJSON() failed [snssai]");
        goto end;
    }

    OpenAPI_snssai_t *snssai_local_nonprim = NULL;
    snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(snssai);

    cJSON *dnns = cJSON_GetObjectItemCaseSensitive(candidate_for_replacementJSON, "dnns");

    OpenAPI_list_t *dnnsList;
    if (dnns) {
    cJSON *dnns_local;
    if (!cJSON_IsArray(dnns)) {
        ogs_error("OpenAPI_candidate_for_replacement_parseFromJSON() failed [dnns]");
        goto end;
    }
    dnnsList = OpenAPI_list_create();

    cJSON_ArrayForEach(dnns_local, dnns) {
    if (!cJSON_IsString(dnns_local)) {
        ogs_error("OpenAPI_candidate_for_replacement_parseFromJSON() failed [dnns]");
        goto end;
    }
    OpenAPI_list_add(dnnsList , ogs_strdup(dnns_local->valuestring));
    }
    }

    candidate_for_replacement_local_var = OpenAPI_candidate_for_replacement_create (
        snssai_local_nonprim,
        dnns ? dnnsList : NULL
    );

    return candidate_for_replacement_local_var;
end:
    return NULL;
}

OpenAPI_candidate_for_replacement_t *OpenAPI_candidate_for_replacement_copy(OpenAPI_candidate_for_replacement_t *dst, OpenAPI_candidate_for_replacement_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_candidate_for_replacement_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_candidate_for_replacement_convertToJSON() failed");
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

    OpenAPI_candidate_for_replacement_free(dst);
    dst = OpenAPI_candidate_for_replacement_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

