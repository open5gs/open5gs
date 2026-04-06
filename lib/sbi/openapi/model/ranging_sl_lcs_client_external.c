
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ranging_sl_lcs_client_external.h"

OpenAPI_ranging_sl_lcs_client_external_t *OpenAPI_ranging_sl_lcs_client_external_create(
    OpenAPI_list_t *allowed_geographic_area,
    OpenAPI_ranging_sl_privacy_check_related_action_e ranging_sl_privacy_check_related_action,
    OpenAPI_valid_time_period_t *valid_time_period
)
{
    OpenAPI_ranging_sl_lcs_client_external_t *ranging_sl_lcs_client_external_local_var = ogs_malloc(sizeof(OpenAPI_ranging_sl_lcs_client_external_t));
    ogs_assert(ranging_sl_lcs_client_external_local_var);

    ranging_sl_lcs_client_external_local_var->allowed_geographic_area = allowed_geographic_area;
    ranging_sl_lcs_client_external_local_var->ranging_sl_privacy_check_related_action = ranging_sl_privacy_check_related_action;
    ranging_sl_lcs_client_external_local_var->valid_time_period = valid_time_period;

    return ranging_sl_lcs_client_external_local_var;
}

void OpenAPI_ranging_sl_lcs_client_external_free(OpenAPI_ranging_sl_lcs_client_external_t *ranging_sl_lcs_client_external)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ranging_sl_lcs_client_external) {
        return;
    }
    if (ranging_sl_lcs_client_external->allowed_geographic_area) {
        OpenAPI_list_for_each(ranging_sl_lcs_client_external->allowed_geographic_area, node) {
            OpenAPI_geographic_area_free(node->data);
        }
        OpenAPI_list_free(ranging_sl_lcs_client_external->allowed_geographic_area);
        ranging_sl_lcs_client_external->allowed_geographic_area = NULL;
    }
    if (ranging_sl_lcs_client_external->valid_time_period) {
        OpenAPI_valid_time_period_free(ranging_sl_lcs_client_external->valid_time_period);
        ranging_sl_lcs_client_external->valid_time_period = NULL;
    }
    ogs_free(ranging_sl_lcs_client_external);
}

