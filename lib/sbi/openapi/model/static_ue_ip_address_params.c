
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "static_ue_ip_address_params.h"

OpenAPI_static_ue_ip_address_params_t *OpenAPI_static_ue_ip_address_params_create(
    char *dnn,
    OpenAPI_snssai_t *snssai,
    OpenAPI_list_t *ip_addresses
)
{
    OpenAPI_static_ue_ip_address_params_t *static_ue_ip_address_params_local_var = ogs_malloc(sizeof(OpenAPI_static_ue_ip_address_params_t));
    ogs_assert(static_ue_ip_address_params_local_var);

    static_ue_ip_address_params_local_var->dnn = dnn;
    static_ue_ip_address_params_local_var->snssai = snssai;
    static_ue_ip_address_params_local_var->ip_addresses = ip_addresses;

    return static_ue_ip_address_params_local_var;
}

void OpenAPI_static_ue_ip_address_params_free(OpenAPI_static_ue_ip_address_params_t *static_ue_ip_address_params)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == static_ue_ip_address_params) {
        return;
    }
    if (static_ue_ip_address_params->dnn) {
        ogs_free(static_ue_ip_address_params->dnn);
        static_ue_ip_address_params->dnn = NULL;
    }
    if (static_ue_ip_address_params->snssai) {
        OpenAPI_snssai_free(static_ue_ip_address_params->snssai);
        static_ue_ip_address_params->snssai = NULL;
    }
    if (static_ue_ip_address_params->ip_addresses) {
        OpenAPI_list_for_each(static_ue_ip_address_params->ip_addresses, node) {
            OpenAPI_ip_address_free(node->data);
        }
        OpenAPI_list_free(static_ue_ip_address_params->ip_addresses);
        static_ue_ip_address_params->ip_addresses = NULL;
    }
    ogs_free(static_ue_ip_address_params);
}

cJSON *OpenAPI_static_ue_ip_address_params_convertToJSON(OpenAPI_static_ue_ip_address_params_t *static_ue_ip_address_params)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (static_ue_ip_address_params == NULL) {
        ogs_error("OpenAPI_static_ue_ip_address_params_convertToJSON() failed [StaticUeIpAddressParams]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!static_ue_ip_address_params->dnn) {
        ogs_error("OpenAPI_static_ue_ip_address_params_convertToJSON() failed [dnn]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "dnn", static_ue_ip_address_params->dnn) == NULL) {
        ogs_error("OpenAPI_static_ue_ip_address_params_convertToJSON() failed [dnn]");
        goto end;
    }

    if (!static_ue_ip_address_params->snssai) {
        ogs_error("OpenAPI_static_ue_ip_address_params_convertToJSON() failed [snssai]");
        return NULL;
    }
    cJSON *snssai_local_JSON = OpenAPI_snssai_convertToJSON(static_ue_ip_address_params->snssai);
    if (snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_static_ue_ip_address_params_convertToJSON() failed [snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "snssai", snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_static_ue_ip_address_params_convertToJSON() failed [snssai]");
        goto end;
    }

    if (!static_ue_ip_address_params->ip_addresses) {
        ogs_error("OpenAPI_static_ue_ip_address_params_convertToJSON() failed [ip_addresses]");
        return NULL;
    }
    cJSON *ip_addressesList = cJSON_AddArrayToObject(item, "ipAddresses");
    if (ip_addressesList == NULL) {
        ogs_error("OpenAPI_static_ue_ip_address_params_convertToJSON() failed [ip_addresses]");
        goto end;
    }
    OpenAPI_list_for_each(static_ue_ip_address_params->ip_addresses, node) {
        cJSON *itemLocal = OpenAPI_ip_address_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_static_ue_ip_address_params_convertToJSON() failed [ip_addresses]");
            goto end;
        }
        cJSON_AddItemToArray(ip_addressesList, itemLocal);
    }

end:
    return item;
}

OpenAPI_static_ue_ip_address_params_t *OpenAPI_static_ue_ip_address_params_parseFromJSON(cJSON *static_ue_ip_address_paramsJSON)
{
    OpenAPI_static_ue_ip_address_params_t *static_ue_ip_address_params_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *dnn = NULL;
    cJSON *snssai = NULL;
    OpenAPI_snssai_t *snssai_local_nonprim = NULL;
    cJSON *ip_addresses = NULL;
    OpenAPI_list_t *ip_addressesList = NULL;
    dnn = cJSON_GetObjectItemCaseSensitive(static_ue_ip_address_paramsJSON, "dnn");
    if (!dnn) {
        ogs_error("OpenAPI_static_ue_ip_address_params_parseFromJSON() failed [dnn]");
        goto end;
    }
    if (!cJSON_IsString(dnn)) {
        ogs_error("OpenAPI_static_ue_ip_address_params_parseFromJSON() failed [dnn]");
        goto end;
    }

    snssai = cJSON_GetObjectItemCaseSensitive(static_ue_ip_address_paramsJSON, "snssai");
    if (!snssai) {
        ogs_error("OpenAPI_static_ue_ip_address_params_parseFromJSON() failed [snssai]");
        goto end;
    }
    snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(snssai);
    if (!snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [snssai]");
        goto end;
    }

    ip_addresses = cJSON_GetObjectItemCaseSensitive(static_ue_ip_address_paramsJSON, "ipAddresses");
    if (!ip_addresses) {
        ogs_error("OpenAPI_static_ue_ip_address_params_parseFromJSON() failed [ip_addresses]");
        goto end;
    }
        cJSON *ip_addresses_local = NULL;
        if (!cJSON_IsArray(ip_addresses)) {
            ogs_error("OpenAPI_static_ue_ip_address_params_parseFromJSON() failed [ip_addresses]");
            goto end;
        }

        ip_addressesList = OpenAPI_list_create();

        cJSON_ArrayForEach(ip_addresses_local, ip_addresses) {
            if (!cJSON_IsObject(ip_addresses_local)) {
                ogs_error("OpenAPI_static_ue_ip_address_params_parseFromJSON() failed [ip_addresses]");
                goto end;
            }
            OpenAPI_ip_address_t *ip_addressesItem = OpenAPI_ip_address_parseFromJSON(ip_addresses_local);
            if (!ip_addressesItem) {
                ogs_error("No ip_addressesItem");
                goto end;
            }
            OpenAPI_list_add(ip_addressesList, ip_addressesItem);
        }

    static_ue_ip_address_params_local_var = OpenAPI_static_ue_ip_address_params_create (
        ogs_strdup(dnn->valuestring),
        snssai_local_nonprim,
        ip_addressesList
    );

    return static_ue_ip_address_params_local_var;
end:
    if (snssai_local_nonprim) {
        OpenAPI_snssai_free(snssai_local_nonprim);
        snssai_local_nonprim = NULL;
    }
    if (ip_addressesList) {
        OpenAPI_list_for_each(ip_addressesList, node) {
            OpenAPI_ip_address_free(node->data);
        }
        OpenAPI_list_free(ip_addressesList);
        ip_addressesList = NULL;
    }
    return NULL;
}

OpenAPI_static_ue_ip_address_params_t *OpenAPI_static_ue_ip_address_params_copy(OpenAPI_static_ue_ip_address_params_t *dst, OpenAPI_static_ue_ip_address_params_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_static_ue_ip_address_params_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_static_ue_ip_address_params_convertToJSON() failed");
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

    OpenAPI_static_ue_ip_address_params_free(dst);
    dst = OpenAPI_static_ue_ip_address_params_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

