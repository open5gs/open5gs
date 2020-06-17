
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "n3ga_location.h"

OpenAPI_n3ga_location_t *OpenAPI_n3ga_location_create(
    OpenAPI_tai_t *n3gpp_tai,
    char *n3_iwf_id,
    char *ue_ipv4_addr,
    char *ue_ipv6_addr,
    int port_number,
    char *ss_id,
    char *bss_id,
    char civic_address,
    OpenAPI_hfc_node_id_t *hfc_node_id,
    char gli
    )
{
    OpenAPI_n3ga_location_t *n3ga_location_local_var = OpenAPI_malloc(sizeof(OpenAPI_n3ga_location_t));
    if (!n3ga_location_local_var) {
        return NULL;
    }
    n3ga_location_local_var->n3gpp_tai = n3gpp_tai;
    n3ga_location_local_var->n3_iwf_id = n3_iwf_id;
    n3ga_location_local_var->ue_ipv4_addr = ue_ipv4_addr;
    n3ga_location_local_var->ue_ipv6_addr = ue_ipv6_addr;
    n3ga_location_local_var->port_number = port_number;
    n3ga_location_local_var->ss_id = ss_id;
    n3ga_location_local_var->bss_id = bss_id;
    n3ga_location_local_var->civic_address = civic_address;
    n3ga_location_local_var->hfc_node_id = hfc_node_id;
    n3ga_location_local_var->gli = gli;

    return n3ga_location_local_var;
}

void OpenAPI_n3ga_location_free(OpenAPI_n3ga_location_t *n3ga_location)
{
    if (NULL == n3ga_location) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_tai_free(n3ga_location->n3gpp_tai);
    ogs_free(n3ga_location->n3_iwf_id);
    ogs_free(n3ga_location->ue_ipv4_addr);
    ogs_free(n3ga_location->ue_ipv6_addr);
    ogs_free(n3ga_location->ss_id);
    ogs_free(n3ga_location->bss_id);
    OpenAPI_hfc_node_id_free(n3ga_location->hfc_node_id);
    ogs_free(n3ga_location);
}

