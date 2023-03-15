
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nr_location.h"

OpenAPI_nr_location_t *OpenAPI_nr_location_create(
    OpenAPI_tai_t *tai,
    OpenAPI_ncgi_t *ncgi,
    bool is_ignore_ncgi,
    int ignore_ncgi,
    bool is_age_of_location_information,
    int age_of_location_information,
    char *ue_location_timestamp,
    char *geographical_information,
    char *geodetic_information,
    OpenAPI_global_ran_node_id_t *global_gnb_id
)
{
    OpenAPI_nr_location_t *nr_location_local_var = ogs_malloc(sizeof(OpenAPI_nr_location_t));
    ogs_assert(nr_location_local_var);

    nr_location_local_var->tai = tai;
    nr_location_local_var->ncgi = ncgi;
    nr_location_local_var->is_ignore_ncgi = is_ignore_ncgi;
    nr_location_local_var->ignore_ncgi = ignore_ncgi;
    nr_location_local_var->is_age_of_location_information = is_age_of_location_information;
    nr_location_local_var->age_of_location_information = age_of_location_information;
    nr_location_local_var->ue_location_timestamp = ue_location_timestamp;
    nr_location_local_var->geographical_information = geographical_information;
    nr_location_local_var->geodetic_information = geodetic_information;
    nr_location_local_var->global_gnb_id = global_gnb_id;

    return nr_location_local_var;
}

void OpenAPI_nr_location_free(OpenAPI_nr_location_t *nr_location)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == nr_location) {
        return;
    }
    if (nr_location->tai) {
        OpenAPI_tai_free(nr_location->tai);
        nr_location->tai = NULL;
    }
    if (nr_location->ncgi) {
        OpenAPI_ncgi_free(nr_location->ncgi);
        nr_location->ncgi = NULL;
    }
    if (nr_location->ue_location_timestamp) {
        ogs_free(nr_location->ue_location_timestamp);
        nr_location->ue_location_timestamp = NULL;
    }
    if (nr_location->geographical_information) {
        ogs_free(nr_location->geographical_information);
        nr_location->geographical_information = NULL;
    }
    if (nr_location->geodetic_information) {
        ogs_free(nr_location->geodetic_information);
        nr_location->geodetic_information = NULL;
    }
    if (nr_location->global_gnb_id) {
        OpenAPI_global_ran_node_id_free(nr_location->global_gnb_id);
        nr_location->global_gnb_id = NULL;
    }
    ogs_free(nr_location);
}

