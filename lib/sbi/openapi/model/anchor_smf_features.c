
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "anchor_smf_features.h"

OpenAPI_anchor_smf_features_t *OpenAPI_anchor_smf_features_create(
    bool is_psetr_support_ind,
    int psetr_support_ind
)
{
    OpenAPI_anchor_smf_features_t *anchor_smf_features_local_var = ogs_malloc(sizeof(OpenAPI_anchor_smf_features_t));
    ogs_assert(anchor_smf_features_local_var);

    anchor_smf_features_local_var->is_psetr_support_ind = is_psetr_support_ind;
    anchor_smf_features_local_var->psetr_support_ind = psetr_support_ind;

    return anchor_smf_features_local_var;
}

void OpenAPI_anchor_smf_features_free(OpenAPI_anchor_smf_features_t *anchor_smf_features)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == anchor_smf_features) {
        return;
    }
    ogs_free(anchor_smf_features);
}

cJSON *OpenAPI_anchor_smf_features_convertToJSON(OpenAPI_anchor_smf_features_t *anchor_smf_features)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (anchor_smf_features == NULL) {
        ogs_error("OpenAPI_anchor_smf_features_convertToJSON() failed [AnchorSmfFeatures]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (anchor_smf_features->is_psetr_support_ind) {
    if (cJSON_AddBoolToObject(item, "psetrSupportInd", anchor_smf_features->psetr_support_ind) == NULL) {
        ogs_error("OpenAPI_anchor_smf_features_convertToJSON() failed [psetr_support_ind]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_anchor_smf_features_t *OpenAPI_anchor_smf_features_parseFromJSON(cJSON *anchor_smf_featuresJSON)
{
    OpenAPI_anchor_smf_features_t *anchor_smf_features_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *psetr_support_ind = NULL;
    psetr_support_ind = cJSON_GetObjectItemCaseSensitive(anchor_smf_featuresJSON, "psetrSupportInd");
    if (psetr_support_ind) {
    if (!cJSON_IsBool(psetr_support_ind)) {
        ogs_error("OpenAPI_anchor_smf_features_parseFromJSON() failed [psetr_support_ind]");
        goto end;
    }
    }

    anchor_smf_features_local_var = OpenAPI_anchor_smf_features_create (
        psetr_support_ind ? true : false,
        psetr_support_ind ? psetr_support_ind->valueint : 0
    );

    return anchor_smf_features_local_var;
end:
    return NULL;
}

OpenAPI_anchor_smf_features_t *OpenAPI_anchor_smf_features_copy(OpenAPI_anchor_smf_features_t *dst, OpenAPI_anchor_smf_features_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_anchor_smf_features_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_anchor_smf_features_convertToJSON() failed");
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

    OpenAPI_anchor_smf_features_free(dst);
    dst = OpenAPI_anchor_smf_features_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

