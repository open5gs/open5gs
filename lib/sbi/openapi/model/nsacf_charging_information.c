
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nsacf_charging_information.h"

OpenAPI_nsacf_charging_information_t *OpenAPI_nsacf_charging_information_create(
    bool is_n_sac_charging_indicator,
    int n_sac_charging_indicator
)
{
    OpenAPI_nsacf_charging_information_t *nsacf_charging_information_local_var = ogs_malloc(sizeof(OpenAPI_nsacf_charging_information_t));
    ogs_assert(nsacf_charging_information_local_var);

    nsacf_charging_information_local_var->is_n_sac_charging_indicator = is_n_sac_charging_indicator;
    nsacf_charging_information_local_var->n_sac_charging_indicator = n_sac_charging_indicator;

    return nsacf_charging_information_local_var;
}

void OpenAPI_nsacf_charging_information_free(OpenAPI_nsacf_charging_information_t *nsacf_charging_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == nsacf_charging_information) {
        return;
    }
    ogs_free(nsacf_charging_information);
}

cJSON *OpenAPI_nsacf_charging_information_convertToJSON(OpenAPI_nsacf_charging_information_t *nsacf_charging_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (nsacf_charging_information == NULL) {
        ogs_error("OpenAPI_nsacf_charging_information_convertToJSON() failed [NSACFChargingInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (nsacf_charging_information->is_n_sac_charging_indicator) {
    if (cJSON_AddBoolToObject(item, "nSACChargingIndicator", nsacf_charging_information->n_sac_charging_indicator) == NULL) {
        ogs_error("OpenAPI_nsacf_charging_information_convertToJSON() failed [n_sac_charging_indicator]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_nsacf_charging_information_t *OpenAPI_nsacf_charging_information_parseFromJSON(cJSON *nsacf_charging_informationJSON)
{
    OpenAPI_nsacf_charging_information_t *nsacf_charging_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *n_sac_charging_indicator = NULL;
    n_sac_charging_indicator = cJSON_GetObjectItemCaseSensitive(nsacf_charging_informationJSON, "nSACChargingIndicator");
    if (n_sac_charging_indicator) {
    if (!cJSON_IsBool(n_sac_charging_indicator)) {
        ogs_error("OpenAPI_nsacf_charging_information_parseFromJSON() failed [n_sac_charging_indicator]");
        goto end;
    }
    }

    nsacf_charging_information_local_var = OpenAPI_nsacf_charging_information_create (
        n_sac_charging_indicator ? true : false,
        n_sac_charging_indicator ? n_sac_charging_indicator->valueint : 0
    );

    return nsacf_charging_information_local_var;
end:
    return NULL;
}

OpenAPI_nsacf_charging_information_t *OpenAPI_nsacf_charging_information_copy(OpenAPI_nsacf_charging_information_t *dst, OpenAPI_nsacf_charging_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nsacf_charging_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nsacf_charging_information_convertToJSON() failed");
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

    OpenAPI_nsacf_charging_information_free(dst);
    dst = OpenAPI_nsacf_charging_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

