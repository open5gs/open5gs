
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "charging_data.h"

OpenAPI_charging_data_t *OpenAPI_charging_data_create(
    char *chg_id,
    OpenAPI_metering_method_e metering_method,
    bool is_offline,
    int offline,
    bool is_online,
    int online,
    bool is_sdf_handl,
    int sdf_handl,
    bool is_rating_group,
    int rating_group,
    OpenAPI_reporting_level_e reporting_level,
    bool is_service_id,
    int service_id,
    char *sponsor_id,
    char *app_svc_prov_id,
    bool is_af_charging_identifier,
    int af_charging_identifier,
    char *af_charg_id
)
{
    OpenAPI_charging_data_t *charging_data_local_var = ogs_malloc(sizeof(OpenAPI_charging_data_t));
    ogs_assert(charging_data_local_var);

    charging_data_local_var->chg_id = chg_id;
    charging_data_local_var->metering_method = metering_method;
    charging_data_local_var->is_offline = is_offline;
    charging_data_local_var->offline = offline;
    charging_data_local_var->is_online = is_online;
    charging_data_local_var->online = online;
    charging_data_local_var->is_sdf_handl = is_sdf_handl;
    charging_data_local_var->sdf_handl = sdf_handl;
    charging_data_local_var->is_rating_group = is_rating_group;
    charging_data_local_var->rating_group = rating_group;
    charging_data_local_var->reporting_level = reporting_level;
    charging_data_local_var->is_service_id = is_service_id;
    charging_data_local_var->service_id = service_id;
    charging_data_local_var->sponsor_id = sponsor_id;
    charging_data_local_var->app_svc_prov_id = app_svc_prov_id;
    charging_data_local_var->is_af_charging_identifier = is_af_charging_identifier;
    charging_data_local_var->af_charging_identifier = af_charging_identifier;
    charging_data_local_var->af_charg_id = af_charg_id;

    return charging_data_local_var;
}

