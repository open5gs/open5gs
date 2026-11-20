
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pfi_container_information.h"

OpenAPI_pfi_container_information_t *OpenAPI_pfi_container_information_create(
    char *p_fi,
    char *report_time,
    char *timeof_first_usage,
    char *timeof_last_usage,
    bool is_qo_s_information_null,
    OpenAPI_qos_data_t *qo_s_information,
    OpenAPI_qos_characteristics_t *qo_s_characteristics,
    OpenAPI_charging_user_location_t *user_location_information,
    char *uetime_zone,
    OpenAPI_list_t* presence_reporting_area_information
)
{
    OpenAPI_pfi_container_information_t *pfi_container_information_local_var = ogs_malloc(sizeof(OpenAPI_pfi_container_information_t));
    ogs_assert(pfi_container_information_local_var);

    pfi_container_information_local_var->p_fi = p_fi;
    pfi_container_information_local_var->report_time = report_time;
    pfi_container_information_local_var->timeof_first_usage = timeof_first_usage;
    pfi_container_information_local_var->timeof_last_usage = timeof_last_usage;
    pfi_container_information_local_var->is_qo_s_information_null = is_qo_s_information_null;
    pfi_container_information_local_var->qo_s_information = qo_s_information;
    pfi_container_information_local_var->qo_s_characteristics = qo_s_characteristics;
    pfi_container_information_local_var->user_location_information = user_location_information;
    pfi_container_information_local_var->uetime_zone = uetime_zone;
    pfi_container_information_local_var->presence_reporting_area_information = presence_reporting_area_information;

    return pfi_container_information_local_var;
}

void OpenAPI_pfi_container_information_free(OpenAPI_pfi_container_information_t *pfi_container_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pfi_container_information) {
        return;
    }
    if (pfi_container_information->p_fi) {
        ogs_free(pfi_container_information->p_fi);
        pfi_container_information->p_fi = NULL;
    }
    if (pfi_container_information->report_time) {
        ogs_free(pfi_container_information->report_time);
        pfi_container_information->report_time = NULL;
    }
    if (pfi_container_information->timeof_first_usage) {
        ogs_free(pfi_container_information->timeof_first_usage);
        pfi_container_information->timeof_first_usage = NULL;
    }
    if (pfi_container_information->timeof_last_usage) {
        ogs_free(pfi_container_information->timeof_last_usage);
        pfi_container_information->timeof_last_usage = NULL;
    }
    if (pfi_container_information->qo_s_information) {
        OpenAPI_qos_data_free(pfi_container_information->qo_s_information);
        pfi_container_information->qo_s_information = NULL;
    }
    if (pfi_container_information->qo_s_characteristics) {
        OpenAPI_qos_characteristics_free(pfi_container_information->qo_s_characteristics);
        pfi_container_information->qo_s_characteristics = NULL;
    }
    if (pfi_container_information->user_location_information) {
        OpenAPI_charging_user_location_free(pfi_container_information->user_location_information);
        pfi_container_information->user_location_information = NULL;
    }
    if (pfi_container_information->uetime_zone) {
        ogs_free(pfi_container_information->uetime_zone);
        pfi_container_information->uetime_zone = NULL;
    }
    if (pfi_container_information->presence_reporting_area_information) {
        OpenAPI_list_for_each(pfi_container_information->presence_reporting_area_information, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_presence_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(pfi_container_information->presence_reporting_area_information);
        pfi_container_information->presence_reporting_area_information = NULL;
    }
    ogs_free(pfi_container_information);
}

