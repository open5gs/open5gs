
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sor_update_info.h"

OpenAPI_sor_update_info_t *OpenAPI_sor_update_info_create(
    OpenAPI_plmn_id_t *vplmn_id
    )
{
    OpenAPI_sor_update_info_t *sor_update_info_local_var = OpenAPI_malloc(sizeof(OpenAPI_sor_update_info_t));
    if (!sor_update_info_local_var) {
        return NULL;
    }
    sor_update_info_local_var->vplmn_id = vplmn_id;

    return sor_update_info_local_var;
}

void OpenAPI_sor_update_info_free(OpenAPI_sor_update_info_t *sor_update_info)
{
    if (NULL == sor_update_info) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_plmn_id_free(sor_update_info->vplmn_id);
    ogs_free(sor_update_info);
}

cJSON *OpenAPI_sor_update_info_convertToJSON(OpenAPI_sor_update_info_t *sor_update_info)
{
    cJSON *item = NULL;

    if (sor_update_info == NULL) {
        ogs_error("OpenAPI_sor_update_info_convertToJSON() failed [SorUpdateInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!sor_update_info->vplmn_id) {
        ogs_error("OpenAPI_sor_update_info_convertToJSON() failed [vplmn_id]");
        goto end;
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

end:
    return item;
}

OpenAPI_sor_update_info_t *OpenAPI_sor_update_info_parseFromJSON(cJSON *sor_update_infoJSON)
{
    OpenAPI_sor_update_info_t *sor_update_info_local_var = NULL;
    cJSON *vplmn_id = cJSON_GetObjectItemCaseSensitive(sor_update_infoJSON, "vplmnId");
    if (!vplmn_id) {
        ogs_error("OpenAPI_sor_update_info_parseFromJSON() failed [vplmn_id]");
        goto end;
    }

    OpenAPI_plmn_id_t *vplmn_id_local_nonprim = NULL;

    vplmn_id_local_nonprim = OpenAPI_plmn_id_parseFromJSON(vplmn_id);

    sor_update_info_local_var = OpenAPI_sor_update_info_create (
        vplmn_id_local_nonprim
        );

    return sor_update_info_local_var;
end:
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

