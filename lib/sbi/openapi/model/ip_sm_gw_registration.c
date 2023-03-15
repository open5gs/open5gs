
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ip_sm_gw_registration.h"

OpenAPI_ip_sm_gw_registration_t *OpenAPI_ip_sm_gw_registration_create(
    char *ip_sm_gw_map_address,
    OpenAPI_network_node_diameter_address_t *ip_sm_gw_diameter_address,
    char *ipsmgw_ipv4,
    char *ipsmgw_ipv6,
    char *ipsmgw_fqdn,
    char *nf_instance_id,
    bool is_unri_indicator,
    int unri_indicator,
    OpenAPI_list_t *reset_ids,
    bool is_ip_sm_gw_sbi_sup_ind,
    int ip_sm_gw_sbi_sup_ind
)
{
    OpenAPI_ip_sm_gw_registration_t *ip_sm_gw_registration_local_var = ogs_malloc(sizeof(OpenAPI_ip_sm_gw_registration_t));
    ogs_assert(ip_sm_gw_registration_local_var);

    ip_sm_gw_registration_local_var->ip_sm_gw_map_address = ip_sm_gw_map_address;
    ip_sm_gw_registration_local_var->ip_sm_gw_diameter_address = ip_sm_gw_diameter_address;
    ip_sm_gw_registration_local_var->ipsmgw_ipv4 = ipsmgw_ipv4;
    ip_sm_gw_registration_local_var->ipsmgw_ipv6 = ipsmgw_ipv6;
    ip_sm_gw_registration_local_var->ipsmgw_fqdn = ipsmgw_fqdn;
    ip_sm_gw_registration_local_var->nf_instance_id = nf_instance_id;
    ip_sm_gw_registration_local_var->is_unri_indicator = is_unri_indicator;
    ip_sm_gw_registration_local_var->unri_indicator = unri_indicator;
    ip_sm_gw_registration_local_var->reset_ids = reset_ids;
    ip_sm_gw_registration_local_var->is_ip_sm_gw_sbi_sup_ind = is_ip_sm_gw_sbi_sup_ind;
    ip_sm_gw_registration_local_var->ip_sm_gw_sbi_sup_ind = ip_sm_gw_sbi_sup_ind;

    return ip_sm_gw_registration_local_var;
}

void OpenAPI_ip_sm_gw_registration_free(OpenAPI_ip_sm_gw_registration_t *ip_sm_gw_registration)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ip_sm_gw_registration) {
        return;
    }
    if (ip_sm_gw_registration->ip_sm_gw_map_address) {
        ogs_free(ip_sm_gw_registration->ip_sm_gw_map_address);
        ip_sm_gw_registration->ip_sm_gw_map_address = NULL;
    }
    if (ip_sm_gw_registration->ip_sm_gw_diameter_address) {
        OpenAPI_network_node_diameter_address_free(ip_sm_gw_registration->ip_sm_gw_diameter_address);
        ip_sm_gw_registration->ip_sm_gw_diameter_address = NULL;
    }
    if (ip_sm_gw_registration->ipsmgw_ipv4) {
        ogs_free(ip_sm_gw_registration->ipsmgw_ipv4);
        ip_sm_gw_registration->ipsmgw_ipv4 = NULL;
    }
    if (ip_sm_gw_registration->ipsmgw_ipv6) {
        ogs_free(ip_sm_gw_registration->ipsmgw_ipv6);
        ip_sm_gw_registration->ipsmgw_ipv6 = NULL;
    }
    if (ip_sm_gw_registration->ipsmgw_fqdn) {
        ogs_free(ip_sm_gw_registration->ipsmgw_fqdn);
        ip_sm_gw_registration->ipsmgw_fqdn = NULL;
    }
    if (ip_sm_gw_registration->nf_instance_id) {
        ogs_free(ip_sm_gw_registration->nf_instance_id);
        ip_sm_gw_registration->nf_instance_id = NULL;
    }
    if (ip_sm_gw_registration->reset_ids) {
        OpenAPI_list_for_each(ip_sm_gw_registration->reset_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ip_sm_gw_registration->reset_ids);
        ip_sm_gw_registration->reset_ids = NULL;
    }
    ogs_free(ip_sm_gw_registration);
}

