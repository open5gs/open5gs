
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "network_node_diameter_address.h"

OpenAPI_network_node_diameter_address_t *OpenAPI_network_node_diameter_address_create(
    char *name,
    char *realm
    )
{
    OpenAPI_network_node_diameter_address_t *network_node_diameter_address_local_var = OpenAPI_malloc(sizeof(OpenAPI_network_node_diameter_address_t));
    if (!network_node_diameter_address_local_var) {
        return NULL;
    }
    network_node_diameter_address_local_var->name = name;
    network_node_diameter_address_local_var->realm = realm;

    return network_node_diameter_address_local_var;
}

void OpenAPI_network_node_diameter_address_free(OpenAPI_network_node_diameter_address_t *network_node_diameter_address)
{
    if (NULL == network_node_diameter_address) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(network_node_diameter_address->name);
    ogs_free(network_node_diameter_address->realm);
    ogs_free(network_node_diameter_address);
}

cJSON *OpenAPI_network_node_diameter_address_convertToJSON(OpenAPI_network_node_diameter_address_t *network_node_diameter_address)
{
    cJSON *item = NULL;

    if (network_node_diameter_address == NULL) {
        ogs_error("OpenAPI_network_node_diameter_address_convertToJSON() failed [NetworkNodeDiameterAddress]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!network_node_diameter_address->name) {
        ogs_error("OpenAPI_network_node_diameter_address_convertToJSON() failed [name]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "name", network_node_diameter_address->name) == NULL) {
        ogs_error("OpenAPI_network_node_diameter_address_convertToJSON() failed [name]");
        goto end;
    }

    if (!network_node_diameter_address->realm) {
        ogs_error("OpenAPI_network_node_diameter_address_convertToJSON() failed [realm]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "realm", network_node_diameter_address->realm) == NULL) {
        ogs_error("OpenAPI_network_node_diameter_address_convertToJSON() failed [realm]");
        goto end;
    }

end:
    return item;
}

OpenAPI_network_node_diameter_address_t *OpenAPI_network_node_diameter_address_parseFromJSON(cJSON *network_node_diameter_addressJSON)
{
    OpenAPI_network_node_diameter_address_t *network_node_diameter_address_local_var = NULL;
    cJSON *name = cJSON_GetObjectItemCaseSensitive(network_node_diameter_addressJSON, "name");
    if (!name) {
        ogs_error("OpenAPI_network_node_diameter_address_parseFromJSON() failed [name]");
        goto end;
    }


    if (!cJSON_IsString(name)) {
        ogs_error("OpenAPI_network_node_diameter_address_parseFromJSON() failed [name]");
        goto end;
    }

    cJSON *realm = cJSON_GetObjectItemCaseSensitive(network_node_diameter_addressJSON, "realm");
    if (!realm) {
        ogs_error("OpenAPI_network_node_diameter_address_parseFromJSON() failed [realm]");
        goto end;
    }


    if (!cJSON_IsString(realm)) {
        ogs_error("OpenAPI_network_node_diameter_address_parseFromJSON() failed [realm]");
        goto end;
    }

    network_node_diameter_address_local_var = OpenAPI_network_node_diameter_address_create (
        ogs_strdup(name->valuestring),
        ogs_strdup(realm->valuestring)
        );

    return network_node_diameter_address_local_var;
end:
    return NULL;
}

