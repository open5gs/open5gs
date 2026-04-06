
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "group_policy_data.h"

OpenAPI_group_policy_data_t *OpenAPI_group_policy_data_create(
    char *max_group_mbr_ul,
    char *max_group_mbr_dl,
    char *remain_group_mbr_ul,
    char *remain_group_mbr_dl,
    char *supp_feat
)
{
    OpenAPI_group_policy_data_t *group_policy_data_local_var = ogs_malloc(sizeof(OpenAPI_group_policy_data_t));
    ogs_assert(group_policy_data_local_var);

    group_policy_data_local_var->max_group_mbr_ul = max_group_mbr_ul;
    group_policy_data_local_var->max_group_mbr_dl = max_group_mbr_dl;
    group_policy_data_local_var->remain_group_mbr_ul = remain_group_mbr_ul;
    group_policy_data_local_var->remain_group_mbr_dl = remain_group_mbr_dl;
    group_policy_data_local_var->supp_feat = supp_feat;

    return group_policy_data_local_var;
}

void OpenAPI_group_policy_data_free(OpenAPI_group_policy_data_t *group_policy_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == group_policy_data) {
        return;
    }
    if (group_policy_data->max_group_mbr_ul) {
        ogs_free(group_policy_data->max_group_mbr_ul);
        group_policy_data->max_group_mbr_ul = NULL;
    }
    if (group_policy_data->max_group_mbr_dl) {
        ogs_free(group_policy_data->max_group_mbr_dl);
        group_policy_data->max_group_mbr_dl = NULL;
    }
    if (group_policy_data->remain_group_mbr_ul) {
        ogs_free(group_policy_data->remain_group_mbr_ul);
        group_policy_data->remain_group_mbr_ul = NULL;
    }
    if (group_policy_data->remain_group_mbr_dl) {
        ogs_free(group_policy_data->remain_group_mbr_dl);
        group_policy_data->remain_group_mbr_dl = NULL;
    }
    if (group_policy_data->supp_feat) {
        ogs_free(group_policy_data->supp_feat);
        group_policy_data->supp_feat = NULL;
    }
    ogs_free(group_policy_data);
}

