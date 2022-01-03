
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "flow_information.h"

OpenAPI_flow_information_t *OpenAPI_flow_information_create(
    char *flow_description,
    OpenAPI_eth_flow_description_t *eth_flow_description,
    char *pack_filt_id,
    bool is_packet_filter_usage,
    int packet_filter_usage,
    char *tos_traffic_class,
    char *spi,
    char *flow_label,
    OpenAPI_flow_direction_e flow_direction
)
{
    OpenAPI_flow_information_t *flow_information_local_var = ogs_malloc(sizeof(OpenAPI_flow_information_t));
    ogs_assert(flow_information_local_var);

    flow_information_local_var->flow_description = flow_description;
    flow_information_local_var->eth_flow_description = eth_flow_description;
    flow_information_local_var->pack_filt_id = pack_filt_id;
    flow_information_local_var->is_packet_filter_usage = is_packet_filter_usage;
    flow_information_local_var->packet_filter_usage = packet_filter_usage;
    flow_information_local_var->tos_traffic_class = tos_traffic_class;
    flow_information_local_var->spi = spi;
    flow_information_local_var->flow_label = flow_label;
    flow_information_local_var->flow_direction = flow_direction;

    return flow_information_local_var;
}

void OpenAPI_flow_information_free(OpenAPI_flow_information_t *flow_information)
{
    if (NULL == flow_information) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(flow_information->flow_description);
    OpenAPI_eth_flow_description_free(flow_information->eth_flow_description);
    ogs_free(flow_information->pack_filt_id);
    ogs_free(flow_information->tos_traffic_class);
    ogs_free(flow_information->spi);
    ogs_free(flow_information->flow_label);
    ogs_free(flow_information);
}

