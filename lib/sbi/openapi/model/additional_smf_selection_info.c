
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "additional_smf_selection_info.h"

OpenAPI_additional_smf_selection_info_t *OpenAPI_additional_smf_selection_info_create(
    OpenAPI_plmn_id_nid_t *target_plmn
)
{
    OpenAPI_additional_smf_selection_info_t *additional_smf_selection_info_local_var = ogs_malloc(sizeof(OpenAPI_additional_smf_selection_info_t));
    ogs_assert(additional_smf_selection_info_local_var);

    additional_smf_selection_info_local_var->target_plmn = target_plmn;

    return additional_smf_selection_info_local_var;
}

void OpenAPI_additional_smf_selection_info_free(OpenAPI_additional_smf_selection_info_t *additional_smf_selection_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == additional_smf_selection_info) {
        return;
    }
    if (additional_smf_selection_info->target_plmn) {
        OpenAPI_plmn_id_nid_free(additional_smf_selection_info->target_plmn);
        additional_smf_selection_info->target_plmn = NULL;
    }
    ogs_free(additional_smf_selection_info);
}

cJSON *OpenAPI_additional_smf_selection_info_convertToJSON(OpenAPI_additional_smf_selection_info_t *additional_smf_selection_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (additional_smf_selection_info == NULL) {
        ogs_error("OpenAPI_additional_smf_selection_info_convertToJSON() failed [AdditionalSmfSelectionInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (additional_smf_selection_info->target_plmn) {
    cJSON *target_plmn_local_JSON = OpenAPI_plmn_id_nid_convertToJSON(additional_smf_selection_info->target_plmn);
    if (target_plmn_local_JSON == NULL) {
        ogs_error("OpenAPI_additional_smf_selection_info_convertToJSON() failed [target_plmn]");
        goto end;
    }
    cJSON_AddItemToObject(item, "targetPlmn", target_plmn_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_additional_smf_selection_info_convertToJSON() failed [target_plmn]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_additional_smf_selection_info_t *OpenAPI_additional_smf_selection_info_parseFromJSON(cJSON *additional_smf_selection_infoJSON)
{
    OpenAPI_additional_smf_selection_info_t *additional_smf_selection_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *target_plmn = NULL;
    OpenAPI_plmn_id_nid_t *target_plmn_local_nonprim = NULL;
    target_plmn = cJSON_GetObjectItemCaseSensitive(additional_smf_selection_infoJSON, "targetPlmn");
    if (target_plmn) {
    target_plmn_local_nonprim = OpenAPI_plmn_id_nid_parseFromJSON(target_plmn);
    if (!target_plmn_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_nid_parseFromJSON failed [target_plmn]");
        goto end;
    }
    }

    additional_smf_selection_info_local_var = OpenAPI_additional_smf_selection_info_create (
        target_plmn ? target_plmn_local_nonprim : NULL
    );

    return additional_smf_selection_info_local_var;
end:
    if (target_plmn_local_nonprim) {
        OpenAPI_plmn_id_nid_free(target_plmn_local_nonprim);
        target_plmn_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_additional_smf_selection_info_t *OpenAPI_additional_smf_selection_info_copy(OpenAPI_additional_smf_selection_info_t *dst, OpenAPI_additional_smf_selection_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_additional_smf_selection_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_additional_smf_selection_info_convertToJSON() failed");
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

    OpenAPI_additional_smf_selection_info_free(dst);
    dst = OpenAPI_additional_smf_selection_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

