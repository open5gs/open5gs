
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lte_m_ind.h"

OpenAPI_lte_m_ind_t *OpenAPI_lte_m_ind_create(
    int lte_cat_m_ind
)
{
    OpenAPI_lte_m_ind_t *lte_m_ind_local_var = ogs_malloc(sizeof(OpenAPI_lte_m_ind_t));
    ogs_assert(lte_m_ind_local_var);

    lte_m_ind_local_var->lte_cat_m_ind = lte_cat_m_ind;

    return lte_m_ind_local_var;
}

void OpenAPI_lte_m_ind_free(OpenAPI_lte_m_ind_t *lte_m_ind)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == lte_m_ind) {
        return;
    }
    ogs_free(lte_m_ind);
}

cJSON *OpenAPI_lte_m_ind_convertToJSON(OpenAPI_lte_m_ind_t *lte_m_ind)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (lte_m_ind == NULL) {
        ogs_error("OpenAPI_lte_m_ind_convertToJSON() failed [LteMInd]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddBoolToObject(item, "lteCatMInd", lte_m_ind->lte_cat_m_ind) == NULL) {
        ogs_error("OpenAPI_lte_m_ind_convertToJSON() failed [lte_cat_m_ind]");
        goto end;
    }

end:
    return item;
}

OpenAPI_lte_m_ind_t *OpenAPI_lte_m_ind_parseFromJSON(cJSON *lte_m_indJSON)
{
    OpenAPI_lte_m_ind_t *lte_m_ind_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *lte_cat_m_ind = NULL;
    lte_cat_m_ind = cJSON_GetObjectItemCaseSensitive(lte_m_indJSON, "lteCatMInd");
    if (!lte_cat_m_ind) {
        ogs_error("OpenAPI_lte_m_ind_parseFromJSON() failed [lte_cat_m_ind]");
        goto end;
    }
    if (!cJSON_IsBool(lte_cat_m_ind)) {
        ogs_error("OpenAPI_lte_m_ind_parseFromJSON() failed [lte_cat_m_ind]");
        goto end;
    }

    lte_m_ind_local_var = OpenAPI_lte_m_ind_create (
        
        lte_cat_m_ind->valueint
    );

    return lte_m_ind_local_var;
end:
    return NULL;
}

OpenAPI_lte_m_ind_t *OpenAPI_lte_m_ind_copy(OpenAPI_lte_m_ind_t *dst, OpenAPI_lte_m_ind_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_lte_m_ind_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_lte_m_ind_convertToJSON() failed");
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

    OpenAPI_lte_m_ind_free(dst);
    dst = OpenAPI_lte_m_ind_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

