
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "acc_net_charging_address.h"

OpenAPI_acc_net_charging_address_t *OpenAPI_acc_net_charging_address_create(
    char *an_charg_ipv4_addr,
    char *an_charg_ipv6_addr
    )
{
    OpenAPI_acc_net_charging_address_t *acc_net_charging_address_local_var = OpenAPI_malloc(sizeof(OpenAPI_acc_net_charging_address_t));
    if (!acc_net_charging_address_local_var) {
        return NULL;
    }
    acc_net_charging_address_local_var->an_charg_ipv4_addr = an_charg_ipv4_addr;
    acc_net_charging_address_local_var->an_charg_ipv6_addr = an_charg_ipv6_addr;

    return acc_net_charging_address_local_var;
}

void OpenAPI_acc_net_charging_address_free(OpenAPI_acc_net_charging_address_t *acc_net_charging_address)
{
    if (NULL == acc_net_charging_address) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(acc_net_charging_address->an_charg_ipv4_addr);
    ogs_free(acc_net_charging_address->an_charg_ipv6_addr);
    ogs_free(acc_net_charging_address);
}

cJSON *OpenAPI_acc_net_charging_address_convertToJSON(OpenAPI_acc_net_charging_address_t *acc_net_charging_address)
{
    cJSON *item = NULL;

    if (acc_net_charging_address == NULL) {
        ogs_error("OpenAPI_acc_net_charging_address_convertToJSON() failed [AccNetChargingAddress]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (acc_net_charging_address->an_charg_ipv4_addr) {
        if (cJSON_AddStringToObject(item, "anChargIpv4Addr", acc_net_charging_address->an_charg_ipv4_addr) == NULL) {
            ogs_error("OpenAPI_acc_net_charging_address_convertToJSON() failed [an_charg_ipv4_addr]");
            goto end;
        }
    }

    if (acc_net_charging_address->an_charg_ipv6_addr) {
        if (cJSON_AddStringToObject(item, "anChargIpv6Addr", acc_net_charging_address->an_charg_ipv6_addr) == NULL) {
            ogs_error("OpenAPI_acc_net_charging_address_convertToJSON() failed [an_charg_ipv6_addr]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_acc_net_charging_address_t *OpenAPI_acc_net_charging_address_parseFromJSON(cJSON *acc_net_charging_addressJSON)
{
    OpenAPI_acc_net_charging_address_t *acc_net_charging_address_local_var = NULL;
    cJSON *an_charg_ipv4_addr = cJSON_GetObjectItemCaseSensitive(acc_net_charging_addressJSON, "anChargIpv4Addr");

    if (an_charg_ipv4_addr) {
        if (!cJSON_IsString(an_charg_ipv4_addr)) {
            ogs_error("OpenAPI_acc_net_charging_address_parseFromJSON() failed [an_charg_ipv4_addr]");
            goto end;
        }
    }

    cJSON *an_charg_ipv6_addr = cJSON_GetObjectItemCaseSensitive(acc_net_charging_addressJSON, "anChargIpv6Addr");

    if (an_charg_ipv6_addr) {
        if (!cJSON_IsString(an_charg_ipv6_addr)) {
            ogs_error("OpenAPI_acc_net_charging_address_parseFromJSON() failed [an_charg_ipv6_addr]");
            goto end;
        }
    }

    acc_net_charging_address_local_var = OpenAPI_acc_net_charging_address_create (
        an_charg_ipv4_addr ? ogs_strdup(an_charg_ipv4_addr->valuestring) : NULL,
        an_charg_ipv6_addr ? ogs_strdup(an_charg_ipv6_addr->valuestring) : NULL
        );

    return acc_net_charging_address_local_var;
end:
    return NULL;
}

OpenAPI_acc_net_charging_address_t *OpenAPI_acc_net_charging_address_copy(OpenAPI_acc_net_charging_address_t *dst, OpenAPI_acc_net_charging_address_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_acc_net_charging_address_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_acc_net_charging_address_convertToJSON() failed");
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

    OpenAPI_acc_net_charging_address_free(dst);
    dst = OpenAPI_acc_net_charging_address_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

