
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "trace_data_1.h"

OpenAPI_trace_data_1_t *OpenAPI_trace_data_1_create(
    char *trace_ref,
    OpenAPI_trace_depth_e trace_depth,
    char *ne_type_list,
    char *event_list,
    char *collection_entity_ipv4_addr,
    char *collection_entity_ipv6_addr,
    char *interface_list
)
{
    OpenAPI_trace_data_1_t *trace_data_1_local_var = ogs_malloc(sizeof(OpenAPI_trace_data_1_t));
    ogs_assert(trace_data_1_local_var);

    trace_data_1_local_var->trace_ref = trace_ref;
    trace_data_1_local_var->trace_depth = trace_depth;
    trace_data_1_local_var->ne_type_list = ne_type_list;
    trace_data_1_local_var->event_list = event_list;
    trace_data_1_local_var->collection_entity_ipv4_addr = collection_entity_ipv4_addr;
    trace_data_1_local_var->collection_entity_ipv6_addr = collection_entity_ipv6_addr;
    trace_data_1_local_var->interface_list = interface_list;

    return trace_data_1_local_var;
}

void OpenAPI_trace_data_1_free(OpenAPI_trace_data_1_t *trace_data_1)
{
    if (NULL == trace_data_1) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(trace_data_1->trace_ref);
    ogs_free(trace_data_1->ne_type_list);
    ogs_free(trace_data_1->event_list);
    ogs_free(trace_data_1->collection_entity_ipv4_addr);
    ogs_free(trace_data_1->collection_entity_ipv6_addr);
    ogs_free(trace_data_1->interface_list);
    ogs_free(trace_data_1);
}

