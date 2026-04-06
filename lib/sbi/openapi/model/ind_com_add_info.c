
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ind_com_add_info.h"

OpenAPI_ind_com_add_info_t *OpenAPI_ind_com_add_info_create(
    bool is_all_nf_types_ind,
    int all_nf_types_ind,
    char *scp_api_root
)
{
    OpenAPI_ind_com_add_info_t *ind_com_add_info_local_var = ogs_malloc(sizeof(OpenAPI_ind_com_add_info_t));
    ogs_assert(ind_com_add_info_local_var);

    ind_com_add_info_local_var->is_all_nf_types_ind = is_all_nf_types_ind;
    ind_com_add_info_local_var->all_nf_types_ind = all_nf_types_ind;
    ind_com_add_info_local_var->scp_api_root = scp_api_root;

    return ind_com_add_info_local_var;
}

void OpenAPI_ind_com_add_info_free(OpenAPI_ind_com_add_info_t *ind_com_add_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ind_com_add_info) {
        return;
    }
    if (ind_com_add_info->scp_api_root) {
        ogs_free(ind_com_add_info->scp_api_root);
        ind_com_add_info->scp_api_root = NULL;
    }
    ogs_free(ind_com_add_info);
}

cJSON *OpenAPI_ind_com_add_info_convertToJSON(OpenAPI_ind_com_add_info_t *ind_com_add_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ind_com_add_info == NULL) {
        ogs_error("OpenAPI_ind_com_add_info_convertToJSON() failed [IndComAddInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ind_com_add_info->is_all_nf_types_ind) {
    if (cJSON_AddBoolToObject(item, "allNfTypesInd", ind_com_add_info->all_nf_types_ind) == NULL) {
        ogs_error("OpenAPI_ind_com_add_info_convertToJSON() failed [all_nf_types_ind]");
        goto end;
    }
    }

    if (ind_com_add_info->scp_api_root) {
    if (cJSON_AddStringToObject(item, "scpApiRoot", ind_com_add_info->scp_api_root) == NULL) {
        ogs_error("OpenAPI_ind_com_add_info_convertToJSON() failed [scp_api_root]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ind_com_add_info_t *OpenAPI_ind_com_add_info_parseFromJSON(cJSON *ind_com_add_infoJSON)
{
    OpenAPI_ind_com_add_info_t *ind_com_add_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *all_nf_types_ind = NULL;
    cJSON *scp_api_root = NULL;
    all_nf_types_ind = cJSON_GetObjectItemCaseSensitive(ind_com_add_infoJSON, "allNfTypesInd");
    if (all_nf_types_ind) {
    if (!cJSON_IsBool(all_nf_types_ind)) {
        ogs_error("OpenAPI_ind_com_add_info_parseFromJSON() failed [all_nf_types_ind]");
        goto end;
    }
    }

    scp_api_root = cJSON_GetObjectItemCaseSensitive(ind_com_add_infoJSON, "scpApiRoot");
    if (scp_api_root) {
    if (!cJSON_IsString(scp_api_root) && !cJSON_IsNull(scp_api_root)) {
        ogs_error("OpenAPI_ind_com_add_info_parseFromJSON() failed [scp_api_root]");
        goto end;
    }
    }

    ind_com_add_info_local_var = OpenAPI_ind_com_add_info_create (
        all_nf_types_ind ? true : false,
        all_nf_types_ind ? all_nf_types_ind->valueint : 0,
        scp_api_root && !cJSON_IsNull(scp_api_root) ? ogs_strdup(scp_api_root->valuestring) : NULL
    );

    return ind_com_add_info_local_var;
end:
    return NULL;
}

OpenAPI_ind_com_add_info_t *OpenAPI_ind_com_add_info_copy(OpenAPI_ind_com_add_info_t *dst, OpenAPI_ind_com_add_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ind_com_add_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ind_com_add_info_convertToJSON() failed");
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

    OpenAPI_ind_com_add_info_free(dst);
    dst = OpenAPI_ind_com_add_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