cJSON *OpenAPI_pfi_container_information_convertToJSON(OpenAPI_pfi_container_information_t *pfi_container_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pfi_container_information == NULL) {
        ogs_error("OpenAPI_pfi_container_information_convertToJSON() failed [PFIContainerInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (pfi_container_information->p_fi) {
    if (cJSON_AddStringToObject(item, "pFI", pfi_container_information->p_fi) == NULL) {
        ogs_error("OpenAPI_pfi_container_information_convertToJSON() failed [p_fi]");
        goto end;
    }
    }

    if (pfi_container_information->report_time) {
    if (cJSON_AddStringToObject(item, "reportTime", pfi_container_information->report_time) == NULL) {
        ogs_error("OpenAPI_pfi_container_information_convertToJSON() failed [report_time]");
        goto end;
    }
    }

    if (pfi_container_information->timeof_first_usage) {
    if (cJSON_AddStringToObject(item, "timeofFirstUsage", pfi_container_information->timeof_first_usage) == NULL) {
        ogs_error("OpenAPI_pfi_container_information_convertToJSON() failed [timeof_first_usage]");
        goto end;
    }
    }

    if (pfi_container_information->timeof_last_usage) {
    if (cJSON_AddStringToObject(item, "timeofLastUsage", pfi_container_information->timeof_last_usage) == NULL) {
        ogs_error("OpenAPI_pfi_container_information_convertToJSON() failed [timeof_last_usage]");
        goto end;
    }
    }

    if (pfi_container_information->qo_s_information) {
    cJSON *qo_s_information_local_JSON = OpenAPI_qos_data_convertToJSON(pfi_container_information->qo_s_information);
    if (qo_s_information_local_JSON == NULL) {
        ogs_error("OpenAPI_pfi_container_information_convertToJSON() failed [qo_s_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "qoSInformation", qo_s_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pfi_container_information_convertToJSON() failed [qo_s_information]");
        goto end;
    }
    } else if (pfi_container_information->is_qo_s_information_null) {
        if (cJSON_AddNullToObject(item, "qoSInformation") == NULL) {
            ogs_error("OpenAPI_pfi_container_information_convertToJSON() failed [qo_s_information]");
            goto end;
        }
    }

    if (pfi_container_information->qo_s_characteristics) {
    cJSON *qo_s_characteristics_local_JSON = OpenAPI_qos_characteristics_convertToJSON(pfi_container_information->qo_s_characteristics);
    if (qo_s_characteristics_local_JSON == NULL) {
        ogs_error("OpenAPI_pfi_container_information_convertToJSON() failed [qo_s_characteristics]");
        goto end;
    }
    cJSON_AddItemToObject(item, "qoSCharacteristics", qo_s_characteristics_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pfi_container_information_convertToJSON() failed [qo_s_characteristics]");
        goto end;
    }
    }

    if (pfi_container_information->user_location_information) {
    cJSON *user_location_information_local_JSON = OpenAPI_charging_user_location_convertToJSON(pfi_container_information->user_location_information);
    if (user_location_information_local_JSON == NULL) {
        ogs_error("OpenAPI_pfi_container_information_convertToJSON() failed [user_location_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "userLocationInformation", user_location_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pfi_container_information_convertToJSON() failed [user_location_information]");
        goto end;
    }
    }

    if (pfi_container_information->uetime_zone) {
    if (cJSON_AddStringToObject(item, "uetimeZone", pfi_container_information->uetime_zone) == NULL) {
        ogs_error("OpenAPI_pfi_container_information_convertToJSON() failed [uetime_zone]");
        goto end;
    }
    }

    if (pfi_container_information->presence_reporting_area_information) {
    cJSON *presence_reporting_area_information = cJSON_AddObjectToObject(item, "presenceReportingAreaInformation");
    if (presence_reporting_area_information == NULL) {
        ogs_error("OpenAPI_pfi_container_information_convertToJSON() failed [presence_reporting_area_information]");
        goto end;
    }
    cJSON *localMapObject = presence_reporting_area_information;
    if (pfi_container_information->presence_reporting_area_information) {
        OpenAPI_list_for_each(pfi_container_information->presence_reporting_area_information, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_pfi_container_information_convertToJSON() failed [presence_reporting_area_information]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_pfi_container_information_convertToJSON() failed [presence_reporting_area_information]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_presence_info_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_pfi_container_information_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

end:
    return item;
}

OpenAPI_pfi_container_information_t *OpenAPI_pfi_container_information_parseFromJSON(cJSON *pfi_container_informationJSON)
{
    OpenAPI_pfi_container_information_t *pfi_container_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *p_fi = NULL;
    cJSON *report_time = NULL;
    cJSON *timeof_first_usage = NULL;
    cJSON *timeof_last_usage = NULL;
    cJSON *qo_s_information = NULL;
    OpenAPI_qos_data_t *qo_s_information_local_nonprim = NULL;
    cJSON *qo_s_characteristics = NULL;
    OpenAPI_qos_characteristics_t *qo_s_characteristics_local_nonprim = NULL;
    cJSON *user_location_information = NULL;
    OpenAPI_charging_user_location_t *user_location_information_local_nonprim = NULL;
    cJSON *uetime_zone = NULL;
    cJSON *presence_reporting_area_information = NULL;
    OpenAPI_list_t *presence_reporting_area_informationList = NULL;
    p_fi = cJSON_GetObjectItemCaseSensitive(pfi_container_informationJSON, "pFI");
    if (p_fi) {
    if (!cJSON_IsString(p_fi) && !cJSON_IsNull(p_fi)) {
        ogs_error("OpenAPI_pfi_container_information_parseFromJSON() failed [p_fi]");
        goto end;
    }
    }

    report_time = cJSON_GetObjectItemCaseSensitive(pfi_container_informationJSON, "reportTime");
    if (report_time) {
    if (!cJSON_IsString(report_time) && !cJSON_IsNull(report_time)) {
        ogs_error("OpenAPI_pfi_container_information_parseFromJSON() failed [report_time]");
        goto end;
    }
    }

    timeof_first_usage = cJSON_GetObjectItemCaseSensitive(pfi_container_informationJSON, "timeofFirstUsage");
    if (timeof_first_usage) {
    if (!cJSON_IsString(timeof_first_usage) && !cJSON_IsNull(timeof_first_usage)) {
        ogs_error("OpenAPI_pfi_container_information_parseFromJSON() failed [timeof_first_usage]");
        goto end;
    }
    }

    timeof_last_usage = cJSON_GetObjectItemCaseSensitive(pfi_container_informationJSON, "timeofLastUsage");
    if (timeof_last_usage) {
    if (!cJSON_IsString(timeof_last_usage) && !cJSON_IsNull(timeof_last_usage)) {
        ogs_error("OpenAPI_pfi_container_information_parseFromJSON() failed [timeof_last_usage]");
        goto end;
    }
    }

    qo_s_information = cJSON_GetObjectItemCaseSensitive(pfi_container_informationJSON, "qoSInformation");
    if (qo_s_information) {
    if (!cJSON_IsNull(qo_s_information)) {
    qo_s_information_local_nonprim = OpenAPI_qos_data_parseFromJSON(qo_s_information);
    if (!qo_s_information_local_nonprim) {
        ogs_error("OpenAPI_qos_data_parseFromJSON failed [qo_s_information]");
        goto end;
    }
    }
    }

    qo_s_characteristics = cJSON_GetObjectItemCaseSensitive(pfi_container_informationJSON, "qoSCharacteristics");
    if (qo_s_characteristics) {
    qo_s_characteristics_local_nonprim = OpenAPI_qos_characteristics_parseFromJSON(qo_s_characteristics);
    if (!qo_s_characteristics_local_nonprim) {
        ogs_error("OpenAPI_qos_characteristics_parseFromJSON failed [qo_s_characteristics]");
        goto end;
    }
    }

    user_location_information = cJSON_GetObjectItemCaseSensitive(pfi_container_informationJSON, "userLocationInformation");
    if (user_location_information) {
    user_location_information_local_nonprim = OpenAPI_charging_user_location_parseFromJSON(user_location_information);
    if (!user_location_information_local_nonprim) {
        ogs_error("OpenAPI_charging_user_location_parseFromJSON failed [user_location_information]");
        goto end;
    }
    }

    uetime_zone = cJSON_GetObjectItemCaseSensitive(pfi_container_informationJSON, "uetimeZone");
    if (uetime_zone) {
    if (!cJSON_IsString(uetime_zone) && !cJSON_IsNull(uetime_zone)) {
        ogs_error("OpenAPI_pfi_container_information_parseFromJSON() failed [uetime_zone]");
        goto end;
    }
    }

    presence_reporting_area_information = cJSON_GetObjectItemCaseSensitive(pfi_container_informationJSON, "presenceReportingAreaInformation");
    if (presence_reporting_area_information) {
        cJSON *presence_reporting_area_information_local_map = NULL;
        if (!cJSON_IsObject(presence_reporting_area_information) && !cJSON_IsNull(presence_reporting_area_information)) {
            ogs_error("OpenAPI_pfi_container_information_parseFromJSON() failed [presence_reporting_area_information]");
            goto end;
        }
        if (cJSON_IsObject(presence_reporting_area_information)) {
            presence_reporting_area_informationList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(presence_reporting_area_information_local_map, presence_reporting_area_information) {
                cJSON *localMapObject = presence_reporting_area_information_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_presence_info_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_pfi_container_information_parseFromJSON() failed [inner]");
                    goto end;
                }
                if (localMapKeyPair == NULL) {
                    ogs_error("OpenAPI_pfi_container_information_parseFromJSON() failed [presence_reporting_area_information]");
                    goto end;
                }
                OpenAPI_list_add(presence_reporting_area_informationList, localMapKeyPair);
            }
        }
    }

    pfi_container_information_local_var = OpenAPI_pfi_container_information_create (
        p_fi && !cJSON_IsNull(p_fi) ? ogs_strdup(p_fi->valuestring) : NULL,
        report_time && !cJSON_IsNull(report_time) ? ogs_strdup(report_time->valuestring) : NULL,
        timeof_first_usage && !cJSON_IsNull(timeof_first_usage) ? ogs_strdup(timeof_first_usage->valuestring) : NULL,
        timeof_last_usage && !cJSON_IsNull(timeof_last_usage) ? ogs_strdup(timeof_last_usage->valuestring) : NULL,
        qo_s_information && cJSON_IsNull(qo_s_information) ? true : false,
        qo_s_information ? qo_s_information_local_nonprim : NULL,
        qo_s_characteristics ? qo_s_characteristics_local_nonprim : NULL,
        user_location_information ? user_location_information_local_nonprim : NULL,
        uetime_zone && !cJSON_IsNull(uetime_zone) ? ogs_strdup(uetime_zone->valuestring) : NULL,
        presence_reporting_area_information ? presence_reporting_area_informationList : NULL
    );

    return pfi_container_information_local_var;
end:
    if (qo_s_information_local_nonprim) {
        OpenAPI_qos_data_free(qo_s_information_local_nonprim);
        qo_s_information_local_nonprim = NULL;
    }
    if (qo_s_characteristics_local_nonprim) {
        OpenAPI_qos_characteristics_free(qo_s_characteristics_local_nonprim);
        qo_s_characteristics_local_nonprim = NULL;
    }
    if (user_location_information_local_nonprim) {
        OpenAPI_charging_user_location_free(user_location_information_local_nonprim);
        user_location_information_local_nonprim = NULL;
    }
    if (presence_reporting_area_informationList) {
        OpenAPI_list_for_each(presence_reporting_area_informationList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_presence_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(presence_reporting_area_informationList);
        presence_reporting_area_informationList = NULL;
    }
    return NULL;
}

OpenAPI_pfi_container_information_t *OpenAPI_pfi_container_information_copy(OpenAPI_pfi_container_information_t *dst, OpenAPI_pfi_container_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pfi_container_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pfi_container_information_convertToJSON() failed");
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

    OpenAPI_pfi_container_information_free(dst);
    dst = OpenAPI_pfi_container_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

