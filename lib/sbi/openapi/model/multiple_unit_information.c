
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "multiple_unit_information.h"

OpenAPI_multiple_unit_information_t *OpenAPI_multiple_unit_information_create(
    OpenAPI_result_code_e result_code,
    int rating_group,
    OpenAPI_granted_unit_t *granted_unit,
    OpenAPI_allocated_unit_t *allocated_unit,
    OpenAPI_list_t *triggers,
    bool is_validity_time,
    int validity_time,
    bool is_quota_holding_time,
    int quota_holding_time,
    OpenAPI_final_unit_indication_t *final_unit_indication,
    bool is_time_quota_threshold,
    int time_quota_threshold,
    bool is_volume_quota_threshold,
    long volume_quota_threshold,
    bool is_unit_quota_threshold,
    int unit_quota_threshold,
    char *u_pfid,
    OpenAPI_announcement_information_t *announcement_information,
    char *m_bupfid
)
{
    OpenAPI_multiple_unit_information_t *multiple_unit_information_local_var = ogs_malloc(sizeof(OpenAPI_multiple_unit_information_t));
    ogs_assert(multiple_unit_information_local_var);

    multiple_unit_information_local_var->result_code = result_code;
    multiple_unit_information_local_var->rating_group = rating_group;
    multiple_unit_information_local_var->granted_unit = granted_unit;
    multiple_unit_information_local_var->allocated_unit = allocated_unit;
    multiple_unit_information_local_var->triggers = triggers;
    multiple_unit_information_local_var->is_validity_time = is_validity_time;
    multiple_unit_information_local_var->validity_time = validity_time;
    multiple_unit_information_local_var->is_quota_holding_time = is_quota_holding_time;
    multiple_unit_information_local_var->quota_holding_time = quota_holding_time;
    multiple_unit_information_local_var->final_unit_indication = final_unit_indication;
    multiple_unit_information_local_var->is_time_quota_threshold = is_time_quota_threshold;
    multiple_unit_information_local_var->time_quota_threshold = time_quota_threshold;
    multiple_unit_information_local_var->is_volume_quota_threshold = is_volume_quota_threshold;
    multiple_unit_information_local_var->volume_quota_threshold = volume_quota_threshold;
    multiple_unit_information_local_var->is_unit_quota_threshold = is_unit_quota_threshold;
    multiple_unit_information_local_var->unit_quota_threshold = unit_quota_threshold;
    multiple_unit_information_local_var->u_pfid = u_pfid;
    multiple_unit_information_local_var->announcement_information = announcement_information;
    multiple_unit_information_local_var->m_bupfid = m_bupfid;

    return multiple_unit_information_local_var;
}

void OpenAPI_multiple_unit_information_free(OpenAPI_multiple_unit_information_t *multiple_unit_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == multiple_unit_information) {
        return;
    }
    if (multiple_unit_information->granted_unit) {
        OpenAPI_granted_unit_free(multiple_unit_information->granted_unit);
        multiple_unit_information->granted_unit = NULL;
    }
    if (multiple_unit_information->allocated_unit) {
        OpenAPI_allocated_unit_free(multiple_unit_information->allocated_unit);
        multiple_unit_information->allocated_unit = NULL;
    }
    if (multiple_unit_information->triggers) {
        OpenAPI_list_for_each(multiple_unit_information->triggers, node) {
            OpenAPI_charging_trigger_free(node->data);
        }
        OpenAPI_list_free(multiple_unit_information->triggers);
        multiple_unit_information->triggers = NULL;
    }
    if (multiple_unit_information->final_unit_indication) {
        OpenAPI_final_unit_indication_free(multiple_unit_information->final_unit_indication);
        multiple_unit_information->final_unit_indication = NULL;
    }
    if (multiple_unit_information->u_pfid) {
        ogs_free(multiple_unit_information->u_pfid);
        multiple_unit_information->u_pfid = NULL;
    }
    if (multiple_unit_information->announcement_information) {
        OpenAPI_announcement_information_free(multiple_unit_information->announcement_information);
        multiple_unit_information->announcement_information = NULL;
    }
    if (multiple_unit_information->m_bupfid) {
        ogs_free(multiple_unit_information->m_bupfid);
        multiple_unit_information->m_bupfid = NULL;
    }
    ogs_free(multiple_unit_information);
}

