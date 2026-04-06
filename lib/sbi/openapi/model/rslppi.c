
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "rslppi.h"

OpenAPI_rslppi_t *OpenAPI_rslppi_create(
    OpenAPI_ranging_sl_privacy_ind_e ranging_sl_privacy_ind,
    OpenAPI_valid_time_period_t *valid_time_period
)
{
    OpenAPI_rslppi_t *rslppi_local_var = ogs_malloc(sizeof(OpenAPI_rslppi_t));
    ogs_assert(rslppi_local_var);

    rslppi_local_var->ranging_sl_privacy_ind = ranging_sl_privacy_ind;
    rslppi_local_var->valid_time_period = valid_time_period;

    return rslppi_local_var;
}

void OpenAPI_rslppi_free(OpenAPI_rslppi_t *rslppi)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == rslppi) {
        return;
    }
    if (rslppi->valid_time_period) {
        OpenAPI_valid_time_period_free(rslppi->valid_time_period);
        rslppi->valid_time_period = NULL;
    }
    ogs_free(rslppi);
}

cJSON *OpenAPI_rslppi_convertToJSON(OpenAPI_rslppi_t *rslppi)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (rslppi == NULL) {
        ogs_error("OpenAPI_rslppi_convertToJSON() failed [Rslppi]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (rslppi->ranging_sl_privacy_ind == OpenAPI_ranging_sl_privacy_ind_NULL) {
        ogs_error("OpenAPI_rslppi_convertToJSON() failed [ranging_sl_privacy_ind]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "rangingSlPrivacyInd", OpenAPI_ranging_sl_privacy_ind_ToString(rslppi->ranging_sl_privacy_ind)) == NULL) {
        ogs_error("OpenAPI_rslppi_convertToJSON() failed [ranging_sl_privacy_ind]");
        goto end;
    }

    if (rslppi->valid_time_period) {
    cJSON *valid_time_period_local_JSON = OpenAPI_valid_time_period_convertToJSON(rslppi->valid_time_period);
    if (valid_time_period_local_JSON == NULL) {
        ogs_error("OpenAPI_rslppi_convertToJSON() failed [valid_time_period]");
        goto end;
    }
    cJSON_AddItemToObject(item, "validTimePeriod", valid_time_period_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_rslppi_convertToJSON() failed [valid_time_period]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_rslppi_t *OpenAPI_rslppi_parseFromJSON(cJSON *rslppiJSON)
{
    OpenAPI_rslppi_t *rslppi_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ranging_sl_privacy_ind = NULL;
    OpenAPI_ranging_sl_privacy_ind_e ranging_sl_privacy_indVariable = 0;
    cJSON *valid_time_period = NULL;
    OpenAPI_valid_time_period_t *valid_time_period_local_nonprim = NULL;
    ranging_sl_privacy_ind = cJSON_GetObjectItemCaseSensitive(rslppiJSON, "rangingSlPrivacyInd");
    if (!ranging_sl_privacy_ind) {
        ogs_error("OpenAPI_rslppi_parseFromJSON() failed [ranging_sl_privacy_ind]");
        goto end;
    }
    if (!cJSON_IsString(ranging_sl_privacy_ind)) {
        ogs_error("OpenAPI_rslppi_parseFromJSON() failed [ranging_sl_privacy_ind]");
        goto end;
    }
    ranging_sl_privacy_indVariable = OpenAPI_ranging_sl_privacy_ind_FromString(ranging_sl_privacy_ind->valuestring);

    valid_time_period = cJSON_GetObjectItemCaseSensitive(rslppiJSON, "validTimePeriod");
    if (valid_time_period) {
    valid_time_period_local_nonprim = OpenAPI_valid_time_period_parseFromJSON(valid_time_period);
    if (!valid_time_period_local_nonprim) {
        ogs_error("OpenAPI_valid_time_period_parseFromJSON failed [valid_time_period]");
        goto end;
    }
    }

    rslppi_local_var = OpenAPI_rslppi_create (
        ranging_sl_privacy_indVariable,
        valid_time_period ? valid_time_period_local_nonprim : NULL
    );

    return rslppi_local_var;
end:
    if (valid_time_period_local_nonprim) {
        OpenAPI_valid_time_period_free(valid_time_period_local_nonprim);
        valid_time_period_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_rslppi_t *OpenAPI_rslppi_copy(OpenAPI_rslppi_t *dst, OpenAPI_rslppi_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_rslppi_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_rslppi_convertToJSON() failed");
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

    OpenAPI_rslppi_free(dst);
    dst = OpenAPI_rslppi_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

