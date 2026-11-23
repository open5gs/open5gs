
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lcs_information.h"

OpenAPI_lcs_information_t *OpenAPI_lcs_information_create(
    char *l_cs_client_id,
    OpenAPI_location_type_e location_type,
    OpenAPI_location_estimate_t *location_estimate,
    char *positioning_data,
    char *target_ueid
)
{
    OpenAPI_lcs_information_t *lcs_information_local_var = ogs_malloc(sizeof(OpenAPI_lcs_information_t));
    ogs_assert(lcs_information_local_var);

    lcs_information_local_var->l_cs_client_id = l_cs_client_id;
    lcs_information_local_var->location_type = location_type;
    lcs_information_local_var->location_estimate = location_estimate;
    lcs_information_local_var->positioning_data = positioning_data;
    lcs_information_local_var->target_ueid = target_ueid;

    return lcs_information_local_var;
}

void OpenAPI_lcs_information_free(OpenAPI_lcs_information_t *lcs_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == lcs_information) {
        return;
    }
    if (lcs_information->l_cs_client_id) {
        ogs_free(lcs_information->l_cs_client_id);
        lcs_information->l_cs_client_id = NULL;
    }
    if (lcs_information->location_estimate) {
        OpenAPI_location_estimate_free(lcs_information->location_estimate);
        lcs_information->location_estimate = NULL;
    }
    if (lcs_information->positioning_data) {
        ogs_free(lcs_information->positioning_data);
        lcs_information->positioning_data = NULL;
    }
    if (lcs_information->target_ueid) {
        ogs_free(lcs_information->target_ueid);
        lcs_information->target_ueid = NULL;
    }
    ogs_free(lcs_information);
}

cJSON *OpenAPI_lcs_information_convertToJSON(OpenAPI_lcs_information_t *lcs_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (lcs_information == NULL) {
        ogs_error("OpenAPI_lcs_information_convertToJSON() failed [LCSInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (lcs_information->l_cs_client_id) {
    if (cJSON_AddStringToObject(item, "lCSClientID", lcs_information->l_cs_client_id) == NULL) {
        ogs_error("OpenAPI_lcs_information_convertToJSON() failed [l_cs_client_id]");
        goto end;
    }
    }

    if (lcs_information->location_type != OpenAPI_location_type_NULL) {
    if (cJSON_AddStringToObject(item, "locationType", OpenAPI_location_type_ToString(lcs_information->location_type)) == NULL) {
        ogs_error("OpenAPI_lcs_information_convertToJSON() failed [location_type]");
        goto end;
    }
    }

    if (lcs_information->location_estimate) {
    cJSON *location_estimate_local_JSON = OpenAPI_location_estimate_convertToJSON(lcs_information->location_estimate);
    if (location_estimate_local_JSON == NULL) {
        ogs_error("OpenAPI_lcs_information_convertToJSON() failed [location_estimate]");
        goto end;
    }
    cJSON_AddItemToObject(item, "locationEstimate", location_estimate_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_lcs_information_convertToJSON() failed [location_estimate]");
        goto end;
    }
    }

    if (lcs_information->positioning_data) {
    if (cJSON_AddStringToObject(item, "positioningData", lcs_information->positioning_data) == NULL) {
        ogs_error("OpenAPI_lcs_information_convertToJSON() failed [positioning_data]");
        goto end;
    }
    }

    if (lcs_information->target_ueid) {
    if (cJSON_AddStringToObject(item, "targetUEID", lcs_information->target_ueid) == NULL) {
        ogs_error("OpenAPI_lcs_information_convertToJSON() failed [target_ueid]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_lcs_information_t *OpenAPI_lcs_information_parseFromJSON(cJSON *lcs_informationJSON)
{
    OpenAPI_lcs_information_t *lcs_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *l_cs_client_id = NULL;
    cJSON *location_type = NULL;
    OpenAPI_location_type_e location_typeVariable = 0;
    cJSON *location_estimate = NULL;
    OpenAPI_location_estimate_t *location_estimate_local_nonprim = NULL;
    cJSON *positioning_data = NULL;
    cJSON *target_ueid = NULL;
    l_cs_client_id = cJSON_GetObjectItemCaseSensitive(lcs_informationJSON, "lCSClientID");
    if (l_cs_client_id) {
    if (!cJSON_IsString(l_cs_client_id) && !cJSON_IsNull(l_cs_client_id)) {
        ogs_error("OpenAPI_lcs_information_parseFromJSON() failed [l_cs_client_id]");
        goto end;
    }
    }

    location_type = cJSON_GetObjectItemCaseSensitive(lcs_informationJSON, "locationType");
    if (location_type) {
    if (!cJSON_IsString(location_type)) {
        ogs_error("OpenAPI_lcs_information_parseFromJSON() failed [location_type]");
        goto end;
    }
    location_typeVariable = OpenAPI_location_type_FromString(location_type->valuestring);
    }

    location_estimate = cJSON_GetObjectItemCaseSensitive(lcs_informationJSON, "locationEstimate");
    if (location_estimate) {
    location_estimate_local_nonprim = OpenAPI_location_estimate_parseFromJSON(location_estimate);
    if (!location_estimate_local_nonprim) {
        ogs_error("OpenAPI_location_estimate_parseFromJSON failed [location_estimate]");
        goto end;
    }
    }

    positioning_data = cJSON_GetObjectItemCaseSensitive(lcs_informationJSON, "positioningData");
    if (positioning_data) {
    if (!cJSON_IsString(positioning_data) && !cJSON_IsNull(positioning_data)) {
        ogs_error("OpenAPI_lcs_information_parseFromJSON() failed [positioning_data]");
        goto end;
    }
    }

    target_ueid = cJSON_GetObjectItemCaseSensitive(lcs_informationJSON, "targetUEID");
    if (target_ueid) {
    if (!cJSON_IsString(target_ueid) && !cJSON_IsNull(target_ueid)) {
        ogs_error("OpenAPI_lcs_information_parseFromJSON() failed [target_ueid]");
        goto end;
    }
    }

    lcs_information_local_var = OpenAPI_lcs_information_create (
        l_cs_client_id && !cJSON_IsNull(l_cs_client_id) ? ogs_strdup(l_cs_client_id->valuestring) : NULL,
        location_type ? location_typeVariable : 0,
        location_estimate ? location_estimate_local_nonprim : NULL,
        positioning_data && !cJSON_IsNull(positioning_data) ? ogs_strdup(positioning_data->valuestring) : NULL,
        target_ueid && !cJSON_IsNull(target_ueid) ? ogs_strdup(target_ueid->valuestring) : NULL
    );

    return lcs_information_local_var;
end:
    if (location_estimate_local_nonprim) {
        OpenAPI_location_estimate_free(location_estimate_local_nonprim);
        location_estimate_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_lcs_information_t *OpenAPI_lcs_information_copy(OpenAPI_lcs_information_t *dst, OpenAPI_lcs_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_lcs_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_lcs_information_convertToJSON() failed");
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

    OpenAPI_lcs_information_free(dst);
    dst = OpenAPI_lcs_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

