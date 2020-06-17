
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lpi.h"

OpenAPI_lpi_t *OpenAPI_lpi_create(
    OpenAPI_location_privacy_ind_e location_privacy_ind,
    OpenAPI_valid_time_period_t *valid_time_period
    )
{
    OpenAPI_lpi_t *lpi_local_var = OpenAPI_malloc(sizeof(OpenAPI_lpi_t));
    if (!lpi_local_var) {
        return NULL;
    }
    lpi_local_var->location_privacy_ind = location_privacy_ind;
    lpi_local_var->valid_time_period = valid_time_period;

    return lpi_local_var;
}

void OpenAPI_lpi_free(OpenAPI_lpi_t *lpi)
{
    if (NULL == lpi) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_valid_time_period_free(lpi->valid_time_period);
    ogs_free(lpi);
}

cJSON *OpenAPI_lpi_convertToJSON(OpenAPI_lpi_t *lpi)
{
    cJSON *item = NULL;

    if (lpi == NULL) {
        ogs_error("OpenAPI_lpi_convertToJSON() failed [Lpi]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!lpi->location_privacy_ind) {
        ogs_error("OpenAPI_lpi_convertToJSON() failed [location_privacy_ind]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "locationPrivacyInd", OpenAPI_location_privacy_ind_ToString(lpi->location_privacy_ind)) == NULL) {
        ogs_error("OpenAPI_lpi_convertToJSON() failed [location_privacy_ind]");
        goto end;
    }

    if (lpi->valid_time_period) {
        cJSON *valid_time_period_local_JSON = OpenAPI_valid_time_period_convertToJSON(lpi->valid_time_period);
        if (valid_time_period_local_JSON == NULL) {
            ogs_error("OpenAPI_lpi_convertToJSON() failed [valid_time_period]");
            goto end;
        }
        cJSON_AddItemToObject(item, "validTimePeriod", valid_time_period_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_lpi_convertToJSON() failed [valid_time_period]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_lpi_t *OpenAPI_lpi_parseFromJSON(cJSON *lpiJSON)
{
    OpenAPI_lpi_t *lpi_local_var = NULL;
    cJSON *location_privacy_ind = cJSON_GetObjectItemCaseSensitive(lpiJSON, "locationPrivacyInd");
    if (!location_privacy_ind) {
        ogs_error("OpenAPI_lpi_parseFromJSON() failed [location_privacy_ind]");
        goto end;
    }

    OpenAPI_location_privacy_ind_e location_privacy_indVariable;

    if (!cJSON_IsString(location_privacy_ind)) {
        ogs_error("OpenAPI_lpi_parseFromJSON() failed [location_privacy_ind]");
        goto end;
    }
    location_privacy_indVariable = OpenAPI_location_privacy_ind_FromString(location_privacy_ind->valuestring);

    cJSON *valid_time_period = cJSON_GetObjectItemCaseSensitive(lpiJSON, "validTimePeriod");

    OpenAPI_valid_time_period_t *valid_time_period_local_nonprim = NULL;
    if (valid_time_period) {
        valid_time_period_local_nonprim = OpenAPI_valid_time_period_parseFromJSON(valid_time_period);
    }

    lpi_local_var = OpenAPI_lpi_create (
        location_privacy_indVariable,
        valid_time_period ? valid_time_period_local_nonprim : NULL
        );

    return lpi_local_var;
end:
    return NULL;
}

OpenAPI_lpi_t *OpenAPI_lpi_copy(OpenAPI_lpi_t *dst, OpenAPI_lpi_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_lpi_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_lpi_convertToJSON() failed");
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

    OpenAPI_lpi_free(dst);
    dst = OpenAPI_lpi_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

