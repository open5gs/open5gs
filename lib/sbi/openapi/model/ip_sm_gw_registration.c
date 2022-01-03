
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ip_sm_gw_registration.h"

OpenAPI_ip_sm_gw_registration_t *OpenAPI_ip_sm_gw_registration_create(
    char *ip_sm_gw_map_address,
    OpenAPI_network_node_diameter_address_t *ip_sm_gw_diameter_address,
    bool is_unri_indicator,
    int unri_indicator
)
{
    OpenAPI_ip_sm_gw_registration_t *ip_sm_gw_registration_local_var = ogs_malloc(sizeof(OpenAPI_ip_sm_gw_registration_t));
    ogs_assert(ip_sm_gw_registration_local_var);

    ip_sm_gw_registration_local_var->ip_sm_gw_map_address = ip_sm_gw_map_address;
    ip_sm_gw_registration_local_var->ip_sm_gw_diameter_address = ip_sm_gw_diameter_address;
    ip_sm_gw_registration_local_var->is_unri_indicator = is_unri_indicator;
    ip_sm_gw_registration_local_var->unri_indicator = unri_indicator;

    return ip_sm_gw_registration_local_var;
}

void OpenAPI_ip_sm_gw_registration_free(OpenAPI_ip_sm_gw_registration_t *ip_sm_gw_registration)
{
    if (NULL == ip_sm_gw_registration) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(ip_sm_gw_registration->ip_sm_gw_map_address);
    OpenAPI_network_node_diameter_address_free(ip_sm_gw_registration->ip_sm_gw_diameter_address);
    ogs_free(ip_sm_gw_registration);
}

cJSON *OpenAPI_ip_sm_gw_registration_convertToJSON(OpenAPI_ip_sm_gw_registration_t *ip_sm_gw_registration)
{
    cJSON *item = NULL;

    if (ip_sm_gw_registration == NULL) {
        ogs_error("OpenAPI_ip_sm_gw_registration_convertToJSON() failed [IpSmGwRegistration]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ip_sm_gw_registration->ip_sm_gw_map_address) {
    if (cJSON_AddStringToObject(item, "ipSmGwMapAddress", ip_sm_gw_registration->ip_sm_gw_map_address) == NULL) {
        ogs_error("OpenAPI_ip_sm_gw_registration_convertToJSON() failed [ip_sm_gw_map_address]");
        goto end;
    }
    }

    if (ip_sm_gw_registration->ip_sm_gw_diameter_address) {
    cJSON *ip_sm_gw_diameter_address_local_JSON = OpenAPI_network_node_diameter_address_convertToJSON(ip_sm_gw_registration->ip_sm_gw_diameter_address);
    if (ip_sm_gw_diameter_address_local_JSON == NULL) {
        ogs_error("OpenAPI_ip_sm_gw_registration_convertToJSON() failed [ip_sm_gw_diameter_address]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ipSmGwDiameterAddress", ip_sm_gw_diameter_address_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ip_sm_gw_registration_convertToJSON() failed [ip_sm_gw_diameter_address]");
        goto end;
    }
    }

    if (ip_sm_gw_registration->is_unri_indicator) {
    if (cJSON_AddBoolToObject(item, "unriIndicator", ip_sm_gw_registration->unri_indicator) == NULL) {
        ogs_error("OpenAPI_ip_sm_gw_registration_convertToJSON() failed [unri_indicator]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ip_sm_gw_registration_t *OpenAPI_ip_sm_gw_registration_parseFromJSON(cJSON *ip_sm_gw_registrationJSON)
{
    OpenAPI_ip_sm_gw_registration_t *ip_sm_gw_registration_local_var = NULL;
    cJSON *ip_sm_gw_map_address = cJSON_GetObjectItemCaseSensitive(ip_sm_gw_registrationJSON, "ipSmGwMapAddress");

    if (ip_sm_gw_map_address) {
    if (!cJSON_IsString(ip_sm_gw_map_address)) {
        ogs_error("OpenAPI_ip_sm_gw_registration_parseFromJSON() failed [ip_sm_gw_map_address]");
        goto end;
    }
    }

    cJSON *ip_sm_gw_diameter_address = cJSON_GetObjectItemCaseSensitive(ip_sm_gw_registrationJSON, "ipSmGwDiameterAddress");

    OpenAPI_network_node_diameter_address_t *ip_sm_gw_diameter_address_local_nonprim = NULL;
    if (ip_sm_gw_diameter_address) {
    ip_sm_gw_diameter_address_local_nonprim = OpenAPI_network_node_diameter_address_parseFromJSON(ip_sm_gw_diameter_address);
    }

    cJSON *unri_indicator = cJSON_GetObjectItemCaseSensitive(ip_sm_gw_registrationJSON, "unriIndicator");

    if (unri_indicator) {
    if (!cJSON_IsBool(unri_indicator)) {
        ogs_error("OpenAPI_ip_sm_gw_registration_parseFromJSON() failed [unri_indicator]");
        goto end;
    }
    }

    ip_sm_gw_registration_local_var = OpenAPI_ip_sm_gw_registration_create (
        ip_sm_gw_map_address ? ogs_strdup(ip_sm_gw_map_address->valuestring) : NULL,
        ip_sm_gw_diameter_address ? ip_sm_gw_diameter_address_local_nonprim : NULL,
        unri_indicator ? true : false,
        unri_indicator ? unri_indicator->valueint : 0
    );

    return ip_sm_gw_registration_local_var;
end:
    return NULL;
}

OpenAPI_ip_sm_gw_registration_t *OpenAPI_ip_sm_gw_registration_copy(OpenAPI_ip_sm_gw_registration_t *dst, OpenAPI_ip_sm_gw_registration_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ip_sm_gw_registration_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ip_sm_gw_registration_convertToJSON() failed");
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

    OpenAPI_ip_sm_gw_registration_free(dst);
    dst = OpenAPI_ip_sm_gw_registration_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

