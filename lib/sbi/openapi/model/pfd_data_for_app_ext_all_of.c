
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pfd_data_for_app_ext_all_of.h"

OpenAPI_pfd_data_for_app_ext_all_of_t *OpenAPI_pfd_data_for_app_ext_all_of_create(
    char *supp_feat
)
{
    OpenAPI_pfd_data_for_app_ext_all_of_t *pfd_data_for_app_ext_all_of_local_var = ogs_malloc(sizeof(OpenAPI_pfd_data_for_app_ext_all_of_t));
    ogs_assert(pfd_data_for_app_ext_all_of_local_var);

    pfd_data_for_app_ext_all_of_local_var->supp_feat = supp_feat;

    return pfd_data_for_app_ext_all_of_local_var;
}

void OpenAPI_pfd_data_for_app_ext_all_of_free(OpenAPI_pfd_data_for_app_ext_all_of_t *pfd_data_for_app_ext_all_of)
{
    if (NULL == pfd_data_for_app_ext_all_of) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(pfd_data_for_app_ext_all_of->supp_feat);
    ogs_free(pfd_data_for_app_ext_all_of);
}

cJSON *OpenAPI_pfd_data_for_app_ext_all_of_convertToJSON(OpenAPI_pfd_data_for_app_ext_all_of_t *pfd_data_for_app_ext_all_of)
{
    cJSON *item = NULL;

    if (pfd_data_for_app_ext_all_of == NULL) {
        ogs_error("OpenAPI_pfd_data_for_app_ext_all_of_convertToJSON() failed [PfdDataForAppExt_allOf]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (pfd_data_for_app_ext_all_of->supp_feat) {
    if (cJSON_AddStringToObject(item, "suppFeat", pfd_data_for_app_ext_all_of->supp_feat) == NULL) {
        ogs_error("OpenAPI_pfd_data_for_app_ext_all_of_convertToJSON() failed [supp_feat]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_pfd_data_for_app_ext_all_of_t *OpenAPI_pfd_data_for_app_ext_all_of_parseFromJSON(cJSON *pfd_data_for_app_ext_all_ofJSON)
{
    OpenAPI_pfd_data_for_app_ext_all_of_t *pfd_data_for_app_ext_all_of_local_var = NULL;
    cJSON *supp_feat = cJSON_GetObjectItemCaseSensitive(pfd_data_for_app_ext_all_ofJSON, "suppFeat");

    if (supp_feat) {
    if (!cJSON_IsString(supp_feat)) {
        ogs_error("OpenAPI_pfd_data_for_app_ext_all_of_parseFromJSON() failed [supp_feat]");
        goto end;
    }
    }

    pfd_data_for_app_ext_all_of_local_var = OpenAPI_pfd_data_for_app_ext_all_of_create (
        supp_feat ? ogs_strdup(supp_feat->valuestring) : NULL
    );

    return pfd_data_for_app_ext_all_of_local_var;
end:
    return NULL;
}

OpenAPI_pfd_data_for_app_ext_all_of_t *OpenAPI_pfd_data_for_app_ext_all_of_copy(OpenAPI_pfd_data_for_app_ext_all_of_t *dst, OpenAPI_pfd_data_for_app_ext_all_of_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pfd_data_for_app_ext_all_of_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pfd_data_for_app_ext_all_of_convertToJSON() failed");
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

    OpenAPI_pfd_data_for_app_ext_all_of_free(dst);
    dst = OpenAPI_pfd_data_for_app_ext_all_of_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

