
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "utra_location.h"

OpenAPI_utra_location_t *OpenAPI_utra_location_create(
    OpenAPI_cell_global_id_t *cgi,
    OpenAPI_service_area_id_t *sai,
    OpenAPI_location_area_id_t *lai,
    OpenAPI_routing_area_id_t *rai,
    bool is_age_of_location_information,
    int age_of_location_information,
    char *ue_location_timestamp,
    char *geographical_information,
    char *geodetic_information
)
{
    OpenAPI_utra_location_t *utra_location_local_var = ogs_malloc(sizeof(OpenAPI_utra_location_t));
    ogs_assert(utra_location_local_var);

    utra_location_local_var->cgi = cgi;
    utra_location_local_var->sai = sai;
    utra_location_local_var->lai = lai;
    utra_location_local_var->rai = rai;
    utra_location_local_var->is_age_of_location_information = is_age_of_location_information;
    utra_location_local_var->age_of_location_information = age_of_location_information;
    utra_location_local_var->ue_location_timestamp = ue_location_timestamp;
    utra_location_local_var->geographical_information = geographical_information;
    utra_location_local_var->geodetic_information = geodetic_information;

    return utra_location_local_var;
}

void OpenAPI_utra_location_free(OpenAPI_utra_location_t *utra_location)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == utra_location) {
        return;
    }
    if (utra_location->cgi) {
        OpenAPI_cell_global_id_free(utra_location->cgi);
        utra_location->cgi = NULL;
    }
    if (utra_location->sai) {
        OpenAPI_service_area_id_free(utra_location->sai);
        utra_location->sai = NULL;
    }
    if (utra_location->lai) {
        OpenAPI_location_area_id_free(utra_location->lai);
        utra_location->lai = NULL;
    }
    if (utra_location->rai) {
        OpenAPI_routing_area_id_free(utra_location->rai);
        utra_location->rai = NULL;
    }
    if (utra_location->ue_location_timestamp) {
        ogs_free(utra_location->ue_location_timestamp);
        utra_location->ue_location_timestamp = NULL;
    }
    if (utra_location->geographical_information) {
        ogs_free(utra_location->geographical_information);
        utra_location->geographical_information = NULL;
    }
    if (utra_location->geodetic_information) {
        ogs_free(utra_location->geodetic_information);
        utra_location->geodetic_information = NULL;
    }
    ogs_free(utra_location);
}