cJSON *OpenAPI_ip_sm_gw_registration_convertToJSON(OpenAPI_ip_sm_gw_registration_t *ip_sm_gw_registration)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

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

    if (ip_sm_gw_registration->ipsmgw_ipv4) {
    if (cJSON_AddStringToObject(item, "ipsmgwIpv4", ip_sm_gw_registration->ipsmgw_ipv4) == NULL) {
        ogs_error("OpenAPI_ip_sm_gw_registration_convertToJSON() failed [ipsmgw_ipv4]");
        goto end;
    }
    }

    if (ip_sm_gw_registration->ipsmgw_ipv6) {
    if (cJSON_AddStringToObject(item, "ipsmgwIpv6", ip_sm_gw_registration->ipsmgw_ipv6) == NULL) {
        ogs_error("OpenAPI_ip_sm_gw_registration_convertToJSON() failed [ipsmgw_ipv6]");
        goto end;
    }
    }

    if (ip_sm_gw_registration->ipsmgw_fqdn) {
    if (cJSON_AddStringToObject(item, "ipsmgwFqdn", ip_sm_gw_registration->ipsmgw_fqdn) == NULL) {
        ogs_error("OpenAPI_ip_sm_gw_registration_convertToJSON() failed [ipsmgw_fqdn]");
        goto end;
    }
    }

    if (ip_sm_gw_registration->nf_instance_id) {
    if (cJSON_AddStringToObject(item, "nfInstanceId", ip_sm_gw_registration->nf_instance_id) == NULL) {
        ogs_error("OpenAPI_ip_sm_gw_registration_convertToJSON() failed [nf_instance_id]");
        goto end;
    }
    }

    if (ip_sm_gw_registration->is_unri_indicator) {
    if (cJSON_AddBoolToObject(item, "unriIndicator", ip_sm_gw_registration->unri_indicator) == NULL) {
        ogs_error("OpenAPI_ip_sm_gw_registration_convertToJSON() failed [unri_indicator]");
        goto end;
    }
    }

    if (ip_sm_gw_registration->reset_ids) {
    cJSON *reset_idsList = cJSON_AddArrayToObject(item, "resetIds");
    if (reset_idsList == NULL) {
        ogs_error("OpenAPI_ip_sm_gw_registration_convertToJSON() failed [reset_ids]");
        goto end;
    }
    OpenAPI_list_for_each(ip_sm_gw_registration->reset_ids, node) {
        if (cJSON_AddStringToObject(reset_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_ip_sm_gw_registration_convertToJSON() failed [reset_ids]");
            goto end;
        }
    }
    }

    if (ip_sm_gw_registration->is_ip_sm_gw_sbi_sup_ind) {
    if (cJSON_AddBoolToObject(item, "ipSmGwSbiSupInd", ip_sm_gw_registration->ip_sm_gw_sbi_sup_ind) == NULL) {
        ogs_error("OpenAPI_ip_sm_gw_registration_convertToJSON() failed [ip_sm_gw_sbi_sup_ind]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ip_sm_gw_registration_t *OpenAPI_ip_sm_gw_registration_parseFromJSON(cJSON *ip_sm_gw_registrationJSON)
{
    OpenAPI_ip_sm_gw_registration_t *ip_sm_gw_registration_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ip_sm_gw_map_address = NULL;
    cJSON *ip_sm_gw_diameter_address = NULL;
    OpenAPI_network_node_diameter_address_t *ip_sm_gw_diameter_address_local_nonprim = NULL;
    cJSON *ipsmgw_ipv4 = NULL;
    cJSON *ipsmgw_ipv6 = NULL;
    cJSON *ipsmgw_fqdn = NULL;
    cJSON *nf_instance_id = NULL;
    cJSON *unri_indicator = NULL;
    cJSON *reset_ids = NULL;
    OpenAPI_list_t *reset_idsList = NULL;
    cJSON *ip_sm_gw_sbi_sup_ind = NULL;
    ip_sm_gw_map_address = cJSON_GetObjectItemCaseSensitive(ip_sm_gw_registrationJSON, "ipSmGwMapAddress");
    if (ip_sm_gw_map_address) {
    if (!cJSON_IsString(ip_sm_gw_map_address) && !cJSON_IsNull(ip_sm_gw_map_address)) {
        ogs_error("OpenAPI_ip_sm_gw_registration_parseFromJSON() failed [ip_sm_gw_map_address]");
        goto end;
    }
    }

    ip_sm_gw_diameter_address = cJSON_GetObjectItemCaseSensitive(ip_sm_gw_registrationJSON, "ipSmGwDiameterAddress");
    if (ip_sm_gw_diameter_address) {
    ip_sm_gw_diameter_address_local_nonprim = OpenAPI_network_node_diameter_address_parseFromJSON(ip_sm_gw_diameter_address);
    if (!ip_sm_gw_diameter_address_local_nonprim) {
        ogs_error("OpenAPI_network_node_diameter_address_parseFromJSON failed [ip_sm_gw_diameter_address]");
        goto end;
    }
    }

    ipsmgw_ipv4 = cJSON_GetObjectItemCaseSensitive(ip_sm_gw_registrationJSON, "ipsmgwIpv4");
    if (ipsmgw_ipv4) {
    if (!cJSON_IsString(ipsmgw_ipv4) && !cJSON_IsNull(ipsmgw_ipv4)) {
        ogs_error("OpenAPI_ip_sm_gw_registration_parseFromJSON() failed [ipsmgw_ipv4]");
        goto end;
    }
    }

    ipsmgw_ipv6 = cJSON_GetObjectItemCaseSensitive(ip_sm_gw_registrationJSON, "ipsmgwIpv6");
    if (ipsmgw_ipv6) {
    if (!cJSON_IsString(ipsmgw_ipv6) && !cJSON_IsNull(ipsmgw_ipv6)) {
        ogs_error("OpenAPI_ip_sm_gw_registration_parseFromJSON() failed [ipsmgw_ipv6]");
        goto end;
    }
    }

    ipsmgw_fqdn = cJSON_GetObjectItemCaseSensitive(ip_sm_gw_registrationJSON, "ipsmgwFqdn");
    if (ipsmgw_fqdn) {
    if (!cJSON_IsString(ipsmgw_fqdn) && !cJSON_IsNull(ipsmgw_fqdn)) {
        ogs_error("OpenAPI_ip_sm_gw_registration_parseFromJSON() failed [ipsmgw_fqdn]");
        goto end;
    }
    }

    nf_instance_id = cJSON_GetObjectItemCaseSensitive(ip_sm_gw_registrationJSON, "nfInstanceId");
    if (nf_instance_id) {
    if (!cJSON_IsString(nf_instance_id) && !cJSON_IsNull(nf_instance_id)) {
        ogs_error("OpenAPI_ip_sm_gw_registration_parseFromJSON() failed [nf_instance_id]");
        goto end;
    }
    }

    unri_indicator = cJSON_GetObjectItemCaseSensitive(ip_sm_gw_registrationJSON, "unriIndicator");
    if (unri_indicator) {
    if (!cJSON_IsBool(unri_indicator)) {
        ogs_error("OpenAPI_ip_sm_gw_registration_parseFromJSON() failed [unri_indicator]");
        goto end;
    }
    }

    reset_ids = cJSON_GetObjectItemCaseSensitive(ip_sm_gw_registrationJSON, "resetIds");
    if (reset_ids) {
        cJSON *reset_ids_local = NULL;
        if (!cJSON_IsArray(reset_ids)) {
            ogs_error("OpenAPI_ip_sm_gw_registration_parseFromJSON() failed [reset_ids]");
            goto end;
        }

        reset_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(reset_ids_local, reset_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(reset_ids_local)) {
                ogs_error("OpenAPI_ip_sm_gw_registration_parseFromJSON() failed [reset_ids]");
                goto end;
            }
            OpenAPI_list_add(reset_idsList, ogs_strdup(reset_ids_local->valuestring));
        }
    }

    ip_sm_gw_sbi_sup_ind = cJSON_GetObjectItemCaseSensitive(ip_sm_gw_registrationJSON, "ipSmGwSbiSupInd");
    if (ip_sm_gw_sbi_sup_ind) {
    if (!cJSON_IsBool(ip_sm_gw_sbi_sup_ind)) {
        ogs_error("OpenAPI_ip_sm_gw_registration_parseFromJSON() failed [ip_sm_gw_sbi_sup_ind]");
        goto end;
    }
    }

    ip_sm_gw_registration_local_var = OpenAPI_ip_sm_gw_registration_create (
        ip_sm_gw_map_address && !cJSON_IsNull(ip_sm_gw_map_address) ? ogs_strdup(ip_sm_gw_map_address->valuestring) : NULL,
        ip_sm_gw_diameter_address ? ip_sm_gw_diameter_address_local_nonprim : NULL,
        ipsmgw_ipv4 && !cJSON_IsNull(ipsmgw_ipv4) ? ogs_strdup(ipsmgw_ipv4->valuestring) : NULL,
        ipsmgw_ipv6 && !cJSON_IsNull(ipsmgw_ipv6) ? ogs_strdup(ipsmgw_ipv6->valuestring) : NULL,
        ipsmgw_fqdn && !cJSON_IsNull(ipsmgw_fqdn) ? ogs_strdup(ipsmgw_fqdn->valuestring) : NULL,
        nf_instance_id && !cJSON_IsNull(nf_instance_id) ? ogs_strdup(nf_instance_id->valuestring) : NULL,
        unri_indicator ? true : false,
        unri_indicator ? unri_indicator->valueint : 0,
        reset_ids ? reset_idsList : NULL,
        ip_sm_gw_sbi_sup_ind ? true : false,
        ip_sm_gw_sbi_sup_ind ? ip_sm_gw_sbi_sup_ind->valueint : 0
    );

    return ip_sm_gw_registration_local_var;
end:
    if (ip_sm_gw_diameter_address_local_nonprim) {
        OpenAPI_network_node_diameter_address_free(ip_sm_gw_diameter_address_local_nonprim);
        ip_sm_gw_diameter_address_local_nonprim = NULL;
    }
    if (reset_idsList) {
        OpenAPI_list_for_each(reset_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(reset_idsList);
        reset_idsList = NULL;
    }
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

