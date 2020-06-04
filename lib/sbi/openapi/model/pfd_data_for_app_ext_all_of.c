
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pfd_data_for_app_ext_all_of.h"

OpenAPI_pfd_data_for_app_ext_all_of_t *OpenAPI_pfd_data_for_app_ext_all_of_create(
    char *supp_feat
    )
{
    OpenAPI_pfd_data_for_app_ext_all_of_t *pfd_data_for_app_ext_all_of_local_var = OpenAPI_malloc(sizeof(OpenAPI_pfd_data_for_app_ext_all_of_t));
    if (!pfd_data_for_app_ext_all_of_local_var) {
        return NULL;
    }
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

