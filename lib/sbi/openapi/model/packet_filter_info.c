
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "packet_filter_info.h"

OpenAPI_packet_filter_info_t *OpenAPI_packet_filter_info_create(
    char *pack_filt_id,
    char *pack_filt_cont,
    char *tos_traffic_class,
    char *spi,
    char *flow_label,
    OpenAPI_flow_direction_e flow_direction
)
{
    OpenAPI_packet_filter_info_t *packet_filter_info_local_var = ogs_malloc(sizeof(OpenAPI_packet_filter_info_t));
    ogs_assert(packet_filter_info_local_var);

    packet_filter_info_local_var->pack_filt_id = pack_filt_id;
    packet_filter_info_local_var->pack_filt_cont = pack_filt_cont;
    packet_filter_info_local_var->tos_traffic_class = tos_traffic_class;
    packet_filter_info_local_var->spi = spi;
    packet_filter_info_local_var->flow_label = flow_label;
    packet_filter_info_local_var->flow_direction = flow_direction;

    return packet_filter_info_local_var;
}

void OpenAPI_packet_filter_info_free(OpenAPI_packet_filter_info_t *packet_filter_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == packet_filter_info) {
        return;
    }
    if (packet_filter_info->pack_filt_id) {
        ogs_free(packet_filter_info->pack_filt_id);
        packet_filter_info->pack_filt_id = NULL;
    }
    if (packet_filter_info->pack_filt_cont) {
        ogs_free(packet_filter_info->pack_filt_cont);
        packet_filter_info->pack_filt_cont = NULL;
    }
    if (packet_filter_info->tos_traffic_class) {
        ogs_free(packet_filter_info->tos_traffic_class);
        packet_filter_info->tos_traffic_class = NULL;
    }
    if (packet_filter_info->spi) {
        ogs_free(packet_filter_info->spi);
        packet_filter_info->spi = NULL;
    }
    if (packet_filter_info->flow_label) {
        ogs_free(packet_filter_info->flow_label);
        packet_filter_info->flow_label = NULL;
    }
    ogs_free(packet_filter_info);
}