cJSON *OpenAPI_n3ga_location_convertToJSON(OpenAPI_n3ga_location_t *n3ga_location)
{
    cJSON *item = NULL;

    if (n3ga_location == NULL) {
        ogs_error("OpenAPI_n3ga_location_convertToJSON() failed [N3gaLocation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (n3ga_location->n3gpp_tai) {
        cJSON *n3gpp_tai_local_JSON = OpenAPI_tai_convertToJSON(n3ga_location->n3gpp_tai);
        if (n3gpp_tai_local_JSON == NULL) {
            ogs_error("OpenAPI_n3ga_location_convertToJSON() failed [n3gpp_tai]");
            goto end;
        }
        cJSON_AddItemToObject(item, "n3gppTai", n3gpp_tai_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_n3ga_location_convertToJSON() failed [n3gpp_tai]");
            goto end;
        }
    }

    if (n3ga_location->n3_iwf_id) {
        if (cJSON_AddStringToObject(item, "n3IwfId", n3ga_location->n3_iwf_id) == NULL) {
            ogs_error("OpenAPI_n3ga_location_convertToJSON() failed [n3_iwf_id]");
            goto end;
        }
    }

    if (n3ga_location->ue_ipv4_addr) {
        if (cJSON_AddStringToObject(item, "ueIpv4Addr", n3ga_location->ue_ipv4_addr) == NULL) {
            ogs_error("OpenAPI_n3ga_location_convertToJSON() failed [ue_ipv4_addr]");
            goto end;
        }
    }

    if (n3ga_location->ue_ipv6_addr) {
        if (cJSON_AddStringToObject(item, "ueIpv6Addr", n3ga_location->ue_ipv6_addr) == NULL) {
            ogs_error("OpenAPI_n3ga_location_convertToJSON() failed [ue_ipv6_addr]");
            goto end;
        }
    }

    if (n3ga_location->port_number) {
        if (cJSON_AddNumberToObject(item, "portNumber", n3ga_location->port_number) == NULL) {
            ogs_error("OpenAPI_n3ga_location_convertToJSON() failed [port_number]");
            goto end;
        }
    }

    if (n3ga_location->ss_id) {
        if (cJSON_AddStringToObject(item, "ssId", n3ga_location->ss_id) == NULL) {
            ogs_error("OpenAPI_n3ga_location_convertToJSON() failed [ss_id]");
            goto end;
        }
    }

    if (n3ga_location->bss_id) {
        if (cJSON_AddStringToObject(item, "bssId", n3ga_location->bss_id) == NULL) {
            ogs_error("OpenAPI_n3ga_location_convertToJSON() failed [bss_id]");
            goto end;
        }
    }

    if (n3ga_location->civic_address) {
        if (cJSON_AddNumberToObject(item, "civicAddress", n3ga_location->civic_address) == NULL) {
            ogs_error("OpenAPI_n3ga_location_convertToJSON() failed [civic_address]");
            goto end;
        }
    }

    if (n3ga_location->hfc_node_id) {
        cJSON *hfc_node_id_local_JSON = OpenAPI_hfc_node_id_convertToJSON(n3ga_location->hfc_node_id);
        if (hfc_node_id_local_JSON == NULL) {
            ogs_error("OpenAPI_n3ga_location_convertToJSON() failed [hfc_node_id]");
            goto end;
        }
        cJSON_AddItemToObject(item, "hfcNodeId", hfc_node_id_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_n3ga_location_convertToJSON() failed [hfc_node_id]");
            goto end;
        }
    }

    if (n3ga_location->gli) {
        if (cJSON_AddNumberToObject(item, "gli", n3ga_location->gli) == NULL) {
            ogs_error("OpenAPI_n3ga_location_convertToJSON() failed [gli]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_n3ga_location_t *OpenAPI_n3ga_location_parseFromJSON(cJSON *n3ga_locationJSON)
{
    OpenAPI_n3ga_location_t *n3ga_location_local_var = NULL;
    cJSON *n3gpp_tai = cJSON_GetObjectItemCaseSensitive(n3ga_locationJSON, "n3gppTai");

    OpenAPI_tai_t *n3gpp_tai_local_nonprim = NULL;
    if (n3gpp_tai) {
        n3gpp_tai_local_nonprim = OpenAPI_tai_parseFromJSON(n3gpp_tai);
    }

    cJSON *n3_iwf_id = cJSON_GetObjectItemCaseSensitive(n3ga_locationJSON, "n3IwfId");

    if (n3_iwf_id) {
        if (!cJSON_IsString(n3_iwf_id)) {
            ogs_error("OpenAPI_n3ga_location_parseFromJSON() failed [n3_iwf_id]");
            goto end;
        }
    }

    cJSON *ue_ipv4_addr = cJSON_GetObjectItemCaseSensitive(n3ga_locationJSON, "ueIpv4Addr");

    if (ue_ipv4_addr) {
        if (!cJSON_IsString(ue_ipv4_addr)) {
            ogs_error("OpenAPI_n3ga_location_parseFromJSON() failed [ue_ipv4_addr]");
            goto end;
        }
    }

    cJSON *ue_ipv6_addr = cJSON_GetObjectItemCaseSensitive(n3ga_locationJSON, "ueIpv6Addr");

    if (ue_ipv6_addr) {
        if (!cJSON_IsString(ue_ipv6_addr)) {
            ogs_error("OpenAPI_n3ga_location_parseFromJSON() failed [ue_ipv6_addr]");
            goto end;
        }
    }

    cJSON *port_number = cJSON_GetObjectItemCaseSensitive(n3ga_locationJSON, "portNumber");

    if (port_number) {
        if (!cJSON_IsNumber(port_number)) {
            ogs_error("OpenAPI_n3ga_location_parseFromJSON() failed [port_number]");
            goto end;
        }
    }

    cJSON *ss_id = cJSON_GetObjectItemCaseSensitive(n3ga_locationJSON, "ssId");

    if (ss_id) {
        if (!cJSON_IsString(ss_id)) {
            ogs_error("OpenAPI_n3ga_location_parseFromJSON() failed [ss_id]");
            goto end;
        }
    }

    cJSON *bss_id = cJSON_GetObjectItemCaseSensitive(n3ga_locationJSON, "bssId");

    if (bss_id) {
        if (!cJSON_IsString(bss_id)) {
            ogs_error("OpenAPI_n3ga_location_parseFromJSON() failed [bss_id]");
            goto end;
        }
    }

    cJSON *civic_address = cJSON_GetObjectItemCaseSensitive(n3ga_locationJSON, "civicAddress");

    if (civic_address) {
        if (!cJSON_IsNumber(civic_address)) {
            ogs_error("OpenAPI_n3ga_location_parseFromJSON() failed [civic_address]");
            goto end;
        }
    }

    cJSON *hfc_node_id = cJSON_GetObjectItemCaseSensitive(n3ga_locationJSON, "hfcNodeId");

    OpenAPI_hfc_node_id_t *hfc_node_id_local_nonprim = NULL;
    if (hfc_node_id) {
        hfc_node_id_local_nonprim = OpenAPI_hfc_node_id_parseFromJSON(hfc_node_id);
    }

    cJSON *gli = cJSON_GetObjectItemCaseSensitive(n3ga_locationJSON, "gli");

    if (gli) {
        if (!cJSON_IsNumber(gli)) {
            ogs_error("OpenAPI_n3ga_location_parseFromJSON() failed [gli]");
            goto end;
        }
    }

    n3ga_location_local_var = OpenAPI_n3ga_location_create (
        n3gpp_tai ? n3gpp_tai_local_nonprim : NULL,
        n3_iwf_id ? ogs_strdup(n3_iwf_id->valuestring) : NULL,
        ue_ipv4_addr ? ogs_strdup(ue_ipv4_addr->valuestring) : NULL,
        ue_ipv6_addr ? ogs_strdup(ue_ipv6_addr->valuestring) : NULL,
        port_number ? port_number->valuedouble : 0,
        ss_id ? ogs_strdup(ss_id->valuestring) : NULL,
        bss_id ? ogs_strdup(bss_id->valuestring) : NULL,
        civic_address ? civic_address->valueint : 0,
        hfc_node_id ? hfc_node_id_local_nonprim : NULL,
        gli ? gli->valueint : 0
        );

    return n3ga_location_local_var;
end:
    return NULL;
}

OpenAPI_n3ga_location_t *OpenAPI_n3ga_location_copy(OpenAPI_n3ga_location_t *dst, OpenAPI_n3ga_location_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_n3ga_location_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_n3ga_location_convertToJSON() failed");
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

    OpenAPI_n3ga_location_free(dst);
    dst = OpenAPI_n3ga_location_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

