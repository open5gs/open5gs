
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sms_router_info.h"

OpenAPI_sms_router_info_t *OpenAPI_sms_router_info_create(
    char *nf_instance_id,
    OpenAPI_network_node_diameter_address_t *diameter_address,
    char *map_address,
    char *router_ipv4,
    char *router_ipv6,
    char *router_fqdn
)
{
    OpenAPI_sms_router_info_t *sms_router_info_local_var = ogs_malloc(sizeof(OpenAPI_sms_router_info_t));
    ogs_assert(sms_router_info_local_var);

    sms_router_info_local_var->nf_instance_id = nf_instance_id;
    sms_router_info_local_var->diameter_address = diameter_address;
    sms_router_info_local_var->map_address = map_address;
    sms_router_info_local_var->router_ipv4 = router_ipv4;
    sms_router_info_local_var->router_ipv6 = router_ipv6;
    sms_router_info_local_var->router_fqdn = router_fqdn;

    return sms_router_info_local_var;
}

void OpenAPI_sms_router_info_free(OpenAPI_sms_router_info_t *sms_router_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == sms_router_info) {
        return;
    }
    if (sms_router_info->nf_instance_id) {
        ogs_free(sms_router_info->nf_instance_id);
        sms_router_info->nf_instance_id = NULL;
    }
    if (sms_router_info->diameter_address) {
        OpenAPI_network_node_diameter_address_free(sms_router_info->diameter_address);
        sms_router_info->diameter_address = NULL;
    }
    if (sms_router_info->map_address) {
        ogs_free(sms_router_info->map_address);
        sms_router_info->map_address = NULL;
    }
    if (sms_router_info->router_ipv4) {
        ogs_free(sms_router_info->router_ipv4);
        sms_router_info->router_ipv4 = NULL;
    }
    if (sms_router_info->router_ipv6) {
        ogs_free(sms_router_info->router_ipv6);
        sms_router_info->router_ipv6 = NULL;
    }
    if (sms_router_info->router_fqdn) {
        ogs_free(sms_router_info->router_fqdn);
        sms_router_info->router_fqdn = NULL;
    }
    ogs_free(sms_router_info);
}