cJSON *OpenAPI_packet_filter_info_convertToJSON(OpenAPI_packet_filter_info_t *packet_filter_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (packet_filter_info == NULL) {
        ogs_error("OpenAPI_packet_filter_info_convertToJSON() failed [PacketFilterInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (packet_filter_info->pack_filt_id) {
    if (cJSON_AddStringToObject(item, "packFiltId", packet_filter_info->pack_filt_id) == NULL) {
        ogs_error("OpenAPI_packet_filter_info_convertToJSON() failed [pack_filt_id]");
        goto end;
    }
    }

    if (packet_filter_info->pack_filt_cont) {
    if (cJSON_AddStringToObject(item, "packFiltCont", packet_filter_info->pack_filt_cont) == NULL) {
        ogs_error("OpenAPI_packet_filter_info_convertToJSON() failed [pack_filt_cont]");
        goto end;
    }
    }

    if (packet_filter_info->tos_traffic_class) {
    if (cJSON_AddStringToObject(item, "tosTrafficClass", packet_filter_info->tos_traffic_class) == NULL) {
        ogs_error("OpenAPI_packet_filter_info_convertToJSON() failed [tos_traffic_class]");
        goto end;
    }
    }

    if (packet_filter_info->spi) {
    if (cJSON_AddStringToObject(item, "spi", packet_filter_info->spi) == NULL) {
        ogs_error("OpenAPI_packet_filter_info_convertToJSON() failed [spi]");
        goto end;
    }
    }

    if (packet_filter_info->flow_label) {
    if (cJSON_AddStringToObject(item, "flowLabel", packet_filter_info->flow_label) == NULL) {
        ogs_error("OpenAPI_packet_filter_info_convertToJSON() failed [flow_label]");
        goto end;
    }
    }

    if (packet_filter_info->flow_direction != OpenAPI_flow_direction_NULL) {
    if (cJSON_AddStringToObject(item, "flowDirection", OpenAPI_flow_direction_ToString(packet_filter_info->flow_direction)) == NULL) {
        ogs_error("OpenAPI_packet_filter_info_convertToJSON() failed [flow_direction]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_packet_filter_info_t *OpenAPI_packet_filter_info_parseFromJSON(cJSON *packet_filter_infoJSON)
{
    OpenAPI_packet_filter_info_t *packet_filter_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *pack_filt_id = NULL;
    cJSON *pack_filt_cont = NULL;
    cJSON *tos_traffic_class = NULL;
    cJSON *spi = NULL;
    cJSON *flow_label = NULL;
    cJSON *flow_direction = NULL;
    OpenAPI_flow_direction_e flow_directionVariable = 0;
    pack_filt_id = cJSON_GetObjectItemCaseSensitive(packet_filter_infoJSON, "packFiltId");
    if (pack_filt_id) {
    if (!cJSON_IsString(pack_filt_id) && !cJSON_IsNull(pack_filt_id)) {
        ogs_error("OpenAPI_packet_filter_info_parseFromJSON() failed [pack_filt_id]");
        goto end;
    }
    }

    pack_filt_cont = cJSON_GetObjectItemCaseSensitive(packet_filter_infoJSON, "packFiltCont");
    if (pack_filt_cont) {
    if (!cJSON_IsString(pack_filt_cont) && !cJSON_IsNull(pack_filt_cont)) {
        ogs_error("OpenAPI_packet_filter_info_parseFromJSON() failed [pack_filt_cont]");
        goto end;
    }
    }

    tos_traffic_class = cJSON_GetObjectItemCaseSensitive(packet_filter_infoJSON, "tosTrafficClass");
    if (tos_traffic_class) {
    if (!cJSON_IsString(tos_traffic_class) && !cJSON_IsNull(tos_traffic_class)) {
        ogs_error("OpenAPI_packet_filter_info_parseFromJSON() failed [tos_traffic_class]");
        goto end;
    }
    }

    spi = cJSON_GetObjectItemCaseSensitive(packet_filter_infoJSON, "spi");
    if (spi) {
    if (!cJSON_IsString(spi) && !cJSON_IsNull(spi)) {
        ogs_error("OpenAPI_packet_filter_info_parseFromJSON() failed [spi]");
        goto end;
    }
    }

    flow_label = cJSON_GetObjectItemCaseSensitive(packet_filter_infoJSON, "flowLabel");
    if (flow_label) {
    if (!cJSON_IsString(flow_label) && !cJSON_IsNull(flow_label)) {
        ogs_error("OpenAPI_packet_filter_info_parseFromJSON() failed [flow_label]");
        goto end;
    }
    }

    flow_direction = cJSON_GetObjectItemCaseSensitive(packet_filter_infoJSON, "flowDirection");
    if (flow_direction) {
    if (!cJSON_IsString(flow_direction)) {
        ogs_error("OpenAPI_packet_filter_info_parseFromJSON() failed [flow_direction]");
        goto end;
    }
    flow_directionVariable = OpenAPI_flow_direction_FromString(flow_direction->valuestring);
    }

    packet_filter_info_local_var = OpenAPI_packet_filter_info_create (
        pack_filt_id && !cJSON_IsNull(pack_filt_id) ? ogs_strdup(pack_filt_id->valuestring) : NULL,
        pack_filt_cont && !cJSON_IsNull(pack_filt_cont) ? ogs_strdup(pack_filt_cont->valuestring) : NULL,
        tos_traffic_class && !cJSON_IsNull(tos_traffic_class) ? ogs_strdup(tos_traffic_class->valuestring) : NULL,
        spi && !cJSON_IsNull(spi) ? ogs_strdup(spi->valuestring) : NULL,
        flow_label && !cJSON_IsNull(flow_label) ? ogs_strdup(flow_label->valuestring) : NULL,
        flow_direction ? flow_directionVariable : 0
    );

    return packet_filter_info_local_var;
end:
    return NULL;
}

OpenAPI_packet_filter_info_t *OpenAPI_packet_filter_info_copy(OpenAPI_packet_filter_info_t *dst, OpenAPI_packet_filter_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_packet_filter_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_packet_filter_info_convertToJSON() failed");
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

    OpenAPI_packet_filter_info_free(dst);
    dst = OpenAPI_packet_filter_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

