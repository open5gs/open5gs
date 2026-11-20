
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ranging_sl_charging_information.h"

OpenAPI_ranging_sl_charging_information_t *OpenAPI_ranging_sl_charging_information_create(
    char *target_ueid,
    char *s_l_reference_ueid,
    char *s_l_positioning_server_ueid,
    char *located_ueid,
    OpenAPI_location_type_e location_type,
    OpenAPI_location_estimate_t *location_estimate
)
{
    OpenAPI_ranging_sl_charging_information_t *ranging_sl_charging_information_local_var = ogs_malloc(sizeof(OpenAPI_ranging_sl_charging_information_t));
    ogs_assert(ranging_sl_charging_information_local_var);

    ranging_sl_charging_information_local_var->target_ueid = target_ueid;
    ranging_sl_charging_information_local_var->s_l_reference_ueid = s_l_reference_ueid;
    ranging_sl_charging_information_local_var->s_l_positioning_server_ueid = s_l_positioning_server_ueid;
    ranging_sl_charging_information_local_var->located_ueid = located_ueid;
    ranging_sl_charging_information_local_var->location_type = location_type;
    ranging_sl_charging_information_local_var->location_estimate = location_estimate;

    return ranging_sl_charging_information_local_var;
}

void OpenAPI_ranging_sl_charging_information_free(OpenAPI_ranging_sl_charging_information_t *ranging_sl_charging_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ranging_sl_charging_information) {
        return;
    }
    if (ranging_sl_charging_information->target_ueid) {
        ogs_free(ranging_sl_charging_information->target_ueid);
        ranging_sl_charging_information->target_ueid = NULL;
    }
    if (ranging_sl_charging_information->s_l_reference_ueid) {
        ogs_free(ranging_sl_charging_information->s_l_reference_ueid);
        ranging_sl_charging_information->s_l_reference_ueid = NULL;
    }
    if (ranging_sl_charging_information->s_l_positioning_server_ueid) {
        ogs_free(ranging_sl_charging_information->s_l_positioning_server_ueid);
        ranging_sl_charging_information->s_l_positioning_server_ueid = NULL;
    }
    if (ranging_sl_charging_information->located_ueid) {
        ogs_free(ranging_sl_charging_information->located_ueid);
        ranging_sl_charging_information->located_ueid = NULL;
    }
    if (ranging_sl_charging_information->location_estimate) {
        OpenAPI_location_estimate_free(ranging_sl_charging_information->location_estimate);
        ranging_sl_charging_information->location_estimate = NULL;
    }
    ogs_free(ranging_sl_charging_information);
}

