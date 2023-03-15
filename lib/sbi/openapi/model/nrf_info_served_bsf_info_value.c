
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nrf_info_served_bsf_info_value.h"

OpenAPI_nrf_info_served_bsf_info_value_t *OpenAPI_nrf_info_served_bsf_info_value_create(
    OpenAPI_list_t *dnn_list,
    OpenAPI_list_t *ip_domain_list,
    OpenAPI_list_t *ipv4_address_ranges,
    OpenAPI_list_t *ipv6_prefix_ranges,
    char *rx_diam_host,
    char *rx_diam_realm,
    char *group_id,
    OpenAPI_list_t *supi_ranges,
    OpenAPI_list_t *gpsi_ranges
)
{
    OpenAPI_nrf_info_served_bsf_info_value_t *nrf_info_served_bsf_info_value_local_var = ogs_malloc(sizeof(OpenAPI_nrf_info_served_bsf_info_value_t));
    ogs_assert(nrf_info_served_bsf_info_value_local_var);

    nrf_info_served_bsf_info_value_local_var->dnn_list = dnn_list;
    nrf_info_served_bsf_info_value_local_var->ip_domain_list = ip_domain_list;
    nrf_info_served_bsf_info_value_local_var->ipv4_address_ranges = ipv4_address_ranges;
    nrf_info_served_bsf_info_value_local_var->ipv6_prefix_ranges = ipv6_prefix_ranges;
    nrf_info_served_bsf_info_value_local_var->rx_diam_host = rx_diam_host;
    nrf_info_served_bsf_info_value_local_var->rx_diam_realm = rx_diam_realm;
    nrf_info_served_bsf_info_value_local_var->group_id = group_id;
    nrf_info_served_bsf_info_value_local_var->supi_ranges = supi_ranges;
    nrf_info_served_bsf_info_value_local_var->gpsi_ranges = gpsi_ranges;

    return nrf_info_served_bsf_info_value_local_var;
}