cJSON *OpenAPI_nr_location_convertToJSON(OpenAPI_nr_location_t *nr_location)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (nr_location == NULL) {
        ogs_error("OpenAPI_nr_location_convertToJSON() failed [NrLocation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!nr_location->tai) {
        ogs_error("OpenAPI_nr_location_convertToJSON() failed [tai]");
        return NULL;
    }
    cJSON *tai_local_JSON = OpenAPI_tai_convertToJSON(nr_location->tai);
    if (tai_local_JSON == NULL) {
        ogs_error("OpenAPI_nr_location_convertToJSON() failed [tai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "tai", tai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_nr_location_convertToJSON() failed [tai]");
        goto end;
    }

    if (!nr_location->ncgi) {
        ogs_error("OpenAPI_nr_location_convertToJSON() failed [ncgi]");
        return NULL;
    }
    cJSON *ncgi_local_JSON = OpenAPI_ncgi_convertToJSON(nr_location->ncgi);
    if (ncgi_local_JSON == NULL) {
        ogs_error("OpenAPI_nr_location_convertToJSON() failed [ncgi]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ncgi", ncgi_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_nr_location_convertToJSON() failed [ncgi]");
        goto end;
    }

    if (nr_location->is_ignore_ncgi) {
    if (cJSON_AddBoolToObject(item, "ignoreNcgi", nr_location->ignore_ncgi) == NULL) {
        ogs_error("OpenAPI_nr_location_convertToJSON() failed [ignore_ncgi]");
        goto end;
    }
    }

    if (nr_location->is_age_of_location_information) {
    if (cJSON_AddNumberToObject(item, "ageOfLocationInformation", nr_location->age_of_location_information) == NULL) {
        ogs_error("OpenAPI_nr_location_convertToJSON() failed [age_of_location_information]");
        goto end;
    }
    }

    if (nr_location->ue_location_timestamp) {
    if (cJSON_AddStringToObject(item, "ueLocationTimestamp", nr_location->ue_location_timestamp) == NULL) {
        ogs_error("OpenAPI_nr_location_convertToJSON() failed [ue_location_timestamp]");
        goto end;
    }
    }

    if (nr_location->geographical_information) {
    if (cJSON_AddStringToObject(item, "geographicalInformation", nr_location->geographical_information) == NULL) {
        ogs_error("OpenAPI_nr_location_convertToJSON() failed [geographical_information]");
        goto end;
    }
    }

    if (nr_location->geodetic_information) {
    if (cJSON_AddStringToObject(item, "geodeticInformation", nr_location->geodetic_information) == NULL) {
        ogs_error("OpenAPI_nr_location_convertToJSON() failed [geodetic_information]");
        goto end;
    }
    }

    if (nr_location->global_gnb_id) {
    cJSON *global_gnb_id_local_JSON = OpenAPI_global_ran_node_id_convertToJSON(nr_location->global_gnb_id);
    if (global_gnb_id_local_JSON == NULL) {
        ogs_error("OpenAPI_nr_location_convertToJSON() failed [global_gnb_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "globalGnbId", global_gnb_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_nr_location_convertToJSON() failed [global_gnb_id]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_nr_location_t *OpenAPI_nr_location_parseFromJSON(cJSON *nr_locationJSON)
{
    OpenAPI_nr_location_t *nr_location_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *tai = NULL;
    OpenAPI_tai_t *tai_local_nonprim = NULL;
    cJSON *ncgi = NULL;
    OpenAPI_ncgi_t *ncgi_local_nonprim = NULL;
    cJSON *ignore_ncgi = NULL;
    cJSON *age_of_location_information = NULL;
    cJSON *ue_location_timestamp = NULL;
    cJSON *geographical_information = NULL;
    cJSON *geodetic_information = NULL;
    cJSON *global_gnb_id = NULL;
    OpenAPI_global_ran_node_id_t *global_gnb_id_local_nonprim = NULL;
    tai = cJSON_GetObjectItemCaseSensitive(nr_locationJSON, "tai");
    if (!tai) {
        ogs_error("OpenAPI_nr_location_parseFromJSON() failed [tai]");
        goto end;
    }
    tai_local_nonprim = OpenAPI_tai_parseFromJSON(tai);
    if (!tai_local_nonprim) {
        ogs_error("OpenAPI_tai_parseFromJSON failed [tai]");
        goto end;
    }

    ncgi = cJSON_GetObjectItemCaseSensitive(nr_locationJSON, "ncgi");
    if (!ncgi) {
        ogs_error("OpenAPI_nr_location_parseFromJSON() failed [ncgi]");
        goto end;
    }
    ncgi_local_nonprim = OpenAPI_ncgi_parseFromJSON(ncgi);
    if (!ncgi_local_nonprim) {
        ogs_error("OpenAPI_ncgi_parseFromJSON failed [ncgi]");
        goto end;
    }

    ignore_ncgi = cJSON_GetObjectItemCaseSensitive(nr_locationJSON, "ignoreNcgi");
    if (ignore_ncgi) {
    if (!cJSON_IsBool(ignore_ncgi)) {
        ogs_error("OpenAPI_nr_location_parseFromJSON() failed [ignore_ncgi]");
        goto end;
    }
    }

    age_of_location_information = cJSON_GetObjectItemCaseSensitive(nr_locationJSON, "ageOfLocationInformation");
    if (age_of_location_information) {
    if (!cJSON_IsNumber(age_of_location_information)) {
        ogs_error("OpenAPI_nr_location_parseFromJSON() failed [age_of_location_information]");
        goto end;
    }
    }

    ue_location_timestamp = cJSON_GetObjectItemCaseSensitive(nr_locationJSON, "ueLocationTimestamp");
    if (ue_location_timestamp) {
    if (!cJSON_IsString(ue_location_timestamp) && !cJSON_IsNull(ue_location_timestamp)) {
        ogs_error("OpenAPI_nr_location_parseFromJSON() failed [ue_location_timestamp]");
        goto end;
    }
    }

    geographical_information = cJSON_GetObjectItemCaseSensitive(nr_locationJSON, "geographicalInformation");
    if (geographical_information) {
    if (!cJSON_IsString(geographical_information) && !cJSON_IsNull(geographical_information)) {
        ogs_error("OpenAPI_nr_location_parseFromJSON() failed [geographical_information]");
        goto end;
    }
    }

    geodetic_information = cJSON_GetObjectItemCaseSensitive(nr_locationJSON, "geodeticInformation");
    if (geodetic_information) {
    if (!cJSON_IsString(geodetic_information) && !cJSON_IsNull(geodetic_information)) {
        ogs_error("OpenAPI_nr_location_parseFromJSON() failed [geodetic_information]");
        goto end;
    }
    }

    global_gnb_id = cJSON_GetObjectItemCaseSensitive(nr_locationJSON, "globalGnbId");
    if (global_gnb_id) {
    global_gnb_id_local_nonprim = OpenAPI_global_ran_node_id_parseFromJSON(global_gnb_id);
    if (!global_gnb_id_local_nonprim) {
        ogs_error("OpenAPI_global_ran_node_id_parseFromJSON failed [global_gnb_id]");
        goto end;
    }
    }

    nr_location_local_var = OpenAPI_nr_location_create (
        tai_local_nonprim,
        ncgi_local_nonprim,
        ignore_ncgi ? true : false,
        ignore_ncgi ? ignore_ncgi->valueint : 0,
        age_of_location_information ? true : false,
        age_of_location_information ? age_of_location_information->valuedouble : 0,
        ue_location_timestamp && !cJSON_IsNull(ue_location_timestamp) ? ogs_strdup(ue_location_timestamp->valuestring) : NULL,
        geographical_information && !cJSON_IsNull(geographical_information) ? ogs_strdup(geographical_information->valuestring) : NULL,
        geodetic_information && !cJSON_IsNull(geodetic_information) ? ogs_strdup(geodetic_information->valuestring) : NULL,
        global_gnb_id ? global_gnb_id_local_nonprim : NULL
    );

    return nr_location_local_var;
end:
    if (tai_local_nonprim) {
        OpenAPI_tai_free(tai_local_nonprim);
        tai_local_nonprim = NULL;
    }
    if (ncgi_local_nonprim) {
        OpenAPI_ncgi_free(ncgi_local_nonprim);
        ncgi_local_nonprim = NULL;
    }
    if (global_gnb_id_local_nonprim) {
        OpenAPI_global_ran_node_id_free(global_gnb_id_local_nonprim);
        global_gnb_id_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_nr_location_t *OpenAPI_nr_location_copy(OpenAPI_nr_location_t *dst, OpenAPI_nr_location_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nr_location_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nr_location_convertToJSON() failed");
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

    OpenAPI_nr_location_free(dst);
    dst = OpenAPI_nr_location_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

