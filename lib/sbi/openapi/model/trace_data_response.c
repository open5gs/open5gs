
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "trace_data_response.h"

OpenAPI_trace_data_response_t *OpenAPI_trace_data_response_create(
    bool is_trace_data_null,
    OpenAPI_trace_data_t *trace_data,
    char *shared_trace_data_id
)
{
    OpenAPI_trace_data_response_t *trace_data_response_local_var = ogs_malloc(sizeof(OpenAPI_trace_data_response_t));
    ogs_assert(trace_data_response_local_var);

    trace_data_response_local_var->is_trace_data_null = is_trace_data_null;
    trace_data_response_local_var->trace_data = trace_data;
    trace_data_response_local_var->shared_trace_data_id = shared_trace_data_id;

    return trace_data_response_local_var;
}

void OpenAPI_trace_data_response_free(OpenAPI_trace_data_response_t *trace_data_response)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == trace_data_response) {
        return;
    }
    if (trace_data_response->trace_data) {
        OpenAPI_trace_data_free(trace_data_response->trace_data);
        trace_data_response->trace_data = NULL;
    }
    if (trace_data_response->shared_trace_data_id) {
        ogs_free(trace_data_response->shared_trace_data_id);
        trace_data_response->shared_trace_data_id = NULL;
    }
    ogs_free(trace_data_response);
}

cJSON *OpenAPI_trace_data_response_convertToJSON(OpenAPI_trace_data_response_t *trace_data_response)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (trace_data_response == NULL) {
        ogs_error("OpenAPI_trace_data_response_convertToJSON() failed [TraceDataResponse]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (trace_data_response->trace_data) {
    cJSON *trace_data_local_JSON = OpenAPI_trace_data_convertToJSON(trace_data_response->trace_data);
    if (trace_data_local_JSON == NULL) {
        ogs_error("OpenAPI_trace_data_response_convertToJSON() failed [trace_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "traceData", trace_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_trace_data_response_convertToJSON() failed [trace_data]");
        goto end;
    }
    } else if (trace_data_response->is_trace_data_null) {
        if (cJSON_AddNullToObject(item, "traceData") == NULL) {
            ogs_error("OpenAPI_trace_data_response_convertToJSON() failed [trace_data]");
            goto end;
        }
    }

    if (trace_data_response->shared_trace_data_id) {
    if (cJSON_AddStringToObject(item, "sharedTraceDataId", trace_data_response->shared_trace_data_id) == NULL) {
        ogs_error("OpenAPI_trace_data_response_convertToJSON() failed [shared_trace_data_id]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_trace_data_response_t *OpenAPI_trace_data_response_parseFromJSON(cJSON *trace_data_responseJSON)
{
    OpenAPI_trace_data_response_t *trace_data_response_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *trace_data = NULL;
    OpenAPI_trace_data_t *trace_data_local_nonprim = NULL;
    cJSON *shared_trace_data_id = NULL;
    trace_data = cJSON_GetObjectItemCaseSensitive(trace_data_responseJSON, "traceData");
    if (trace_data) {
    if (!cJSON_IsNull(trace_data)) {
    trace_data_local_nonprim = OpenAPI_trace_data_parseFromJSON(trace_data);
    if (!trace_data_local_nonprim) {
        ogs_error("OpenAPI_trace_data_parseFromJSON failed [trace_data]");
        goto end;
    }
    }
    }

    shared_trace_data_id = cJSON_GetObjectItemCaseSensitive(trace_data_responseJSON, "sharedTraceDataId");
    if (shared_trace_data_id) {
    if (!cJSON_IsString(shared_trace_data_id) && !cJSON_IsNull(shared_trace_data_id)) {
        ogs_error("OpenAPI_trace_data_response_parseFromJSON() failed [shared_trace_data_id]");
        goto end;
    }
    }

    trace_data_response_local_var = OpenAPI_trace_data_response_create (
        trace_data && cJSON_IsNull(trace_data) ? true : false,
        trace_data ? trace_data_local_nonprim : NULL,
        shared_trace_data_id && !cJSON_IsNull(shared_trace_data_id) ? ogs_strdup(shared_trace_data_id->valuestring) : NULL
    );

    return trace_data_response_local_var;
end:
    if (trace_data_local_nonprim) {
        OpenAPI_trace_data_free(trace_data_local_nonprim);
        trace_data_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_trace_data_response_t *OpenAPI_trace_data_response_copy(OpenAPI_trace_data_response_t *dst, OpenAPI_trace_data_response_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_trace_data_response_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_trace_data_response_convertToJSON() failed");
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

    OpenAPI_trace_data_response_free(dst);
    dst = OpenAPI_trace_data_response_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

