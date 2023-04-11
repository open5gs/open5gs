
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "top_application.h"

OpenAPI_top_application_t *OpenAPI_top_application_create(
    char *app_id,
    OpenAPI_flow_info_t *ip_traffic_filter,
    bool is_ratio,
    int ratio
)
{
    OpenAPI_top_application_t *top_application_local_var = ogs_malloc(sizeof(OpenAPI_top_application_t));
    ogs_assert(top_application_local_var);

    top_application_local_var->app_id = app_id;
    top_application_local_var->ip_traffic_filter = ip_traffic_filter;
    top_application_local_var->is_ratio = is_ratio;
    top_application_local_var->ratio = ratio;

    return top_application_local_var;
}

void OpenAPI_top_application_free(OpenAPI_top_application_t *top_application)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == top_application) {
        return;
    }
    if (top_application->app_id) {
        ogs_free(top_application->app_id);
        top_application->app_id = NULL;
    }
    if (top_application->ip_traffic_filter) {
        OpenAPI_flow_info_free(top_application->ip_traffic_filter);
        top_application->ip_traffic_filter = NULL;
    }
    ogs_free(top_application);
}

cJSON *OpenAPI_top_application_convertToJSON(OpenAPI_top_application_t *top_application)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (top_application == NULL) {
        ogs_error("OpenAPI_top_application_convertToJSON() failed [TopApplication]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (top_application->app_id) {
    if (cJSON_AddStringToObject(item, "appId", top_application->app_id) == NULL) {
        ogs_error("OpenAPI_top_application_convertToJSON() failed [app_id]");
        goto end;
    }
    }

    if (top_application->ip_traffic_filter) {
    cJSON *ip_traffic_filter_local_JSON = OpenAPI_flow_info_convertToJSON(top_application->ip_traffic_filter);
    if (ip_traffic_filter_local_JSON == NULL) {
        ogs_error("OpenAPI_top_application_convertToJSON() failed [ip_traffic_filter]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ipTrafficFilter", ip_traffic_filter_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_top_application_convertToJSON() failed [ip_traffic_filter]");
        goto end;
    }
    }

    if (top_application->is_ratio) {
    if (cJSON_AddNumberToObject(item, "ratio", top_application->ratio) == NULL) {
        ogs_error("OpenAPI_top_application_convertToJSON() failed [ratio]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_top_application_t *OpenAPI_top_application_parseFromJSON(cJSON *top_applicationJSON)
{
    OpenAPI_top_application_t *top_application_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *app_id = NULL;
    cJSON *ip_traffic_filter = NULL;
    OpenAPI_flow_info_t *ip_traffic_filter_local_nonprim = NULL;
    cJSON *ratio = NULL;
    app_id = cJSON_GetObjectItemCaseSensitive(top_applicationJSON, "appId");
    if (app_id) {
    if (!cJSON_IsString(app_id) && !cJSON_IsNull(app_id)) {
        ogs_error("OpenAPI_top_application_parseFromJSON() failed [app_id]");
        goto end;
    }
    }

    ip_traffic_filter = cJSON_GetObjectItemCaseSensitive(top_applicationJSON, "ipTrafficFilter");
    if (ip_traffic_filter) {
    ip_traffic_filter_local_nonprim = OpenAPI_flow_info_parseFromJSON(ip_traffic_filter);
    if (!ip_traffic_filter_local_nonprim) {
        ogs_error("OpenAPI_flow_info_parseFromJSON failed [ip_traffic_filter]");
        goto end;
    }
    }

    ratio = cJSON_GetObjectItemCaseSensitive(top_applicationJSON, "ratio");
    if (ratio) {
    if (!cJSON_IsNumber(ratio)) {
        ogs_error("OpenAPI_top_application_parseFromJSON() failed [ratio]");
        goto end;
    }
    }

    top_application_local_var = OpenAPI_top_application_create (
        app_id && !cJSON_IsNull(app_id) ? ogs_strdup(app_id->valuestring) : NULL,
        ip_traffic_filter ? ip_traffic_filter_local_nonprim : NULL,
        ratio ? true : false,
        ratio ? ratio->valuedouble : 0
    );

    return top_application_local_var;
end:
    if (ip_traffic_filter_local_nonprim) {
        OpenAPI_flow_info_free(ip_traffic_filter_local_nonprim);
        ip_traffic_filter_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_top_application_t *OpenAPI_top_application_copy(OpenAPI_top_application_t *dst, OpenAPI_top_application_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_top_application_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_top_application_convertToJSON() failed");
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

    OpenAPI_top_application_free(dst);
    dst = OpenAPI_top_application_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

