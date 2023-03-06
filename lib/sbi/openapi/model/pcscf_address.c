
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pcscf_address.h"

OpenAPI_pcscf_address_t *OpenAPI_pcscf_address_create(
    OpenAPI_list_t *ipv4_addrs,
    OpenAPI_list_t *ipv6_addrs,
    char *fqdn
)
{
    OpenAPI_pcscf_address_t *pcscf_address_local_var = ogs_malloc(sizeof(OpenAPI_pcscf_address_t));
    ogs_assert(pcscf_address_local_var);

    pcscf_address_local_var->ipv4_addrs = ipv4_addrs;
    pcscf_address_local_var->ipv6_addrs = ipv6_addrs;
    pcscf_address_local_var->fqdn = fqdn;

    return pcscf_address_local_var;
}

void OpenAPI_pcscf_address_free(OpenAPI_pcscf_address_t *pcscf_address)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pcscf_address) {
        return;
    }
    if (pcscf_address->ipv4_addrs) {
        OpenAPI_list_for_each(pcscf_address->ipv4_addrs, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(pcscf_address->ipv4_addrs);
        pcscf_address->ipv4_addrs = NULL;
    }
    if (pcscf_address->ipv6_addrs) {
        OpenAPI_list_for_each(pcscf_address->ipv6_addrs, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(pcscf_address->ipv6_addrs);
        pcscf_address->ipv6_addrs = NULL;
    }
    if (pcscf_address->fqdn) {
        ogs_free(pcscf_address->fqdn);
        pcscf_address->fqdn = NULL;
    }
    ogs_free(pcscf_address);
}

cJSON *OpenAPI_pcscf_address_convertToJSON(OpenAPI_pcscf_address_t *pcscf_address)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pcscf_address == NULL) {
        ogs_error("OpenAPI_pcscf_address_convertToJSON() failed [PcscfAddress]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (pcscf_address->ipv4_addrs) {
    cJSON *ipv4_addrsList = cJSON_AddArrayToObject(item, "ipv4Addrs");
    if (ipv4_addrsList == NULL) {
        ogs_error("OpenAPI_pcscf_address_convertToJSON() failed [ipv4_addrs]");
        goto end;
    }
    OpenAPI_list_for_each(pcscf_address->ipv4_addrs, node) {
        if (cJSON_AddStringToObject(ipv4_addrsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_pcscf_address_convertToJSON() failed [ipv4_addrs]");
            goto end;
        }
    }
    }

    if (pcscf_address->ipv6_addrs) {
    cJSON *ipv6_addrsList = cJSON_AddArrayToObject(item, "ipv6Addrs");
    if (ipv6_addrsList == NULL) {
        ogs_error("OpenAPI_pcscf_address_convertToJSON() failed [ipv6_addrs]");
        goto end;
    }
    OpenAPI_list_for_each(pcscf_address->ipv6_addrs, node) {
        if (cJSON_AddStringToObject(ipv6_addrsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_pcscf_address_convertToJSON() failed [ipv6_addrs]");
            goto end;
        }
    }
    }

    if (pcscf_address->fqdn) {
    if (cJSON_AddStringToObject(item, "fqdn", pcscf_address->fqdn) == NULL) {
        ogs_error("OpenAPI_pcscf_address_convertToJSON() failed [fqdn]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_pcscf_address_t *OpenAPI_pcscf_address_parseFromJSON(cJSON *pcscf_addressJSON)
{
    OpenAPI_pcscf_address_t *pcscf_address_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ipv4_addrs = NULL;
    OpenAPI_list_t *ipv4_addrsList = NULL;
    cJSON *ipv6_addrs = NULL;
    OpenAPI_list_t *ipv6_addrsList = NULL;
    cJSON *fqdn = NULL;
    ipv4_addrs = cJSON_GetObjectItemCaseSensitive(pcscf_addressJSON, "ipv4Addrs");
    if (ipv4_addrs) {
        cJSON *ipv4_addrs_local = NULL;
        if (!cJSON_IsArray(ipv4_addrs)) {
            ogs_error("OpenAPI_pcscf_address_parseFromJSON() failed [ipv4_addrs]");
            goto end;
        }

        ipv4_addrsList = OpenAPI_list_create();

        cJSON_ArrayForEach(ipv4_addrs_local, ipv4_addrs) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(ipv4_addrs_local)) {
                ogs_error("OpenAPI_pcscf_address_parseFromJSON() failed [ipv4_addrs]");
                goto end;
            }
            OpenAPI_list_add(ipv4_addrsList, ogs_strdup(ipv4_addrs_local->valuestring));
        }
    }

    ipv6_addrs = cJSON_GetObjectItemCaseSensitive(pcscf_addressJSON, "ipv6Addrs");
    if (ipv6_addrs) {
        cJSON *ipv6_addrs_local = NULL;
        if (!cJSON_IsArray(ipv6_addrs)) {
            ogs_error("OpenAPI_pcscf_address_parseFromJSON() failed [ipv6_addrs]");
            goto end;
        }

        ipv6_addrsList = OpenAPI_list_create();

        cJSON_ArrayForEach(ipv6_addrs_local, ipv6_addrs) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(ipv6_addrs_local)) {
                ogs_error("OpenAPI_pcscf_address_parseFromJSON() failed [ipv6_addrs]");
                goto end;
            }
            OpenAPI_list_add(ipv6_addrsList, ogs_strdup(ipv6_addrs_local->valuestring));
        }
    }

    fqdn = cJSON_GetObjectItemCaseSensitive(pcscf_addressJSON, "fqdn");
    if (fqdn) {
    if (!cJSON_IsString(fqdn) && !cJSON_IsNull(fqdn)) {
        ogs_error("OpenAPI_pcscf_address_parseFromJSON() failed [fqdn]");
        goto end;
    }
    }

    pcscf_address_local_var = OpenAPI_pcscf_address_create (
        ipv4_addrs ? ipv4_addrsList : NULL,
        ipv6_addrs ? ipv6_addrsList : NULL,
        fqdn && !cJSON_IsNull(fqdn) ? ogs_strdup(fqdn->valuestring) : NULL
    );

    return pcscf_address_local_var;
end:
    if (ipv4_addrsList) {
        OpenAPI_list_for_each(ipv4_addrsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ipv4_addrsList);
        ipv4_addrsList = NULL;
    }
    if (ipv6_addrsList) {
        OpenAPI_list_for_each(ipv6_addrsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ipv6_addrsList);
        ipv6_addrsList = NULL;
    }
    return NULL;
}

OpenAPI_pcscf_address_t *OpenAPI_pcscf_address_copy(OpenAPI_pcscf_address_t *dst, OpenAPI_pcscf_address_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pcscf_address_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pcscf_address_convertToJSON() failed");
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

    OpenAPI_pcscf_address_free(dst);
    dst = OpenAPI_pcscf_address_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