cJSON *OpenAPI_trace_data_1_convertToJSON(OpenAPI_trace_data_1_t *trace_data_1)
{
    cJSON *item = NULL;

    if (trace_data_1 == NULL) {
        ogs_error("OpenAPI_trace_data_1_convertToJSON() failed [TraceData_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddStringToObject(item, "traceRef", trace_data_1->trace_ref) == NULL) {
        ogs_error("OpenAPI_trace_data_1_convertToJSON() failed [trace_ref]");
        goto end;
    }

    if (cJSON_AddStringToObject(item, "traceDepth", OpenAPI_trace_depth_ToString(trace_data_1->trace_depth)) == NULL) {
        ogs_error("OpenAPI_trace_data_1_convertToJSON() failed [trace_depth]");
        goto end;
    }

    if (cJSON_AddStringToObject(item, "neTypeList", trace_data_1->ne_type_list) == NULL) {
        ogs_error("OpenAPI_trace_data_1_convertToJSON() failed [ne_type_list]");
        goto end;
    }

    if (cJSON_AddStringToObject(item, "eventList", trace_data_1->event_list) == NULL) {
        ogs_error("OpenAPI_trace_data_1_convertToJSON() failed [event_list]");
        goto end;
    }

    if (trace_data_1->collection_entity_ipv4_addr) {
    if (cJSON_AddStringToObject(item, "collectionEntityIpv4Addr", trace_data_1->collection_entity_ipv4_addr) == NULL) {
        ogs_error("OpenAPI_trace_data_1_convertToJSON() failed [collection_entity_ipv4_addr]");
        goto end;
    }
    }

    if (trace_data_1->collection_entity_ipv6_addr) {
    if (cJSON_AddStringToObject(item, "collectionEntityIpv6Addr", trace_data_1->collection_entity_ipv6_addr) == NULL) {
        ogs_error("OpenAPI_trace_data_1_convertToJSON() failed [collection_entity_ipv6_addr]");
        goto end;
    }
    }

    if (trace_data_1->interface_list) {
    if (cJSON_AddStringToObject(item, "interfaceList", trace_data_1->interface_list) == NULL) {
        ogs_error("OpenAPI_trace_data_1_convertToJSON() failed [interface_list]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_trace_data_1_t *OpenAPI_trace_data_1_parseFromJSON(cJSON *trace_data_1JSON)
{
    OpenAPI_trace_data_1_t *trace_data_1_local_var = NULL;
    cJSON *trace_ref = cJSON_GetObjectItemCaseSensitive(trace_data_1JSON, "traceRef");
    if (!trace_ref) {
        ogs_error("OpenAPI_trace_data_1_parseFromJSON() failed [trace_ref]");
        goto end;
    }

    if (!cJSON_IsString(trace_ref)) {
        ogs_error("OpenAPI_trace_data_1_parseFromJSON() failed [trace_ref]");
        goto end;
    }

    cJSON *trace_depth = cJSON_GetObjectItemCaseSensitive(trace_data_1JSON, "traceDepth");
    if (!trace_depth) {
        ogs_error("OpenAPI_trace_data_1_parseFromJSON() failed [trace_depth]");
        goto end;
    }

    OpenAPI_trace_depth_e trace_depthVariable;
    if (!cJSON_IsString(trace_depth)) {
        ogs_error("OpenAPI_trace_data_1_parseFromJSON() failed [trace_depth]");
        goto end;
    }
    trace_depthVariable = OpenAPI_trace_depth_FromString(trace_depth->valuestring);

    cJSON *ne_type_list = cJSON_GetObjectItemCaseSensitive(trace_data_1JSON, "neTypeList");
    if (!ne_type_list) {
        ogs_error("OpenAPI_trace_data_1_parseFromJSON() failed [ne_type_list]");
        goto end;
    }

    if (!cJSON_IsString(ne_type_list)) {
        ogs_error("OpenAPI_trace_data_1_parseFromJSON() failed [ne_type_list]");
        goto end;
    }

    cJSON *event_list = cJSON_GetObjectItemCaseSensitive(trace_data_1JSON, "eventList");
    if (!event_list) {
        ogs_error("OpenAPI_trace_data_1_parseFromJSON() failed [event_list]");
        goto end;
    }

    if (!cJSON_IsString(event_list)) {
        ogs_error("OpenAPI_trace_data_1_parseFromJSON() failed [event_list]");
        goto end;
    }

    cJSON *collection_entity_ipv4_addr = cJSON_GetObjectItemCaseSensitive(trace_data_1JSON, "collectionEntityIpv4Addr");

    if (collection_entity_ipv4_addr) {
    if (!cJSON_IsString(collection_entity_ipv4_addr)) {
        ogs_error("OpenAPI_trace_data_1_parseFromJSON() failed [collection_entity_ipv4_addr]");
        goto end;
    }
    }

    cJSON *collection_entity_ipv6_addr = cJSON_GetObjectItemCaseSensitive(trace_data_1JSON, "collectionEntityIpv6Addr");

    if (collection_entity_ipv6_addr) {
    if (!cJSON_IsString(collection_entity_ipv6_addr)) {
        ogs_error("OpenAPI_trace_data_1_parseFromJSON() failed [collection_entity_ipv6_addr]");
        goto end;
    }
    }

    cJSON *interface_list = cJSON_GetObjectItemCaseSensitive(trace_data_1JSON, "interfaceList");

    if (interface_list) {
    if (!cJSON_IsString(interface_list)) {
        ogs_error("OpenAPI_trace_data_1_parseFromJSON() failed [interface_list]");
        goto end;
    }
    }

    trace_data_1_local_var = OpenAPI_trace_data_1_create (
        ogs_strdup(trace_ref->valuestring),
        trace_depthVariable,
        ogs_strdup(ne_type_list->valuestring),
        ogs_strdup(event_list->valuestring),
        collection_entity_ipv4_addr ? ogs_strdup(collection_entity_ipv4_addr->valuestring) : NULL,
        collection_entity_ipv6_addr ? ogs_strdup(collection_entity_ipv6_addr->valuestring) : NULL,
        interface_list ? ogs_strdup(interface_list->valuestring) : NULL
    );

    return trace_data_1_local_var;
end:
    return NULL;
}

OpenAPI_trace_data_1_t *OpenAPI_trace_data_1_copy(OpenAPI_trace_data_1_t *dst, OpenAPI_trace_data_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_trace_data_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_trace_data_1_convertToJSON() failed");
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

    OpenAPI_trace_data_1_free(dst);
    dst = OpenAPI_trace_data_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

