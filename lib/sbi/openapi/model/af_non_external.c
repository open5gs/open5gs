
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "af_non_external.h"

OpenAPI_af_non_external_t *OpenAPI_af_non_external_create(
    char *af_id,
    OpenAPI_list_t *allowed_geographic_area,
    OpenAPI_privacy_check_related_action_e privacy_check_related_action,
    OpenAPI_code_word_ind_e code_word_ind,
    OpenAPI_valid_time_period_t *valid_time_period
    )
{
    OpenAPI_af_non_external_t *af_non_external_local_var = OpenAPI_malloc(sizeof(OpenAPI_af_non_external_t));
    if (!af_non_external_local_var) {
        return NULL;
    }
    af_non_external_local_var->af_id = af_id;
    af_non_external_local_var->allowed_geographic_area = allowed_geographic_area;
    af_non_external_local_var->privacy_check_related_action = privacy_check_related_action;
    af_non_external_local_var->code_word_ind = code_word_ind;
    af_non_external_local_var->valid_time_period = valid_time_period;

    return af_non_external_local_var;
}

void OpenAPI_af_non_external_free(OpenAPI_af_non_external_t *af_non_external)
{
    if (NULL == af_non_external) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(af_non_external->af_id);
    OpenAPI_list_for_each(af_non_external->allowed_geographic_area, node) {
        OpenAPI_geographic_area_free(node->data);
    }
    OpenAPI_list_free(af_non_external->allowed_geographic_area);
    OpenAPI_valid_time_period_free(af_non_external->valid_time_period);
    ogs_free(af_non_external);
}