cJSON *OpenAPI_utra_location_convertToJSON(OpenAPI_utra_location_t *utra_location)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (utra_location == NULL) {
        ogs_error("OpenAPI_utra_location_convertToJSON() failed [UtraLocation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (utra_location->cgi) {
    cJSON *cgi_local_JSON = OpenAPI_cell_global_id_convertToJSON(utra_location->cgi);
    if (cgi_local_JSON == NULL) {
        ogs_error("OpenAPI_utra_location_convertToJSON() failed [cgi]");
        goto end;
    }
    cJSON_AddItemToObject(item, "cgi", cgi_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_utra_location_convertToJSON() failed [cgi]");
        goto end;
    }
    }

    if (utra_location->sai) {
    cJSON *sai_local_JSON = OpenAPI_service_area_id_convertToJSON(utra_location->sai);
    if (sai_local_JSON == NULL) {
        ogs_error("OpenAPI_utra_location_convertToJSON() failed [sai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sai", sai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_utra_location_convertToJSON() failed [sai]");
        goto end;
    }
    }

    if (utra_location->lai) {
    cJSON *lai_local_JSON = OpenAPI_location_area_id_convertToJSON(utra_location->lai);
    if (lai_local_JSON == NULL) {
        ogs_error("OpenAPI_utra_location_convertToJSON() failed [lai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "lai", lai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_utra_location_convertToJSON() failed [lai]");
        goto end;
    }
    }

    if (utra_location->rai) {
    cJSON *rai_local_JSON = OpenAPI_routing_area_id_convertToJSON(utra_location->rai);
    if (rai_local_JSON == NULL) {
        ogs_error("OpenAPI_utra_location_convertToJSON() failed [rai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "rai", rai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_utra_location_convertToJSON() failed [rai]");
        goto end;
    }
    }

    if (utra_location->is_age_of_location_information) {
    if (cJSON_AddNumberToObject(item, "ageOfLocationInformation", utra_location->age_of_location_information) == NULL) {
        ogs_error("OpenAPI_utra_location_convertToJSON() failed [age_of_location_information]");
        goto end;
    }
    }

    if (utra_location->ue_location_timestamp) {
    if (cJSON_AddStringToObject(item, "ueLocationTimestamp", utra_location->ue_location_timestamp) == NULL) {
        ogs_error("OpenAPI_utra_location_convertToJSON() failed [ue_location_timestamp]");
        goto end;
    }
    }

    if (utra_location->geographical_information) {
    if (cJSON_AddStringToObject(item, "geographicalInformation", utra_location->geographical_information) == NULL) {
        ogs_error("OpenAPI_utra_location_convertToJSON() failed [geographical_information]");
        goto end;
    }
    }

    if (utra_location->geodetic_information) {
    if (cJSON_AddStringToObject(item, "geodeticInformation", utra_location->geodetic_information) == NULL) {
        ogs_error("OpenAPI_utra_location_convertToJSON() failed [geodetic_information]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_utra_location_t *OpenAPI_utra_location_parseFromJSON(cJSON *utra_locationJSON)
{
    OpenAPI_utra_location_t *utra_location_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *cgi = NULL;
    OpenAPI_cell_global_id_t *cgi_local_nonprim = NULL;
    cJSON *sai = NULL;
    OpenAPI_service_area_id_t *sai_local_nonprim = NULL;
    cJSON *lai = NULL;
    OpenAPI_location_area_id_t *lai_local_nonprim = NULL;
    cJSON *rai = NULL;
    OpenAPI_routing_area_id_t *rai_local_nonprim = NULL;
    cJSON *age_of_location_information = NULL;
    cJSON *ue_location_timestamp = NULL;
    cJSON *geographical_information = NULL;
    cJSON *geodetic_information = NULL;
    cgi = cJSON_GetObjectItemCaseSensitive(utra_locationJSON, "cgi");
    if (cgi) {
    cgi_local_nonprim = OpenAPI_cell_global_id_parseFromJSON(cgi);
    if (!cgi_local_nonprim) {
        ogs_error("OpenAPI_cell_global_id_parseFromJSON failed [cgi]");
        goto end;
    }
    }

    sai = cJSON_GetObjectItemCaseSensitive(utra_locationJSON, "sai");
    if (sai) {
    sai_local_nonprim = OpenAPI_service_area_id_parseFromJSON(sai);
    if (!sai_local_nonprim) {
        ogs_error("OpenAPI_service_area_id_parseFromJSON failed [sai]");
        goto end;
    }
    }

    lai = cJSON_GetObjectItemCaseSensitive(utra_locationJSON, "lai");
    if (lai) {
    lai_local_nonprim = OpenAPI_location_area_id_parseFromJSON(lai);
    if (!lai_local_nonprim) {
        ogs_error("OpenAPI_location_area_id_parseFromJSON failed [lai]");
        goto end;
    }
    }

    rai = cJSON_GetObjectItemCaseSensitive(utra_locationJSON, "rai");
    if (rai) {
    rai_local_nonprim = OpenAPI_routing_area_id_parseFromJSON(rai);
    if (!rai_local_nonprim) {
        ogs_error("OpenAPI_routing_area_id_parseFromJSON failed [rai]");
        goto end;
    }
    }

    age_of_location_information = cJSON_GetObjectItemCaseSensitive(utra_locationJSON, "ageOfLocationInformation");
    if (age_of_location_information) {
    if (!cJSON_IsNumber(age_of_location_information)) {
        ogs_error("OpenAPI_utra_location_parseFromJSON() failed [age_of_location_information]");
        goto end;
    }
    }

    ue_location_timestamp = cJSON_GetObjectItemCaseSensitive(utra_locationJSON, "ueLocationTimestamp");
    if (ue_location_timestamp) {
    if (!cJSON_IsString(ue_location_timestamp) && !cJSON_IsNull(ue_location_timestamp)) {
        ogs_error("OpenAPI_utra_location_parseFromJSON() failed [ue_location_timestamp]");
        goto end;
    }
    }

    geographical_information = cJSON_GetObjectItemCaseSensitive(utra_locationJSON, "geographicalInformation");
    if (geographical_information) {
    if (!cJSON_IsString(geographical_information) && !cJSON_IsNull(geographical_information)) {
        ogs_error("OpenAPI_utra_location_parseFromJSON() failed [geographical_information]");
        goto end;
    }
    }

    geodetic_information = cJSON_GetObjectItemCaseSensitive(utra_locationJSON, "geodeticInformation");
    if (geodetic_information) {
    if (!cJSON_IsString(geodetic_information) && !cJSON_IsNull(geodetic_information)) {
        ogs_error("OpenAPI_utra_location_parseFromJSON() failed [geodetic_information]");
        goto end;
    }
    }

    utra_location_local_var = OpenAPI_utra_location_create (
        cgi ? cgi_local_nonprim : NULL,
        sai ? sai_local_nonprim : NULL,
        lai ? lai_local_nonprim : NULL,
        rai ? rai_local_nonprim : NULL,
        age_of_location_information ? true : false,
        age_of_location_information ? age_of_location_information->valuedouble : 0,
        ue_location_timestamp && !cJSON_IsNull(ue_location_timestamp) ? ogs_strdup(ue_location_timestamp->valuestring) : NULL,
        geographical_information && !cJSON_IsNull(geographical_information) ? ogs_strdup(geographical_information->valuestring) : NULL,
        geodetic_information && !cJSON_IsNull(geodetic_information) ? ogs_strdup(geodetic_information->valuestring) : NULL
    );

    return utra_location_local_var;
end:
    if (cgi_local_nonprim) {
        OpenAPI_cell_global_id_free(cgi_local_nonprim);
        cgi_local_nonprim = NULL;
    }
    if (sai_local_nonprim) {
        OpenAPI_service_area_id_free(sai_local_nonprim);
        sai_local_nonprim = NULL;
    }
    if (lai_local_nonprim) {
        OpenAPI_location_area_id_free(lai_local_nonprim);
        lai_local_nonprim = NULL;
    }
    if (rai_local_nonprim) {
        OpenAPI_routing_area_id_free(rai_local_nonprim);
        rai_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_utra_location_t *OpenAPI_utra_location_copy(OpenAPI_utra_location_t *dst, OpenAPI_utra_location_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_utra_location_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_utra_location_convertToJSON() failed");
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

    OpenAPI_utra_location_free(dst);
    dst = OpenAPI_utra_location_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