void OpenAPI_charging_data_free(OpenAPI_charging_data_t *charging_data)
{
    if (NULL == charging_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(charging_data->chg_id);
    ogs_free(charging_data->sponsor_id);
    ogs_free(charging_data->app_svc_prov_id);
    ogs_free(charging_data->af_charg_id);
    ogs_free(charging_data);
}

cJSON *OpenAPI_charging_data_convertToJSON(OpenAPI_charging_data_t *charging_data)
{
    cJSON *item = NULL;

    if (charging_data == NULL) {
        ogs_error("OpenAPI_charging_data_convertToJSON() failed [ChargingData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddStringToObject(item, "chgId", charging_data->chg_id) == NULL) {
        ogs_error("OpenAPI_charging_data_convertToJSON() failed [chg_id]");
        goto end;
    }

    if (charging_data->metering_method) {
    if (cJSON_AddStringToObject(item, "meteringMethod", OpenAPI_metering_method_ToString(charging_data->metering_method)) == NULL) {
        ogs_error("OpenAPI_charging_data_convertToJSON() failed [metering_method]");
        goto end;
    }
    }

    if (charging_data->is_offline) {
    if (cJSON_AddBoolToObject(item, "offline", charging_data->offline) == NULL) {
        ogs_error("OpenAPI_charging_data_convertToJSON() failed [offline]");
        goto end;
    }
    }

    if (charging_data->is_online) {
    if (cJSON_AddBoolToObject(item, "online", charging_data->online) == NULL) {
        ogs_error("OpenAPI_charging_data_convertToJSON() failed [online]");
        goto end;
    }
    }

    if (charging_data->is_sdf_handl) {
    if (cJSON_AddBoolToObject(item, "sdfHandl", charging_data->sdf_handl) == NULL) {
        ogs_error("OpenAPI_charging_data_convertToJSON() failed [sdf_handl]");
        goto end;
    }
    }

    if (charging_data->is_rating_group) {
    if (cJSON_AddNumberToObject(item, "ratingGroup", charging_data->rating_group) == NULL) {
        ogs_error("OpenAPI_charging_data_convertToJSON() failed [rating_group]");
        goto end;
    }
    }

    if (charging_data->reporting_level) {
    if (cJSON_AddStringToObject(item, "reportingLevel", OpenAPI_reporting_level_ToString(charging_data->reporting_level)) == NULL) {
        ogs_error("OpenAPI_charging_data_convertToJSON() failed [reporting_level]");
        goto end;
    }
    }

    if (charging_data->is_service_id) {
    if (cJSON_AddNumberToObject(item, "serviceId", charging_data->service_id) == NULL) {
        ogs_error("OpenAPI_charging_data_convertToJSON() failed [service_id]");
        goto end;
    }
    }

    if (charging_data->sponsor_id) {
    if (cJSON_AddStringToObject(item, "sponsorId", charging_data->sponsor_id) == NULL) {
        ogs_error("OpenAPI_charging_data_convertToJSON() failed [sponsor_id]");
        goto end;
    }
    }

    if (charging_data->app_svc_prov_id) {
    if (cJSON_AddStringToObject(item, "appSvcProvId", charging_data->app_svc_prov_id) == NULL) {
        ogs_error("OpenAPI_charging_data_convertToJSON() failed [app_svc_prov_id]");
        goto end;
    }
    }

    if (charging_data->is_af_charging_identifier) {
    if (cJSON_AddNumberToObject(item, "afChargingIdentifier", charging_data->af_charging_identifier) == NULL) {
        ogs_error("OpenAPI_charging_data_convertToJSON() failed [af_charging_identifier]");
        goto end;
    }
    }

    if (charging_data->af_charg_id) {
    if (cJSON_AddStringToObject(item, "afChargId", charging_data->af_charg_id) == NULL) {
        ogs_error("OpenAPI_charging_data_convertToJSON() failed [af_charg_id]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_charging_data_t *OpenAPI_charging_data_parseFromJSON(cJSON *charging_dataJSON)
{
    OpenAPI_charging_data_t *charging_data_local_var = NULL;
    cJSON *chg_id = cJSON_GetObjectItemCaseSensitive(charging_dataJSON, "chgId");
    if (!chg_id) {
        ogs_error("OpenAPI_charging_data_parseFromJSON() failed [chg_id]");
        goto end;
    }

    if (!cJSON_IsString(chg_id)) {
        ogs_error("OpenAPI_charging_data_parseFromJSON() failed [chg_id]");
        goto end;
    }

    cJSON *metering_method = cJSON_GetObjectItemCaseSensitive(charging_dataJSON, "meteringMethod");

    OpenAPI_metering_method_e metering_methodVariable;
    if (metering_method) {
    if (!cJSON_IsString(metering_method)) {
        ogs_error("OpenAPI_charging_data_parseFromJSON() failed [metering_method]");
        goto end;
    }
    metering_methodVariable = OpenAPI_metering_method_FromString(metering_method->valuestring);
    }

    cJSON *offline = cJSON_GetObjectItemCaseSensitive(charging_dataJSON, "offline");

    if (offline) {
    if (!cJSON_IsBool(offline)) {
        ogs_error("OpenAPI_charging_data_parseFromJSON() failed [offline]");
        goto end;
    }
    }

    cJSON *online = cJSON_GetObjectItemCaseSensitive(charging_dataJSON, "online");

    if (online) {
    if (!cJSON_IsBool(online)) {
        ogs_error("OpenAPI_charging_data_parseFromJSON() failed [online]");
        goto end;
    }
    }

    cJSON *sdf_handl = cJSON_GetObjectItemCaseSensitive(charging_dataJSON, "sdfHandl");

    if (sdf_handl) {
    if (!cJSON_IsBool(sdf_handl)) {
        ogs_error("OpenAPI_charging_data_parseFromJSON() failed [sdf_handl]");
        goto end;
    }
    }

    cJSON *rating_group = cJSON_GetObjectItemCaseSensitive(charging_dataJSON, "ratingGroup");

    if (rating_group) {
    if (!cJSON_IsNumber(rating_group)) {
        ogs_error("OpenAPI_charging_data_parseFromJSON() failed [rating_group]");
        goto end;
    }
    }

    cJSON *reporting_level = cJSON_GetObjectItemCaseSensitive(charging_dataJSON, "reportingLevel");

    OpenAPI_reporting_level_e reporting_levelVariable;
    if (reporting_level) {
    if (!cJSON_IsString(reporting_level)) {
        ogs_error("OpenAPI_charging_data_parseFromJSON() failed [reporting_level]");
        goto end;
    }
    reporting_levelVariable = OpenAPI_reporting_level_FromString(reporting_level->valuestring);
    }

    cJSON *service_id = cJSON_GetObjectItemCaseSensitive(charging_dataJSON, "serviceId");

    if (service_id) {
    if (!cJSON_IsNumber(service_id)) {
        ogs_error("OpenAPI_charging_data_parseFromJSON() failed [service_id]");
        goto end;
    }
    }

    cJSON *sponsor_id = cJSON_GetObjectItemCaseSensitive(charging_dataJSON, "sponsorId");

    if (sponsor_id) {
    if (!cJSON_IsString(sponsor_id)) {
        ogs_error("OpenAPI_charging_data_parseFromJSON() failed [sponsor_id]");
        goto end;
    }
    }

    cJSON *app_svc_prov_id = cJSON_GetObjectItemCaseSensitive(charging_dataJSON, "appSvcProvId");

    if (app_svc_prov_id) {
    if (!cJSON_IsString(app_svc_prov_id)) {
        ogs_error("OpenAPI_charging_data_parseFromJSON() failed [app_svc_prov_id]");
        goto end;
    }
    }

    cJSON *af_charging_identifier = cJSON_GetObjectItemCaseSensitive(charging_dataJSON, "afChargingIdentifier");

    if (af_charging_identifier) {
    if (!cJSON_IsNumber(af_charging_identifier)) {
        ogs_error("OpenAPI_charging_data_parseFromJSON() failed [af_charging_identifier]");
        goto end;
    }
    }

    cJSON *af_charg_id = cJSON_GetObjectItemCaseSensitive(charging_dataJSON, "afChargId");

    if (af_charg_id) {
    if (!cJSON_IsString(af_charg_id)) {
        ogs_error("OpenAPI_charging_data_parseFromJSON() failed [af_charg_id]");
        goto end;
    }
    }

    charging_data_local_var = OpenAPI_charging_data_create (
        ogs_strdup(chg_id->valuestring),
        metering_method ? metering_methodVariable : 0,
        offline ? true : false,
        offline ? offline->valueint : 0,
        online ? true : false,
        online ? online->valueint : 0,
        sdf_handl ? true : false,
        sdf_handl ? sdf_handl->valueint : 0,
        rating_group ? true : false,
        rating_group ? rating_group->valuedouble : 0,
        reporting_level ? reporting_levelVariable : 0,
        service_id ? true : false,
        service_id ? service_id->valuedouble : 0,
        sponsor_id ? ogs_strdup(sponsor_id->valuestring) : NULL,
        app_svc_prov_id ? ogs_strdup(app_svc_prov_id->valuestring) : NULL,
        af_charging_identifier ? true : false,
        af_charging_identifier ? af_charging_identifier->valuedouble : 0,
        af_charg_id ? ogs_strdup(af_charg_id->valuestring) : NULL
    );

    return charging_data_local_var;
end:
    return NULL;
}

OpenAPI_charging_data_t *OpenAPI_charging_data_copy(OpenAPI_charging_data_t *dst, OpenAPI_charging_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_charging_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_charging_data_convertToJSON() failed");
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

    OpenAPI_charging_data_free(dst);
    dst = OpenAPI_charging_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

