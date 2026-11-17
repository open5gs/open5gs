
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "location_reporting_charging_information.h"

OpenAPI_location_reporting_charging_information_t *OpenAPI_location_reporting_charging_information_create(
    int location_reporting_message_type,
    OpenAPI_user_information_t *user_information,
    OpenAPI_charging_user_location_t *user_locationinfo,
    OpenAPI_ps_cell_information_t *p_s_cell_information,
    char *uetime_zone,
    OpenAPI_rat_type_e r_at_type,
    OpenAPI_list_t* presence_reporting_area_information,
    bool is_satellite_access_indicator,
    int satellite_access_indicator
)
{
    OpenAPI_location_reporting_charging_information_t *location_reporting_charging_information_local_var = ogs_malloc(sizeof(OpenAPI_location_reporting_charging_information_t));
    ogs_assert(location_reporting_charging_information_local_var);

    location_reporting_charging_information_local_var->location_reporting_message_type = location_reporting_message_type;
    location_reporting_charging_information_local_var->user_information = user_information;
    location_reporting_charging_information_local_var->user_locationinfo = user_locationinfo;
    location_reporting_charging_information_local_var->p_s_cell_information = p_s_cell_information;
    location_reporting_charging_information_local_var->uetime_zone = uetime_zone;
    location_reporting_charging_information_local_var->r_at_type = r_at_type;
    location_reporting_charging_information_local_var->presence_reporting_area_information = presence_reporting_area_information;
    location_reporting_charging_information_local_var->is_satellite_access_indicator = is_satellite_access_indicator;
    location_reporting_charging_information_local_var->satellite_access_indicator = satellite_access_indicator;

    return location_reporting_charging_information_local_var;
}

void OpenAPI_location_reporting_charging_information_free(OpenAPI_location_reporting_charging_information_t *location_reporting_charging_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == location_reporting_charging_information) {
        return;
    }
    if (location_reporting_charging_information->user_information) {
        OpenAPI_user_information_free(location_reporting_charging_information->user_information);
        location_reporting_charging_information->user_information = NULL;
    }
    if (location_reporting_charging_information->user_locationinfo) {
        OpenAPI_charging_user_location_free(location_reporting_charging_information->user_locationinfo);
        location_reporting_charging_information->user_locationinfo = NULL;
    }
    if (location_reporting_charging_information->p_s_cell_information) {
        OpenAPI_ps_cell_information_free(location_reporting_charging_information->p_s_cell_information);
        location_reporting_charging_information->p_s_cell_information = NULL;
    }
    if (location_reporting_charging_information->uetime_zone) {
        ogs_free(location_reporting_charging_information->uetime_zone);
        location_reporting_charging_information->uetime_zone = NULL;
    }
    if (location_reporting_charging_information->presence_reporting_area_information) {
        OpenAPI_list_for_each(location_reporting_charging_information->presence_reporting_area_information, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_presence_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(location_reporting_charging_information->presence_reporting_area_information);
        location_reporting_charging_information->presence_reporting_area_information = NULL;
    }
    ogs_free(location_reporting_charging_information);
}

