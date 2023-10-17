
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "aerial_ue_subscription_info.h"

OpenAPI_aerial_ue_subscription_info_t *OpenAPI_aerial_ue_subscription_info_create(
    OpenAPI_aerial_ue_indication_e aerial_ue_ind,
    char *_3gpp_uav_id
)
{
    OpenAPI_aerial_ue_subscription_info_t *aerial_ue_subscription_info_local_var = ogs_malloc(sizeof(OpenAPI_aerial_ue_subscription_info_t));
    ogs_assert(aerial_ue_subscription_info_local_var);

    aerial_ue_subscription_info_local_var->aerial_ue_ind = aerial_ue_ind;
    aerial_ue_subscription_info_local_var->_3gpp_uav_id = _3gpp_uav_id;

    return aerial_ue_subscription_info_local_var;
}

void OpenAPI_aerial_ue_subscription_info_free(OpenAPI_aerial_ue_subscription_info_t *aerial_ue_subscription_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == aerial_ue_subscription_info) {
        return;
    }
    if (aerial_ue_subscription_info->_3gpp_uav_id) {
        ogs_free(aerial_ue_subscription_info->_3gpp_uav_id);
        aerial_ue_subscription_info->_3gpp_uav_id = NULL;
    }
    ogs_free(aerial_ue_subscription_info);
}

cJSON *OpenAPI_aerial_ue_subscription_info_convertToJSON(OpenAPI_aerial_ue_subscription_info_t *aerial_ue_subscription_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (aerial_ue_subscription_info == NULL) {
        ogs_error("OpenAPI_aerial_ue_subscription_info_convertToJSON() failed [AerialUeSubscriptionInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (aerial_ue_subscription_info->aerial_ue_ind == OpenAPI_aerial_ue_indication_NULL) {
        ogs_error("OpenAPI_aerial_ue_subscription_info_convertToJSON() failed [aerial_ue_ind]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "aerialUeInd", OpenAPI_aerial_ue_indication_ToString(aerial_ue_subscription_info->aerial_ue_ind)) == NULL) {
        ogs_error("OpenAPI_aerial_ue_subscription_info_convertToJSON() failed [aerial_ue_ind]");
        goto end;
    }

    if (aerial_ue_subscription_info->_3gpp_uav_id) {
    if (cJSON_AddStringToObject(item, "3gppUavId", aerial_ue_subscription_info->_3gpp_uav_id) == NULL) {
        ogs_error("OpenAPI_aerial_ue_subscription_info_convertToJSON() failed [_3gpp_uav_id]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_aerial_ue_subscription_info_t *OpenAPI_aerial_ue_subscription_info_parseFromJSON(cJSON *aerial_ue_subscription_infoJSON)
{
    OpenAPI_aerial_ue_subscription_info_t *aerial_ue_subscription_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *aerial_ue_ind = NULL;
    OpenAPI_aerial_ue_indication_e aerial_ue_indVariable = 0;
    cJSON *_3gpp_uav_id = NULL;
    aerial_ue_ind = cJSON_GetObjectItemCaseSensitive(aerial_ue_subscription_infoJSON, "aerialUeInd");
    if (!aerial_ue_ind) {
        ogs_error("OpenAPI_aerial_ue_subscription_info_parseFromJSON() failed [aerial_ue_ind]");
        goto end;
    }
    if (!cJSON_IsString(aerial_ue_ind)) {
        ogs_error("OpenAPI_aerial_ue_subscription_info_parseFromJSON() failed [aerial_ue_ind]");
        goto end;
    }
    aerial_ue_indVariable = OpenAPI_aerial_ue_indication_FromString(aerial_ue_ind->valuestring);

    _3gpp_uav_id = cJSON_GetObjectItemCaseSensitive(aerial_ue_subscription_infoJSON, "3gppUavId");
    if (_3gpp_uav_id) {
    if (!cJSON_IsString(_3gpp_uav_id) && !cJSON_IsNull(_3gpp_uav_id)) {
        ogs_error("OpenAPI_aerial_ue_subscription_info_parseFromJSON() failed [_3gpp_uav_id]");
        goto end;
    }
    }

    aerial_ue_subscription_info_local_var = OpenAPI_aerial_ue_subscription_info_create (
        aerial_ue_indVariable,
        _3gpp_uav_id && !cJSON_IsNull(_3gpp_uav_id) ? ogs_strdup(_3gpp_uav_id->valuestring) : NULL
    );

    return aerial_ue_subscription_info_local_var;
end:
    return NULL;
}

OpenAPI_aerial_ue_subscription_info_t *OpenAPI_aerial_ue_subscription_info_copy(OpenAPI_aerial_ue_subscription_info_t *dst, OpenAPI_aerial_ue_subscription_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_aerial_ue_subscription_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_aerial_ue_subscription_info_convertToJSON() failed");
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

    OpenAPI_aerial_ue_subscription_info_free(dst);
    dst = OpenAPI_aerial_ue_subscription_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

