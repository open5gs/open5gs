
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "network_node_diameter_address_1.h"

OpenAPI_network_node_diameter_address_1_t *OpenAPI_network_node_diameter_address_1_create(
    char *name,
    char *realm
)
{
    OpenAPI_network_node_diameter_address_1_t *network_node_diameter_address_1_local_var = ogs_malloc(sizeof(OpenAPI_network_node_diameter_address_1_t));
    ogs_assert(network_node_diameter_address_1_local_var);

    network_node_diameter_address_1_local_var->name = name;
    network_node_diameter_address_1_local_var->realm = realm;

    return network_node_diameter_address_1_local_var;
}

void OpenAPI_network_node_diameter_address_1_free(OpenAPI_network_node_diameter_address_1_t *network_node_diameter_address_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == network_node_diameter_address_1) {
        return;
    }
    if (network_node_diameter_address_1->name) {
        ogs_free(network_node_diameter_address_1->name);
        network_node_diameter_address_1->name = NULL;
    }
    if (network_node_diameter_address_1->realm) {
        ogs_free(network_node_diameter_address_1->realm);
        network_node_diameter_address_1->realm = NULL;
    }
    ogs_free(network_node_diameter_address_1);
}

cJSON *OpenAPI_network_node_diameter_address_1_convertToJSON(OpenAPI_network_node_diameter_address_1_t *network_node_diameter_address_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (network_node_diameter_address_1 == NULL) {
        ogs_error("OpenAPI_network_node_diameter_address_1_convertToJSON() failed [NetworkNodeDiameterAddress_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!network_node_diameter_address_1->name) {
        ogs_error("OpenAPI_network_node_diameter_address_1_convertToJSON() failed [name]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "name", network_node_diameter_address_1->name) == NULL) {
        ogs_error("OpenAPI_network_node_diameter_address_1_convertToJSON() failed [name]");
        goto end;
    }

    if (!network_node_diameter_address_1->realm) {
        ogs_error("OpenAPI_network_node_diameter_address_1_convertToJSON() failed [realm]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "realm", network_node_diameter_address_1->realm) == NULL) {
        ogs_error("OpenAPI_network_node_diameter_address_1_convertToJSON() failed [realm]");
        goto end;
    }

end:
    return item;
}

OpenAPI_network_node_diameter_address_1_t *OpenAPI_network_node_diameter_address_1_parseFromJSON(cJSON *network_node_diameter_address_1JSON)
{
    OpenAPI_network_node_diameter_address_1_t *network_node_diameter_address_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *name = NULL;
    cJSON *realm = NULL;
    name = cJSON_GetObjectItemCaseSensitive(network_node_diameter_address_1JSON, "name");
    if (!name) {
        ogs_error("OpenAPI_network_node_diameter_address_1_parseFromJSON() failed [name]");
        goto end;
    }
    if (!cJSON_IsString(name)) {
        ogs_error("OpenAPI_network_node_diameter_address_1_parseFromJSON() failed [name]");
        goto end;
    }

    realm = cJSON_GetObjectItemCaseSensitive(network_node_diameter_address_1JSON, "realm");
    if (!realm) {
        ogs_error("OpenAPI_network_node_diameter_address_1_parseFromJSON() failed [realm]");
        goto end;
    }
    if (!cJSON_IsString(realm)) {
        ogs_error("OpenAPI_network_node_diameter_address_1_parseFromJSON() failed [realm]");
        goto end;
    }

    network_node_diameter_address_1_local_var = OpenAPI_network_node_diameter_address_1_create (
        ogs_strdup(name->valuestring),
        ogs_strdup(realm->valuestring)
    );

    return network_node_diameter_address_1_local_var;
end:
    return NULL;
}

OpenAPI_network_node_diameter_address_1_t *OpenAPI_network_node_diameter_address_1_copy(OpenAPI_network_node_diameter_address_1_t *dst, OpenAPI_network_node_diameter_address_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_network_node_diameter_address_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_network_node_diameter_address_1_convertToJSON() failed");
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

    OpenAPI_network_node_diameter_address_1_free(dst);
    dst = OpenAPI_network_node_diameter_address_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

