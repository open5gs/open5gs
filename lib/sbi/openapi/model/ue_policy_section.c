
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_policy_section.h"

OpenAPI_ue_policy_section_t *OpenAPI_ue_policy_section_create(
    char *ue_policy_section_info,
    char *upsi
)
{
    OpenAPI_ue_policy_section_t *ue_policy_section_local_var = ogs_malloc(sizeof(OpenAPI_ue_policy_section_t));
    ogs_assert(ue_policy_section_local_var);

    ue_policy_section_local_var->ue_policy_section_info = ue_policy_section_info;
    ue_policy_section_local_var->upsi = upsi;

    return ue_policy_section_local_var;
}

void OpenAPI_ue_policy_section_free(OpenAPI_ue_policy_section_t *ue_policy_section)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ue_policy_section) {
        return;
    }
    if (ue_policy_section->ue_policy_section_info) {
        ogs_free(ue_policy_section->ue_policy_section_info);
        ue_policy_section->ue_policy_section_info = NULL;
    }
    if (ue_policy_section->upsi) {
        ogs_free(ue_policy_section->upsi);
        ue_policy_section->upsi = NULL;
    }
    ogs_free(ue_policy_section);
}

cJSON *OpenAPI_ue_policy_section_convertToJSON(OpenAPI_ue_policy_section_t *ue_policy_section)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ue_policy_section == NULL) {
        ogs_error("OpenAPI_ue_policy_section_convertToJSON() failed [UePolicySection]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!ue_policy_section->ue_policy_section_info) {
        ogs_error("OpenAPI_ue_policy_section_convertToJSON() failed [ue_policy_section_info]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "uePolicySectionInfo", ue_policy_section->ue_policy_section_info) == NULL) {
        ogs_error("OpenAPI_ue_policy_section_convertToJSON() failed [ue_policy_section_info]");
        goto end;
    }

    if (!ue_policy_section->upsi) {
        ogs_error("OpenAPI_ue_policy_section_convertToJSON() failed [upsi]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "upsi", ue_policy_section->upsi) == NULL) {
        ogs_error("OpenAPI_ue_policy_section_convertToJSON() failed [upsi]");
        goto end;
    }

end:
    return item;
}

OpenAPI_ue_policy_section_t *OpenAPI_ue_policy_section_parseFromJSON(cJSON *ue_policy_sectionJSON)
{
    OpenAPI_ue_policy_section_t *ue_policy_section_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ue_policy_section_info = NULL;
    cJSON *upsi = NULL;
    ue_policy_section_info = cJSON_GetObjectItemCaseSensitive(ue_policy_sectionJSON, "uePolicySectionInfo");
    if (!ue_policy_section_info) {
        ogs_error("OpenAPI_ue_policy_section_parseFromJSON() failed [ue_policy_section_info]");
        goto end;
    }
    if (!cJSON_IsString(ue_policy_section_info)) {
        ogs_error("OpenAPI_ue_policy_section_parseFromJSON() failed [ue_policy_section_info]");
        goto end;
    }

    upsi = cJSON_GetObjectItemCaseSensitive(ue_policy_sectionJSON, "upsi");
    if (!upsi) {
        ogs_error("OpenAPI_ue_policy_section_parseFromJSON() failed [upsi]");
        goto end;
    }
    if (!cJSON_IsString(upsi)) {
        ogs_error("OpenAPI_ue_policy_section_parseFromJSON() failed [upsi]");
        goto end;
    }

    ue_policy_section_local_var = OpenAPI_ue_policy_section_create (
        ogs_strdup(ue_policy_section_info->valuestring),
        ogs_strdup(upsi->valuestring)
    );

    return ue_policy_section_local_var;
end:
    return NULL;
}

OpenAPI_ue_policy_section_t *OpenAPI_ue_policy_section_copy(OpenAPI_ue_policy_section_t *dst, OpenAPI_ue_policy_section_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ue_policy_section_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ue_policy_section_convertToJSON() failed");
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

    OpenAPI_ue_policy_section_free(dst);
    dst = OpenAPI_ue_policy_section_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