cJSON *OpenAPI_ranging_sl_lcs_client_external_convertToJSON(OpenAPI_ranging_sl_lcs_client_external_t *ranging_sl_lcs_client_external)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ranging_sl_lcs_client_external == NULL) {
        ogs_error("OpenAPI_ranging_sl_lcs_client_external_convertToJSON() failed [RangingSlLcsClientExternal]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ranging_sl_lcs_client_external->allowed_geographic_area) {
    cJSON *allowed_geographic_areaList = cJSON_AddArrayToObject(item, "allowedGeographicArea");
    if (allowed_geographic_areaList == NULL) {
        ogs_error("OpenAPI_ranging_sl_lcs_client_external_convertToJSON() failed [allowed_geographic_area]");
        goto end;
    }
    OpenAPI_list_for_each(ranging_sl_lcs_client_external->allowed_geographic_area, node) {
        cJSON *itemLocal = OpenAPI_geographic_area_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_ranging_sl_lcs_client_external_convertToJSON() failed [allowed_geographic_area]");
            goto end;
        }
        cJSON_AddItemToArray(allowed_geographic_areaList, itemLocal);
    }
    }

    if (ranging_sl_lcs_client_external->ranging_sl_privacy_check_related_action != OpenAPI_ranging_sl_privacy_check_related_action_NULL) {
    if (cJSON_AddStringToObject(item, "rangingSlPrivacyCheckRelatedAction", OpenAPI_ranging_sl_privacy_check_related_action_ToString(ranging_sl_lcs_client_external->ranging_sl_privacy_check_related_action)) == NULL) {
        ogs_error("OpenAPI_ranging_sl_lcs_client_external_convertToJSON() failed [ranging_sl_privacy_check_related_action]");
        goto end;
    }
    }

    if (ranging_sl_lcs_client_external->valid_time_period) {
    cJSON *valid_time_period_local_JSON = OpenAPI_valid_time_period_convertToJSON(ranging_sl_lcs_client_external->valid_time_period);
    if (valid_time_period_local_JSON == NULL) {
        ogs_error("OpenAPI_ranging_sl_lcs_client_external_convertToJSON() failed [valid_time_period]");
        goto end;
    }
    cJSON_AddItemToObject(item, "validTimePeriod", valid_time_period_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ranging_sl_lcs_client_external_convertToJSON() failed [valid_time_period]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ranging_sl_lcs_client_external_t *OpenAPI_ranging_sl_lcs_client_external_parseFromJSON(cJSON *ranging_sl_lcs_client_externalJSON)
{
    OpenAPI_ranging_sl_lcs_client_external_t *ranging_sl_lcs_client_external_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *allowed_geographic_area = NULL;
    OpenAPI_list_t *allowed_geographic_areaList = NULL;
    cJSON *ranging_sl_privacy_check_related_action = NULL;
    OpenAPI_ranging_sl_privacy_check_related_action_e ranging_sl_privacy_check_related_actionVariable = 0;
    cJSON *valid_time_period = NULL;
    OpenAPI_valid_time_period_t *valid_time_period_local_nonprim = NULL;
    allowed_geographic_area = cJSON_GetObjectItemCaseSensitive(ranging_sl_lcs_client_externalJSON, "allowedGeographicArea");
    if (allowed_geographic_area) {
        cJSON *allowed_geographic_area_local = NULL;
        if (!cJSON_IsArray(allowed_geographic_area)) {
            ogs_error("OpenAPI_ranging_sl_lcs_client_external_parseFromJSON() failed [allowed_geographic_area]");
            goto end;
        }

        allowed_geographic_areaList = OpenAPI_list_create();

        cJSON_ArrayForEach(allowed_geographic_area_local, allowed_geographic_area) {
            if (!cJSON_IsObject(allowed_geographic_area_local)) {
                ogs_error("OpenAPI_ranging_sl_lcs_client_external_parseFromJSON() failed [allowed_geographic_area]");
                goto end;
            }
            OpenAPI_geographic_area_t *allowed_geographic_areaItem = OpenAPI_geographic_area_parseFromJSON(allowed_geographic_area_local);
            if (!allowed_geographic_areaItem) {
                ogs_error("No allowed_geographic_areaItem");
                goto end;
            }
            OpenAPI_list_add(allowed_geographic_areaList, allowed_geographic_areaItem);
        }
    }

    ranging_sl_privacy_check_related_action = cJSON_GetObjectItemCaseSensitive(ranging_sl_lcs_client_externalJSON, "rangingSlPrivacyCheckRelatedAction");
    if (ranging_sl_privacy_check_related_action) {
    if (!cJSON_IsString(ranging_sl_privacy_check_related_action)) {
        ogs_error("OpenAPI_ranging_sl_lcs_client_external_parseFromJSON() failed [ranging_sl_privacy_check_related_action]");
        goto end;
    }
    ranging_sl_privacy_check_related_actionVariable = OpenAPI_ranging_sl_privacy_check_related_action_FromString(ranging_sl_privacy_check_related_action->valuestring);
    }

    valid_time_period = cJSON_GetObjectItemCaseSensitive(ranging_sl_lcs_client_externalJSON, "validTimePeriod");
    if (valid_time_period) {
    valid_time_period_local_nonprim = OpenAPI_valid_time_period_parseFromJSON(valid_time_period);
    if (!valid_time_period_local_nonprim) {
        ogs_error("OpenAPI_valid_time_period_parseFromJSON failed [valid_time_period]");
        goto end;
    }
    }

    ranging_sl_lcs_client_external_local_var = OpenAPI_ranging_sl_lcs_client_external_create (
        allowed_geographic_area ? allowed_geographic_areaList : NULL,
        ranging_sl_privacy_check_related_action ? ranging_sl_privacy_check_related_actionVariable : 0,
        valid_time_period ? valid_time_period_local_nonprim : NULL
    );

    return ranging_sl_lcs_client_external_local_var;
end:
    if (allowed_geographic_areaList) {
        OpenAPI_list_for_each(allowed_geographic_areaList, node) {
            OpenAPI_geographic_area_free(node->data);
        }
        OpenAPI_list_free(allowed_geographic_areaList);
        allowed_geographic_areaList = NULL;
    }
    if (valid_time_period_local_nonprim) {
        OpenAPI_valid_time_period_free(valid_time_period_local_nonprim);
        valid_time_period_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_ranging_sl_lcs_client_external_t *OpenAPI_ranging_sl_lcs_client_external_copy(OpenAPI_ranging_sl_lcs_client_external_t *dst, OpenAPI_ranging_sl_lcs_client_external_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ranging_sl_lcs_client_external_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ranging_sl_lcs_client_external_convertToJSON() failed");
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

    OpenAPI_ranging_sl_lcs_client_external_free(dst);
    dst = OpenAPI_ranging_sl_lcs_client_external_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