void OpenAPI_nrf_info_served_bsf_info_value_free(OpenAPI_nrf_info_served_bsf_info_value_t *nrf_info_served_bsf_info_value)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == nrf_info_served_bsf_info_value) {
        return;
    }
    if (nrf_info_served_bsf_info_value->dnn_list) {
        OpenAPI_list_for_each(nrf_info_served_bsf_info_value->dnn_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(nrf_info_served_bsf_info_value->dnn_list);
        nrf_info_served_bsf_info_value->dnn_list = NULL;
    }
    if (nrf_info_served_bsf_info_value->ip_domain_list) {
        OpenAPI_list_for_each(nrf_info_served_bsf_info_value->ip_domain_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(nrf_info_served_bsf_info_value->ip_domain_list);
        nrf_info_served_bsf_info_value->ip_domain_list = NULL;
    }
    if (nrf_info_served_bsf_info_value->ipv4_address_ranges) {
        OpenAPI_list_for_each(nrf_info_served_bsf_info_value->ipv4_address_ranges, node) {
            OpenAPI_ipv4_address_range_free(node->data);
        }
        OpenAPI_list_free(nrf_info_served_bsf_info_value->ipv4_address_ranges);
        nrf_info_served_bsf_info_value->ipv4_address_ranges = NULL;
    }
    if (nrf_info_served_bsf_info_value->ipv6_prefix_ranges) {
        OpenAPI_list_for_each(nrf_info_served_bsf_info_value->ipv6_prefix_ranges, node) {
            OpenAPI_ipv6_prefix_range_free(node->data);
        }
        OpenAPI_list_free(nrf_info_served_bsf_info_value->ipv6_prefix_ranges);
        nrf_info_served_bsf_info_value->ipv6_prefix_ranges = NULL;
    }
    if (nrf_info_served_bsf_info_value->rx_diam_host) {
        ogs_free(nrf_info_served_bsf_info_value->rx_diam_host);
        nrf_info_served_bsf_info_value->rx_diam_host = NULL;
    }
    if (nrf_info_served_bsf_info_value->rx_diam_realm) {
        ogs_free(nrf_info_served_bsf_info_value->rx_diam_realm);
        nrf_info_served_bsf_info_value->rx_diam_realm = NULL;
    }
    if (nrf_info_served_bsf_info_value->group_id) {
        ogs_free(nrf_info_served_bsf_info_value->group_id);
        nrf_info_served_bsf_info_value->group_id = NULL;
    }
    if (nrf_info_served_bsf_info_value->supi_ranges) {
        OpenAPI_list_for_each(nrf_info_served_bsf_info_value->supi_ranges, node) {
            OpenAPI_supi_range_free(node->data);
        }
        OpenAPI_list_free(nrf_info_served_bsf_info_value->supi_ranges);
        nrf_info_served_bsf_info_value->supi_ranges = NULL;
    }
    if (nrf_info_served_bsf_info_value->gpsi_ranges) {
        OpenAPI_list_for_each(nrf_info_served_bsf_info_value->gpsi_ranges, node) {
            OpenAPI_identity_range_free(node->data);
        }
        OpenAPI_list_free(nrf_info_served_bsf_info_value->gpsi_ranges);
        nrf_info_served_bsf_info_value->gpsi_ranges = NULL;
    }
    ogs_free(nrf_info_served_bsf_info_value);
}

cJSON *OpenAPI_nrf_info_served_bsf_info_value_convertToJSON(OpenAPI_nrf_info_served_bsf_info_value_t *nrf_info_served_bsf_info_value)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (nrf_info_served_bsf_info_value == NULL) {
        ogs_error("OpenAPI_nrf_info_served_bsf_info_value_convertToJSON() failed [NrfInfo_servedBsfInfo_value]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (nrf_info_served_bsf_info_value->dnn_list) {
    cJSON *dnn_listList = cJSON_AddArrayToObject(item, "dnnList");
    if (dnn_listList == NULL) {
        ogs_error("OpenAPI_nrf_info_served_bsf_info_value_convertToJSON() failed [dnn_list]");
        goto end;
    }
    OpenAPI_list_for_each(nrf_info_served_bsf_info_value->dnn_list, node) {
        if (cJSON_AddStringToObject(dnn_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_nrf_info_served_bsf_info_value_convertToJSON() failed [dnn_list]");
            goto end;
        }
    }
    }

    if (nrf_info_served_bsf_info_value->ip_domain_list) {
    cJSON *ip_domain_listList = cJSON_AddArrayToObject(item, "ipDomainList");
    if (ip_domain_listList == NULL) {
        ogs_error("OpenAPI_nrf_info_served_bsf_info_value_convertToJSON() failed [ip_domain_list]");
        goto end;
    }
    OpenAPI_list_for_each(nrf_info_served_bsf_info_value->ip_domain_list, node) {
        if (cJSON_AddStringToObject(ip_domain_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_nrf_info_served_bsf_info_value_convertToJSON() failed [ip_domain_list]");
            goto end;
        }
    }
    }

    if (nrf_info_served_bsf_info_value->ipv4_address_ranges) {
    cJSON *ipv4_address_rangesList = cJSON_AddArrayToObject(item, "ipv4AddressRanges");
    if (ipv4_address_rangesList == NULL) {
        ogs_error("OpenAPI_nrf_info_served_bsf_info_value_convertToJSON() failed [ipv4_address_ranges]");
        goto end;
    }
    OpenAPI_list_for_each(nrf_info_served_bsf_info_value->ipv4_address_ranges, node) {
        cJSON *itemLocal = OpenAPI_ipv4_address_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nrf_info_served_bsf_info_value_convertToJSON() failed [ipv4_address_ranges]");
            goto end;
        }
        cJSON_AddItemToArray(ipv4_address_rangesList, itemLocal);
    }
    }

    if (nrf_info_served_bsf_info_value->ipv6_prefix_ranges) {
    cJSON *ipv6_prefix_rangesList = cJSON_AddArrayToObject(item, "ipv6PrefixRanges");
    if (ipv6_prefix_rangesList == NULL) {
        ogs_error("OpenAPI_nrf_info_served_bsf_info_value_convertToJSON() failed [ipv6_prefix_ranges]");
        goto end;
    }
    OpenAPI_list_for_each(nrf_info_served_bsf_info_value->ipv6_prefix_ranges, node) {
        cJSON *itemLocal = OpenAPI_ipv6_prefix_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nrf_info_served_bsf_info_value_convertToJSON() failed [ipv6_prefix_ranges]");
            goto end;
        }
        cJSON_AddItemToArray(ipv6_prefix_rangesList, itemLocal);
    }
    }

    if (nrf_info_served_bsf_info_value->rx_diam_host) {
    if (cJSON_AddStringToObject(item, "rxDiamHost", nrf_info_served_bsf_info_value->rx_diam_host) == NULL) {
        ogs_error("OpenAPI_nrf_info_served_bsf_info_value_convertToJSON() failed [rx_diam_host]");
        goto end;
    }
    }

    if (nrf_info_served_bsf_info_value->rx_diam_realm) {
    if (cJSON_AddStringToObject(item, "rxDiamRealm", nrf_info_served_bsf_info_value->rx_diam_realm) == NULL) {
        ogs_error("OpenAPI_nrf_info_served_bsf_info_value_convertToJSON() failed [rx_diam_realm]");
        goto end;
    }
    }

    if (nrf_info_served_bsf_info_value->group_id) {
    if (cJSON_AddStringToObject(item, "groupId", nrf_info_served_bsf_info_value->group_id) == NULL) {
        ogs_error("OpenAPI_nrf_info_served_bsf_info_value_convertToJSON() failed [group_id]");
        goto end;
    }
    }

    if (nrf_info_served_bsf_info_value->supi_ranges) {
    cJSON *supi_rangesList = cJSON_AddArrayToObject(item, "supiRanges");
    if (supi_rangesList == NULL) {
        ogs_error("OpenAPI_nrf_info_served_bsf_info_value_convertToJSON() failed [supi_ranges]");
        goto end;
    }
    OpenAPI_list_for_each(nrf_info_served_bsf_info_value->supi_ranges, node) {
        cJSON *itemLocal = OpenAPI_supi_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nrf_info_served_bsf_info_value_convertToJSON() failed [supi_ranges]");
            goto end;
        }
        cJSON_AddItemToArray(supi_rangesList, itemLocal);
    }
    }

    if (nrf_info_served_bsf_info_value->gpsi_ranges) {
    cJSON *gpsi_rangesList = cJSON_AddArrayToObject(item, "gpsiRanges");
    if (gpsi_rangesList == NULL) {
        ogs_error("OpenAPI_nrf_info_served_bsf_info_value_convertToJSON() failed [gpsi_ranges]");
        goto end;
    }
    OpenAPI_list_for_each(nrf_info_served_bsf_info_value->gpsi_ranges, node) {
        cJSON *itemLocal = OpenAPI_identity_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nrf_info_served_bsf_info_value_convertToJSON() failed [gpsi_ranges]");
            goto end;
        }
        cJSON_AddItemToArray(gpsi_rangesList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_nrf_info_served_bsf_info_value_t *OpenAPI_nrf_info_served_bsf_info_value_parseFromJSON(cJSON *nrf_info_served_bsf_info_valueJSON)
{
    OpenAPI_nrf_info_served_bsf_info_value_t *nrf_info_served_bsf_info_value_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *dnn_list = NULL;
    OpenAPI_list_t *dnn_listList = NULL;
    cJSON *ip_domain_list = NULL;
    OpenAPI_list_t *ip_domain_listList = NULL;
    cJSON *ipv4_address_ranges = NULL;
    OpenAPI_list_t *ipv4_address_rangesList = NULL;
    cJSON *ipv6_prefix_ranges = NULL;
    OpenAPI_list_t *ipv6_prefix_rangesList = NULL;
    cJSON *rx_diam_host = NULL;
    cJSON *rx_diam_realm = NULL;
    cJSON *group_id = NULL;
    cJSON *supi_ranges = NULL;
    OpenAPI_list_t *supi_rangesList = NULL;
    cJSON *gpsi_ranges = NULL;
    OpenAPI_list_t *gpsi_rangesList = NULL;
    dnn_list = cJSON_GetObjectItemCaseSensitive(nrf_info_served_bsf_info_valueJSON, "dnnList");
    if (dnn_list) {
        cJSON *dnn_list_local = NULL;
        if (!cJSON_IsArray(dnn_list)) {
            ogs_error("OpenAPI_nrf_info_served_bsf_info_value_parseFromJSON() failed [dnn_list]");
            goto end;
        }

        dnn_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(dnn_list_local, dnn_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(dnn_list_local)) {
                ogs_error("OpenAPI_nrf_info_served_bsf_info_value_parseFromJSON() failed [dnn_list]");
                goto end;
            }
            OpenAPI_list_add(dnn_listList, ogs_strdup(dnn_list_local->valuestring));
        }
    }

    ip_domain_list = cJSON_GetObjectItemCaseSensitive(nrf_info_served_bsf_info_valueJSON, "ipDomainList");
    if (ip_domain_list) {
        cJSON *ip_domain_list_local = NULL;
        if (!cJSON_IsArray(ip_domain_list)) {
            ogs_error("OpenAPI_nrf_info_served_bsf_info_value_parseFromJSON() failed [ip_domain_list]");
            goto end;
        }

        ip_domain_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(ip_domain_list_local, ip_domain_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(ip_domain_list_local)) {
                ogs_error("OpenAPI_nrf_info_served_bsf_info_value_parseFromJSON() failed [ip_domain_list]");
                goto end;
            }
            OpenAPI_list_add(ip_domain_listList, ogs_strdup(ip_domain_list_local->valuestring));
        }
    }

    ipv4_address_ranges = cJSON_GetObjectItemCaseSensitive(nrf_info_served_bsf_info_valueJSON, "ipv4AddressRanges");
    if (ipv4_address_ranges) {
        cJSON *ipv4_address_ranges_local = NULL;
        if (!cJSON_IsArray(ipv4_address_ranges)) {
            ogs_error("OpenAPI_nrf_info_served_bsf_info_value_parseFromJSON() failed [ipv4_address_ranges]");
            goto end;
        }

        ipv4_address_rangesList = OpenAPI_list_create();

        cJSON_ArrayForEach(ipv4_address_ranges_local, ipv4_address_ranges) {
            if (!cJSON_IsObject(ipv4_address_ranges_local)) {
                ogs_error("OpenAPI_nrf_info_served_bsf_info_value_parseFromJSON() failed [ipv4_address_ranges]");
                goto end;
            }
            OpenAPI_ipv4_address_range_t *ipv4_address_rangesItem = OpenAPI_ipv4_address_range_parseFromJSON(ipv4_address_ranges_local);
            if (!ipv4_address_rangesItem) {
                ogs_error("No ipv4_address_rangesItem");
                goto end;
            }
            OpenAPI_list_add(ipv4_address_rangesList, ipv4_address_rangesItem);
        }
    }

    ipv6_prefix_ranges = cJSON_GetObjectItemCaseSensitive(nrf_info_served_bsf_info_valueJSON, "ipv6PrefixRanges");
    if (ipv6_prefix_ranges) {
        cJSON *ipv6_prefix_ranges_local = NULL;
        if (!cJSON_IsArray(ipv6_prefix_ranges)) {
            ogs_error("OpenAPI_nrf_info_served_bsf_info_value_parseFromJSON() failed [ipv6_prefix_ranges]");
            goto end;
        }

        ipv6_prefix_rangesList = OpenAPI_list_create();

        cJSON_ArrayForEach(ipv6_prefix_ranges_local, ipv6_prefix_ranges) {
            if (!cJSON_IsObject(ipv6_prefix_ranges_local)) {
                ogs_error("OpenAPI_nrf_info_served_bsf_info_value_parseFromJSON() failed [ipv6_prefix_ranges]");
                goto end;
            }
            OpenAPI_ipv6_prefix_range_t *ipv6_prefix_rangesItem = OpenAPI_ipv6_prefix_range_parseFromJSON(ipv6_prefix_ranges_local);
            if (!ipv6_prefix_rangesItem) {
                ogs_error("No ipv6_prefix_rangesItem");
                goto end;
            }
            OpenAPI_list_add(ipv6_prefix_rangesList, ipv6_prefix_rangesItem);
        }
    }

    rx_diam_host = cJSON_GetObjectItemCaseSensitive(nrf_info_served_bsf_info_valueJSON, "rxDiamHost");
    if (rx_diam_host) {
    if (!cJSON_IsString(rx_diam_host) && !cJSON_IsNull(rx_diam_host)) {
        ogs_error("OpenAPI_nrf_info_served_bsf_info_value_parseFromJSON() failed [rx_diam_host]");
        goto end;
    }
    }

    rx_diam_realm = cJSON_GetObjectItemCaseSensitive(nrf_info_served_bsf_info_valueJSON, "rxDiamRealm");
    if (rx_diam_realm) {
    if (!cJSON_IsString(rx_diam_realm) && !cJSON_IsNull(rx_diam_realm)) {
        ogs_error("OpenAPI_nrf_info_served_bsf_info_value_parseFromJSON() failed [rx_diam_realm]");
        goto end;
    }
    }

    group_id = cJSON_GetObjectItemCaseSensitive(nrf_info_served_bsf_info_valueJSON, "groupId");
    if (group_id) {
    if (!cJSON_IsString(group_id) && !cJSON_IsNull(group_id)) {
        ogs_error("OpenAPI_nrf_info_served_bsf_info_value_parseFromJSON() failed [group_id]");
        goto end;
    }
    }

    supi_ranges = cJSON_GetObjectItemCaseSensitive(nrf_info_served_bsf_info_valueJSON, "supiRanges");
    if (supi_ranges) {
        cJSON *supi_ranges_local = NULL;
        if (!cJSON_IsArray(supi_ranges)) {
            ogs_error("OpenAPI_nrf_info_served_bsf_info_value_parseFromJSON() failed [supi_ranges]");
            goto end;
        }

        supi_rangesList = OpenAPI_list_create();

        cJSON_ArrayForEach(supi_ranges_local, supi_ranges) {
            if (!cJSON_IsObject(supi_ranges_local)) {
                ogs_error("OpenAPI_nrf_info_served_bsf_info_value_parseFromJSON() failed [supi_ranges]");
                goto end;
            }
            OpenAPI_supi_range_t *supi_rangesItem = OpenAPI_supi_range_parseFromJSON(supi_ranges_local);
            if (!supi_rangesItem) {
                ogs_error("No supi_rangesItem");
                goto end;
            }
            OpenAPI_list_add(supi_rangesList, supi_rangesItem);
        }
    }

    gpsi_ranges = cJSON_GetObjectItemCaseSensitive(nrf_info_served_bsf_info_valueJSON, "gpsiRanges");
    if (gpsi_ranges) {
        cJSON *gpsi_ranges_local = NULL;
        if (!cJSON_IsArray(gpsi_ranges)) {
            ogs_error("OpenAPI_nrf_info_served_bsf_info_value_parseFromJSON() failed [gpsi_ranges]");
            goto end;
        }

        gpsi_rangesList = OpenAPI_list_create();

        cJSON_ArrayForEach(gpsi_ranges_local, gpsi_ranges) {
            if (!cJSON_IsObject(gpsi_ranges_local)) {
                ogs_error("OpenAPI_nrf_info_served_bsf_info_value_parseFromJSON() failed [gpsi_ranges]");
                goto end;
            }
            OpenAPI_identity_range_t *gpsi_rangesItem = OpenAPI_identity_range_parseFromJSON(gpsi_ranges_local);
            if (!gpsi_rangesItem) {
                ogs_error("No gpsi_rangesItem");
                goto end;
            }
            OpenAPI_list_add(gpsi_rangesList, gpsi_rangesItem);
        }
    }

    nrf_info_served_bsf_info_value_local_var = OpenAPI_nrf_info_served_bsf_info_value_create (
        dnn_list ? dnn_listList : NULL,
        ip_domain_list ? ip_domain_listList : NULL,
        ipv4_address_ranges ? ipv4_address_rangesList : NULL,
        ipv6_prefix_ranges ? ipv6_prefix_rangesList : NULL,
        rx_diam_host && !cJSON_IsNull(rx_diam_host) ? ogs_strdup(rx_diam_host->valuestring) : NULL,
        rx_diam_realm && !cJSON_IsNull(rx_diam_realm) ? ogs_strdup(rx_diam_realm->valuestring) : NULL,
        group_id && !cJSON_IsNull(group_id) ? ogs_strdup(group_id->valuestring) : NULL,
        supi_ranges ? supi_rangesList : NULL,
        gpsi_ranges ? gpsi_rangesList : NULL
    );

    return nrf_info_served_bsf_info_value_local_var;
end:
    if (dnn_listList) {
        OpenAPI_list_for_each(dnn_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(dnn_listList);
        dnn_listList = NULL;
    }
    if (ip_domain_listList) {
        OpenAPI_list_for_each(ip_domain_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ip_domain_listList);
        ip_domain_listList = NULL;
    }
    if (ipv4_address_rangesList) {
        OpenAPI_list_for_each(ipv4_address_rangesList, node) {
            OpenAPI_ipv4_address_range_free(node->data);
        }
        OpenAPI_list_free(ipv4_address_rangesList);
        ipv4_address_rangesList = NULL;
    }
    if (ipv6_prefix_rangesList) {
        OpenAPI_list_for_each(ipv6_prefix_rangesList, node) {
            OpenAPI_ipv6_prefix_range_free(node->data);
        }
        OpenAPI_list_free(ipv6_prefix_rangesList);
        ipv6_prefix_rangesList = NULL;
    }
    if (supi_rangesList) {
        OpenAPI_list_for_each(supi_rangesList, node) {
            OpenAPI_supi_range_free(node->data);
        }
        OpenAPI_list_free(supi_rangesList);
        supi_rangesList = NULL;
    }
    if (gpsi_rangesList) {
        OpenAPI_list_for_each(gpsi_rangesList, node) {
            OpenAPI_identity_range_free(node->data);
        }
        OpenAPI_list_free(gpsi_rangesList);
        gpsi_rangesList = NULL;
    }
    return NULL;
}

OpenAPI_nrf_info_served_bsf_info_value_t *OpenAPI_nrf_info_served_bsf_info_value_copy(OpenAPI_nrf_info_served_bsf_info_value_t *dst, OpenAPI_nrf_info_served_bsf_info_value_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nrf_info_served_bsf_info_value_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nrf_info_served_bsf_info_value_convertToJSON() failed");
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

    OpenAPI_nrf_info_served_bsf_info_value_free(dst);
    dst = OpenAPI_nrf_info_served_bsf_info_value_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

