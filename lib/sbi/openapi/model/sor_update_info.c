
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sor_update_info.h"

OpenAPI_sor_update_info_t *OpenAPI_sor_update_info_create(
    OpenAPI_plmn_id_t *vplmn_id,
    char *supported_features
)
{
    OpenAPI_sor_update_info_t *sor_update_info_local_var = ogs_malloc(sizeof(OpenAPI_sor_update_info_t));
    ogs_assert(sor_update_info_local_var);

    sor_update_info_local_var->vplmn_id = vplmn_id;
    sor_update_info_local_var->supported_features = supported_features;

    return sor_update_info_local_var;
}

void OpenAPI_sor_update_info_free(OpenAPI_sor_update_info_t *sor_update_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == sor_update_info) {
        return;
    }
    if (sor_update_info->vplmn_id) {
        OpenAPI_plmn_id_free(sor_update_info->vplmn_id);
        sor_update_info->vplmn_id = NULL;
    }
    if (sor_update_info->supported_features) {
        ogs_free(sor_update_info->supported_features);
        sor_update_info->supported_features = NULL;
    }
    ogs_free(sor_update_info);
}

cJSON *OpenAPI_sor_update_info_convertToJSON(OpenAPI_sor_update_info_t *sor_update_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (sor_update_info == NULL) {
        ogs_error("OpenAPI_sor_update_info_convertToJSON() failed [SorUpdateInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!sor_update_info->vplmn_id) {
        ogs_error("OpenAPI_sor_update_info_convertToJSON() failed [vplmn_id]");
        return NULL;
    }
    cJSON *vplmn_id_local_JSON = OpenAPI_plmn_id_convertToJSON(sor_update_info->vplmn_id);
    if (vplmn_id_local_JSON == NULL) {
        ogs_error("OpenAPI_sor_update_info_convertToJSON() failed [vplmn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "vplmnId", vplmn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sor_update_info_convertToJSON() failed [vplmn_id]");
        goto end;
    }

    if (sor_update_info->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", sor_update_info->supported_features) == NULL) {
        ogs_error("OpenAPI_sor_update_info_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_sor_update_info_t *OpenAPI_sor_update_info_parseFromJSON(cJSON *sor_update_infoJSON)
{
    OpenAPI_sor_update_info_t *sor_update_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *vplmn_id = NULL;
    OpenAPI_plmn_id_t *vplmn_id_local_nonprim = NULL;
    cJSON *supported_features = NULL;
    vplmn_id = cJSON_GetObjectItemCaseSensitive(sor_update_infoJSON, "vplmnId");
    if (!vplmn_id) {
        ogs_error("OpenAPI_sor_update_info_parseFromJSON() failed [vplmn_id]");
        goto end;
    }
    vplmn_id_local_nonprim = OpenAPI_plmn_id_parseFromJSON(vplmn_id);
    if (!vplmn_id_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_parseFromJSON failed [vplmn_id]");
        goto end;
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(sor_update_infoJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_sor_update_info_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    sor_update_info_local_var = OpenAPI_sor_update_info_create (
        vplmn_id_local_nonprim,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL
    );

    return sor_update_info_local_var;
end:
    if (vplmn_id_local_nonprim) {
        OpenAPI_plmn_id_free(vplmn_id_local_nonprim);
        vplmn_id_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_sor_update_info_t *OpenAPI_sor_update_info_copy(OpenAPI_sor_update_info_t *dst, OpenAPI_sor_update_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sor_update_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sor_update_info_convertToJSON() failed");
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

    OpenAPI_sor_update_info_free(dst);
    dst = OpenAPI_sor_update_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