cJSON *OpenAPI_location_reporting_charging_information_convertToJSON(OpenAPI_location_reporting_charging_information_t *location_reporting_charging_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (location_reporting_charging_information == NULL) {
        ogs_error("OpenAPI_location_reporting_charging_information_convertToJSON() failed [LocationReportingChargingInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "locationReportingMessageType", location_reporting_charging_information->location_reporting_message_type) == NULL) {
        ogs_error("OpenAPI_location_reporting_charging_information_convertToJSON() failed [location_reporting_message_type]");
        goto end;
    }

    if (location_reporting_charging_information->user_information) {
    cJSON *user_information_local_JSON = OpenAPI_user_information_convertToJSON(location_reporting_charging_information->user_information);
    if (user_information_local_JSON == NULL) {
        ogs_error("OpenAPI_location_reporting_charging_information_convertToJSON() failed [user_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "userInformation", user_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_location_reporting_charging_information_convertToJSON() failed [user_information]");
        goto end;
    }
    }

    if (location_reporting_charging_information->user_locationinfo) {
    cJSON *user_locationinfo_local_JSON = OpenAPI_charging_user_location_convertToJSON(location_reporting_charging_information->user_locationinfo);
    if (user_locationinfo_local_JSON == NULL) {
        ogs_error("OpenAPI_location_reporting_charging_information_convertToJSON() failed [user_locationinfo]");
        goto end;
    }
    cJSON_AddItemToObject(item, "userLocationinfo", user_locationinfo_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_location_reporting_charging_information_convertToJSON() failed [user_locationinfo]");
        goto end;
    }
    }

    if (location_reporting_charging_information->p_s_cell_information) {
    cJSON *p_s_cell_information_local_JSON = OpenAPI_ps_cell_information_convertToJSON(location_reporting_charging_information->p_s_cell_information);
    if (p_s_cell_information_local_JSON == NULL) {
        ogs_error("OpenAPI_location_reporting_charging_information_convertToJSON() failed [p_s_cell_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "pSCellInformation", p_s_cell_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_location_reporting_charging_information_convertToJSON() failed [p_s_cell_information]");
        goto end;
    }
    }

    if (location_reporting_charging_information->uetime_zone) {
    if (cJSON_AddStringToObject(item, "uetimeZone", location_reporting_charging_information->uetime_zone) == NULL) {
        ogs_error("OpenAPI_location_reporting_charging_information_convertToJSON() failed [uetime_zone]");
        goto end;
    }
    }

    if (location_reporting_charging_information->r_at_type != OpenAPI_rat_type_NULL) {
    if (cJSON_AddStringToObject(item, "rATType", OpenAPI_rat_type_ToString(location_reporting_charging_information->r_at_type)) == NULL) {
        ogs_error("OpenAPI_location_reporting_charging_information_convertToJSON() failed [r_at_type]");
        goto end;
    }
    }

    if (location_reporting_charging_information->presence_reporting_area_information) {
    cJSON *presence_reporting_area_information = cJSON_AddObjectToObject(item, "presenceReportingAreaInformation");
    if (presence_reporting_area_information == NULL) {
        ogs_error("OpenAPI_location_reporting_charging_information_convertToJSON() failed [presence_reporting_area_information]");
        goto end;
    }
    cJSON *localMapObject = presence_reporting_area_information;
    if (location_reporting_charging_information->presence_reporting_area_information) {
        OpenAPI_list_for_each(location_reporting_charging_information->presence_reporting_area_information, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_location_reporting_charging_information_convertToJSON() failed [presence_reporting_area_information]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_location_reporting_charging_information_convertToJSON() failed [presence_reporting_area_information]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_presence_info_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_location_reporting_charging_information_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (location_reporting_charging_information->is_satellite_access_indicator) {
    if (cJSON_AddBoolToObject(item, "satelliteAccessIndicator", location_reporting_charging_information->satellite_access_indicator) == NULL) {
        ogs_error("OpenAPI_location_reporting_charging_information_convertToJSON() failed [satellite_access_indicator]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_location_reporting_charging_information_t *OpenAPI_location_reporting_charging_information_parseFromJSON(cJSON *location_reporting_charging_informationJSON)
{
    OpenAPI_location_reporting_charging_information_t *location_reporting_charging_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *location_reporting_message_type = NULL;
    cJSON *user_information = NULL;
    OpenAPI_user_information_t *user_information_local_nonprim = NULL;
    cJSON *user_locationinfo = NULL;
    OpenAPI_charging_user_location_t *user_locationinfo_local_nonprim = NULL;
    cJSON *p_s_cell_information = NULL;
    OpenAPI_ps_cell_information_t *p_s_cell_information_local_nonprim = NULL;
    cJSON *uetime_zone = NULL;
    cJSON *r_at_type = NULL;
    OpenAPI_rat_type_e r_at_typeVariable = 0;
    cJSON *presence_reporting_area_information = NULL;
    OpenAPI_list_t *presence_reporting_area_informationList = NULL;
    cJSON *satellite_access_indicator = NULL;
    location_reporting_message_type = cJSON_GetObjectItemCaseSensitive(location_reporting_charging_informationJSON, "locationReportingMessageType");
    if (!location_reporting_message_type) {
        ogs_error("OpenAPI_location_reporting_charging_information_parseFromJSON() failed [location_reporting_message_type]");
        goto end;
    }
    if (!cJSON_IsNumber(location_reporting_message_type)) {
        ogs_error("OpenAPI_location_reporting_charging_information_parseFromJSON() failed [location_reporting_message_type]");
        goto end;
    }

    user_information = cJSON_GetObjectItemCaseSensitive(location_reporting_charging_informationJSON, "userInformation");
    if (user_information) {
    user_information_local_nonprim = OpenAPI_user_information_parseFromJSON(user_information);
    if (!user_information_local_nonprim) {
        ogs_error("OpenAPI_user_information_parseFromJSON failed [user_information]");
        goto end;
    }
    }

    user_locationinfo = cJSON_GetObjectItemCaseSensitive(location_reporting_charging_informationJSON, "userLocationinfo");
    if (user_locationinfo) {
    user_locationinfo_local_nonprim = OpenAPI_charging_user_location_parseFromJSON(user_locationinfo);
    if (!user_locationinfo_local_nonprim) {
        ogs_error("OpenAPI_charging_user_location_parseFromJSON failed [user_locationinfo]");
        goto end;
    }
    }

    p_s_cell_information = cJSON_GetObjectItemCaseSensitive(location_reporting_charging_informationJSON, "pSCellInformation");
    if (p_s_cell_information) {
    p_s_cell_information_local_nonprim = OpenAPI_ps_cell_information_parseFromJSON(p_s_cell_information);
    if (!p_s_cell_information_local_nonprim) {
        ogs_error("OpenAPI_ps_cell_information_parseFromJSON failed [p_s_cell_information]");
        goto end;
    }
    }

    uetime_zone = cJSON_GetObjectItemCaseSensitive(location_reporting_charging_informationJSON, "uetimeZone");
    if (uetime_zone) {
    if (!cJSON_IsString(uetime_zone) && !cJSON_IsNull(uetime_zone)) {
        ogs_error("OpenAPI_location_reporting_charging_information_parseFromJSON() failed [uetime_zone]");
        goto end;
    }
    }

    r_at_type = cJSON_GetObjectItemCaseSensitive(location_reporting_charging_informationJSON, "rATType");
    if (r_at_type) {
    if (!cJSON_IsString(r_at_type)) {
        ogs_error("OpenAPI_location_reporting_charging_information_parseFromJSON() failed [r_at_type]");
        goto end;
    }
    r_at_typeVariable = OpenAPI_rat_type_FromString(r_at_type->valuestring);
    }

    presence_reporting_area_information = cJSON_GetObjectItemCaseSensitive(location_reporting_charging_informationJSON, "presenceReportingAreaInformation");
    if (presence_reporting_area_information) {
        cJSON *presence_reporting_area_information_local_map = NULL;
        if (!cJSON_IsObject(presence_reporting_area_information) && !cJSON_IsNull(presence_reporting_area_information)) {
            ogs_error("OpenAPI_location_reporting_charging_information_parseFromJSON() failed [presence_reporting_area_information]");
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
                    ogs_error("OpenAPI_location_reporting_charging_information_parseFromJSON() failed [inner]");
                    goto end;
                }
                if (localMapKeyPair == NULL) {
                    ogs_error("OpenAPI_location_reporting_charging_information_parseFromJSON() failed [presence_reporting_area_information]");
                    goto end;
                }
                OpenAPI_list_add(presence_reporting_area_informationList, localMapKeyPair);
            }
        }
    }

    satellite_access_indicator = cJSON_GetObjectItemCaseSensitive(location_reporting_charging_informationJSON, "satelliteAccessIndicator");
    if (satellite_access_indicator) {
    if (!cJSON_IsBool(satellite_access_indicator)) {
        ogs_error("OpenAPI_location_reporting_charging_information_parseFromJSON() failed [satellite_access_indicator]");
        goto end;
    }
    }

    location_reporting_charging_information_local_var = OpenAPI_location_reporting_charging_information_create (
        
        location_reporting_message_type->valuedouble,
        user_information ? user_information_local_nonprim : NULL,
        user_locationinfo ? user_locationinfo_local_nonprim : NULL,
        p_s_cell_information ? p_s_cell_information_local_nonprim : NULL,
        uetime_zone && !cJSON_IsNull(uetime_zone) ? ogs_strdup(uetime_zone->valuestring) : NULL,
        r_at_type ? r_at_typeVariable : 0,
        presence_reporting_area_information ? presence_reporting_area_informationList : NULL,
        satellite_access_indicator ? true : false,
        satellite_access_indicator ? satellite_access_indicator->valueint : 0
    );

    return location_reporting_charging_information_local_var;
end:
    if (user_information_local_nonprim) {
        OpenAPI_user_information_free(user_information_local_nonprim);
        user_information_local_nonprim = NULL;
    }
    if (user_locationinfo_local_nonprim) {
        OpenAPI_charging_user_location_free(user_locationinfo_local_nonprim);
        user_locationinfo_local_nonprim = NULL;
    }
    if (p_s_cell_information_local_nonprim) {
        OpenAPI_ps_cell_information_free(p_s_cell_information_local_nonprim);
        p_s_cell_information_local_nonprim = NULL;
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

OpenAPI_location_reporting_charging_information_t *OpenAPI_location_reporting_charging_information_copy(OpenAPI_location_reporting_charging_information_t *dst, OpenAPI_location_reporting_charging_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_location_reporting_charging_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_location_reporting_charging_information_convertToJSON() failed");
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

    OpenAPI_location_reporting_charging_information_free(dst);
    dst = OpenAPI_location_reporting_charging_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

