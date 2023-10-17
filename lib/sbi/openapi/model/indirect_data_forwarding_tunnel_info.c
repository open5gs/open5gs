
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "indirect_data_forwarding_tunnel_info.h"

OpenAPI_indirect_data_forwarding_tunnel_info_t *OpenAPI_indirect_data_forwarding_tunnel_info_create(
    char *ipv4_addr,
    char *ipv6_addr,
    char *gtp_teid,
    bool is_drb_id,
    int drb_id,
    bool is_additional_tnl_nb,
    int additional_tnl_nb
)
{
    OpenAPI_indirect_data_forwarding_tunnel_info_t *indirect_data_forwarding_tunnel_info_local_var = ogs_malloc(sizeof(OpenAPI_indirect_data_forwarding_tunnel_info_t));
    ogs_assert(indirect_data_forwarding_tunnel_info_local_var);

    indirect_data_forwarding_tunnel_info_local_var->ipv4_addr = ipv4_addr;
    indirect_data_forwarding_tunnel_info_local_var->ipv6_addr = ipv6_addr;
    indirect_data_forwarding_tunnel_info_local_var->gtp_teid = gtp_teid;
    indirect_data_forwarding_tunnel_info_local_var->is_drb_id = is_drb_id;
    indirect_data_forwarding_tunnel_info_local_var->drb_id = drb_id;
    indirect_data_forwarding_tunnel_info_local_var->is_additional_tnl_nb = is_additional_tnl_nb;
    indirect_data_forwarding_tunnel_info_local_var->additional_tnl_nb = additional_tnl_nb;

    return indirect_data_forwarding_tunnel_info_local_var;
}

void OpenAPI_indirect_data_forwarding_tunnel_info_free(OpenAPI_indirect_data_forwarding_tunnel_info_t *indirect_data_forwarding_tunnel_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == indirect_data_forwarding_tunnel_info) {
        return;
    }
    if (indirect_data_forwarding_tunnel_info->ipv4_addr) {
        ogs_free(indirect_data_forwarding_tunnel_info->ipv4_addr);
        indirect_data_forwarding_tunnel_info->ipv4_addr = NULL;
    }
    if (indirect_data_forwarding_tunnel_info->ipv6_addr) {
        ogs_free(indirect_data_forwarding_tunnel_info->ipv6_addr);
        indirect_data_forwarding_tunnel_info->ipv6_addr = NULL;
    }
    if (indirect_data_forwarding_tunnel_info->gtp_teid) {
        ogs_free(indirect_data_forwarding_tunnel_info->gtp_teid);
        indirect_data_forwarding_tunnel_info->gtp_teid = NULL;
    }
    ogs_free(indirect_data_forwarding_tunnel_info);
}

