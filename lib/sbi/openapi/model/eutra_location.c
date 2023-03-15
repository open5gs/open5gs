
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "eutra_location.h"

OpenAPI_eutra_location_t *OpenAPI_eutra_location_create(
    OpenAPI_tai_t *tai,
    bool is_ignore_tai,
    int ignore_tai,
    OpenAPI_ecgi_t *ecgi,
    bool is_ignore_ecgi,
    int ignore_ecgi,
    bool is_age_of_location_information,
    int age_of_location_information,
    char *ue_location_timestamp,
    char *geographical_information,
    char *geodetic_information,
    OpenAPI_global_ran_node_id_t *global_ngenb_id,
    OpenAPI_global_ran_node_id_t *global_enb_id
)
{
    OpenAPI_eutra_location_t *eutra_location_local_var = ogs_malloc(sizeof(OpenAPI_eutra_location_t));
    ogs_assert(eutra_location_local_var);

    eutra_location_local_var->tai = tai;
    eutra_location_local_var->is_ignore_tai = is_ignore_tai;
    eutra_location_local_var->ignore_tai = ignore_tai;
    eutra_location_local_var->ecgi = ecgi;
    eutra_location_local_var->is_ignore_ecgi = is_ignore_ecgi;
    eutra_location_local_var->ignore_ecgi = ignore_ecgi;
    eutra_location_local_var->is_age_of_location_information = is_age_of_location_information;
    eutra_location_local_var->age_of_location_information = age_of_location_information;
    eutra_location_local_var->ue_location_timestamp = ue_location_timestamp;
    eutra_location_local_var->geographical_information = geographical_information;
    eutra_location_local_var->geodetic_information = geodetic_information;
    eutra_location_local_var->global_ngenb_id = global_ngenb_id;
    eutra_location_local_var->global_enb_id = global_enb_id;

    return eutra_location_local_var;
}

void OpenAPI_eutra_location_free(OpenAPI_eutra_location_t *eutra_location)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == eutra_location) {
        return;
    }
    if (eutra_location->tai) {
        OpenAPI_tai_free(eutra_location->tai);
        eutra_location->tai = NULL;
    }
    if (eutra_location->ecgi) {
        OpenAPI_ecgi_free(eutra_location->ecgi);
        eutra_location->ecgi = NULL;
    }
    if (eutra_location->ue_location_timestamp) {
        ogs_free(eutra_location->ue_location_timestamp);
        eutra_location->ue_location_timestamp = NULL;
    }
    if (eutra_location->geographical_information) {
        ogs_free(eutra_location->geographical_information);
        eutra_location->geographical_information = NULL;
    }
    if (eutra_location->geodetic_information) {
        ogs_free(eutra_location->geodetic_information);
        eutra_location->geodetic_information = NULL;
    }
    if (eutra_location->global_ngenb_id) {
        OpenAPI_global_ran_node_id_free(eutra_location->global_ngenb_id);
        eutra_location->global_ngenb_id = NULL;
    }
    if (eutra_location->global_enb_id) {
        OpenAPI_global_ran_node_id_free(eutra_location->global_enb_id);
        eutra_location->global_enb_id = NULL;
    }
    ogs_free(eutra_location);
}

