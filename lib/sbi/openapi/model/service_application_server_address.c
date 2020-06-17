
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "service_application_server_address.h"

OpenAPI_service_application_server_address_t *OpenAPI_service_application_server_address_create(
    OpenAPI_list_t *ser_ids,
    char *fqdn,
    OpenAPI_list_t *ipv4_addrs,
    OpenAPI_list_t *ipv6_addrs,
    OpenAPI_list_t *udp_port_numbers,
    OpenAPI_list_t *tcp_port_numbers,
    OpenAPI_list_t *plmns,
    char *geopraphical_area
    )
{
    OpenAPI_service_application_server_address_t *service_application_server_address_local_var = OpenAPI_malloc(sizeof(OpenAPI_service_application_server_address_t));
    if (!service_application_server_address_local_var) {
        return NULL;
    }
    service_application_server_address_local_var->ser_ids = ser_ids;
    service_application_server_address_local_var->fqdn = fqdn;
    service_application_server_address_local_var->ipv4_addrs = ipv4_addrs;
    service_application_server_address_local_var->ipv6_addrs = ipv6_addrs;
    service_application_server_address_local_var->udp_port_numbers = udp_port_numbers;
    service_application_server_address_local_var->tcp_port_numbers = tcp_port_numbers;
    service_application_server_address_local_var->plmns = plmns;
    service_application_server_address_local_var->geopraphical_area = geopraphical_area;

    return service_application_server_address_local_var;
}

