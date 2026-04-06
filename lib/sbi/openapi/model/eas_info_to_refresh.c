
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "eas_info_to_refresh.h"

OpenAPI_eas_info_to_refresh_t *OpenAPI_eas_info_to_refresh_create(
    OpenAPI_list_t *ipv4_address_ranges,
    OpenAPI_list_t *ipv6_address_ranges,
    OpenAPI_list_t *fqdn_list
)
{
    OpenAPI_eas_info_to_refresh_t *eas_info_to_refresh_local_var = ogs_malloc(sizeof(OpenAPI_eas_info_to_refresh_t));
    ogs_assert(eas_info_to_refresh_local_var);

    eas_info_to_refresh_local_var->ipv4_address_ranges = ipv4_address_ranges;
    eas_info_to_refresh_local_var->ipv6_address_ranges = ipv6_address_ranges;
    eas_info_to_refresh_local_var->fqdn_list = fqdn_list;

    return eas_info_to_refresh_local_var;
}

void OpenAPI_eas_info_to_refresh_free(OpenAPI_eas_info_to_refresh_t *eas_info_to_refresh)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == eas_info_to_refresh) {
        return;
    }
    if (eas_info_to_refresh->ipv4_address_ranges) {
        OpenAPI_list_for_each(eas_info_to_refresh->ipv4_address_ranges, node) {
            OpenAPI_ipv4_address_range_free(node->data);
        }
        OpenAPI_list_free(eas_info_to_refresh->ipv4_address_ranges);
        eas_info_to_refresh->ipv4_address_ranges = NULL;
    }
    if (eas_info_to_refresh->ipv6_address_ranges) {
        OpenAPI_list_for_each(eas_info_to_refresh->ipv6_address_ranges, node) {
            OpenAPI_ipv6_address_range_free(node->data);
        }
        OpenAPI_list_free(eas_info_to_refresh->ipv6_address_ranges);
        eas_info_to_refresh->ipv6_address_ranges = NULL;
    }
    if (eas_info_to_refresh->fqdn_list) {
        OpenAPI_list_for_each(eas_info_to_refresh->fqdn_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(eas_info_to_refresh->fqdn_list);
        eas_info_to_refresh->fqdn_list = NULL;
    }
    ogs_free(eas_info_to_refresh);
}

