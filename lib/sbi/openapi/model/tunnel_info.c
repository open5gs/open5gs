
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tunnel_info.h"

OpenAPI_tunnel_info_t *OpenAPI_tunnel_info_create(
    char *ipv4_addr,
    char *ipv6_addr,
    char *gtp_teid,
    OpenAPI_access_type_e an_type
)
{
    OpenAPI_tunnel_info_t *tunnel_info_local_var = ogs_malloc(sizeof(OpenAPI_tunnel_info_t));
    ogs_assert(tunnel_info_local_var);

    tunnel_info_local_var->ipv4_addr = ipv4_addr;
    tunnel_info_local_var->ipv6_addr = ipv6_addr;
    tunnel_info_local_var->gtp_teid = gtp_teid;
    tunnel_info_local_var->an_type = an_type;

    return tunnel_info_local_var;
}

void OpenAPI_tunnel_info_free(OpenAPI_tunnel_info_t *tunnel_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == tunnel_info) {
        return;
    }
    if (tunnel_info->ipv4_addr) {
        ogs_free(tunnel_info->ipv4_addr);
        tunnel_info->ipv4_addr = NULL;
    }
    if (tunnel_info->ipv6_addr) {
        ogs_free(tunnel_info->ipv6_addr);
        tunnel_info->ipv6_addr = NULL;
    }
    if (tunnel_info->gtp_teid) {
        ogs_free(tunnel_info->gtp_teid);
        tunnel_info->gtp_teid = NULL;
    }
    ogs_free(tunnel_info);
}

cJSON *OpenAPI_tunnel_info_convertToJSON(OpenAPI_tunnel_info_t *tunnel_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (tunnel_info == NULL) {
        ogs_error("OpenAPI_tunnel_info_convertToJSON() failed [TunnelInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (tunnel_info->ipv4_addr) {
    if (cJSON_AddStringToObject(item, "ipv4Addr", tunnel_info->ipv4_addr) == NULL) {
        ogs_error("OpenAPI_tunnel_info_convertToJSON() failed [ipv4_addr]");
        goto end;
    }
    }

    if (tunnel_info->ipv6_addr) {
    if (cJSON_AddStringToObject(item, "ipv6Addr", tunnel_info->ipv6_addr) == NULL) {
        ogs_error("OpenAPI_tunnel_info_convertToJSON() failed [ipv6_addr]");
        goto end;
    }
    }

    if (!tunnel_info->gtp_teid) {
        ogs_error("OpenAPI_tunnel_info_convertToJSON() failed [gtp_teid]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "gtpTeid", tunnel_info->gtp_teid) == NULL) {
        ogs_error("OpenAPI_tunnel_info_convertToJSON() failed [gtp_teid]");
        goto end;
    }

    if (tunnel_info->an_type != OpenAPI_access_type_NULL) {
    if (cJSON_AddStringToObject(item, "anType", OpenAPI_access_type_ToString(tunnel_info->an_type)) == NULL) {
        ogs_error("OpenAPI_tunnel_info_convertToJSON() failed [an_type]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_tunnel_info_t *OpenAPI_tunnel_info_parseFromJSON(cJSON *tunnel_infoJSON)
{
    OpenAPI_tunnel_info_t *tunnel_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ipv4_addr = NULL;
    cJSON *ipv6_addr = NULL;
    cJSON *gtp_teid = NULL;
    cJSON *an_type = NULL;
    OpenAPI_access_type_e an_typeVariable = 0;
    ipv4_addr = cJSON_GetObjectItemCaseSensitive(tunnel_infoJSON, "ipv4Addr");
    if (ipv4_addr) {
    if (!cJSON_IsString(ipv4_addr) && !cJSON_IsNull(ipv4_addr)) {
        ogs_error("OpenAPI_tunnel_info_parseFromJSON() failed [ipv4_addr]");
        goto end;
    }
    }

    ipv6_addr = cJSON_GetObjectItemCaseSensitive(tunnel_infoJSON, "ipv6Addr");
    if (ipv6_addr) {
    if (!cJSON_IsString(ipv6_addr) && !cJSON_IsNull(ipv6_addr)) {
        ogs_error("OpenAPI_tunnel_info_parseFromJSON() failed [ipv6_addr]");
        goto end;
    }
    }

    gtp_teid = cJSON_GetObjectItemCaseSensitive(tunnel_infoJSON, "gtpTeid");
    if (!gtp_teid) {
        ogs_error("OpenAPI_tunnel_info_parseFromJSON() failed [gtp_teid]");
        goto end;
    }
    if (!cJSON_IsString(gtp_teid)) {
        ogs_error("OpenAPI_tunnel_info_parseFromJSON() failed [gtp_teid]");
        goto end;
    }

    an_type = cJSON_GetObjectItemCaseSensitive(tunnel_infoJSON, "anType");
    if (an_type) {
    if (!cJSON_IsString(an_type)) {
        ogs_error("OpenAPI_tunnel_info_parseFromJSON() failed [an_type]");
        goto end;
    }
    an_typeVariable = OpenAPI_access_type_FromString(an_type->valuestring);
    }

    tunnel_info_local_var = OpenAPI_tunnel_info_create (
        ipv4_addr && !cJSON_IsNull(ipv4_addr) ? ogs_strdup(ipv4_addr->valuestring) : NULL,
        ipv6_addr && !cJSON_IsNull(ipv6_addr) ? ogs_strdup(ipv6_addr->valuestring) : NULL,
        ogs_strdup(gtp_teid->valuestring),
        an_type ? an_typeVariable : 0
    );

    return tunnel_info_local_var;
end:
    return NULL;
}

OpenAPI_tunnel_info_t *OpenAPI_tunnel_info_copy(OpenAPI_tunnel_info_t *dst, OpenAPI_tunnel_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_tunnel_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_tunnel_info_convertToJSON() failed");
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

    OpenAPI_tunnel_info_free(dst);
    dst = OpenAPI_tunnel_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