void OpenAPI_service_application_server_address_free(OpenAPI_service_application_server_address_t *service_application_server_address)
{
    if (NULL == service_application_server_address) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(service_application_server_address->ser_ids, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(service_application_server_address->ser_ids);
    ogs_free(service_application_server_address->fqdn);
    OpenAPI_list_for_each(service_application_server_address->ipv4_addrs, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(service_application_server_address->ipv4_addrs);
    OpenAPI_list_for_each(service_application_server_address->ipv6_addrs, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(service_application_server_address->ipv6_addrs);
    OpenAPI_list_for_each(service_application_server_address->udp_port_numbers, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(service_application_server_address->udp_port_numbers);
    OpenAPI_list_for_each(service_application_server_address->tcp_port_numbers, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(service_application_server_address->tcp_port_numbers);
    OpenAPI_list_for_each(service_application_server_address->plmns, node) {
        OpenAPI_plmn_id_free(node->data);
    }
    OpenAPI_list_free(service_application_server_address->plmns);
    ogs_free(service_application_server_address->geopraphical_area);
    ogs_free(service_application_server_address);
}

cJSON *OpenAPI_service_application_server_address_convertToJSON(OpenAPI_service_application_server_address_t *service_application_server_address)
{
    cJSON *item = NULL;

    if (service_application_server_address == NULL) {
        ogs_error("OpenAPI_service_application_server_address_convertToJSON() failed [ServiceApplicationServerAddress]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!service_application_server_address->ser_ids) {
        ogs_error("OpenAPI_service_application_server_address_convertToJSON() failed [ser_ids]");
        goto end;
    }
    cJSON *ser_ids = cJSON_AddArrayToObject(item, "serIds");
    if (ser_ids == NULL) {
        ogs_error("OpenAPI_service_application_server_address_convertToJSON() failed [ser_ids]");
        goto end;
    }

    OpenAPI_lnode_t *ser_ids_node;
    OpenAPI_list_for_each(service_application_server_address->ser_ids, ser_ids_node)  {
        if (cJSON_AddStringToObject(ser_ids, "", (char*)ser_ids_node->data) == NULL) {
            ogs_error("OpenAPI_service_application_server_address_convertToJSON() failed [ser_ids]");
            goto end;
        }
    }

    if (service_application_server_address->fqdn) {
        if (cJSON_AddStringToObject(item, "fqdn", service_application_server_address->fqdn) == NULL) {
            ogs_error("OpenAPI_service_application_server_address_convertToJSON() failed [fqdn]");
            goto end;
        }
    }

    if (service_application_server_address->ipv4_addrs) {
        cJSON *ipv4_addrs = cJSON_AddArrayToObject(item, "Ipv4Addrs");
        if (ipv4_addrs == NULL) {
            ogs_error("OpenAPI_service_application_server_address_convertToJSON() failed [ipv4_addrs]");
            goto end;
        }

        OpenAPI_lnode_t *ipv4_addrs_node;
        OpenAPI_list_for_each(service_application_server_address->ipv4_addrs, ipv4_addrs_node)  {
            if (cJSON_AddStringToObject(ipv4_addrs, "", (char*)ipv4_addrs_node->data) == NULL) {
                ogs_error("OpenAPI_service_application_server_address_convertToJSON() failed [ipv4_addrs]");
                goto end;
            }
        }
    }

    if (service_application_server_address->ipv6_addrs) {
        cJSON *ipv6_addrs = cJSON_AddArrayToObject(item, "Ipv6Addrs");
        if (ipv6_addrs == NULL) {
            ogs_error("OpenAPI_service_application_server_address_convertToJSON() failed [ipv6_addrs]");
            goto end;
        }

        OpenAPI_lnode_t *ipv6_addrs_node;
        OpenAPI_list_for_each(service_application_server_address->ipv6_addrs, ipv6_addrs_node)  {
            if (cJSON_AddStringToObject(ipv6_addrs, "", (char*)ipv6_addrs_node->data) == NULL) {
                ogs_error("OpenAPI_service_application_server_address_convertToJSON() failed [ipv6_addrs]");
                goto end;
            }
        }
    }

    if (service_application_server_address->udp_port_numbers) {
        cJSON *udp_port_numbers = cJSON_AddArrayToObject(item, "udpPortNumbers");
        if (udp_port_numbers == NULL) {
            ogs_error("OpenAPI_service_application_server_address_convertToJSON() failed [udp_port_numbers]");
            goto end;
        }

        OpenAPI_lnode_t *udp_port_numbers_node;
        OpenAPI_list_for_each(service_application_server_address->udp_port_numbers, udp_port_numbers_node)  {
            if (cJSON_AddNumberToObject(udp_port_numbers, "", *(double *)udp_port_numbers_node->data) == NULL) {
                ogs_error("OpenAPI_service_application_server_address_convertToJSON() failed [udp_port_numbers]");
                goto end;
            }
        }
    }

    if (service_application_server_address->tcp_port_numbers) {
        cJSON *tcp_port_numbers = cJSON_AddArrayToObject(item, "tcpPortNumbers");
        if (tcp_port_numbers == NULL) {
            ogs_error("OpenAPI_service_application_server_address_convertToJSON() failed [tcp_port_numbers]");
            goto end;
        }

        OpenAPI_lnode_t *tcp_port_numbers_node;
        OpenAPI_list_for_each(service_application_server_address->tcp_port_numbers, tcp_port_numbers_node)  {
            if (cJSON_AddNumberToObject(tcp_port_numbers, "", *(double *)tcp_port_numbers_node->data) == NULL) {
                ogs_error("OpenAPI_service_application_server_address_convertToJSON() failed [tcp_port_numbers]");
                goto end;
            }
        }
    }

    if (!service_application_server_address->plmns) {
        ogs_error("OpenAPI_service_application_server_address_convertToJSON() failed [plmns]");
        goto end;
    }
    cJSON *plmnsList = cJSON_AddArrayToObject(item, "plmns");
    if (plmnsList == NULL) {
        ogs_error("OpenAPI_service_application_server_address_convertToJSON() failed [plmns]");
        goto end;
    }

    OpenAPI_lnode_t *plmns_node;
    if (service_application_server_address->plmns) {
        OpenAPI_list_for_each(service_application_server_address->plmns, plmns_node) {
            cJSON *itemLocal = OpenAPI_plmn_id_convertToJSON(plmns_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_service_application_server_address_convertToJSON() failed [plmns]");
                goto end;
            }
            cJSON_AddItemToArray(plmnsList, itemLocal);
        }
    }

    if (service_application_server_address->geopraphical_area) {
        if (cJSON_AddStringToObject(item, "geopraphicalArea", service_application_server_address->geopraphical_area) == NULL) {
            ogs_error("OpenAPI_service_application_server_address_convertToJSON() failed [geopraphical_area]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_service_application_server_address_t *OpenAPI_service_application_server_address_parseFromJSON(cJSON *service_application_server_addressJSON)
{
    OpenAPI_service_application_server_address_t *service_application_server_address_local_var = NULL;
    cJSON *ser_ids = cJSON_GetObjectItemCaseSensitive(service_application_server_addressJSON, "serIds");
    if (!ser_ids) {
        ogs_error("OpenAPI_service_application_server_address_parseFromJSON() failed [ser_ids]");
        goto end;
    }

    OpenAPI_list_t *ser_idsList;

    cJSON *ser_ids_local;
    if (!cJSON_IsArray(ser_ids)) {
        ogs_error("OpenAPI_service_application_server_address_parseFromJSON() failed [ser_ids]");
        goto end;
    }
    ser_idsList = OpenAPI_list_create();

    cJSON_ArrayForEach(ser_ids_local, ser_ids) {
        if (!cJSON_IsString(ser_ids_local)) {
            ogs_error("OpenAPI_service_application_server_address_parseFromJSON() failed [ser_ids]");
            goto end;
        }
        OpenAPI_list_add(ser_idsList, ogs_strdup(ser_ids_local->valuestring));
    }

    cJSON *fqdn = cJSON_GetObjectItemCaseSensitive(service_application_server_addressJSON, "fqdn");

    if (fqdn) {
        if (!cJSON_IsString(fqdn)) {
            ogs_error("OpenAPI_service_application_server_address_parseFromJSON() failed [fqdn]");
            goto end;
        }
    }

    cJSON *ipv4_addrs = cJSON_GetObjectItemCaseSensitive(service_application_server_addressJSON, "Ipv4Addrs");

    OpenAPI_list_t *ipv4_addrsList;
    if (ipv4_addrs) {
        cJSON *ipv4_addrs_local;
        if (!cJSON_IsArray(ipv4_addrs)) {
            ogs_error("OpenAPI_service_application_server_address_parseFromJSON() failed [ipv4_addrs]");
            goto end;
        }
        ipv4_addrsList = OpenAPI_list_create();

        cJSON_ArrayForEach(ipv4_addrs_local, ipv4_addrs) {
            if (!cJSON_IsString(ipv4_addrs_local)) {
                ogs_error("OpenAPI_service_application_server_address_parseFromJSON() failed [ipv4_addrs]");
                goto end;
            }
            OpenAPI_list_add(ipv4_addrsList, ogs_strdup(ipv4_addrs_local->valuestring));
        }
    }

    cJSON *ipv6_addrs = cJSON_GetObjectItemCaseSensitive(service_application_server_addressJSON, "Ipv6Addrs");

    OpenAPI_list_t *ipv6_addrsList;
    if (ipv6_addrs) {
        cJSON *ipv6_addrs_local;
        if (!cJSON_IsArray(ipv6_addrs)) {
            ogs_error("OpenAPI_service_application_server_address_parseFromJSON() failed [ipv6_addrs]");
            goto end;
        }
        ipv6_addrsList = OpenAPI_list_create();

        cJSON_ArrayForEach(ipv6_addrs_local, ipv6_addrs) {
            if (!cJSON_IsString(ipv6_addrs_local)) {
                ogs_error("OpenAPI_service_application_server_address_parseFromJSON() failed [ipv6_addrs]");
                goto end;
            }
            OpenAPI_list_add(ipv6_addrsList, ogs_strdup(ipv6_addrs_local->valuestring));
        }
    }

    cJSON *udp_port_numbers = cJSON_GetObjectItemCaseSensitive(service_application_server_addressJSON, "udpPortNumbers");

    OpenAPI_list_t *udp_port_numbersList;
    if (udp_port_numbers) {
        cJSON *udp_port_numbers_local;
        if (!cJSON_IsArray(udp_port_numbers)) {
            ogs_error("OpenAPI_service_application_server_address_parseFromJSON() failed [udp_port_numbers]");
            goto end;
        }
        udp_port_numbersList = OpenAPI_list_create();

        cJSON_ArrayForEach(udp_port_numbers_local, udp_port_numbers) {
            if (!cJSON_IsNumber(udp_port_numbers_local)) {
                ogs_error("OpenAPI_service_application_server_address_parseFromJSON() failed [udp_port_numbers]");
                goto end;
            }
            OpenAPI_list_add(udp_port_numbersList, &udp_port_numbers_local->valuedouble);
        }
    }

    cJSON *tcp_port_numbers = cJSON_GetObjectItemCaseSensitive(service_application_server_addressJSON, "tcpPortNumbers");

    OpenAPI_list_t *tcp_port_numbersList;
    if (tcp_port_numbers) {
        cJSON *tcp_port_numbers_local;
        if (!cJSON_IsArray(tcp_port_numbers)) {
            ogs_error("OpenAPI_service_application_server_address_parseFromJSON() failed [tcp_port_numbers]");
            goto end;
        }
        tcp_port_numbersList = OpenAPI_list_create();

        cJSON_ArrayForEach(tcp_port_numbers_local, tcp_port_numbers) {
            if (!cJSON_IsNumber(tcp_port_numbers_local)) {
                ogs_error("OpenAPI_service_application_server_address_parseFromJSON() failed [tcp_port_numbers]");
                goto end;
            }
            OpenAPI_list_add(tcp_port_numbersList, &tcp_port_numbers_local->valuedouble);
        }
    }

    cJSON *plmns = cJSON_GetObjectItemCaseSensitive(service_application_server_addressJSON, "plmns");
    if (!plmns) {
        ogs_error("OpenAPI_service_application_server_address_parseFromJSON() failed [plmns]");
        goto end;
    }

    OpenAPI_list_t *plmnsList;

    cJSON *plmns_local_nonprimitive;
    if (!cJSON_IsArray(plmns)) {
        ogs_error("OpenAPI_service_application_server_address_parseFromJSON() failed [plmns]");
        goto end;
    }

    plmnsList = OpenAPI_list_create();

    cJSON_ArrayForEach(plmns_local_nonprimitive, plmns ) {
        if (!cJSON_IsObject(plmns_local_nonprimitive)) {
            ogs_error("OpenAPI_service_application_server_address_parseFromJSON() failed [plmns]");
            goto end;
        }
        OpenAPI_plmn_id_t *plmnsItem = OpenAPI_plmn_id_parseFromJSON(plmns_local_nonprimitive);

        OpenAPI_list_add(plmnsList, plmnsItem);
    }

    cJSON *geopraphical_area = cJSON_GetObjectItemCaseSensitive(service_application_server_addressJSON, "geopraphicalArea");

    if (geopraphical_area) {
        if (!cJSON_IsString(geopraphical_area)) {
            ogs_error("OpenAPI_service_application_server_address_parseFromJSON() failed [geopraphical_area]");
            goto end;
        }
    }

    service_application_server_address_local_var = OpenAPI_service_application_server_address_create (
        ser_idsList,
        fqdn ? ogs_strdup(fqdn->valuestring) : NULL,
        ipv4_addrs ? ipv4_addrsList : NULL,
        ipv6_addrs ? ipv6_addrsList : NULL,
        udp_port_numbers ? udp_port_numbersList : NULL,
        tcp_port_numbers ? tcp_port_numbersList : NULL,
        plmnsList,
        geopraphical_area ? ogs_strdup(geopraphical_area->valuestring) : NULL
        );

    return service_application_server_address_local_var;
end:
    return NULL;
}

OpenAPI_service_application_server_address_t *OpenAPI_service_application_server_address_copy(OpenAPI_service_application_server_address_t *dst, OpenAPI_service_application_server_address_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_service_application_server_address_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_service_application_server_address_convertToJSON() failed");
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

    OpenAPI_service_application_server_address_free(dst);
    dst = OpenAPI_service_application_server_address_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