cJSON *OpenAPI_flow_information_convertToJSON(OpenAPI_flow_information_t *flow_information)
{
    cJSON *item = NULL;

    if (flow_information == NULL) {
        ogs_error("OpenAPI_flow_information_convertToJSON() failed [FlowInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (flow_information->flow_description) {
    if (cJSON_AddStringToObject(item, "flowDescription", flow_information->flow_description) == NULL) {
        ogs_error("OpenAPI_flow_information_convertToJSON() failed [flow_description]");
        goto end;
    }
    }

    if (flow_information->eth_flow_description) {
    cJSON *eth_flow_description_local_JSON = OpenAPI_eth_flow_description_convertToJSON(flow_information->eth_flow_description);
    if (eth_flow_description_local_JSON == NULL) {
        ogs_error("OpenAPI_flow_information_convertToJSON() failed [eth_flow_description]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ethFlowDescription", eth_flow_description_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_flow_information_convertToJSON() failed [eth_flow_description]");
        goto end;
    }
    }

    if (flow_information->pack_filt_id) {
    if (cJSON_AddStringToObject(item, "packFiltId", flow_information->pack_filt_id) == NULL) {
        ogs_error("OpenAPI_flow_information_convertToJSON() failed [pack_filt_id]");
        goto end;
    }
    }

    if (flow_information->is_packet_filter_usage) {
    if (cJSON_AddBoolToObject(item, "packetFilterUsage", flow_information->packet_filter_usage) == NULL) {
        ogs_error("OpenAPI_flow_information_convertToJSON() failed [packet_filter_usage]");
        goto end;
    }
    }

    if (flow_information->tos_traffic_class) {
    if (cJSON_AddStringToObject(item, "tosTrafficClass", flow_information->tos_traffic_class) == NULL) {
        ogs_error("OpenAPI_flow_information_convertToJSON() failed [tos_traffic_class]");
        goto end;
    }
    }

    if (flow_information->spi) {
    if (cJSON_AddStringToObject(item, "spi", flow_information->spi) == NULL) {
        ogs_error("OpenAPI_flow_information_convertToJSON() failed [spi]");
        goto end;
    }
    }

    if (flow_information->flow_label) {
    if (cJSON_AddStringToObject(item, "flowLabel", flow_information->flow_label) == NULL) {
        ogs_error("OpenAPI_flow_information_convertToJSON() failed [flow_label]");
        goto end;
    }
    }

    if (flow_information->flow_direction) {
    if (cJSON_AddStringToObject(item, "flowDirection", OpenAPI_flow_direction_ToString(flow_information->flow_direction)) == NULL) {
        ogs_error("OpenAPI_flow_information_convertToJSON() failed [flow_direction]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_flow_information_t *OpenAPI_flow_information_parseFromJSON(cJSON *flow_informationJSON)
{
    OpenAPI_flow_information_t *flow_information_local_var = NULL;
    cJSON *flow_description = cJSON_GetObjectItemCaseSensitive(flow_informationJSON, "flowDescription");

    if (flow_description) {
    if (!cJSON_IsString(flow_description)) {
        ogs_error("OpenAPI_flow_information_parseFromJSON() failed [flow_description]");
        goto end;
    }
    }

    cJSON *eth_flow_description = cJSON_GetObjectItemCaseSensitive(flow_informationJSON, "ethFlowDescription");

    OpenAPI_eth_flow_description_t *eth_flow_description_local_nonprim = NULL;
    if (eth_flow_description) {
    eth_flow_description_local_nonprim = OpenAPI_eth_flow_description_parseFromJSON(eth_flow_description);
    }

    cJSON *pack_filt_id = cJSON_GetObjectItemCaseSensitive(flow_informationJSON, "packFiltId");

    if (pack_filt_id) {
    if (!cJSON_IsString(pack_filt_id)) {
        ogs_error("OpenAPI_flow_information_parseFromJSON() failed [pack_filt_id]");
        goto end;
    }
    }

    cJSON *packet_filter_usage = cJSON_GetObjectItemCaseSensitive(flow_informationJSON, "packetFilterUsage");

    if (packet_filter_usage) {
    if (!cJSON_IsBool(packet_filter_usage)) {
        ogs_error("OpenAPI_flow_information_parseFromJSON() failed [packet_filter_usage]");
        goto end;
    }
    }

    cJSON *tos_traffic_class = cJSON_GetObjectItemCaseSensitive(flow_informationJSON, "tosTrafficClass");

    if (tos_traffic_class) {
    if (!cJSON_IsString(tos_traffic_class)) {
        ogs_error("OpenAPI_flow_information_parseFromJSON() failed [tos_traffic_class]");
        goto end;
    }
    }

    cJSON *spi = cJSON_GetObjectItemCaseSensitive(flow_informationJSON, "spi");

    if (spi) {
    if (!cJSON_IsString(spi)) {
        ogs_error("OpenAPI_flow_information_parseFromJSON() failed [spi]");
        goto end;
    }
    }

    cJSON *flow_label = cJSON_GetObjectItemCaseSensitive(flow_informationJSON, "flowLabel");

    if (flow_label) {
    if (!cJSON_IsString(flow_label)) {
        ogs_error("OpenAPI_flow_information_parseFromJSON() failed [flow_label]");
        goto end;
    }
    }

    cJSON *flow_direction = cJSON_GetObjectItemCaseSensitive(flow_informationJSON, "flowDirection");

    OpenAPI_flow_direction_e flow_directionVariable;
    if (flow_direction) {
    if (!cJSON_IsString(flow_direction)) {
        ogs_error("OpenAPI_flow_information_parseFromJSON() failed [flow_direction]");
        goto end;
    }
    flow_directionVariable = OpenAPI_flow_direction_FromString(flow_direction->valuestring);
    }

    flow_information_local_var = OpenAPI_flow_information_create (
        flow_description ? ogs_strdup(flow_description->valuestring) : NULL,
        eth_flow_description ? eth_flow_description_local_nonprim : NULL,
        pack_filt_id ? ogs_strdup(pack_filt_id->valuestring) : NULL,
        packet_filter_usage ? true : false,
        packet_filter_usage ? packet_filter_usage->valueint : 0,
        tos_traffic_class ? ogs_strdup(tos_traffic_class->valuestring) : NULL,
        spi ? ogs_strdup(spi->valuestring) : NULL,
        flow_label ? ogs_strdup(flow_label->valuestring) : NULL,
        flow_direction ? flow_directionVariable : 0
    );

    return flow_information_local_var;
end:
    return NULL;
}

OpenAPI_flow_information_t *OpenAPI_flow_information_copy(OpenAPI_flow_information_t *dst, OpenAPI_flow_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_flow_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_flow_information_convertToJSON() failed");
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

    OpenAPI_flow_information_free(dst);
    dst = OpenAPI_flow_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

