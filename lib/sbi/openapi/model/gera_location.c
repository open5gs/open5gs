
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "gera_location.h"

OpenAPI_gera_location_t *OpenAPI_gera_location_create(
    char *location_number,
    OpenAPI_cell_global_id_t *cgi,
    OpenAPI_routing_area_id_t *rai,
    OpenAPI_service_area_id_t *sai,
    OpenAPI_location_area_id_t *lai,
    char *vlr_number,
    char *msc_number,
    bool is_age_of_location_information,
    int age_of_location_information,
    char *ue_location_timestamp,
    char *geographical_information,
    char *geodetic_information
)
{
    OpenAPI_gera_location_t *gera_location_local_var = ogs_malloc(sizeof(OpenAPI_gera_location_t));
    ogs_assert(gera_location_local_var);

    gera_location_local_var->location_number = location_number;
    gera_location_local_var->cgi = cgi;
    gera_location_local_var->rai = rai;
    gera_location_local_var->sai = sai;
    gera_location_local_var->lai = lai;
    gera_location_local_var->vlr_number = vlr_number;
    gera_location_local_var->msc_number = msc_number;
    gera_location_local_var->is_age_of_location_information = is_age_of_location_information;
    gera_location_local_var->age_of_location_information = age_of_location_information;
    gera_location_local_var->ue_location_timestamp = ue_location_timestamp;
    gera_location_local_var->geographical_information = geographical_information;
    gera_location_local_var->geodetic_information = geodetic_information;

    return gera_location_local_var;
}

void OpenAPI_gera_location_free(OpenAPI_gera_location_t *gera_location)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == gera_location) {
        return;
    }
    if (gera_location->location_number) {
        ogs_free(gera_location->location_number);
        gera_location->location_number = NULL;
    }
    if (gera_location->cgi) {
        OpenAPI_cell_global_id_free(gera_location->cgi);
        gera_location->cgi = NULL;
    }
    if (gera_location->rai) {
        OpenAPI_routing_area_id_free(gera_location->rai);
        gera_location->rai = NULL;
    }
    if (gera_location->sai) {
        OpenAPI_service_area_id_free(gera_location->sai);
        gera_location->sai = NULL;
    }
    if (gera_location->lai) {
        OpenAPI_location_area_id_free(gera_location->lai);
        gera_location->lai = NULL;
    }
    if (gera_location->vlr_number) {
        ogs_free(gera_location->vlr_number);
        gera_location->vlr_number = NULL;
    }
    if (gera_location->msc_number) {
        ogs_free(gera_location->msc_number);
        gera_location->msc_number = NULL;
    }
    if (gera_location->ue_location_timestamp) {
        ogs_free(gera_location->ue_location_timestamp);
        gera_location->ue_location_timestamp = NULL;
    }
    if (gera_location->geographical_information) {
        ogs_free(gera_location->geographical_information);
        gera_location->geographical_information = NULL;
    }
    if (gera_location->geodetic_information) {
        ogs_free(gera_location->geodetic_information);
        gera_location->geodetic_information = NULL;
    }
    ogs_free(gera_location);
}

