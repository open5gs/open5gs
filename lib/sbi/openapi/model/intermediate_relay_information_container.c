
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "intermediate_relay_information_container.h"

OpenAPI_intermediate_relay_information_container_t *OpenAPI_intermediate_relay_information_container_create(
    OpenAPI_ip_addr_t *intermediate_relay_ip_address,
    char *prose_ueto_network_intermediate_relay_ueid
)
{
    OpenAPI_intermediate_relay_information_container_t *intermediate_relay_information_container_local_var = ogs_malloc(sizeof(OpenAPI_intermediate_relay_information_container_t));
    ogs_assert(intermediate_relay_information_container_local_var);

    intermediate_relay_information_container_local_var->intermediate_relay_ip_address = intermediate_relay_ip_address;
    intermediate_relay_information_container_local_var->prose_ueto_network_intermediate_relay_ueid = prose_ueto_network_intermediate_relay_ueid;

    return intermediate_relay_information_container_local_var;
}

void OpenAPI_intermediate_relay_information_container_free(OpenAPI_intermediate_relay_information_container_t *intermediate_relay_information_container)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == intermediate_relay_information_container) {
        return;
    }
    if (intermediate_relay_information_container->intermediate_relay_ip_address) {
        OpenAPI_ip_addr_free(intermediate_relay_information_container->intermediate_relay_ip_address);
        intermediate_relay_information_container->intermediate_relay_ip_address = NULL;
    }
    if (intermediate_relay_information_container->prose_ueto_network_intermediate_relay_ueid) {
        ogs_free(intermediate_relay_information_container->prose_ueto_network_intermediate_relay_ueid);
        intermediate_relay_information_container->prose_ueto_network_intermediate_relay_ueid = NULL;
    }
    ogs_free(intermediate_relay_information_container);
}

cJSON *OpenAPI_intermediate_relay_information_container_convertToJSON(OpenAPI_intermediate_relay_information_container_t *intermediate_relay_information_container)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (intermediate_relay_information_container == NULL) {
        ogs_error("OpenAPI_intermediate_relay_information_container_convertToJSON() failed [IntermediateRelayInformationContainer]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (intermediate_relay_information_container->intermediate_relay_ip_address) {
    cJSON *intermediate_relay_ip_address_local_JSON = OpenAPI_ip_addr_convertToJSON(intermediate_relay_information_container->intermediate_relay_ip_address);
    if (intermediate_relay_ip_address_local_JSON == NULL) {
        ogs_error("OpenAPI_intermediate_relay_information_container_convertToJSON() failed [intermediate_relay_ip_address]");
        goto end;
    }
    cJSON_AddItemToObject(item, "intermediateRelayIPAddress", intermediate_relay_ip_address_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_intermediate_relay_information_container_convertToJSON() failed [intermediate_relay_ip_address]");
        goto end;
    }
    }

    if (intermediate_relay_information_container->prose_ueto_network_intermediate_relay_ueid) {
    if (cJSON_AddStringToObject(item, "proseUEToNetworkIntermediateRelayUEID", intermediate_relay_information_container->prose_ueto_network_intermediate_relay_ueid) == NULL) {
        ogs_error("OpenAPI_intermediate_relay_information_container_convertToJSON() failed [prose_ueto_network_intermediate_relay_ueid]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_intermediate_relay_information_container_t *OpenAPI_intermediate_relay_information_container_parseFromJSON(cJSON *intermediate_relay_information_containerJSON)
{
    OpenAPI_intermediate_relay_information_container_t *intermediate_relay_information_container_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *intermediate_relay_ip_address = NULL;
    OpenAPI_ip_addr_t *intermediate_relay_ip_address_local_nonprim = NULL;
    cJSON *prose_ueto_network_intermediate_relay_ueid = NULL;
    intermediate_relay_ip_address = cJSON_GetObjectItemCaseSensitive(intermediate_relay_information_containerJSON, "intermediateRelayIPAddress");
    if (intermediate_relay_ip_address) {
    intermediate_relay_ip_address_local_nonprim = OpenAPI_ip_addr_parseFromJSON(intermediate_relay_ip_address);
    if (!intermediate_relay_ip_address_local_nonprim) {
        ogs_error("OpenAPI_ip_addr_parseFromJSON failed [intermediate_relay_ip_address]");
        goto end;
    }
    }

    prose_ueto_network_intermediate_relay_ueid = cJSON_GetObjectItemCaseSensitive(intermediate_relay_information_containerJSON, "proseUEToNetworkIntermediateRelayUEID");
    if (prose_ueto_network_intermediate_relay_ueid) {
    if (!cJSON_IsString(prose_ueto_network_intermediate_relay_ueid) && !cJSON_IsNull(prose_ueto_network_intermediate_relay_ueid)) {
        ogs_error("OpenAPI_intermediate_relay_information_container_parseFromJSON() failed [prose_ueto_network_intermediate_relay_ueid]");
        goto end;
    }
    }

    intermediate_relay_information_container_local_var = OpenAPI_intermediate_relay_information_container_create (
        intermediate_relay_ip_address ? intermediate_relay_ip_address_local_nonprim : NULL,
        prose_ueto_network_intermediate_relay_ueid && !cJSON_IsNull(prose_ueto_network_intermediate_relay_ueid) ? ogs_strdup(prose_ueto_network_intermediate_relay_ueid->valuestring) : NULL
    );

    return intermediate_relay_information_container_local_var;
end:
    if (intermediate_relay_ip_address_local_nonprim) {
        OpenAPI_ip_addr_free(intermediate_relay_ip_address_local_nonprim);
        intermediate_relay_ip_address_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_intermediate_relay_information_container_t *OpenAPI_intermediate_relay_information_container_copy(OpenAPI_intermediate_relay_information_container_t *dst, OpenAPI_intermediate_relay_information_container_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_intermediate_relay_information_container_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_intermediate_relay_information_container_convertToJSON() failed");
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

    OpenAPI_intermediate_relay_information_container_free(dst);
    dst = OpenAPI_intermediate_relay_information_container_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