cJSON *OpenAPI_eutra_location_convertToJSON(OpenAPI_eutra_location_t *eutra_location)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (eutra_location == NULL) {
        ogs_error("OpenAPI_eutra_location_convertToJSON() failed [EutraLocation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!eutra_location->tai) {
        ogs_error("OpenAPI_eutra_location_convertToJSON() failed [tai]");
        return NULL;
    }
    cJSON *tai_local_JSON = OpenAPI_tai_convertToJSON(eutra_location->tai);
    if (tai_local_JSON == NULL) {
        ogs_error("OpenAPI_eutra_location_convertToJSON() failed [tai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "tai", tai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_eutra_location_convertToJSON() failed [tai]");
        goto end;
    }

    if (eutra_location->is_ignore_tai) {
    if (cJSON_AddBoolToObject(item, "ignoreTai", eutra_location->ignore_tai) == NULL) {
        ogs_error("OpenAPI_eutra_location_convertToJSON() failed [ignore_tai]");
        goto end;
    }
    }

    if (!eutra_location->ecgi) {
        ogs_error("OpenAPI_eutra_location_convertToJSON() failed [ecgi]");
        return NULL;
    }
    cJSON *ecgi_local_JSON = OpenAPI_ecgi_convertToJSON(eutra_location->ecgi);
    if (ecgi_local_JSON == NULL) {
        ogs_error("OpenAPI_eutra_location_convertToJSON() failed [ecgi]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ecgi", ecgi_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_eutra_location_convertToJSON() failed [ecgi]");
        goto end;
    }

    if (eutra_location->is_ignore_ecgi) {
    if (cJSON_AddBoolToObject(item, "ignoreEcgi", eutra_location->ignore_ecgi) == NULL) {
        ogs_error("OpenAPI_eutra_location_convertToJSON() failed [ignore_ecgi]");
        goto end;
    }
    }

    if (eutra_location->is_age_of_location_information) {
    if (cJSON_AddNumberToObject(item, "ageOfLocationInformation", eutra_location->age_of_location_information) == NULL) {
        ogs_error("OpenAPI_eutra_location_convertToJSON() failed [age_of_location_information]");
        goto end;
    }
    }

    if (eutra_location->ue_location_timestamp) {
    if (cJSON_AddStringToObject(item, "ueLocationTimestamp", eutra_location->ue_location_timestamp) == NULL) {
        ogs_error("OpenAPI_eutra_location_convertToJSON() failed [ue_location_timestamp]");
        goto end;
    }
    }

    if (eutra_location->geographical_information) {
    if (cJSON_AddStringToObject(item, "geographicalInformation", eutra_location->geographical_information) == NULL) {
        ogs_error("OpenAPI_eutra_location_convertToJSON() failed [geographical_information]");
        goto end;
    }
    }

    if (eutra_location->geodetic_information) {
    if (cJSON_AddStringToObject(item, "geodeticInformation", eutra_location->geodetic_information) == NULL) {
        ogs_error("OpenAPI_eutra_location_convertToJSON() failed [geodetic_information]");
        goto end;
    }
    }

    if (eutra_location->global_ngenb_id) {
    cJSON *global_ngenb_id_local_JSON = OpenAPI_global_ran_node_id_convertToJSON(eutra_location->global_ngenb_id);
    if (global_ngenb_id_local_JSON == NULL) {
        ogs_error("OpenAPI_eutra_location_convertToJSON() failed [global_ngenb_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "globalNgenbId", global_ngenb_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_eutra_location_convertToJSON() failed [global_ngenb_id]");
        goto end;
    }
    }

    if (eutra_location->global_enb_id) {
    cJSON *global_enb_id_local_JSON = OpenAPI_global_ran_node_id_convertToJSON(eutra_location->global_enb_id);
    if (global_enb_id_local_JSON == NULL) {
        ogs_error("OpenAPI_eutra_location_convertToJSON() failed [global_enb_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "globalENbId", global_enb_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_eutra_location_convertToJSON() failed [global_enb_id]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_eutra_location_t *OpenAPI_eutra_location_parseFromJSON(cJSON *eutra_locationJSON)
{
    OpenAPI_eutra_location_t *eutra_location_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *tai = NULL;
    OpenAPI_tai_t *tai_local_nonprim = NULL;
    cJSON *ignore_tai = NULL;
    cJSON *ecgi = NULL;
    OpenAPI_ecgi_t *ecgi_local_nonprim = NULL;
    cJSON *ignore_ecgi = NULL;
    cJSON *age_of_location_information = NULL;
    cJSON *ue_location_timestamp = NULL;
    cJSON *geographical_information = NULL;
    cJSON *geodetic_information = NULL;
    cJSON *global_ngenb_id = NULL;
    OpenAPI_global_ran_node_id_t *global_ngenb_id_local_nonprim = NULL;
    cJSON *global_enb_id = NULL;
    OpenAPI_global_ran_node_id_t *global_enb_id_local_nonprim = NULL;
    tai = cJSON_GetObjectItemCaseSensitive(eutra_locationJSON, "tai");
    if (!tai) {
        ogs_error("OpenAPI_eutra_location_parseFromJSON() failed [tai]");
        goto end;
    }
    tai_local_nonprim = OpenAPI_tai_parseFromJSON(tai);
    if (!tai_local_nonprim) {
        ogs_error("OpenAPI_tai_parseFromJSON failed [tai]");
        goto end;
    }

    ignore_tai = cJSON_GetObjectItemCaseSensitive(eutra_locationJSON, "ignoreTai");
    if (ignore_tai) {
    if (!cJSON_IsBool(ignore_tai)) {
        ogs_error("OpenAPI_eutra_location_parseFromJSON() failed [ignore_tai]");
        goto end;
    }
    }

    ecgi = cJSON_GetObjectItemCaseSensitive(eutra_locationJSON, "ecgi");
    if (!ecgi) {
        ogs_error("OpenAPI_eutra_location_parseFromJSON() failed [ecgi]");
        goto end;
    }
    ecgi_local_nonprim = OpenAPI_ecgi_parseFromJSON(ecgi);
    if (!ecgi_local_nonprim) {
        ogs_error("OpenAPI_ecgi_parseFromJSON failed [ecgi]");
        goto end;
    }

    ignore_ecgi = cJSON_GetObjectItemCaseSensitive(eutra_locationJSON, "ignoreEcgi");
    if (ignore_ecgi) {
    if (!cJSON_IsBool(ignore_ecgi)) {
        ogs_error("OpenAPI_eutra_location_parseFromJSON() failed [ignore_ecgi]");
        goto end;
    }
    }

    age_of_location_information = cJSON_GetObjectItemCaseSensitive(eutra_locationJSON, "ageOfLocationInformation");
    if (age_of_location_information) {
    if (!cJSON_IsNumber(age_of_location_information)) {
        ogs_error("OpenAPI_eutra_location_parseFromJSON() failed [age_of_location_information]");
        goto end;
    }
    }

    ue_location_timestamp = cJSON_GetObjectItemCaseSensitive(eutra_locationJSON, "ueLocationTimestamp");
    if (ue_location_timestamp) {
    if (!cJSON_IsString(ue_location_timestamp) && !cJSON_IsNull(ue_location_timestamp)) {
        ogs_error("OpenAPI_eutra_location_parseFromJSON() failed [ue_location_timestamp]");
        goto end;
    }
    }

    geographical_information = cJSON_GetObjectItemCaseSensitive(eutra_locationJSON, "geographicalInformation");
    if (geographical_information) {
    if (!cJSON_IsString(geographical_information) && !cJSON_IsNull(geographical_information)) {
        ogs_error("OpenAPI_eutra_location_parseFromJSON() failed [geographical_information]");
        goto end;
    }
    }

    geodetic_information = cJSON_GetObjectItemCaseSensitive(eutra_locationJSON, "geodeticInformation");
    if (geodetic_information) {
    if (!cJSON_IsString(geodetic_information) && !cJSON_IsNull(geodetic_information)) {
        ogs_error("OpenAPI_eutra_location_parseFromJSON() failed [geodetic_information]");
        goto end;
    }
    }

    global_ngenb_id = cJSON_GetObjectItemCaseSensitive(eutra_locationJSON, "globalNgenbId");
    if (global_ngenb_id) {
    global_ngenb_id_local_nonprim = OpenAPI_global_ran_node_id_parseFromJSON(global_ngenb_id);
    if (!global_ngenb_id_local_nonprim) {
        ogs_error("OpenAPI_global_ran_node_id_parseFromJSON failed [global_ngenb_id]");
        goto end;
    }
    }

    global_enb_id = cJSON_GetObjectItemCaseSensitive(eutra_locationJSON, "globalENbId");
    if (global_enb_id) {
    global_enb_id_local_nonprim = OpenAPI_global_ran_node_id_parseFromJSON(global_enb_id);
    if (!global_enb_id_local_nonprim) {
        ogs_error("OpenAPI_global_ran_node_id_parseFromJSON failed [global_enb_id]");
        goto end;
    }
    }

    eutra_location_local_var = OpenAPI_eutra_location_create (
        tai_local_nonprim,
        ignore_tai ? true : false,
        ignore_tai ? ignore_tai->valueint : 0,
        ecgi_local_nonprim,
        ignore_ecgi ? true : false,
        ignore_ecgi ? ignore_ecgi->valueint : 0,
        age_of_location_information ? true : false,
        age_of_location_information ? age_of_location_information->valuedouble : 0,
        ue_location_timestamp && !cJSON_IsNull(ue_location_timestamp) ? ogs_strdup(ue_location_timestamp->valuestring) : NULL,
        geographical_information && !cJSON_IsNull(geographical_information) ? ogs_strdup(geographical_information->valuestring) : NULL,
        geodetic_information && !cJSON_IsNull(geodetic_information) ? ogs_strdup(geodetic_information->valuestring) : NULL,
        global_ngenb_id ? global_ngenb_id_local_nonprim : NULL,
        global_enb_id ? global_enb_id_local_nonprim : NULL
    );

    return eutra_location_local_var;
end:
    if (tai_local_nonprim) {
        OpenAPI_tai_free(tai_local_nonprim);
        tai_local_nonprim = NULL;
    }
    if (ecgi_local_nonprim) {
        OpenAPI_ecgi_free(ecgi_local_nonprim);
        ecgi_local_nonprim = NULL;
    }
    if (global_ngenb_id_local_nonprim) {
        OpenAPI_global_ran_node_id_free(global_ngenb_id_local_nonprim);
        global_ngenb_id_local_nonprim = NULL;
    }
    if (global_enb_id_local_nonprim) {
        OpenAPI_global_ran_node_id_free(global_enb_id_local_nonprim);
        global_enb_id_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_eutra_location_t *OpenAPI_eutra_location_copy(OpenAPI_eutra_location_t *dst, OpenAPI_eutra_location_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_eutra_location_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_eutra_location_convertToJSON() failed");
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

    OpenAPI_eutra_location_free(dst);
    dst = OpenAPI_eutra_location_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

