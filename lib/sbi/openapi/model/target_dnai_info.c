
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "target_dnai_info.h"

OpenAPI_target_dnai_info_t *OpenAPI_target_dnai_info_create(
    char *target_dnai,
    OpenAPI_smf_selection_type_e smf_selection_type
)
{
    OpenAPI_target_dnai_info_t *target_dnai_info_local_var = ogs_malloc(sizeof(OpenAPI_target_dnai_info_t));
    ogs_assert(target_dnai_info_local_var);

    target_dnai_info_local_var->target_dnai = target_dnai;
    target_dnai_info_local_var->smf_selection_type = smf_selection_type;

    return target_dnai_info_local_var;
}

void OpenAPI_target_dnai_info_free(OpenAPI_target_dnai_info_t *target_dnai_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == target_dnai_info) {
        return;
    }
    if (target_dnai_info->target_dnai) {
        ogs_free(target_dnai_info->target_dnai);
        target_dnai_info->target_dnai = NULL;
    }
    ogs_free(target_dnai_info);
}

cJSON *OpenAPI_target_dnai_info_convertToJSON(OpenAPI_target_dnai_info_t *target_dnai_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (target_dnai_info == NULL) {
        ogs_error("OpenAPI_target_dnai_info_convertToJSON() failed [TargetDnaiInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (target_dnai_info->target_dnai) {
    if (cJSON_AddStringToObject(item, "targetDnai", target_dnai_info->target_dnai) == NULL) {
        ogs_error("OpenAPI_target_dnai_info_convertToJSON() failed [target_dnai]");
        goto end;
    }
    }

    if (target_dnai_info->smf_selection_type == OpenAPI_smf_selection_type_NULL) {
        ogs_error("OpenAPI_target_dnai_info_convertToJSON() failed [smf_selection_type]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "smfSelectionType", OpenAPI_smf_selection_type_ToString(target_dnai_info->smf_selection_type)) == NULL) {
        ogs_error("OpenAPI_target_dnai_info_convertToJSON() failed [smf_selection_type]");
        goto end;
    }

end:
    return item;
}

OpenAPI_target_dnai_info_t *OpenAPI_target_dnai_info_parseFromJSON(cJSON *target_dnai_infoJSON)
{
    OpenAPI_target_dnai_info_t *target_dnai_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *target_dnai = NULL;
    cJSON *smf_selection_type = NULL;
    OpenAPI_smf_selection_type_e smf_selection_typeVariable = 0;
    target_dnai = cJSON_GetObjectItemCaseSensitive(target_dnai_infoJSON, "targetDnai");
    if (target_dnai) {
    if (!cJSON_IsString(target_dnai) && !cJSON_IsNull(target_dnai)) {
        ogs_error("OpenAPI_target_dnai_info_parseFromJSON() failed [target_dnai]");
        goto end;
    }
    }

    smf_selection_type = cJSON_GetObjectItemCaseSensitive(target_dnai_infoJSON, "smfSelectionType");
    if (!smf_selection_type) {
        ogs_error("OpenAPI_target_dnai_info_parseFromJSON() failed [smf_selection_type]");
        goto end;
    }
    if (!cJSON_IsString(smf_selection_type)) {
        ogs_error("OpenAPI_target_dnai_info_parseFromJSON() failed [smf_selection_type]");
        goto end;
    }
    smf_selection_typeVariable = OpenAPI_smf_selection_type_FromString(smf_selection_type->valuestring);

    target_dnai_info_local_var = OpenAPI_target_dnai_info_create (
        target_dnai && !cJSON_IsNull(target_dnai) ? ogs_strdup(target_dnai->valuestring) : NULL,
        smf_selection_typeVariable
    );

    return target_dnai_info_local_var;
end:
    return NULL;
}

OpenAPI_target_dnai_info_t *OpenAPI_target_dnai_info_copy(OpenAPI_target_dnai_info_t *dst, OpenAPI_target_dnai_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_target_dnai_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_target_dnai_info_convertToJSON() failed");
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

    OpenAPI_target_dnai_info_free(dst);
    dst = OpenAPI_target_dnai_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