cJSON *OpenAPI_multiple_unit_information_convertToJSON(OpenAPI_multiple_unit_information_t *multiple_unit_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (multiple_unit_information == NULL) {
        ogs_error("OpenAPI_multiple_unit_information_convertToJSON() failed [MultipleUnitInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (multiple_unit_information->result_code != OpenAPI_result_code_NULL) {
    if (cJSON_AddStringToObject(item, "resultCode", OpenAPI_result_code_ToString(multiple_unit_information->result_code)) == NULL) {
        ogs_error("OpenAPI_multiple_unit_information_convertToJSON() failed [result_code]");
        goto end;
    }
    }

    if (cJSON_AddNumberToObject(item, "ratingGroup", multiple_unit_information->rating_group) == NULL) {
        ogs_error("OpenAPI_multiple_unit_information_convertToJSON() failed [rating_group]");
        goto end;
    }

    if (multiple_unit_information->granted_unit) {
    cJSON *granted_unit_local_JSON = OpenAPI_granted_unit_convertToJSON(multiple_unit_information->granted_unit);
    if (granted_unit_local_JSON == NULL) {
        ogs_error("OpenAPI_multiple_unit_information_convertToJSON() failed [granted_unit]");
        goto end;
    }
    cJSON_AddItemToObject(item, "grantedUnit", granted_unit_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_multiple_unit_information_convertToJSON() failed [granted_unit]");
        goto end;
    }
    }

    if (multiple_unit_information->allocated_unit) {
    cJSON *allocated_unit_local_JSON = OpenAPI_allocated_unit_convertToJSON(multiple_unit_information->allocated_unit);
    if (allocated_unit_local_JSON == NULL) {
        ogs_error("OpenAPI_multiple_unit_information_convertToJSON() failed [allocated_unit]");
        goto end;
    }
    cJSON_AddItemToObject(item, "allocatedUnit", allocated_unit_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_multiple_unit_information_convertToJSON() failed [allocated_unit]");
        goto end;
    }
    }

    if (multiple_unit_information->triggers) {
    cJSON *triggersList = cJSON_AddArrayToObject(item, "triggers");
    if (triggersList == NULL) {
        ogs_error("OpenAPI_multiple_unit_information_convertToJSON() failed [triggers]");
        goto end;
    }
    OpenAPI_list_for_each(multiple_unit_information->triggers, node) {
        cJSON *itemLocal = OpenAPI_charging_trigger_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_multiple_unit_information_convertToJSON() failed [triggers]");
            goto end;
        }
        cJSON_AddItemToArray(triggersList, itemLocal);
    }
    }

    if (multiple_unit_information->is_validity_time) {
    if (cJSON_AddNumberToObject(item, "validityTime", multiple_unit_information->validity_time) == NULL) {
        ogs_error("OpenAPI_multiple_unit_information_convertToJSON() failed [validity_time]");
        goto end;
    }
    }

    if (multiple_unit_information->is_quota_holding_time) {
    if (cJSON_AddNumberToObject(item, "quotaHoldingTime", multiple_unit_information->quota_holding_time) == NULL) {
        ogs_error("OpenAPI_multiple_unit_information_convertToJSON() failed [quota_holding_time]");
        goto end;
    }
    }

    if (multiple_unit_information->final_unit_indication) {
    cJSON *final_unit_indication_local_JSON = OpenAPI_final_unit_indication_convertToJSON(multiple_unit_information->final_unit_indication);
    if (final_unit_indication_local_JSON == NULL) {
        ogs_error("OpenAPI_multiple_unit_information_convertToJSON() failed [final_unit_indication]");
        goto end;
    }
    cJSON_AddItemToObject(item, "finalUnitIndication", final_unit_indication_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_multiple_unit_information_convertToJSON() failed [final_unit_indication]");
        goto end;
    }
    }

    if (multiple_unit_information->is_time_quota_threshold) {
    if (cJSON_AddNumberToObject(item, "timeQuotaThreshold", multiple_unit_information->time_quota_threshold) == NULL) {
        ogs_error("OpenAPI_multiple_unit_information_convertToJSON() failed [time_quota_threshold]");
        goto end;
    }
    }

    if (multiple_unit_information->is_volume_quota_threshold) {
    if (cJSON_AddNumberToObject(item, "volumeQuotaThreshold", multiple_unit_information->volume_quota_threshold) == NULL) {
        ogs_error("OpenAPI_multiple_unit_information_convertToJSON() failed [volume_quota_threshold]");
        goto end;
    }
    }

    if (multiple_unit_information->is_unit_quota_threshold) {
    if (cJSON_AddNumberToObject(item, "unitQuotaThreshold", multiple_unit_information->unit_quota_threshold) == NULL) {
        ogs_error("OpenAPI_multiple_unit_information_convertToJSON() failed [unit_quota_threshold]");
        goto end;
    }
    }

    if (multiple_unit_information->u_pfid) {
    if (cJSON_AddStringToObject(item, "uPFID", multiple_unit_information->u_pfid) == NULL) {
        ogs_error("OpenAPI_multiple_unit_information_convertToJSON() failed [u_pfid]");
        goto end;
    }
    }

    if (multiple_unit_information->announcement_information) {
    cJSON *announcement_information_local_JSON = OpenAPI_announcement_information_convertToJSON(multiple_unit_information->announcement_information);
    if (announcement_information_local_JSON == NULL) {
        ogs_error("OpenAPI_multiple_unit_information_convertToJSON() failed [announcement_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "announcementInformation", announcement_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_multiple_unit_information_convertToJSON() failed [announcement_information]");
        goto end;
    }
    }

    if (multiple_unit_information->m_bupfid) {
    if (cJSON_AddStringToObject(item, "mBUPFID", multiple_unit_information->m_bupfid) == NULL) {
        ogs_error("OpenAPI_multiple_unit_information_convertToJSON() failed [m_bupfid]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_multiple_unit_information_t *OpenAPI_multiple_unit_information_parseFromJSON(cJSON *multiple_unit_informationJSON)
{
    OpenAPI_multiple_unit_information_t *multiple_unit_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *result_code = NULL;
    OpenAPI_result_code_e result_codeVariable = 0;
    cJSON *rating_group = NULL;
    cJSON *granted_unit = NULL;
    OpenAPI_granted_unit_t *granted_unit_local_nonprim = NULL;
    cJSON *allocated_unit = NULL;
    OpenAPI_allocated_unit_t *allocated_unit_local_nonprim = NULL;
    cJSON *triggers = NULL;
    OpenAPI_list_t *triggersList = NULL;
    cJSON *validity_time = NULL;
    cJSON *quota_holding_time = NULL;
    cJSON *final_unit_indication = NULL;
    OpenAPI_final_unit_indication_t *final_unit_indication_local_nonprim = NULL;
    cJSON *time_quota_threshold = NULL;
    cJSON *volume_quota_threshold = NULL;
    cJSON *unit_quota_threshold = NULL;
    cJSON *u_pfid = NULL;
    cJSON *announcement_information = NULL;
    OpenAPI_announcement_information_t *announcement_information_local_nonprim = NULL;
    cJSON *m_bupfid = NULL;
    result_code = cJSON_GetObjectItemCaseSensitive(multiple_unit_informationJSON, "resultCode");
    if (result_code) {
    if (!cJSON_IsString(result_code)) {
        ogs_error("OpenAPI_multiple_unit_information_parseFromJSON() failed [result_code]");
        goto end;
    }
    result_codeVariable = OpenAPI_result_code_FromString(result_code->valuestring);
    }

    rating_group = cJSON_GetObjectItemCaseSensitive(multiple_unit_informationJSON, "ratingGroup");
    if (!rating_group) {
        ogs_error("OpenAPI_multiple_unit_information_parseFromJSON() failed [rating_group]");
        goto end;
    }
    if (!cJSON_IsNumber(rating_group)) {
        ogs_error("OpenAPI_multiple_unit_information_parseFromJSON() failed [rating_group]");
        goto end;
    }

    granted_unit = cJSON_GetObjectItemCaseSensitive(multiple_unit_informationJSON, "grantedUnit");
    if (granted_unit) {
    granted_unit_local_nonprim = OpenAPI_granted_unit_parseFromJSON(granted_unit);
    if (!granted_unit_local_nonprim) {
        ogs_error("OpenAPI_granted_unit_parseFromJSON failed [granted_unit]");
        goto end;
    }
    }

    allocated_unit = cJSON_GetObjectItemCaseSensitive(multiple_unit_informationJSON, "allocatedUnit");
    if (allocated_unit) {
    allocated_unit_local_nonprim = OpenAPI_allocated_unit_parseFromJSON(allocated_unit);
    if (!allocated_unit_local_nonprim) {
        ogs_error("OpenAPI_allocated_unit_parseFromJSON failed [allocated_unit]");
        goto end;
    }
    }

    triggers = cJSON_GetObjectItemCaseSensitive(multiple_unit_informationJSON, "triggers");
    if (triggers) {
        cJSON *triggers_local = NULL;
        if (!cJSON_IsArray(triggers)) {
            ogs_error("OpenAPI_multiple_unit_information_parseFromJSON() failed [triggers]");
            goto end;
        }

        triggersList = OpenAPI_list_create();

        cJSON_ArrayForEach(triggers_local, triggers) {
            if (!cJSON_IsObject(triggers_local)) {
                ogs_error("OpenAPI_multiple_unit_information_parseFromJSON() failed [triggers]");
                goto end;
            }
            OpenAPI_charging_trigger_t *triggersItem = OpenAPI_charging_trigger_parseFromJSON(triggers_local);
            if (!triggersItem) {
                ogs_error("No triggersItem");
                goto end;
            }
            OpenAPI_list_add(triggersList, triggersItem);
        }
    }

    validity_time = cJSON_GetObjectItemCaseSensitive(multiple_unit_informationJSON, "validityTime");
    if (validity_time) {
    if (!cJSON_IsNumber(validity_time)) {
        ogs_error("OpenAPI_multiple_unit_information_parseFromJSON() failed [validity_time]");
        goto end;
    }
    }

    quota_holding_time = cJSON_GetObjectItemCaseSensitive(multiple_unit_informationJSON, "quotaHoldingTime");
    if (quota_holding_time) {
    if (!cJSON_IsNumber(quota_holding_time)) {
        ogs_error("OpenAPI_multiple_unit_information_parseFromJSON() failed [quota_holding_time]");
        goto end;
    }
    }

    final_unit_indication = cJSON_GetObjectItemCaseSensitive(multiple_unit_informationJSON, "finalUnitIndication");
    if (final_unit_indication) {
    final_unit_indication_local_nonprim = OpenAPI_final_unit_indication_parseFromJSON(final_unit_indication);
    if (!final_unit_indication_local_nonprim) {
        ogs_error("OpenAPI_final_unit_indication_parseFromJSON failed [final_unit_indication]");
        goto end;
    }
    }

    time_quota_threshold = cJSON_GetObjectItemCaseSensitive(multiple_unit_informationJSON, "timeQuotaThreshold");
    if (time_quota_threshold) {
    if (!cJSON_IsNumber(time_quota_threshold)) {
        ogs_error("OpenAPI_multiple_unit_information_parseFromJSON() failed [time_quota_threshold]");
        goto end;
    }
    }

    volume_quota_threshold = cJSON_GetObjectItemCaseSensitive(multiple_unit_informationJSON, "volumeQuotaThreshold");
    if (volume_quota_threshold) {
    if (!cJSON_IsNumber(volume_quota_threshold)) {
        ogs_error("OpenAPI_multiple_unit_information_parseFromJSON() failed [volume_quota_threshold]");
        goto end;
    }
    }

    unit_quota_threshold = cJSON_GetObjectItemCaseSensitive(multiple_unit_informationJSON, "unitQuotaThreshold");
    if (unit_quota_threshold) {
    if (!cJSON_IsNumber(unit_quota_threshold)) {
        ogs_error("OpenAPI_multiple_unit_information_parseFromJSON() failed [unit_quota_threshold]");
        goto end;
    }
    }

    u_pfid = cJSON_GetObjectItemCaseSensitive(multiple_unit_informationJSON, "uPFID");
    if (u_pfid) {
    if (!cJSON_IsString(u_pfid) && !cJSON_IsNull(u_pfid)) {
        ogs_error("OpenAPI_multiple_unit_information_parseFromJSON() failed [u_pfid]");
        goto end;
    }
    }

    announcement_information = cJSON_GetObjectItemCaseSensitive(multiple_unit_informationJSON, "announcementInformation");
    if (announcement_information) {
    announcement_information_local_nonprim = OpenAPI_announcement_information_parseFromJSON(announcement_information);
    if (!announcement_information_local_nonprim) {
        ogs_error("OpenAPI_announcement_information_parseFromJSON failed [announcement_information]");
        goto end;
    }
    }

    m_bupfid = cJSON_GetObjectItemCaseSensitive(multiple_unit_informationJSON, "mBUPFID");
    if (m_bupfid) {
    if (!cJSON_IsString(m_bupfid) && !cJSON_IsNull(m_bupfid)) {
        ogs_error("OpenAPI_multiple_unit_information_parseFromJSON() failed [m_bupfid]");
        goto end;
    }
    }

    multiple_unit_information_local_var = OpenAPI_multiple_unit_information_create (
        result_code ? result_codeVariable : 0,
        
        rating_group->valuedouble,
        granted_unit ? granted_unit_local_nonprim : NULL,
        allocated_unit ? allocated_unit_local_nonprim : NULL,
        triggers ? triggersList : NULL,
        validity_time ? true : false,
        validity_time ? validity_time->valuedouble : 0,
        quota_holding_time ? true : false,
        quota_holding_time ? quota_holding_time->valuedouble : 0,
        final_unit_indication ? final_unit_indication_local_nonprim : NULL,
        time_quota_threshold ? true : false,
        time_quota_threshold ? time_quota_threshold->valuedouble : 0,
        volume_quota_threshold ? true : false,
        volume_quota_threshold ? volume_quota_threshold->valuedouble : 0,
        unit_quota_threshold ? true : false,
        unit_quota_threshold ? unit_quota_threshold->valuedouble : 0,
        u_pfid && !cJSON_IsNull(u_pfid) ? ogs_strdup(u_pfid->valuestring) : NULL,
        announcement_information ? announcement_information_local_nonprim : NULL,
        m_bupfid && !cJSON_IsNull(m_bupfid) ? ogs_strdup(m_bupfid->valuestring) : NULL
    );

    return multiple_unit_information_local_var;
end:
    if (granted_unit_local_nonprim) {
        OpenAPI_granted_unit_free(granted_unit_local_nonprim);
        granted_unit_local_nonprim = NULL;
    }
    if (allocated_unit_local_nonprim) {
        OpenAPI_allocated_unit_free(allocated_unit_local_nonprim);
        allocated_unit_local_nonprim = NULL;
    }
    if (triggersList) {
        OpenAPI_list_for_each(triggersList, node) {
            OpenAPI_charging_trigger_free(node->data);
        }
        OpenAPI_list_free(triggersList);
        triggersList = NULL;
    }
    if (final_unit_indication_local_nonprim) {
        OpenAPI_final_unit_indication_free(final_unit_indication_local_nonprim);
        final_unit_indication_local_nonprim = NULL;
    }
    if (announcement_information_local_nonprim) {
        OpenAPI_announcement_information_free(announcement_information_local_nonprim);
        announcement_information_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_multiple_unit_information_t *OpenAPI_multiple_unit_information_copy(OpenAPI_multiple_unit_information_t *dst, OpenAPI_multiple_unit_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_multiple_unit_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_multiple_unit_information_convertToJSON() failed");
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

    OpenAPI_multiple_unit_information_free(dst);
    dst = OpenAPI_multiple_unit_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