cJSON *OpenAPI_eas_info_to_refresh_convertToJSON(OpenAPI_eas_info_to_refresh_t *eas_info_to_refresh)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (eas_info_to_refresh == NULL) {
        ogs_error("OpenAPI_eas_info_to_refresh_convertToJSON() failed [EasInfoToRefresh]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (eas_info_to_refresh->ipv4_address_ranges) {
    cJSON *ipv4_address_rangesList = cJSON_AddArrayToObject(item, "ipv4AddressRanges");
    if (ipv4_address_rangesList == NULL) {
        ogs_error("OpenAPI_eas_info_to_refresh_convertToJSON() failed [ipv4_address_ranges]");
        goto end;
    }
    OpenAPI_list_for_each(eas_info_to_refresh->ipv4_address_ranges, node) {
        cJSON *itemLocal = OpenAPI_ipv4_address_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_eas_info_to_refresh_convertToJSON() failed [ipv4_address_ranges]");
            goto end;
        }
        cJSON_AddItemToArray(ipv4_address_rangesList, itemLocal);
    }
    }

    if (eas_info_to_refresh->ipv6_address_ranges) {
    cJSON *ipv6_address_rangesList = cJSON_AddArrayToObject(item, "ipv6AddressRanges");
    if (ipv6_address_rangesList == NULL) {
        ogs_error("OpenAPI_eas_info_to_refresh_convertToJSON() failed [ipv6_address_ranges]");
        goto end;
    }
    OpenAPI_list_for_each(eas_info_to_refresh->ipv6_address_ranges, node) {
        cJSON *itemLocal = OpenAPI_ipv6_address_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_eas_info_to_refresh_convertToJSON() failed [ipv6_address_ranges]");
            goto end;
        }
        cJSON_AddItemToArray(ipv6_address_rangesList, itemLocal);
    }
    }

    if (eas_info_to_refresh->fqdn_list) {
    cJSON *fqdn_listList = cJSON_AddArrayToObject(item, "fqdnList");
    if (fqdn_listList == NULL) {
        ogs_error("OpenAPI_eas_info_to_refresh_convertToJSON() failed [fqdn_list]");
        goto end;
    }
    OpenAPI_list_for_each(eas_info_to_refresh->fqdn_list, node) {
        if (cJSON_AddStringToObject(fqdn_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_eas_info_to_refresh_convertToJSON() failed [fqdn_list]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_eas_info_to_refresh_t *OpenAPI_eas_info_to_refresh_parseFromJSON(cJSON *eas_info_to_refreshJSON)
{
    OpenAPI_eas_info_to_refresh_t *eas_info_to_refresh_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ipv4_address_ranges = NULL;
    OpenAPI_list_t *ipv4_address_rangesList = NULL;
    cJSON *ipv6_address_ranges = NULL;
    OpenAPI_list_t *ipv6_address_rangesList = NULL;
    cJSON *fqdn_list = NULL;
    OpenAPI_list_t *fqdn_listList = NULL;
    ipv4_address_ranges = cJSON_GetObjectItemCaseSensitive(eas_info_to_refreshJSON, "ipv4AddressRanges");
    if (ipv4_address_ranges) {
        cJSON *ipv4_address_ranges_local = NULL;
        if (!cJSON_IsArray(ipv4_address_ranges)) {
            ogs_error("OpenAPI_eas_info_to_refresh_parseFromJSON() failed [ipv4_address_ranges]");
            goto end;
        }

        ipv4_address_rangesList = OpenAPI_list_create();

        cJSON_ArrayForEach(ipv4_address_ranges_local, ipv4_address_ranges) {
            if (!cJSON_IsObject(ipv4_address_ranges_local)) {
                ogs_error("OpenAPI_eas_info_to_refresh_parseFromJSON() failed [ipv4_address_ranges]");
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

    ipv6_address_ranges = cJSON_GetObjectItemCaseSensitive(eas_info_to_refreshJSON, "ipv6AddressRanges");
    if (ipv6_address_ranges) {
        cJSON *ipv6_address_ranges_local = NULL;
        if (!cJSON_IsArray(ipv6_address_ranges)) {
            ogs_error("OpenAPI_eas_info_to_refresh_parseFromJSON() failed [ipv6_address_ranges]");
            goto end;
        }

        ipv6_address_rangesList = OpenAPI_list_create();

        cJSON_ArrayForEach(ipv6_address_ranges_local, ipv6_address_ranges) {
            if (!cJSON_IsObject(ipv6_address_ranges_local)) {
                ogs_error("OpenAPI_eas_info_to_refresh_parseFromJSON() failed [ipv6_address_ranges]");
                goto end;
            }
            OpenAPI_ipv6_address_range_t *ipv6_address_rangesItem = OpenAPI_ipv6_address_range_parseFromJSON(ipv6_address_ranges_local);
            if (!ipv6_address_rangesItem) {
                ogs_error("No ipv6_address_rangesItem");
                goto end;
            }
            OpenAPI_list_add(ipv6_address_rangesList, ipv6_address_rangesItem);
        }
    }

    fqdn_list = cJSON_GetObjectItemCaseSensitive(eas_info_to_refreshJSON, "fqdnList");
    if (fqdn_list) {
        cJSON *fqdn_list_local = NULL;
        if (!cJSON_IsArray(fqdn_list)) {
            ogs_error("OpenAPI_eas_info_to_refresh_parseFromJSON() failed [fqdn_list]");
            goto end;
        }

        fqdn_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(fqdn_list_local, fqdn_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(fqdn_list_local)) {
                ogs_error("OpenAPI_eas_info_to_refresh_parseFromJSON() failed [fqdn_list]");
                goto end;
            }
            OpenAPI_list_add(fqdn_listList, ogs_strdup(fqdn_list_local->valuestring));
        }
    }

    eas_info_to_refresh_local_var = OpenAPI_eas_info_to_refresh_create (
        ipv4_address_ranges ? ipv4_address_rangesList : NULL,
        ipv6_address_ranges ? ipv6_address_rangesList : NULL,
        fqdn_list ? fqdn_listList : NULL
    );

    return eas_info_to_refresh_local_var;
end:
    if (ipv4_address_rangesList) {
        OpenAPI_list_for_each(ipv4_address_rangesList, node) {
            OpenAPI_ipv4_address_range_free(node->data);
        }
        OpenAPI_list_free(ipv4_address_rangesList);
        ipv4_address_rangesList = NULL;
    }
    if (ipv6_address_rangesList) {
        OpenAPI_list_for_each(ipv6_address_rangesList, node) {
            OpenAPI_ipv6_address_range_free(node->data);
        }
        OpenAPI_list_free(ipv6_address_rangesList);
        ipv6_address_rangesList = NULL;
    }
    if (fqdn_listList) {
        OpenAPI_list_for_each(fqdn_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(fqdn_listList);
        fqdn_listList = NULL;
    }
    return NULL;
}

OpenAPI_eas_info_to_refresh_t *OpenAPI_eas_info_to_refresh_copy(OpenAPI_eas_info_to_refresh_t *dst, OpenAPI_eas_info_to_refresh_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_eas_info_to_refresh_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_eas_info_to_refresh_convertToJSON() failed");
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

    OpenAPI_eas_info_to_refresh_free(dst);
    dst = OpenAPI_eas_info_to_refresh_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