cJSON *OpenAPI_group_policy_data_convertToJSON(OpenAPI_group_policy_data_t *group_policy_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (group_policy_data == NULL) {
        ogs_error("OpenAPI_group_policy_data_convertToJSON() failed [GroupPolicyData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (group_policy_data->max_group_mbr_ul) {
    if (cJSON_AddStringToObject(item, "maxGroupMbrUl", group_policy_data->max_group_mbr_ul) == NULL) {
        ogs_error("OpenAPI_group_policy_data_convertToJSON() failed [max_group_mbr_ul]");
        goto end;
    }
    }

    if (group_policy_data->max_group_mbr_dl) {
    if (cJSON_AddStringToObject(item, "maxGroupMbrDl", group_policy_data->max_group_mbr_dl) == NULL) {
        ogs_error("OpenAPI_group_policy_data_convertToJSON() failed [max_group_mbr_dl]");
        goto end;
    }
    }

    if (group_policy_data->remain_group_mbr_ul) {
    if (cJSON_AddStringToObject(item, "remainGroupMbrUl", group_policy_data->remain_group_mbr_ul) == NULL) {
        ogs_error("OpenAPI_group_policy_data_convertToJSON() failed [remain_group_mbr_ul]");
        goto end;
    }
    }

    if (group_policy_data->remain_group_mbr_dl) {
    if (cJSON_AddStringToObject(item, "remainGroupMbrDl", group_policy_data->remain_group_mbr_dl) == NULL) {
        ogs_error("OpenAPI_group_policy_data_convertToJSON() failed [remain_group_mbr_dl]");
        goto end;
    }
    }

    if (group_policy_data->supp_feat) {
    if (cJSON_AddStringToObject(item, "suppFeat", group_policy_data->supp_feat) == NULL) {
        ogs_error("OpenAPI_group_policy_data_convertToJSON() failed [supp_feat]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_group_policy_data_t *OpenAPI_group_policy_data_parseFromJSON(cJSON *group_policy_dataJSON)
{
    OpenAPI_group_policy_data_t *group_policy_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *max_group_mbr_ul = NULL;
    cJSON *max_group_mbr_dl = NULL;
    cJSON *remain_group_mbr_ul = NULL;
    cJSON *remain_group_mbr_dl = NULL;
    cJSON *supp_feat = NULL;
    max_group_mbr_ul = cJSON_GetObjectItemCaseSensitive(group_policy_dataJSON, "maxGroupMbrUl");
    if (max_group_mbr_ul) {
    if (!cJSON_IsString(max_group_mbr_ul) && !cJSON_IsNull(max_group_mbr_ul)) {
        ogs_error("OpenAPI_group_policy_data_parseFromJSON() failed [max_group_mbr_ul]");
        goto end;
    }
    }

    max_group_mbr_dl = cJSON_GetObjectItemCaseSensitive(group_policy_dataJSON, "maxGroupMbrDl");
    if (max_group_mbr_dl) {
    if (!cJSON_IsString(max_group_mbr_dl) && !cJSON_IsNull(max_group_mbr_dl)) {
        ogs_error("OpenAPI_group_policy_data_parseFromJSON() failed [max_group_mbr_dl]");
        goto end;
    }
    }

    remain_group_mbr_ul = cJSON_GetObjectItemCaseSensitive(group_policy_dataJSON, "remainGroupMbrUl");
    if (remain_group_mbr_ul) {
    if (!cJSON_IsString(remain_group_mbr_ul) && !cJSON_IsNull(remain_group_mbr_ul)) {
        ogs_error("OpenAPI_group_policy_data_parseFromJSON() failed [remain_group_mbr_ul]");
        goto end;
    }
    }

    remain_group_mbr_dl = cJSON_GetObjectItemCaseSensitive(group_policy_dataJSON, "remainGroupMbrDl");
    if (remain_group_mbr_dl) {
    if (!cJSON_IsString(remain_group_mbr_dl) && !cJSON_IsNull(remain_group_mbr_dl)) {
        ogs_error("OpenAPI_group_policy_data_parseFromJSON() failed [remain_group_mbr_dl]");
        goto end;
    }
    }

    supp_feat = cJSON_GetObjectItemCaseSensitive(group_policy_dataJSON, "suppFeat");
    if (supp_feat) {
    if (!cJSON_IsString(supp_feat) && !cJSON_IsNull(supp_feat)) {
        ogs_error("OpenAPI_group_policy_data_parseFromJSON() failed [supp_feat]");
        goto end;
    }
    }

    group_policy_data_local_var = OpenAPI_group_policy_data_create (
        max_group_mbr_ul && !cJSON_IsNull(max_group_mbr_ul) ? ogs_strdup(max_group_mbr_ul->valuestring) : NULL,
        max_group_mbr_dl && !cJSON_IsNull(max_group_mbr_dl) ? ogs_strdup(max_group_mbr_dl->valuestring) : NULL,
        remain_group_mbr_ul && !cJSON_IsNull(remain_group_mbr_ul) ? ogs_strdup(remain_group_mbr_ul->valuestring) : NULL,
        remain_group_mbr_dl && !cJSON_IsNull(remain_group_mbr_dl) ? ogs_strdup(remain_group_mbr_dl->valuestring) : NULL,
        supp_feat && !cJSON_IsNull(supp_feat) ? ogs_strdup(supp_feat->valuestring) : NULL
    );

    return group_policy_data_local_var;
end:
    return NULL;
}

OpenAPI_group_policy_data_t *OpenAPI_group_policy_data_copy(OpenAPI_group_policy_data_t *dst, OpenAPI_group_policy_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_group_policy_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_group_policy_data_convertToJSON() failed");
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

    OpenAPI_group_policy_data_free(dst);
    dst = OpenAPI_group_policy_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