cJSON *OpenAPI_af_non_external_convertToJSON(OpenAPI_af_non_external_t *af_non_external)
{
    cJSON *item = NULL;

    if (af_non_external == NULL) {
        ogs_error("OpenAPI_af_non_external_convertToJSON() failed [AfNonExternal]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!af_non_external->af_id) {
        ogs_error("OpenAPI_af_non_external_convertToJSON() failed [af_id]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "afId", af_non_external->af_id) == NULL) {
        ogs_error("OpenAPI_af_non_external_convertToJSON() failed [af_id]");
        goto end;
    }

    if (af_non_external->allowed_geographic_area) {
        cJSON *allowed_geographic_areaList = cJSON_AddArrayToObject(item, "allowedGeographicArea");
        if (allowed_geographic_areaList == NULL) {
            ogs_error("OpenAPI_af_non_external_convertToJSON() failed [allowed_geographic_area]");
            goto end;
        }

        OpenAPI_lnode_t *allowed_geographic_area_node;
        if (af_non_external->allowed_geographic_area) {
            OpenAPI_list_for_each(af_non_external->allowed_geographic_area, allowed_geographic_area_node) {
                cJSON *itemLocal = OpenAPI_geographic_area_convertToJSON(allowed_geographic_area_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_af_non_external_convertToJSON() failed [allowed_geographic_area]");
                    goto end;
                }
                cJSON_AddItemToArray(allowed_geographic_areaList, itemLocal);
            }
        }
    }

    if (af_non_external->privacy_check_related_action) {
        if (cJSON_AddStringToObject(item, "privacyCheckRelatedAction", OpenAPI_privacy_check_related_action_ToString(af_non_external->privacy_check_related_action)) == NULL) {
            ogs_error("OpenAPI_af_non_external_convertToJSON() failed [privacy_check_related_action]");
            goto end;
        }
    }

    if (af_non_external->code_word_ind) {
        if (cJSON_AddStringToObject(item, "codeWordInd", OpenAPI_code_word_ind_ToString(af_non_external->code_word_ind)) == NULL) {
            ogs_error("OpenAPI_af_non_external_convertToJSON() failed [code_word_ind]");
            goto end;
        }
    }

    if (af_non_external->valid_time_period) {
        cJSON *valid_time_period_local_JSON = OpenAPI_valid_time_period_convertToJSON(af_non_external->valid_time_period);
        if (valid_time_period_local_JSON == NULL) {
            ogs_error("OpenAPI_af_non_external_convertToJSON() failed [valid_time_period]");
            goto end;
        }
        cJSON_AddItemToObject(item, "validTimePeriod", valid_time_period_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_af_non_external_convertToJSON() failed [valid_time_period]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_af_non_external_t *OpenAPI_af_non_external_parseFromJSON(cJSON *af_non_externalJSON)
{
    OpenAPI_af_non_external_t *af_non_external_local_var = NULL;
    cJSON *af_id = cJSON_GetObjectItemCaseSensitive(af_non_externalJSON, "afId");
    if (!af_id) {
        ogs_error("OpenAPI_af_non_external_parseFromJSON() failed [af_id]");
        goto end;
    }


    if (!cJSON_IsString(af_id)) {
        ogs_error("OpenAPI_af_non_external_parseFromJSON() failed [af_id]");
        goto end;
    }

    cJSON *allowed_geographic_area = cJSON_GetObjectItemCaseSensitive(af_non_externalJSON, "allowedGeographicArea");

    OpenAPI_list_t *allowed_geographic_areaList;
    if (allowed_geographic_area) {
        cJSON *allowed_geographic_area_local_nonprimitive;
        if (!cJSON_IsArray(allowed_geographic_area)) {
            ogs_error("OpenAPI_af_non_external_parseFromJSON() failed [allowed_geographic_area]");
            goto end;
        }

        allowed_geographic_areaList = OpenAPI_list_create();

        cJSON_ArrayForEach(allowed_geographic_area_local_nonprimitive, allowed_geographic_area ) {
            if (!cJSON_IsObject(allowed_geographic_area_local_nonprimitive)) {
                ogs_error("OpenAPI_af_non_external_parseFromJSON() failed [allowed_geographic_area]");
                goto end;
            }
            OpenAPI_geographic_area_t *allowed_geographic_areaItem = OpenAPI_geographic_area_parseFromJSON(allowed_geographic_area_local_nonprimitive);

            OpenAPI_list_add(allowed_geographic_areaList, allowed_geographic_areaItem);
        }
    }

    cJSON *privacy_check_related_action = cJSON_GetObjectItemCaseSensitive(af_non_externalJSON, "privacyCheckRelatedAction");

    OpenAPI_privacy_check_related_action_e privacy_check_related_actionVariable;
    if (privacy_check_related_action) {
        if (!cJSON_IsString(privacy_check_related_action)) {
            ogs_error("OpenAPI_af_non_external_parseFromJSON() failed [privacy_check_related_action]");
            goto end;
        }
        privacy_check_related_actionVariable = OpenAPI_privacy_check_related_action_FromString(privacy_check_related_action->valuestring);
    }

    cJSON *code_word_ind = cJSON_GetObjectItemCaseSensitive(af_non_externalJSON, "codeWordInd");

    OpenAPI_code_word_ind_e code_word_indVariable;
    if (code_word_ind) {
        if (!cJSON_IsString(code_word_ind)) {
            ogs_error("OpenAPI_af_non_external_parseFromJSON() failed [code_word_ind]");
            goto end;
        }
        code_word_indVariable = OpenAPI_code_word_ind_FromString(code_word_ind->valuestring);
    }

    cJSON *valid_time_period = cJSON_GetObjectItemCaseSensitive(af_non_externalJSON, "validTimePeriod");

    OpenAPI_valid_time_period_t *valid_time_period_local_nonprim = NULL;
    if (valid_time_period) {
        valid_time_period_local_nonprim = OpenAPI_valid_time_period_parseFromJSON(valid_time_period);
    }

    af_non_external_local_var = OpenAPI_af_non_external_create (
        ogs_strdup(af_id->valuestring),
        allowed_geographic_area ? allowed_geographic_areaList : NULL,
        privacy_check_related_action ? privacy_check_related_actionVariable : 0,
        code_word_ind ? code_word_indVariable : 0,
        valid_time_period ? valid_time_period_local_nonprim : NULL
        );

    return af_non_external_local_var;
end:
    return NULL;
}

OpenAPI_af_non_external_t *OpenAPI_af_non_external_copy(OpenAPI_af_non_external_t *dst, OpenAPI_af_non_external_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_af_non_external_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_af_non_external_convertToJSON() failed");
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

    OpenAPI_af_non_external_free(dst);
    dst = OpenAPI_af_non_external_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