cJSON *OpenAPI_sms_router_info_convertToJSON(OpenAPI_sms_router_info_t *sms_router_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (sms_router_info == NULL) {
        ogs_error("OpenAPI_sms_router_info_convertToJSON() failed [SmsRouterInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (sms_router_info->nf_instance_id) {
    if (cJSON_AddStringToObject(item, "nfInstanceId", sms_router_info->nf_instance_id) == NULL) {
        ogs_error("OpenAPI_sms_router_info_convertToJSON() failed [nf_instance_id]");
        goto end;
    }
    }

    if (sms_router_info->diameter_address) {
    cJSON *diameter_address_local_JSON = OpenAPI_network_node_diameter_address_convertToJSON(sms_router_info->diameter_address);
    if (diameter_address_local_JSON == NULL) {
        ogs_error("OpenAPI_sms_router_info_convertToJSON() failed [diameter_address]");
        goto end;
    }
    cJSON_AddItemToObject(item, "diameterAddress", diameter_address_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sms_router_info_convertToJSON() failed [diameter_address]");
        goto end;
    }
    }

    if (sms_router_info->map_address) {
    if (cJSON_AddStringToObject(item, "mapAddress", sms_router_info->map_address) == NULL) {
        ogs_error("OpenAPI_sms_router_info_convertToJSON() failed [map_address]");
        goto end;
    }
    }

    if (sms_router_info->router_ipv4) {
    if (cJSON_AddStringToObject(item, "routerIpv4", sms_router_info->router_ipv4) == NULL) {
        ogs_error("OpenAPI_sms_router_info_convertToJSON() failed [router_ipv4]");
        goto end;
    }
    }

    if (sms_router_info->router_ipv6) {
    if (cJSON_AddStringToObject(item, "routerIpv6", sms_router_info->router_ipv6) == NULL) {
        ogs_error("OpenAPI_sms_router_info_convertToJSON() failed [router_ipv6]");
        goto end;
    }
    }

    if (sms_router_info->router_fqdn) {
    if (cJSON_AddStringToObject(item, "routerFqdn", sms_router_info->router_fqdn) == NULL) {
        ogs_error("OpenAPI_sms_router_info_convertToJSON() failed [router_fqdn]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_sms_router_info_t *OpenAPI_sms_router_info_parseFromJSON(cJSON *sms_router_infoJSON)
{
    OpenAPI_sms_router_info_t *sms_router_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *nf_instance_id = NULL;
    cJSON *diameter_address = NULL;
    OpenAPI_network_node_diameter_address_t *diameter_address_local_nonprim = NULL;
    cJSON *map_address = NULL;
    cJSON *router_ipv4 = NULL;
    cJSON *router_ipv6 = NULL;
    cJSON *router_fqdn = NULL;
    nf_instance_id = cJSON_GetObjectItemCaseSensitive(sms_router_infoJSON, "nfInstanceId");
    if (nf_instance_id) {
    if (!cJSON_IsString(nf_instance_id) && !cJSON_IsNull(nf_instance_id)) {
        ogs_error("OpenAPI_sms_router_info_parseFromJSON() failed [nf_instance_id]");
        goto end;
    }
    }

    diameter_address = cJSON_GetObjectItemCaseSensitive(sms_router_infoJSON, "diameterAddress");
    if (diameter_address) {
    diameter_address_local_nonprim = OpenAPI_network_node_diameter_address_parseFromJSON(diameter_address);
    if (!diameter_address_local_nonprim) {
        ogs_error("OpenAPI_network_node_diameter_address_parseFromJSON failed [diameter_address]");
        goto end;
    }
    }

    map_address = cJSON_GetObjectItemCaseSensitive(sms_router_infoJSON, "mapAddress");
    if (map_address) {
    if (!cJSON_IsString(map_address) && !cJSON_IsNull(map_address)) {
        ogs_error("OpenAPI_sms_router_info_parseFromJSON() failed [map_address]");
        goto end;
    }
    }

    router_ipv4 = cJSON_GetObjectItemCaseSensitive(sms_router_infoJSON, "routerIpv4");
    if (router_ipv4) {
    if (!cJSON_IsString(router_ipv4) && !cJSON_IsNull(router_ipv4)) {
        ogs_error("OpenAPI_sms_router_info_parseFromJSON() failed [router_ipv4]");
        goto end;
    }
    }

    router_ipv6 = cJSON_GetObjectItemCaseSensitive(sms_router_infoJSON, "routerIpv6");
    if (router_ipv6) {
    if (!cJSON_IsString(router_ipv6) && !cJSON_IsNull(router_ipv6)) {
        ogs_error("OpenAPI_sms_router_info_parseFromJSON() failed [router_ipv6]");
        goto end;
    }
    }

    router_fqdn = cJSON_GetObjectItemCaseSensitive(sms_router_infoJSON, "routerFqdn");
    if (router_fqdn) {
    if (!cJSON_IsString(router_fqdn) && !cJSON_IsNull(router_fqdn)) {
        ogs_error("OpenAPI_sms_router_info_parseFromJSON() failed [router_fqdn]");
        goto end;
    }
    }

    sms_router_info_local_var = OpenAPI_sms_router_info_create (
        nf_instance_id && !cJSON_IsNull(nf_instance_id) ? ogs_strdup(nf_instance_id->valuestring) : NULL,
        diameter_address ? diameter_address_local_nonprim : NULL,
        map_address && !cJSON_IsNull(map_address) ? ogs_strdup(map_address->valuestring) : NULL,
        router_ipv4 && !cJSON_IsNull(router_ipv4) ? ogs_strdup(router_ipv4->valuestring) : NULL,
        router_ipv6 && !cJSON_IsNull(router_ipv6) ? ogs_strdup(router_ipv6->valuestring) : NULL,
        router_fqdn && !cJSON_IsNull(router_fqdn) ? ogs_strdup(router_fqdn->valuestring) : NULL
    );

    return sms_router_info_local_var;
end:
    if (diameter_address_local_nonprim) {
        OpenAPI_network_node_diameter_address_free(diameter_address_local_nonprim);
        diameter_address_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_sms_router_info_t *OpenAPI_sms_router_info_copy(OpenAPI_sms_router_info_t *dst, OpenAPI_sms_router_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sms_router_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sms_router_info_convertToJSON() failed");
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

    OpenAPI_sms_router_info_free(dst);
    dst = OpenAPI_sms_router_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

