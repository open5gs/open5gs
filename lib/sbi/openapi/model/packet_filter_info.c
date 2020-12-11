
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
    OpenAPI_flow_direction_t *flow_direction
    )
{
    OpenAPI_packet_filter_info_t *packet_filter_info_local_var = OpenAPI_malloc(sizeof(OpenAPI_packet_filter_info_t));
    if (!packet_filter_info_local_var) {
        return NULL;
    }
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
    if (NULL == packet_filter_info) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(packet_filter_info->pack_filt_id);
    ogs_free(packet_filter_info->pack_filt_cont);
    ogs_free(packet_filter_info->tos_traffic_class);
    ogs_free(packet_filter_info->spi);
    ogs_free(packet_filter_info->flow_label);
    OpenAPI_flow_direction_free(packet_filter_info->flow_direction);
    ogs_free(packet_filter_info);
}

cJSON *OpenAPI_packet_filter_info_convertToJSON(OpenAPI_packet_filter_info_t *packet_filter_info)
{
    cJSON *item = NULL;

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

    if (packet_filter_info->flow_direction) {
        cJSON *flow_direction_local_JSON = OpenAPI_flow_direction_convertToJSON(packet_filter_info->flow_direction);
        if (flow_direction_local_JSON == NULL) {
            ogs_error("OpenAPI_packet_filter_info_convertToJSON() failed [flow_direction]");
            goto end;
        }
        cJSON_AddItemToObject(item, "flowDirection", flow_direction_local_JSON);
        if (item->child == NULL) {
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
    cJSON *pack_filt_id = cJSON_GetObjectItemCaseSensitive(packet_filter_infoJSON, "packFiltId");

    if (pack_filt_id) {
        if (!cJSON_IsString(pack_filt_id)) {
            ogs_error("OpenAPI_packet_filter_info_parseFromJSON() failed [pack_filt_id]");
            goto end;
        }
    }

    cJSON *pack_filt_cont = cJSON_GetObjectItemCaseSensitive(packet_filter_infoJSON, "packFiltCont");

    if (pack_filt_cont) {
        if (!cJSON_IsString(pack_filt_cont)) {
            ogs_error("OpenAPI_packet_filter_info_parseFromJSON() failed [pack_filt_cont]");
            goto end;
        }
    }

    cJSON *tos_traffic_class = cJSON_GetObjectItemCaseSensitive(packet_filter_infoJSON, "tosTrafficClass");

    if (tos_traffic_class) {
        if (!cJSON_IsString(tos_traffic_class)) {
            ogs_error("OpenAPI_packet_filter_info_parseFromJSON() failed [tos_traffic_class]");
            goto end;
        }
    }

    cJSON *spi = cJSON_GetObjectItemCaseSensitive(packet_filter_infoJSON, "spi");

    if (spi) {
        if (!cJSON_IsString(spi)) {
            ogs_error("OpenAPI_packet_filter_info_parseFromJSON() failed [spi]");
            goto end;
        }
    }

    cJSON *flow_label = cJSON_GetObjectItemCaseSensitive(packet_filter_infoJSON, "flowLabel");

    if (flow_label) {
        if (!cJSON_IsString(flow_label)) {
            ogs_error("OpenAPI_packet_filter_info_parseFromJSON() failed [flow_label]");
            goto end;
        }
    }

    cJSON *flow_direction = cJSON_GetObjectItemCaseSensitive(packet_filter_infoJSON, "flowDirection");

    OpenAPI_flow_direction_t *flow_direction_local_nonprim = NULL;
    if (flow_direction) {
        flow_direction_local_nonprim = OpenAPI_flow_direction_parseFromJSON(flow_direction);
    }

    packet_filter_info_local_var = OpenAPI_packet_filter_info_create (
        pack_filt_id ? ogs_strdup(pack_filt_id->valuestring) : NULL,
        pack_filt_cont ? ogs_strdup(pack_filt_cont->valuestring) : NULL,
        tos_traffic_class ? ogs_strdup(tos_traffic_class->valuestring) : NULL,
        spi ? ogs_strdup(spi->valuestring) : NULL,
        flow_label ? ogs_strdup(flow_label->valuestring) : NULL,
        flow_direction ? flow_direction_local_nonprim : NULL
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

