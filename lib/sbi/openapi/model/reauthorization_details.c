
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "reauthorization_details.h"

OpenAPI_reauthorization_details_t *OpenAPI_reauthorization_details_create(
    bool is_service_id,
    int service_id,
    bool is_rating_group,
    int rating_group,
    OpenAPI_quota_management_indicator_e quota_management_indicator
)
{
    OpenAPI_reauthorization_details_t *reauthorization_details_local_var = ogs_malloc(sizeof(OpenAPI_reauthorization_details_t));
    ogs_assert(reauthorization_details_local_var);

    reauthorization_details_local_var->is_service_id = is_service_id;
    reauthorization_details_local_var->service_id = service_id;
    reauthorization_details_local_var->is_rating_group = is_rating_group;
    reauthorization_details_local_var->rating_group = rating_group;
    reauthorization_details_local_var->quota_management_indicator = quota_management_indicator;

    return reauthorization_details_local_var;
}

void OpenAPI_reauthorization_details_free(OpenAPI_reauthorization_details_t *reauthorization_details)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == reauthorization_details) {
        return;
    }
    ogs_free(reauthorization_details);
}

cJSON *OpenAPI_reauthorization_details_convertToJSON(OpenAPI_reauthorization_details_t *reauthorization_details)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (reauthorization_details == NULL) {
        ogs_error("OpenAPI_reauthorization_details_convertToJSON() failed [ReauthorizationDetails]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (reauthorization_details->is_service_id) {
    if (cJSON_AddNumberToObject(item, "serviceId", reauthorization_details->service_id) == NULL) {
        ogs_error("OpenAPI_reauthorization_details_convertToJSON() failed [service_id]");
        goto end;
    }
    }

    if (reauthorization_details->is_rating_group) {
    if (cJSON_AddNumberToObject(item, "ratingGroup", reauthorization_details->rating_group) == NULL) {
        ogs_error("OpenAPI_reauthorization_details_convertToJSON() failed [rating_group]");
        goto end;
    }
    }

    if (reauthorization_details->quota_management_indicator != OpenAPI_quota_management_indicator_NULL) {
    if (cJSON_AddStringToObject(item, "quotaManagementIndicator", OpenAPI_quota_management_indicator_ToString(reauthorization_details->quota_management_indicator)) == NULL) {
        ogs_error("OpenAPI_reauthorization_details_convertToJSON() failed [quota_management_indicator]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_reauthorization_details_t *OpenAPI_reauthorization_details_parseFromJSON(cJSON *reauthorization_detailsJSON)
{
    OpenAPI_reauthorization_details_t *reauthorization_details_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *service_id = NULL;
    cJSON *rating_group = NULL;
    cJSON *quota_management_indicator = NULL;
    OpenAPI_quota_management_indicator_e quota_management_indicatorVariable = 0;
    service_id = cJSON_GetObjectItemCaseSensitive(reauthorization_detailsJSON, "serviceId");
    if (service_id) {
    if (!cJSON_IsNumber(service_id)) {
        ogs_error("OpenAPI_reauthorization_details_parseFromJSON() failed [service_id]");
        goto end;
    }
    }

    rating_group = cJSON_GetObjectItemCaseSensitive(reauthorization_detailsJSON, "ratingGroup");
    if (rating_group) {
    if (!cJSON_IsNumber(rating_group)) {
        ogs_error("OpenAPI_reauthorization_details_parseFromJSON() failed [rating_group]");
        goto end;
    }
    }

    quota_management_indicator = cJSON_GetObjectItemCaseSensitive(reauthorization_detailsJSON, "quotaManagementIndicator");
    if (quota_management_indicator) {
    if (!cJSON_IsString(quota_management_indicator)) {
        ogs_error("OpenAPI_reauthorization_details_parseFromJSON() failed [quota_management_indicator]");
        goto end;
    }
    quota_management_indicatorVariable = OpenAPI_quota_management_indicator_FromString(quota_management_indicator->valuestring);
    }

    reauthorization_details_local_var = OpenAPI_reauthorization_details_create (
        service_id ? true : false,
        service_id ? service_id->valuedouble : 0,
        rating_group ? true : false,
        rating_group ? rating_group->valuedouble : 0,
        quota_management_indicator ? quota_management_indicatorVariable : 0
    );

    return reauthorization_details_local_var;
end:
    return NULL;
}

OpenAPI_reauthorization_details_t *OpenAPI_reauthorization_details_copy(OpenAPI_reauthorization_details_t *dst, OpenAPI_reauthorization_details_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_reauthorization_details_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_reauthorization_details_convertToJSON() failed");
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

    OpenAPI_reauthorization_details_free(dst);
    dst = OpenAPI_reauthorization_details_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