cJSON *OpenAPI_ranging_sl_charging_information_convertToJSON(OpenAPI_ranging_sl_charging_information_t *ranging_sl_charging_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ranging_sl_charging_information == NULL) {
        ogs_error("OpenAPI_ranging_sl_charging_information_convertToJSON() failed [RangingSLChargingInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ranging_sl_charging_information->target_ueid) {
    if (cJSON_AddStringToObject(item, "targetUEID", ranging_sl_charging_information->target_ueid) == NULL) {
        ogs_error("OpenAPI_ranging_sl_charging_information_convertToJSON() failed [target_ueid]");
        goto end;
    }
    }

    if (ranging_sl_charging_information->s_l_reference_ueid) {
    if (cJSON_AddStringToObject(item, "sLReferenceUEID", ranging_sl_charging_information->s_l_reference_ueid) == NULL) {
        ogs_error("OpenAPI_ranging_sl_charging_information_convertToJSON() failed [s_l_reference_ueid]");
        goto end;
    }
    }

    if (ranging_sl_charging_information->s_l_positioning_server_ueid) {
    if (cJSON_AddStringToObject(item, "sLPositioningServerUEID", ranging_sl_charging_information->s_l_positioning_server_ueid) == NULL) {
        ogs_error("OpenAPI_ranging_sl_charging_information_convertToJSON() failed [s_l_positioning_server_ueid]");
        goto end;
    }
    }

    if (ranging_sl_charging_information->located_ueid) {
    if (cJSON_AddStringToObject(item, "locatedUEID", ranging_sl_charging_information->located_ueid) == NULL) {
        ogs_error("OpenAPI_ranging_sl_charging_information_convertToJSON() failed [located_ueid]");
        goto end;
    }
    }

    if (ranging_sl_charging_information->location_type != OpenAPI_location_type_NULL) {
    if (cJSON_AddStringToObject(item, "locationType", OpenAPI_location_type_ToString(ranging_sl_charging_information->location_type)) == NULL) {
        ogs_error("OpenAPI_ranging_sl_charging_information_convertToJSON() failed [location_type]");
        goto end;
    }
    }

    if (ranging_sl_charging_information->location_estimate) {
    cJSON *location_estimate_local_JSON = OpenAPI_location_estimate_convertToJSON(ranging_sl_charging_information->location_estimate);
    if (location_estimate_local_JSON == NULL) {
        ogs_error("OpenAPI_ranging_sl_charging_information_convertToJSON() failed [location_estimate]");
        goto end;
    }
    cJSON_AddItemToObject(item, "locationEstimate", location_estimate_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ranging_sl_charging_information_convertToJSON() failed [location_estimate]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ranging_sl_charging_information_t *OpenAPI_ranging_sl_charging_information_parseFromJSON(cJSON *ranging_sl_charging_informationJSON)
{
    OpenAPI_ranging_sl_charging_information_t *ranging_sl_charging_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *target_ueid = NULL;
    cJSON *s_l_reference_ueid = NULL;
    cJSON *s_l_positioning_server_ueid = NULL;
    cJSON *located_ueid = NULL;
    cJSON *location_type = NULL;
    OpenAPI_location_type_e location_typeVariable = 0;
    cJSON *location_estimate = NULL;
    OpenAPI_location_estimate_t *location_estimate_local_nonprim = NULL;
    target_ueid = cJSON_GetObjectItemCaseSensitive(ranging_sl_charging_informationJSON, "targetUEID");
    if (target_ueid) {
    if (!cJSON_IsString(target_ueid) && !cJSON_IsNull(target_ueid)) {
        ogs_error("OpenAPI_ranging_sl_charging_information_parseFromJSON() failed [target_ueid]");
        goto end;
    }
    }

    s_l_reference_ueid = cJSON_GetObjectItemCaseSensitive(ranging_sl_charging_informationJSON, "sLReferenceUEID");
    if (s_l_reference_ueid) {
    if (!cJSON_IsString(s_l_reference_ueid) && !cJSON_IsNull(s_l_reference_ueid)) {
        ogs_error("OpenAPI_ranging_sl_charging_information_parseFromJSON() failed [s_l_reference_ueid]");
        goto end;
    }
    }

    s_l_positioning_server_ueid = cJSON_GetObjectItemCaseSensitive(ranging_sl_charging_informationJSON, "sLPositioningServerUEID");
    if (s_l_positioning_server_ueid) {
    if (!cJSON_IsString(s_l_positioning_server_ueid) && !cJSON_IsNull(s_l_positioning_server_ueid)) {
        ogs_error("OpenAPI_ranging_sl_charging_information_parseFromJSON() failed [s_l_positioning_server_ueid]");
        goto end;
    }
    }

    located_ueid = cJSON_GetObjectItemCaseSensitive(ranging_sl_charging_informationJSON, "locatedUEID");
    if (located_ueid) {
    if (!cJSON_IsString(located_ueid) && !cJSON_IsNull(located_ueid)) {
        ogs_error("OpenAPI_ranging_sl_charging_information_parseFromJSON() failed [located_ueid]");
        goto end;
    }
    }

    location_type = cJSON_GetObjectItemCaseSensitive(ranging_sl_charging_informationJSON, "locationType");
    if (location_type) {
    if (!cJSON_IsString(location_type)) {
        ogs_error("OpenAPI_ranging_sl_charging_information_parseFromJSON() failed [location_type]");
        goto end;
    }
    location_typeVariable = OpenAPI_location_type_FromString(location_type->valuestring);
    }

    location_estimate = cJSON_GetObjectItemCaseSensitive(ranging_sl_charging_informationJSON, "locationEstimate");
    if (location_estimate) {
    location_estimate_local_nonprim = OpenAPI_location_estimate_parseFromJSON(location_estimate);
    if (!location_estimate_local_nonprim) {
        ogs_error("OpenAPI_location_estimate_parseFromJSON failed [location_estimate]");
        goto end;
    }
    }

    ranging_sl_charging_information_local_var = OpenAPI_ranging_sl_charging_information_create (
        target_ueid && !cJSON_IsNull(target_ueid) ? ogs_strdup(target_ueid->valuestring) : NULL,
        s_l_reference_ueid && !cJSON_IsNull(s_l_reference_ueid) ? ogs_strdup(s_l_reference_ueid->valuestring) : NULL,
        s_l_positioning_server_ueid && !cJSON_IsNull(s_l_positioning_server_ueid) ? ogs_strdup(s_l_positioning_server_ueid->valuestring) : NULL,
        located_ueid && !cJSON_IsNull(located_ueid) ? ogs_strdup(located_ueid->valuestring) : NULL,
        location_type ? location_typeVariable : 0,
        location_estimate ? location_estimate_local_nonprim : NULL
    );

    return ranging_sl_charging_information_local_var;
end:
    if (location_estimate_local_nonprim) {
        OpenAPI_location_estimate_free(location_estimate_local_nonprim);
        location_estimate_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_ranging_sl_charging_information_t *OpenAPI_ranging_sl_charging_information_copy(OpenAPI_ranging_sl_charging_information_t *dst, OpenAPI_ranging_sl_charging_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ranging_sl_charging_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ranging_sl_charging_information_convertToJSON() failed");
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

    OpenAPI_ranging_sl_charging_information_free(dst);
    dst = OpenAPI_ranging_sl_charging_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

