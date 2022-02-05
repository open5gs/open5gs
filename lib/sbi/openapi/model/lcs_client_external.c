
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lcs_client_external.h"

OpenAPI_lcs_client_external_t *OpenAPI_lcs_client_external_create(
    OpenAPI_list_t *allowed_geographic_area,
    OpenAPI_privacy_check_related_action_e privacy_check_related_action,
    OpenAPI_valid_time_period_t *valid_time_period
)
{
    OpenAPI_lcs_client_external_t *lcs_client_external_local_var = ogs_malloc(sizeof(OpenAPI_lcs_client_external_t));
    ogs_assert(lcs_client_external_local_var);

    lcs_client_external_local_var->allowed_geographic_area = allowed_geographic_area;
    lcs_client_external_local_var->privacy_check_related_action = privacy_check_related_action;
    lcs_client_external_local_var->valid_time_period = valid_time_period;

    return lcs_client_external_local_var;
}

void OpenAPI_lcs_client_external_free(OpenAPI_lcs_client_external_t *lcs_client_external)
{
    if (NULL == lcs_client_external) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(lcs_client_external->allowed_geographic_area, node) {
        OpenAPI_geographic_area_free(node->data);
    }
    OpenAPI_list_free(lcs_client_external->allowed_geographic_area);
    OpenAPI_valid_time_period_free(lcs_client_external->valid_time_period);
    ogs_free(lcs_client_external);
}

cJSON *OpenAPI_lcs_client_external_convertToJSON(OpenAPI_lcs_client_external_t *lcs_client_external)
{
    cJSON *item = NULL;

    if (lcs_client_external == NULL) {
        ogs_error("OpenAPI_lcs_client_external_convertToJSON() failed [LcsClientExternal]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (lcs_client_external->allowed_geographic_area) {
    cJSON *allowed_geographic_areaList = cJSON_AddArrayToObject(item, "allowedGeographicArea");
    if (allowed_geographic_areaList == NULL) {
        ogs_error("OpenAPI_lcs_client_external_convertToJSON() failed [allowed_geographic_area]");
        goto end;
    }

    OpenAPI_lnode_t *allowed_geographic_area_node;
    if (lcs_client_external->allowed_geographic_area) {
        OpenAPI_list_for_each(lcs_client_external->allowed_geographic_area, allowed_geographic_area_node) {
            cJSON *itemLocal = OpenAPI_geographic_area_convertToJSON(allowed_geographic_area_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_lcs_client_external_convertToJSON() failed [allowed_geographic_area]");
                goto end;
            }
            cJSON_AddItemToArray(allowed_geographic_areaList, itemLocal);
        }
    }
    }

    if (lcs_client_external->privacy_check_related_action) {
    if (cJSON_AddStringToObject(item, "privacyCheckRelatedAction", OpenAPI_privacy_check_related_action_ToString(lcs_client_external->privacy_check_related_action)) == NULL) {
        ogs_error("OpenAPI_lcs_client_external_convertToJSON() failed [privacy_check_related_action]");
        goto end;
    }
    }

    if (lcs_client_external->valid_time_period) {
    cJSON *valid_time_period_local_JSON = OpenAPI_valid_time_period_convertToJSON(lcs_client_external->valid_time_period);
    if (valid_time_period_local_JSON == NULL) {
        ogs_error("OpenAPI_lcs_client_external_convertToJSON() failed [valid_time_period]");
        goto end;
    }
    cJSON_AddItemToObject(item, "validTimePeriod", valid_time_period_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_lcs_client_external_convertToJSON() failed [valid_time_period]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_lcs_client_external_t *OpenAPI_lcs_client_external_parseFromJSON(cJSON *lcs_client_externalJSON)
{
    OpenAPI_lcs_client_external_t *lcs_client_external_local_var = NULL;
    cJSON *allowed_geographic_area = cJSON_GetObjectItemCaseSensitive(lcs_client_externalJSON, "allowedGeographicArea");

    OpenAPI_list_t *allowed_geographic_areaList;
    if (allowed_geographic_area) {
    cJSON *allowed_geographic_area_local_nonprimitive;
    if (!cJSON_IsArray(allowed_geographic_area)){
        ogs_error("OpenAPI_lcs_client_external_parseFromJSON() failed [allowed_geographic_area]");
        goto end;
    }

    allowed_geographic_areaList = OpenAPI_list_create();

    cJSON_ArrayForEach(allowed_geographic_area_local_nonprimitive, allowed_geographic_area ) {
        if (!cJSON_IsObject(allowed_geographic_area_local_nonprimitive)) {
            ogs_error("OpenAPI_lcs_client_external_parseFromJSON() failed [allowed_geographic_area]");
            goto end;
        }
        OpenAPI_geographic_area_t *allowed_geographic_areaItem = OpenAPI_geographic_area_parseFromJSON(allowed_geographic_area_local_nonprimitive);

        if (!allowed_geographic_areaItem) {
            ogs_error("No allowed_geographic_areaItem");
            OpenAPI_list_free(allowed_geographic_areaList);
            goto end;
        }

        OpenAPI_list_add(allowed_geographic_areaList, allowed_geographic_areaItem);
    }
    }

    cJSON *privacy_check_related_action = cJSON_GetObjectItemCaseSensitive(lcs_client_externalJSON, "privacyCheckRelatedAction");

    OpenAPI_privacy_check_related_action_e privacy_check_related_actionVariable;
    if (privacy_check_related_action) {
    if (!cJSON_IsString(privacy_check_related_action)) {
        ogs_error("OpenAPI_lcs_client_external_parseFromJSON() failed [privacy_check_related_action]");
        goto end;
    }
    privacy_check_related_actionVariable = OpenAPI_privacy_check_related_action_FromString(privacy_check_related_action->valuestring);
    }

    cJSON *valid_time_period = cJSON_GetObjectItemCaseSensitive(lcs_client_externalJSON, "validTimePeriod");

    OpenAPI_valid_time_period_t *valid_time_period_local_nonprim = NULL;
    if (valid_time_period) {
    valid_time_period_local_nonprim = OpenAPI_valid_time_period_parseFromJSON(valid_time_period);
    }

    lcs_client_external_local_var = OpenAPI_lcs_client_external_create (
        allowed_geographic_area ? allowed_geographic_areaList : NULL,
        privacy_check_related_action ? privacy_check_related_actionVariable : 0,
        valid_time_period ? valid_time_period_local_nonprim : NULL
    );

    return lcs_client_external_local_var;
end:
    return NULL;
}

OpenAPI_lcs_client_external_t *OpenAPI_lcs_client_external_copy(OpenAPI_lcs_client_external_t *dst, OpenAPI_lcs_client_external_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_lcs_client_external_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_lcs_client_external_convertToJSON() failed");
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

    OpenAPI_lcs_client_external_free(dst);
    dst = OpenAPI_lcs_client_external_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

