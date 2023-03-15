
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "_application_data_influence_data_subs_to_notify_post_request_inner.h"

OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_t *OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_create(
    char *res_uri,
    OpenAPI_traffic_influ_data_t *traffic_influ_data
)
{
    OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_t *_application_data_influence_data_subs_to_notify_post_request_inner_local_var = ogs_malloc(sizeof(OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_t));
    ogs_assert(_application_data_influence_data_subs_to_notify_post_request_inner_local_var);

    _application_data_influence_data_subs_to_notify_post_request_inner_local_var->res_uri = res_uri;
    _application_data_influence_data_subs_to_notify_post_request_inner_local_var->traffic_influ_data = traffic_influ_data;

    return _application_data_influence_data_subs_to_notify_post_request_inner_local_var;
}

void OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_free(OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_t *_application_data_influence_data_subs_to_notify_post_request_inner)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == _application_data_influence_data_subs_to_notify_post_request_inner) {
        return;
    }
    if (_application_data_influence_data_subs_to_notify_post_request_inner->res_uri) {
        ogs_free(_application_data_influence_data_subs_to_notify_post_request_inner->res_uri);
        _application_data_influence_data_subs_to_notify_post_request_inner->res_uri = NULL;
    }
    if (_application_data_influence_data_subs_to_notify_post_request_inner->traffic_influ_data) {
        OpenAPI_traffic_influ_data_free(_application_data_influence_data_subs_to_notify_post_request_inner->traffic_influ_data);
        _application_data_influence_data_subs_to_notify_post_request_inner->traffic_influ_data = NULL;
    }
    ogs_free(_application_data_influence_data_subs_to_notify_post_request_inner);
}

cJSON *OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON(OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_t *_application_data_influence_data_subs_to_notify_post_request_inner)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (_application_data_influence_data_subs_to_notify_post_request_inner == NULL) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [_application_data_influenceData_subs_to_notify_post_request_inner]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!_application_data_influence_data_subs_to_notify_post_request_inner->res_uri) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [res_uri]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "resUri", _application_data_influence_data_subs_to_notify_post_request_inner->res_uri) == NULL) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [res_uri]");
        goto end;
    }

    if (_application_data_influence_data_subs_to_notify_post_request_inner->traffic_influ_data) {
    cJSON *traffic_influ_data_local_JSON = OpenAPI_traffic_influ_data_convertToJSON(_application_data_influence_data_subs_to_notify_post_request_inner->traffic_influ_data);
    if (traffic_influ_data_local_JSON == NULL) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [traffic_influ_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "trafficInfluData", traffic_influ_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [traffic_influ_data]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_t *OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_parseFromJSON(cJSON *_application_data_influence_data_subs_to_notify_post_request_innerJSON)
{
    OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_t *_application_data_influence_data_subs_to_notify_post_request_inner_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *res_uri = NULL;
    cJSON *traffic_influ_data = NULL;
    OpenAPI_traffic_influ_data_t *traffic_influ_data_local_nonprim = NULL;
    res_uri = cJSON_GetObjectItemCaseSensitive(_application_data_influence_data_subs_to_notify_post_request_innerJSON, "resUri");
    if (!res_uri) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_parseFromJSON() failed [res_uri]");
        goto end;
    }
    if (!cJSON_IsString(res_uri)) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_parseFromJSON() failed [res_uri]");
        goto end;
    }

    traffic_influ_data = cJSON_GetObjectItemCaseSensitive(_application_data_influence_data_subs_to_notify_post_request_innerJSON, "trafficInfluData");
    if (traffic_influ_data) {
    traffic_influ_data_local_nonprim = OpenAPI_traffic_influ_data_parseFromJSON(traffic_influ_data);
    if (!traffic_influ_data_local_nonprim) {
        ogs_error("OpenAPI_traffic_influ_data_parseFromJSON failed [traffic_influ_data]");
        goto end;
    }
    }

    _application_data_influence_data_subs_to_notify_post_request_inner_local_var = OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_create (
        ogs_strdup(res_uri->valuestring),
        traffic_influ_data ? traffic_influ_data_local_nonprim : NULL
    );

    return _application_data_influence_data_subs_to_notify_post_request_inner_local_var;
end:
    if (traffic_influ_data_local_nonprim) {
        OpenAPI_traffic_influ_data_free(traffic_influ_data_local_nonprim);
        traffic_influ_data_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_t *OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_copy(OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_t *dst, OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed");
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

    OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_free(dst);
    dst = OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

