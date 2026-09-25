
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "trace_data_or_shared_trace_data_id.h"

OpenAPI_trace_data_or_shared_trace_data_id_t *OpenAPI_trace_data_or_shared_trace_data_id_create(
    bool is_trace_data_null,
    OpenAPI_trace_data_t *trace_data,
    char *shared_data_id
)
{
    OpenAPI_trace_data_or_shared_trace_data_id_t *trace_data_or_shared_trace_data_id_local_var = ogs_malloc(sizeof(OpenAPI_trace_data_or_shared_trace_data_id_t));
    ogs_assert(trace_data_or_shared_trace_data_id_local_var);

    trace_data_or_shared_trace_data_id_local_var->is_trace_data_null = is_trace_data_null;
    trace_data_or_shared_trace_data_id_local_var->trace_data = trace_data;
    trace_data_or_shared_trace_data_id_local_var->shared_data_id = shared_data_id;

    return trace_data_or_shared_trace_data_id_local_var;
}

void OpenAPI_trace_data_or_shared_trace_data_id_free(OpenAPI_trace_data_or_shared_trace_data_id_t *trace_data_or_shared_trace_data_id)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == trace_data_or_shared_trace_data_id) {
        return;
    }
    if (trace_data_or_shared_trace_data_id->trace_data) {
        OpenAPI_trace_data_free(trace_data_or_shared_trace_data_id->trace_data);
        trace_data_or_shared_trace_data_id->trace_data = NULL;
    }
    if (trace_data_or_shared_trace_data_id->shared_data_id) {
        ogs_free(trace_data_or_shared_trace_data_id->shared_data_id);
        trace_data_or_shared_trace_data_id->shared_data_id = NULL;
    }
    ogs_free(trace_data_or_shared_trace_data_id);
}

/*
 * oneOf union (x-open5gs-union): the members are the alternatives of the
 * original oneOf. Exactly one is set, and it is the JSON value itself
 * rather than a property of a wrapper object.
 */
cJSON *OpenAPI_trace_data_or_shared_trace_data_id_convertToJSON(OpenAPI_trace_data_or_shared_trace_data_id_t *trace_data_or_shared_trace_data_id)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (trace_data_or_shared_trace_data_id == NULL) {
        ogs_error("OpenAPI_trace_data_or_shared_trace_data_id_convertToJSON() failed [TraceDataOrSharedTraceDataId]");
        return NULL;
    }

    if ((trace_data_or_shared_trace_data_id->is_trace_data_null != false) + (trace_data_or_shared_trace_data_id->trace_data != NULL) +
            (trace_data_or_shared_trace_data_id->shared_data_id != NULL) != 1) {
        ogs_error("OpenAPI_trace_data_or_shared_trace_data_id_convertToJSON() failed [TraceDataOrSharedTraceDataId]: exactly one alternative must be set");
        return NULL;
    }

    if (trace_data_or_shared_trace_data_id->is_trace_data_null) {
        return cJSON_CreateNull();
    }
    if (trace_data_or_shared_trace_data_id->trace_data) {
        return OpenAPI_trace_data_convertToJSON(trace_data_or_shared_trace_data_id->trace_data);
    }
    if (trace_data_or_shared_trace_data_id->shared_data_id) {
        return cJSON_CreateString(trace_data_or_shared_trace_data_id->shared_data_id);
    }

    ogs_error("OpenAPI_trace_data_or_shared_trace_data_id_convertToJSON() failed [TraceDataOrSharedTraceDataId]");
    return NULL;
}

OpenAPI_trace_data_or_shared_trace_data_id_t *OpenAPI_trace_data_or_shared_trace_data_id_parseFromJSON(cJSON *trace_data_or_shared_trace_data_idJSON)
{
    OpenAPI_lnode_t *node = NULL;
    bool is_trace_data_null = false;
    OpenAPI_trace_data_t *trace_data = NULL;
    char *shared_data_id = NULL;

    if (cJSON_IsObject(trace_data_or_shared_trace_data_idJSON)) {
        trace_data = OpenAPI_trace_data_parseFromJSON(trace_data_or_shared_trace_data_idJSON);
        if (!trace_data) {
            ogs_error("OpenAPI_trace_data_parseFromJSON failed [trace_data]");
            goto end;
        }
    } else if (cJSON_IsString(trace_data_or_shared_trace_data_idJSON)) {
        shared_data_id = ogs_strdup(trace_data_or_shared_trace_data_idJSON->valuestring);
        ogs_assert(shared_data_id);
    } else if (cJSON_IsNull(trace_data_or_shared_trace_data_idJSON)) {
        is_trace_data_null = true;
    } else {
        ogs_error("OpenAPI_trace_data_or_shared_trace_data_id_parseFromJSON() failed [TraceDataOrSharedTraceDataId]");
        goto end;
    }

    return OpenAPI_trace_data_or_shared_trace_data_id_create(is_trace_data_null, trace_data, shared_data_id);
end:
    if (trace_data) {
        OpenAPI_trace_data_free(trace_data);
        trace_data = NULL;
    }
    if (shared_data_id) {
        ogs_free(shared_data_id);
        shared_data_id = NULL;
    }
    return NULL;
}

OpenAPI_trace_data_or_shared_trace_data_id_t *OpenAPI_trace_data_or_shared_trace_data_id_copy(OpenAPI_trace_data_or_shared_trace_data_id_t *dst, OpenAPI_trace_data_or_shared_trace_data_id_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_trace_data_or_shared_trace_data_id_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_trace_data_or_shared_trace_data_id_convertToJSON() failed");
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

    OpenAPI_trace_data_or_shared_trace_data_id_free(dst);
    dst = OpenAPI_trace_data_or_shared_trace_data_id_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