cJSON *OpenAPI_indirect_data_forwarding_tunnel_info_convertToJSON(OpenAPI_indirect_data_forwarding_tunnel_info_t *indirect_data_forwarding_tunnel_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (indirect_data_forwarding_tunnel_info == NULL) {
        ogs_error("OpenAPI_indirect_data_forwarding_tunnel_info_convertToJSON() failed [IndirectDataForwardingTunnelInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (indirect_data_forwarding_tunnel_info->ipv4_addr) {
    if (cJSON_AddStringToObject(item, "ipv4Addr", indirect_data_forwarding_tunnel_info->ipv4_addr) == NULL) {
        ogs_error("OpenAPI_indirect_data_forwarding_tunnel_info_convertToJSON() failed [ipv4_addr]");
        goto end;
    }
    }

    if (indirect_data_forwarding_tunnel_info->ipv6_addr) {
    if (cJSON_AddStringToObject(item, "ipv6Addr", indirect_data_forwarding_tunnel_info->ipv6_addr) == NULL) {
        ogs_error("OpenAPI_indirect_data_forwarding_tunnel_info_convertToJSON() failed [ipv6_addr]");
        goto end;
    }
    }

    if (!indirect_data_forwarding_tunnel_info->gtp_teid) {
        ogs_error("OpenAPI_indirect_data_forwarding_tunnel_info_convertToJSON() failed [gtp_teid]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "gtpTeid", indirect_data_forwarding_tunnel_info->gtp_teid) == NULL) {
        ogs_error("OpenAPI_indirect_data_forwarding_tunnel_info_convertToJSON() failed [gtp_teid]");
        goto end;
    }

    if (indirect_data_forwarding_tunnel_info->is_drb_id) {
    if (cJSON_AddNumberToObject(item, "drbId", indirect_data_forwarding_tunnel_info->drb_id) == NULL) {
        ogs_error("OpenAPI_indirect_data_forwarding_tunnel_info_convertToJSON() failed [drb_id]");
        goto end;
    }
    }

    if (indirect_data_forwarding_tunnel_info->is_additional_tnl_nb) {
    if (cJSON_AddNumberToObject(item, "additionalTnlNb", indirect_data_forwarding_tunnel_info->additional_tnl_nb) == NULL) {
        ogs_error("OpenAPI_indirect_data_forwarding_tunnel_info_convertToJSON() failed [additional_tnl_nb]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_indirect_data_forwarding_tunnel_info_t *OpenAPI_indirect_data_forwarding_tunnel_info_parseFromJSON(cJSON *indirect_data_forwarding_tunnel_infoJSON)
{
    OpenAPI_indirect_data_forwarding_tunnel_info_t *indirect_data_forwarding_tunnel_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ipv4_addr = NULL;
    cJSON *ipv6_addr = NULL;
    cJSON *gtp_teid = NULL;
    cJSON *drb_id = NULL;
    cJSON *additional_tnl_nb = NULL;
    ipv4_addr = cJSON_GetObjectItemCaseSensitive(indirect_data_forwarding_tunnel_infoJSON, "ipv4Addr");
    if (ipv4_addr) {
    if (!cJSON_IsString(ipv4_addr) && !cJSON_IsNull(ipv4_addr)) {
        ogs_error("OpenAPI_indirect_data_forwarding_tunnel_info_parseFromJSON() failed [ipv4_addr]");
        goto end;
    }
    }

    ipv6_addr = cJSON_GetObjectItemCaseSensitive(indirect_data_forwarding_tunnel_infoJSON, "ipv6Addr");
    if (ipv6_addr) {
    if (!cJSON_IsString(ipv6_addr) && !cJSON_IsNull(ipv6_addr)) {
        ogs_error("OpenAPI_indirect_data_forwarding_tunnel_info_parseFromJSON() failed [ipv6_addr]");
        goto end;
    }
    }

    gtp_teid = cJSON_GetObjectItemCaseSensitive(indirect_data_forwarding_tunnel_infoJSON, "gtpTeid");
    if (!gtp_teid) {
        ogs_error("OpenAPI_indirect_data_forwarding_tunnel_info_parseFromJSON() failed [gtp_teid]");
        goto end;
    }
    if (!cJSON_IsString(gtp_teid)) {
        ogs_error("OpenAPI_indirect_data_forwarding_tunnel_info_parseFromJSON() failed [gtp_teid]");
        goto end;
    }

    drb_id = cJSON_GetObjectItemCaseSensitive(indirect_data_forwarding_tunnel_infoJSON, "drbId");
    if (drb_id) {
    if (!cJSON_IsNumber(drb_id)) {
        ogs_error("OpenAPI_indirect_data_forwarding_tunnel_info_parseFromJSON() failed [drb_id]");
        goto end;
    }
    }

    additional_tnl_nb = cJSON_GetObjectItemCaseSensitive(indirect_data_forwarding_tunnel_infoJSON, "additionalTnlNb");
    if (additional_tnl_nb) {
    if (!cJSON_IsNumber(additional_tnl_nb)) {
        ogs_error("OpenAPI_indirect_data_forwarding_tunnel_info_parseFromJSON() failed [additional_tnl_nb]");
        goto end;
    }
    }

    indirect_data_forwarding_tunnel_info_local_var = OpenAPI_indirect_data_forwarding_tunnel_info_create (
        ipv4_addr && !cJSON_IsNull(ipv4_addr) ? ogs_strdup(ipv4_addr->valuestring) : NULL,
        ipv6_addr && !cJSON_IsNull(ipv6_addr) ? ogs_strdup(ipv6_addr->valuestring) : NULL,
        ogs_strdup(gtp_teid->valuestring),
        drb_id ? true : false,
        drb_id ? drb_id->valuedouble : 0,
        additional_tnl_nb ? true : false,
        additional_tnl_nb ? additional_tnl_nb->valuedouble : 0
    );

    return indirect_data_forwarding_tunnel_info_local_var;
end:
    return NULL;
}

OpenAPI_indirect_data_forwarding_tunnel_info_t *OpenAPI_indirect_data_forwarding_tunnel_info_copy(OpenAPI_indirect_data_forwarding_tunnel_info_t *dst, OpenAPI_indirect_data_forwarding_tunnel_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_indirect_data_forwarding_tunnel_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_indirect_data_forwarding_tunnel_info_convertToJSON() failed");
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

    OpenAPI_indirect_data_forwarding_tunnel_info_free(dst);
    dst = OpenAPI_indirect_data_forwarding_tunnel_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

