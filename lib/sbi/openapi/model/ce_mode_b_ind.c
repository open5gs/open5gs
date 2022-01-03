
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ce_mode_b_ind.h"

OpenAPI_ce_mode_b_ind_t *OpenAPI_ce_mode_b_ind_create(
    int ce_mode_b_support_ind
)
{
    OpenAPI_ce_mode_b_ind_t *ce_mode_b_ind_local_var = ogs_malloc(sizeof(OpenAPI_ce_mode_b_ind_t));
    ogs_assert(ce_mode_b_ind_local_var);

    ce_mode_b_ind_local_var->ce_mode_b_support_ind = ce_mode_b_support_ind;

    return ce_mode_b_ind_local_var;
}

void OpenAPI_ce_mode_b_ind_free(OpenAPI_ce_mode_b_ind_t *ce_mode_b_ind)
{
    if (NULL == ce_mode_b_ind) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(ce_mode_b_ind);
}

cJSON *OpenAPI_ce_mode_b_ind_convertToJSON(OpenAPI_ce_mode_b_ind_t *ce_mode_b_ind)
{
    cJSON *item = NULL;

    if (ce_mode_b_ind == NULL) {
        ogs_error("OpenAPI_ce_mode_b_ind_convertToJSON() failed [CeModeBInd]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddBoolToObject(item, "ceModeBSupportInd", ce_mode_b_ind->ce_mode_b_support_ind) == NULL) {
        ogs_error("OpenAPI_ce_mode_b_ind_convertToJSON() failed [ce_mode_b_support_ind]");
        goto end;
    }

end:
    return item;
}

OpenAPI_ce_mode_b_ind_t *OpenAPI_ce_mode_b_ind_parseFromJSON(cJSON *ce_mode_b_indJSON)
{
    OpenAPI_ce_mode_b_ind_t *ce_mode_b_ind_local_var = NULL;
    cJSON *ce_mode_b_support_ind = cJSON_GetObjectItemCaseSensitive(ce_mode_b_indJSON, "ceModeBSupportInd");
    if (!ce_mode_b_support_ind) {
        ogs_error("OpenAPI_ce_mode_b_ind_parseFromJSON() failed [ce_mode_b_support_ind]");
        goto end;
    }

    if (!cJSON_IsBool(ce_mode_b_support_ind)) {
        ogs_error("OpenAPI_ce_mode_b_ind_parseFromJSON() failed [ce_mode_b_support_ind]");
        goto end;
    }

    ce_mode_b_ind_local_var = OpenAPI_ce_mode_b_ind_create (
        
        ce_mode_b_support_ind->valueint
    );

    return ce_mode_b_ind_local_var;
end:
    return NULL;
}

OpenAPI_ce_mode_b_ind_t *OpenAPI_ce_mode_b_ind_copy(OpenAPI_ce_mode_b_ind_t *dst, OpenAPI_ce_mode_b_ind_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ce_mode_b_ind_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ce_mode_b_ind_convertToJSON() failed");
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

    OpenAPI_ce_mode_b_ind_free(dst);
    dst = OpenAPI_ce_mode_b_ind_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