cJSON *OpenAPI_gera_location_convertToJSON(OpenAPI_gera_location_t *gera_location)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (gera_location == NULL) {
        ogs_error("OpenAPI_gera_location_convertToJSON() failed [GeraLocation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (gera_location->location_number) {
    if (cJSON_AddStringToObject(item, "locationNumber", gera_location->location_number) == NULL) {
        ogs_error("OpenAPI_gera_location_convertToJSON() failed [location_number]");
        goto end;
    }
    }

    if (gera_location->cgi) {
    cJSON *cgi_local_JSON = OpenAPI_cell_global_id_convertToJSON(gera_location->cgi);
    if (cgi_local_JSON == NULL) {
        ogs_error("OpenAPI_gera_location_convertToJSON() failed [cgi]");
        goto end;
    }
    cJSON_AddItemToObject(item, "cgi", cgi_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_gera_location_convertToJSON() failed [cgi]");
        goto end;
    }
    }

    if (gera_location->rai) {
    cJSON *rai_local_JSON = OpenAPI_routing_area_id_convertToJSON(gera_location->rai);
    if (rai_local_JSON == NULL) {
        ogs_error("OpenAPI_gera_location_convertToJSON() failed [rai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "rai", rai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_gera_location_convertToJSON() failed [rai]");
        goto end;
    }
    }

    if (gera_location->sai) {
    cJSON *sai_local_JSON = OpenAPI_service_area_id_convertToJSON(gera_location->sai);
    if (sai_local_JSON == NULL) {
        ogs_error("OpenAPI_gera_location_convertToJSON() failed [sai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sai", sai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_gera_location_convertToJSON() failed [sai]");
        goto end;
    }
    }

    if (gera_location->lai) {
    cJSON *lai_local_JSON = OpenAPI_location_area_id_convertToJSON(gera_location->lai);
    if (lai_local_JSON == NULL) {
        ogs_error("OpenAPI_gera_location_convertToJSON() failed [lai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "lai", lai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_gera_location_convertToJSON() failed [lai]");
        goto end;
    }
    }

    if (gera_location->vlr_number) {
    if (cJSON_AddStringToObject(item, "vlrNumber", gera_location->vlr_number) == NULL) {
        ogs_error("OpenAPI_gera_location_convertToJSON() failed [vlr_number]");
        goto end;
    }
    }

    if (gera_location->msc_number) {
    if (cJSON_AddStringToObject(item, "mscNumber", gera_location->msc_number) == NULL) {
        ogs_error("OpenAPI_gera_location_convertToJSON() failed [msc_number]");
        goto end;
    }
    }

    if (gera_location->is_age_of_location_information) {
    if (cJSON_AddNumberToObject(item, "ageOfLocationInformation", gera_location->age_of_location_information) == NULL) {
        ogs_error("OpenAPI_gera_location_convertToJSON() failed [age_of_location_information]");
        goto end;
    }
    }

    if (gera_location->ue_location_timestamp) {
    if (cJSON_AddStringToObject(item, "ueLocationTimestamp", gera_location->ue_location_timestamp) == NULL) {
        ogs_error("OpenAPI_gera_location_convertToJSON() failed [ue_location_timestamp]");
        goto end;
    }
    }

    if (gera_location->geographical_information) {
    if (cJSON_AddStringToObject(item, "geographicalInformation", gera_location->geographical_information) == NULL) {
        ogs_error("OpenAPI_gera_location_convertToJSON() failed [geographical_information]");
        goto end;
    }
    }

    if (gera_location->geodetic_information) {
    if (cJSON_AddStringToObject(item, "geodeticInformation", gera_location->geodetic_information) == NULL) {
        ogs_error("OpenAPI_gera_location_convertToJSON() failed [geodetic_information]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_gera_location_t *OpenAPI_gera_location_parseFromJSON(cJSON *gera_locationJSON)
{
    OpenAPI_gera_location_t *gera_location_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *location_number = NULL;
    cJSON *cgi = NULL;
    OpenAPI_cell_global_id_t *cgi_local_nonprim = NULL;
    cJSON *rai = NULL;
    OpenAPI_routing_area_id_t *rai_local_nonprim = NULL;
    cJSON *sai = NULL;
    OpenAPI_service_area_id_t *sai_local_nonprim = NULL;
    cJSON *lai = NULL;
    OpenAPI_location_area_id_t *lai_local_nonprim = NULL;
    cJSON *vlr_number = NULL;
    cJSON *msc_number = NULL;
    cJSON *age_of_location_information = NULL;
    cJSON *ue_location_timestamp = NULL;
    cJSON *geographical_information = NULL;
    cJSON *geodetic_information = NULL;
    location_number = cJSON_GetObjectItemCaseSensitive(gera_locationJSON, "locationNumber");
    if (location_number) {
    if (!cJSON_IsString(location_number) && !cJSON_IsNull(location_number)) {
        ogs_error("OpenAPI_gera_location_parseFromJSON() failed [location_number]");
        goto end;
    }
    }

    cgi = cJSON_GetObjectItemCaseSensitive(gera_locationJSON, "cgi");
    if (cgi) {
    cgi_local_nonprim = OpenAPI_cell_global_id_parseFromJSON(cgi);
    if (!cgi_local_nonprim) {
        ogs_error("OpenAPI_cell_global_id_parseFromJSON failed [cgi]");
        goto end;
    }
    }

    rai = cJSON_GetObjectItemCaseSensitive(gera_locationJSON, "rai");
    if (rai) {
    rai_local_nonprim = OpenAPI_routing_area_id_parseFromJSON(rai);
    if (!rai_local_nonprim) {
        ogs_error("OpenAPI_routing_area_id_parseFromJSON failed [rai]");
        goto end;
    }
    }

    sai = cJSON_GetObjectItemCaseSensitive(gera_locationJSON, "sai");
    if (sai) {
    sai_local_nonprim = OpenAPI_service_area_id_parseFromJSON(sai);
    if (!sai_local_nonprim) {
        ogs_error("OpenAPI_service_area_id_parseFromJSON failed [sai]");
        goto end;
    }
    }

    lai = cJSON_GetObjectItemCaseSensitive(gera_locationJSON, "lai");
    if (lai) {
    lai_local_nonprim = OpenAPI_location_area_id_parseFromJSON(lai);
    if (!lai_local_nonprim) {
        ogs_error("OpenAPI_location_area_id_parseFromJSON failed [lai]");
        goto end;
    }
    }

    vlr_number = cJSON_GetObjectItemCaseSensitive(gera_locationJSON, "vlrNumber");
    if (vlr_number) {
    if (!cJSON_IsString(vlr_number) && !cJSON_IsNull(vlr_number)) {
        ogs_error("OpenAPI_gera_location_parseFromJSON() failed [vlr_number]");
        goto end;
    }
    }

    msc_number = cJSON_GetObjectItemCaseSensitive(gera_locationJSON, "mscNumber");
    if (msc_number) {
    if (!cJSON_IsString(msc_number) && !cJSON_IsNull(msc_number)) {
        ogs_error("OpenAPI_gera_location_parseFromJSON() failed [msc_number]");
        goto end;
    }
    }

    age_of_location_information = cJSON_GetObjectItemCaseSensitive(gera_locationJSON, "ageOfLocationInformation");
    if (age_of_location_information) {
    if (!cJSON_IsNumber(age_of_location_information)) {
        ogs_error("OpenAPI_gera_location_parseFromJSON() failed [age_of_location_information]");
        goto end;
    }
    }

    ue_location_timestamp = cJSON_GetObjectItemCaseSensitive(gera_locationJSON, "ueLocationTimestamp");
    if (ue_location_timestamp) {
    if (!cJSON_IsString(ue_location_timestamp) && !cJSON_IsNull(ue_location_timestamp)) {
        ogs_error("OpenAPI_gera_location_parseFromJSON() failed [ue_location_timestamp]");
        goto end;
    }
    }

    geographical_information = cJSON_GetObjectItemCaseSensitive(gera_locationJSON, "geographicalInformation");
    if (geographical_information) {
    if (!cJSON_IsString(geographical_information) && !cJSON_IsNull(geographical_information)) {
        ogs_error("OpenAPI_gera_location_parseFromJSON() failed [geographical_information]");
        goto end;
    }
    }

    geodetic_information = cJSON_GetObjectItemCaseSensitive(gera_locationJSON, "geodeticInformation");
    if (geodetic_information) {
    if (!cJSON_IsString(geodetic_information) && !cJSON_IsNull(geodetic_information)) {
        ogs_error("OpenAPI_gera_location_parseFromJSON() failed [geodetic_information]");
        goto end;
    }
    }

    gera_location_local_var = OpenAPI_gera_location_create (
        location_number && !cJSON_IsNull(location_number) ? ogs_strdup(location_number->valuestring) : NULL,
        cgi ? cgi_local_nonprim : NULL,
        rai ? rai_local_nonprim : NULL,
        sai ? sai_local_nonprim : NULL,
        lai ? lai_local_nonprim : NULL,
        vlr_number && !cJSON_IsNull(vlr_number) ? ogs_strdup(vlr_number->valuestring) : NULL,
        msc_number && !cJSON_IsNull(msc_number) ? ogs_strdup(msc_number->valuestring) : NULL,
        age_of_location_information ? true : false,
        age_of_location_information ? age_of_location_information->valuedouble : 0,
        ue_location_timestamp && !cJSON_IsNull(ue_location_timestamp) ? ogs_strdup(ue_location_timestamp->valuestring) : NULL,
        geographical_information && !cJSON_IsNull(geographical_information) ? ogs_strdup(geographical_information->valuestring) : NULL,
        geodetic_information && !cJSON_IsNull(geodetic_information) ? ogs_strdup(geodetic_information->valuestring) : NULL
    );

    return gera_location_local_var;
end:
    if (cgi_local_nonprim) {
        OpenAPI_cell_global_id_free(cgi_local_nonprim);
        cgi_local_nonprim = NULL;
    }
    if (rai_local_nonprim) {
        OpenAPI_routing_area_id_free(rai_local_nonprim);
        rai_local_nonprim = NULL;
    }
    if (sai_local_nonprim) {
        OpenAPI_service_area_id_free(sai_local_nonprim);
        sai_local_nonprim = NULL;
    }
    if (lai_local_nonprim) {
        OpenAPI_location_area_id_free(lai_local_nonprim);
        lai_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_gera_location_t *OpenAPI_gera_location_copy(OpenAPI_gera_location_t *dst, OpenAPI_gera_location_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_gera_location_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_gera_location_convertToJSON() failed");
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

    OpenAPI_gera_location_free(dst);
    dst = OpenAPI_gera_location_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

