
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ri_error_information.h"

OpenAPI_ri_error_information_t *OpenAPI_ri_error_information_create(
    OpenAPI_n32_release_indication_e n32f_connection_rel_ind,
    OpenAPI_n32_release_indication_e n32f_context_rel_ind,
    char *alternative_ri
)
{
    OpenAPI_ri_error_information_t *ri_error_information_local_var = ogs_malloc(sizeof(OpenAPI_ri_error_information_t));
    ogs_assert(ri_error_information_local_var);

    ri_error_information_local_var->n32f_connection_rel_ind = n32f_connection_rel_ind;
    ri_error_information_local_var->n32f_context_rel_ind = n32f_context_rel_ind;
    ri_error_information_local_var->alternative_ri = alternative_ri;

    return ri_error_information_local_var;
}

void OpenAPI_ri_error_information_free(OpenAPI_ri_error_information_t *ri_error_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ri_error_information) {
        return;
    }
    if (ri_error_information->alternative_ri) {
        ogs_free(ri_error_information->alternative_ri);
        ri_error_information->alternative_ri = NULL;
    }
    ogs_free(ri_error_information);
}

cJSON *OpenAPI_ri_error_information_convertToJSON(OpenAPI_ri_error_information_t *ri_error_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ri_error_information == NULL) {
        ogs_error("OpenAPI_ri_error_information_convertToJSON() failed [RiErrorInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ri_error_information->n32f_connection_rel_ind != OpenAPI_n32_release_indication_NULL) {
    if (cJSON_AddStringToObject(item, "n32fConnectionRelInd", OpenAPI_n32_release_indication_ToString(ri_error_information->n32f_connection_rel_ind)) == NULL) {
        ogs_error("OpenAPI_ri_error_information_convertToJSON() failed [n32f_connection_rel_ind]");
        goto end;
    }
    }

    if (ri_error_information->n32f_context_rel_ind != OpenAPI_n32_release_indication_NULL) {
    if (cJSON_AddStringToObject(item, "n32fContextRelInd", OpenAPI_n32_release_indication_ToString(ri_error_information->n32f_context_rel_ind)) == NULL) {
        ogs_error("OpenAPI_ri_error_information_convertToJSON() failed [n32f_context_rel_ind]");
        goto end;
    }
    }

    if (ri_error_information->alternative_ri) {
    if (cJSON_AddStringToObject(item, "alternativeRi", ri_error_information->alternative_ri) == NULL) {
        ogs_error("OpenAPI_ri_error_information_convertToJSON() failed [alternative_ri]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ri_error_information_t *OpenAPI_ri_error_information_parseFromJSON(cJSON *ri_error_informationJSON)
{
    OpenAPI_ri_error_information_t *ri_error_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *n32f_connection_rel_ind = NULL;
    OpenAPI_n32_release_indication_e n32f_connection_rel_indVariable = 0;
    cJSON *n32f_context_rel_ind = NULL;
    OpenAPI_n32_release_indication_e n32f_context_rel_indVariable = 0;
    cJSON *alternative_ri = NULL;
    n32f_connection_rel_ind = cJSON_GetObjectItemCaseSensitive(ri_error_informationJSON, "n32fConnectionRelInd");
    if (n32f_connection_rel_ind) {
    if (!cJSON_IsString(n32f_connection_rel_ind)) {
        ogs_error("OpenAPI_ri_error_information_parseFromJSON() failed [n32f_connection_rel_ind]");
        goto end;
    }
    n32f_connection_rel_indVariable = OpenAPI_n32_release_indication_FromString(n32f_connection_rel_ind->valuestring);
    }

    n32f_context_rel_ind = cJSON_GetObjectItemCaseSensitive(ri_error_informationJSON, "n32fContextRelInd");
    if (n32f_context_rel_ind) {
    if (!cJSON_IsString(n32f_context_rel_ind)) {
        ogs_error("OpenAPI_ri_error_information_parseFromJSON() failed [n32f_context_rel_ind]");
        goto end;
    }
    n32f_context_rel_indVariable = OpenAPI_n32_release_indication_FromString(n32f_context_rel_ind->valuestring);
    }

    alternative_ri = cJSON_GetObjectItemCaseSensitive(ri_error_informationJSON, "alternativeRi");
    if (alternative_ri) {
    if (!cJSON_IsString(alternative_ri) && !cJSON_IsNull(alternative_ri)) {
        ogs_error("OpenAPI_ri_error_information_parseFromJSON() failed [alternative_ri]");
        goto end;
    }
    }

    ri_error_information_local_var = OpenAPI_ri_error_information_create (
        n32f_connection_rel_ind ? n32f_connection_rel_indVariable : 0,
        n32f_context_rel_ind ? n32f_context_rel_indVariable : 0,
        alternative_ri && !cJSON_IsNull(alternative_ri) ? ogs_strdup(alternative_ri->valuestring) : NULL
    );

    return ri_error_information_local_var;
end:
    return NULL;
}

OpenAPI_ri_error_information_t *OpenAPI_ri_error_information_copy(OpenAPI_ri_error_information_t *dst, OpenAPI_ri_error_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ri_error_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ri_error_information_convertToJSON() failed");
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

    OpenAPI_ri_error_information_free(dst);
    dst = OpenAPI_ri_error_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

