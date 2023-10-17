
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "model_5_g_ddnmf_info.h"

OpenAPI_model_5_g_ddnmf_info_t *OpenAPI_model_5_g_ddnmf_info_create(
    OpenAPI_plmn_id_t *plmn_id
)
{
    OpenAPI_model_5_g_ddnmf_info_t *model_5_g_ddnmf_info_local_var = ogs_malloc(sizeof(OpenAPI_model_5_g_ddnmf_info_t));
    ogs_assert(model_5_g_ddnmf_info_local_var);

    model_5_g_ddnmf_info_local_var->plmn_id = plmn_id;

    return model_5_g_ddnmf_info_local_var;
}

void OpenAPI_model_5_g_ddnmf_info_free(OpenAPI_model_5_g_ddnmf_info_t *model_5_g_ddnmf_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == model_5_g_ddnmf_info) {
        return;
    }
    if (model_5_g_ddnmf_info->plmn_id) {
        OpenAPI_plmn_id_free(model_5_g_ddnmf_info->plmn_id);
        model_5_g_ddnmf_info->plmn_id = NULL;
    }
    ogs_free(model_5_g_ddnmf_info);
}

cJSON *OpenAPI_model_5_g_ddnmf_info_convertToJSON(OpenAPI_model_5_g_ddnmf_info_t *model_5_g_ddnmf_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (model_5_g_ddnmf_info == NULL) {
        ogs_error("OpenAPI_model_5_g_ddnmf_info_convertToJSON() failed [5GDdnmfInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!model_5_g_ddnmf_info->plmn_id) {
        ogs_error("OpenAPI_model_5_g_ddnmf_info_convertToJSON() failed [plmn_id]");
        return NULL;
    }
    cJSON *plmn_id_local_JSON = OpenAPI_plmn_id_convertToJSON(model_5_g_ddnmf_info->plmn_id);
    if (plmn_id_local_JSON == NULL) {
        ogs_error("OpenAPI_model_5_g_ddnmf_info_convertToJSON() failed [plmn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "plmnId", plmn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_model_5_g_ddnmf_info_convertToJSON() failed [plmn_id]");
        goto end;
    }

end:
    return item;
}

OpenAPI_model_5_g_ddnmf_info_t *OpenAPI_model_5_g_ddnmf_info_parseFromJSON(cJSON *model_5_g_ddnmf_infoJSON)
{
    OpenAPI_model_5_g_ddnmf_info_t *model_5_g_ddnmf_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *plmn_id = NULL;
    OpenAPI_plmn_id_t *plmn_id_local_nonprim = NULL;
    plmn_id = cJSON_GetObjectItemCaseSensitive(model_5_g_ddnmf_infoJSON, "plmnId");
    if (!plmn_id) {
        ogs_error("OpenAPI_model_5_g_ddnmf_info_parseFromJSON() failed [plmn_id]");
        goto end;
    }
    plmn_id_local_nonprim = OpenAPI_plmn_id_parseFromJSON(plmn_id);
    if (!plmn_id_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_parseFromJSON failed [plmn_id]");
        goto end;
    }

    model_5_g_ddnmf_info_local_var = OpenAPI_model_5_g_ddnmf_info_create (
        plmn_id_local_nonprim
    );

    return model_5_g_ddnmf_info_local_var;
end:
    if (plmn_id_local_nonprim) {
        OpenAPI_plmn_id_free(plmn_id_local_nonprim);
        plmn_id_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_model_5_g_ddnmf_info_t *OpenAPI_model_5_g_ddnmf_info_copy(OpenAPI_model_5_g_ddnmf_info_t *dst, OpenAPI_model_5_g_ddnmf_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_model_5_g_ddnmf_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_model_5_g_ddnmf_info_convertToJSON() failed");
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

    OpenAPI_model_5_g_ddnmf_info_free(dst);
    dst = OpenAPI_model_5_g_ddnmf_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

