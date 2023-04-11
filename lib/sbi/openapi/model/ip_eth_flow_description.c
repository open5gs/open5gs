
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ip_eth_flow_description.h"

OpenAPI_ip_eth_flow_description_t *OpenAPI_ip_eth_flow_description_create(
    char *ip_traffic_filter,
    OpenAPI_eth_flow_description_t *eth_traffic_filter
)
{
    OpenAPI_ip_eth_flow_description_t *ip_eth_flow_description_local_var = ogs_malloc(sizeof(OpenAPI_ip_eth_flow_description_t));
    ogs_assert(ip_eth_flow_description_local_var);

    ip_eth_flow_description_local_var->ip_traffic_filter = ip_traffic_filter;
    ip_eth_flow_description_local_var->eth_traffic_filter = eth_traffic_filter;

    return ip_eth_flow_description_local_var;
}

void OpenAPI_ip_eth_flow_description_free(OpenAPI_ip_eth_flow_description_t *ip_eth_flow_description)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ip_eth_flow_description) {
        return;
    }
    if (ip_eth_flow_description->ip_traffic_filter) {
        ogs_free(ip_eth_flow_description->ip_traffic_filter);
        ip_eth_flow_description->ip_traffic_filter = NULL;
    }
    if (ip_eth_flow_description->eth_traffic_filter) {
        OpenAPI_eth_flow_description_free(ip_eth_flow_description->eth_traffic_filter);
        ip_eth_flow_description->eth_traffic_filter = NULL;
    }
    ogs_free(ip_eth_flow_description);
}

cJSON *OpenAPI_ip_eth_flow_description_convertToJSON(OpenAPI_ip_eth_flow_description_t *ip_eth_flow_description)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ip_eth_flow_description == NULL) {
        ogs_error("OpenAPI_ip_eth_flow_description_convertToJSON() failed [IpEthFlowDescription]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ip_eth_flow_description->ip_traffic_filter) {
    if (cJSON_AddStringToObject(item, "ipTrafficFilter", ip_eth_flow_description->ip_traffic_filter) == NULL) {
        ogs_error("OpenAPI_ip_eth_flow_description_convertToJSON() failed [ip_traffic_filter]");
        goto end;
    }
    }

    if (ip_eth_flow_description->eth_traffic_filter) {
    cJSON *eth_traffic_filter_local_JSON = OpenAPI_eth_flow_description_convertToJSON(ip_eth_flow_description->eth_traffic_filter);
    if (eth_traffic_filter_local_JSON == NULL) {
        ogs_error("OpenAPI_ip_eth_flow_description_convertToJSON() failed [eth_traffic_filter]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ethTrafficFilter", eth_traffic_filter_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ip_eth_flow_description_convertToJSON() failed [eth_traffic_filter]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ip_eth_flow_description_t *OpenAPI_ip_eth_flow_description_parseFromJSON(cJSON *ip_eth_flow_descriptionJSON)
{
    OpenAPI_ip_eth_flow_description_t *ip_eth_flow_description_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ip_traffic_filter = NULL;
    cJSON *eth_traffic_filter = NULL;
    OpenAPI_eth_flow_description_t *eth_traffic_filter_local_nonprim = NULL;
    ip_traffic_filter = cJSON_GetObjectItemCaseSensitive(ip_eth_flow_descriptionJSON, "ipTrafficFilter");
    if (ip_traffic_filter) {
    if (!cJSON_IsString(ip_traffic_filter) && !cJSON_IsNull(ip_traffic_filter)) {
        ogs_error("OpenAPI_ip_eth_flow_description_parseFromJSON() failed [ip_traffic_filter]");
        goto end;
    }
    }

    eth_traffic_filter = cJSON_GetObjectItemCaseSensitive(ip_eth_flow_descriptionJSON, "ethTrafficFilter");
    if (eth_traffic_filter) {
    eth_traffic_filter_local_nonprim = OpenAPI_eth_flow_description_parseFromJSON(eth_traffic_filter);
    if (!eth_traffic_filter_local_nonprim) {
        ogs_error("OpenAPI_eth_flow_description_parseFromJSON failed [eth_traffic_filter]");
        goto end;
    }
    }

    ip_eth_flow_description_local_var = OpenAPI_ip_eth_flow_description_create (
        ip_traffic_filter && !cJSON_IsNull(ip_traffic_filter) ? ogs_strdup(ip_traffic_filter->valuestring) : NULL,
        eth_traffic_filter ? eth_traffic_filter_local_nonprim : NULL
    );

    return ip_eth_flow_description_local_var;
end:
    if (eth_traffic_filter_local_nonprim) {
        OpenAPI_eth_flow_description_free(eth_traffic_filter_local_nonprim);
        eth_traffic_filter_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_ip_eth_flow_description_t *OpenAPI_ip_eth_flow_description_copy(OpenAPI_ip_eth_flow_description_t *dst, OpenAPI_ip_eth_flow_description_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ip_eth_flow_description_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ip_eth_flow_description_convertToJSON() failed");
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

    OpenAPI_ip_eth_flow_description_free(dst);
    dst = OpenAPI_ip_eth_flow_description_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

