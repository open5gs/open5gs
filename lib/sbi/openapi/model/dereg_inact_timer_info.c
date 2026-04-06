
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dereg_inact_timer_info.h"

OpenAPI_dereg_inact_timer_info_t *OpenAPI_dereg_inact_timer_info_create(
    OpenAPI_nssaa_status_t *dereg_inact_expiry_time
)
{
    OpenAPI_dereg_inact_timer_info_t *dereg_inact_timer_info_local_var = ogs_malloc(sizeof(OpenAPI_dereg_inact_timer_info_t));
    ogs_assert(dereg_inact_timer_info_local_var);

    dereg_inact_timer_info_local_var->dereg_inact_expiry_time = dereg_inact_expiry_time;

    return dereg_inact_timer_info_local_var;
}

void OpenAPI_dereg_inact_timer_info_free(OpenAPI_dereg_inact_timer_info_t *dereg_inact_timer_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == dereg_inact_timer_info) {
        return;
    }
    if (dereg_inact_timer_info->dereg_inact_expiry_time) {
        OpenAPI_nssaa_status_free(dereg_inact_timer_info->dereg_inact_expiry_time);
        dereg_inact_timer_info->dereg_inact_expiry_time = NULL;
    }
    ogs_free(dereg_inact_timer_info);
}

cJSON *OpenAPI_dereg_inact_timer_info_convertToJSON(OpenAPI_dereg_inact_timer_info_t *dereg_inact_timer_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (dereg_inact_timer_info == NULL) {
        ogs_error("OpenAPI_dereg_inact_timer_info_convertToJSON() failed [DeregInactTimerInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!dereg_inact_timer_info->dereg_inact_expiry_time) {
        ogs_error("OpenAPI_dereg_inact_timer_info_convertToJSON() failed [dereg_inact_expiry_time]");
        return NULL;
    }
    cJSON *dereg_inact_expiry_time_local_JSON = OpenAPI_nssaa_status_convertToJSON(dereg_inact_timer_info->dereg_inact_expiry_time);
    if (dereg_inact_expiry_time_local_JSON == NULL) {
        ogs_error("OpenAPI_dereg_inact_timer_info_convertToJSON() failed [dereg_inact_expiry_time]");
        goto end;
    }
    cJSON_AddItemToObject(item, "deregInactExpiryTime", dereg_inact_expiry_time_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_dereg_inact_timer_info_convertToJSON() failed [dereg_inact_expiry_time]");
        goto end;
    }

end:
    return item;
}

OpenAPI_dereg_inact_timer_info_t *OpenAPI_dereg_inact_timer_info_parseFromJSON(cJSON *dereg_inact_timer_infoJSON)
{
    OpenAPI_dereg_inact_timer_info_t *dereg_inact_timer_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *dereg_inact_expiry_time = NULL;
    OpenAPI_nssaa_status_t *dereg_inact_expiry_time_local_nonprim = NULL;
    dereg_inact_expiry_time = cJSON_GetObjectItemCaseSensitive(dereg_inact_timer_infoJSON, "deregInactExpiryTime");
    if (!dereg_inact_expiry_time) {
        ogs_error("OpenAPI_dereg_inact_timer_info_parseFromJSON() failed [dereg_inact_expiry_time]");
        goto end;
    }
    dereg_inact_expiry_time_local_nonprim = OpenAPI_nssaa_status_parseFromJSON(dereg_inact_expiry_time);
    if (!dereg_inact_expiry_time_local_nonprim) {
        ogs_error("OpenAPI_nssaa_status_parseFromJSON failed [dereg_inact_expiry_time]");
        goto end;
    }

    dereg_inact_timer_info_local_var = OpenAPI_dereg_inact_timer_info_create (
        dereg_inact_expiry_time_local_nonprim
    );

    return dereg_inact_timer_info_local_var;
end:
    if (dereg_inact_expiry_time_local_nonprim) {
        OpenAPI_nssaa_status_free(dereg_inact_expiry_time_local_nonprim);
        dereg_inact_expiry_time_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_dereg_inact_timer_info_t *OpenAPI_dereg_inact_timer_info_copy(OpenAPI_dereg_inact_timer_info_t *dst, OpenAPI_dereg_inact_timer_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_dereg_inact_timer_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_dereg_inact_timer_info_convertToJSON() failed");
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

    OpenAPI_dereg_inact_timer_info_free(dst);
    dst = OpenAPI_dereg_inact_timer_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

