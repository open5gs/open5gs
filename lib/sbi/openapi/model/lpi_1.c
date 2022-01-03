
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lpi_1.h"

OpenAPI_lpi_1_t *OpenAPI_lpi_1_create(
    OpenAPI_location_privacy_ind_e location_privacy_ind,
    OpenAPI_valid_time_period_1_t *valid_time_period
)
{
    OpenAPI_lpi_1_t *lpi_1_local_var = ogs_malloc(sizeof(OpenAPI_lpi_1_t));
    ogs_assert(lpi_1_local_var);

    lpi_1_local_var->location_privacy_ind = location_privacy_ind;
    lpi_1_local_var->valid_time_period = valid_time_period;

    return lpi_1_local_var;
}

void OpenAPI_lpi_1_free(OpenAPI_lpi_1_t *lpi_1)
{
    if (NULL == lpi_1) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_valid_time_period_1_free(lpi_1->valid_time_period);
    ogs_free(lpi_1);
}

cJSON *OpenAPI_lpi_1_convertToJSON(OpenAPI_lpi_1_t *lpi_1)
{
    cJSON *item = NULL;

    if (lpi_1 == NULL) {
        ogs_error("OpenAPI_lpi_1_convertToJSON() failed [Lpi_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddStringToObject(item, "locationPrivacyInd", OpenAPI_location_privacy_ind_ToString(lpi_1->location_privacy_ind)) == NULL) {
        ogs_error("OpenAPI_lpi_1_convertToJSON() failed [location_privacy_ind]");
        goto end;
    }

    if (lpi_1->valid_time_period) {
    cJSON *valid_time_period_local_JSON = OpenAPI_valid_time_period_1_convertToJSON(lpi_1->valid_time_period);
    if (valid_time_period_local_JSON == NULL) {
        ogs_error("OpenAPI_lpi_1_convertToJSON() failed [valid_time_period]");
        goto end;
    }
    cJSON_AddItemToObject(item, "validTimePeriod", valid_time_period_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_lpi_1_convertToJSON() failed [valid_time_period]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_lpi_1_t *OpenAPI_lpi_1_parseFromJSON(cJSON *lpi_1JSON)
{
    OpenAPI_lpi_1_t *lpi_1_local_var = NULL;
    cJSON *location_privacy_ind = cJSON_GetObjectItemCaseSensitive(lpi_1JSON, "locationPrivacyInd");
    if (!location_privacy_ind) {
        ogs_error("OpenAPI_lpi_1_parseFromJSON() failed [location_privacy_ind]");
        goto end;
    }

    OpenAPI_location_privacy_ind_e location_privacy_indVariable;
    if (!cJSON_IsString(location_privacy_ind)) {
        ogs_error("OpenAPI_lpi_1_parseFromJSON() failed [location_privacy_ind]");
        goto end;
    }
    location_privacy_indVariable = OpenAPI_location_privacy_ind_FromString(location_privacy_ind->valuestring);

    cJSON *valid_time_period = cJSON_GetObjectItemCaseSensitive(lpi_1JSON, "validTimePeriod");

    OpenAPI_valid_time_period_1_t *valid_time_period_local_nonprim = NULL;
    if (valid_time_period) {
    valid_time_period_local_nonprim = OpenAPI_valid_time_period_1_parseFromJSON(valid_time_period);
    }

    lpi_1_local_var = OpenAPI_lpi_1_create (
        location_privacy_indVariable,
        valid_time_period ? valid_time_period_local_nonprim : NULL
    );

    return lpi_1_local_var;
end:
    return NULL;
}

OpenAPI_lpi_1_t *OpenAPI_lpi_1_copy(OpenAPI_lpi_1_t *dst, OpenAPI_lpi_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_lpi_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_lpi_1_convertToJSON() failed");
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

    OpenAPI_lpi_1_free(dst);
    dst = OpenAPI_lpi_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

