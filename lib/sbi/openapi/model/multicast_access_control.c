
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "multicast_access_control.h"

OpenAPI_multicast_access_control_t *OpenAPI_multicast_access_control_create(
    char *src_ipv4_addr,
    char *src_ipv6_addr,
    char *multicast_v4_addr,
    char *multicast_v6_addr,
    OpenAPI_access_right_status_t *acc_status
)
{
    OpenAPI_multicast_access_control_t *multicast_access_control_local_var = ogs_malloc(sizeof(OpenAPI_multicast_access_control_t));
    ogs_assert(multicast_access_control_local_var);

    multicast_access_control_local_var->src_ipv4_addr = src_ipv4_addr;
    multicast_access_control_local_var->src_ipv6_addr = src_ipv6_addr;
    multicast_access_control_local_var->multicast_v4_addr = multicast_v4_addr;
    multicast_access_control_local_var->multicast_v6_addr = multicast_v6_addr;
    multicast_access_control_local_var->acc_status = acc_status;

    return multicast_access_control_local_var;
}

void OpenAPI_multicast_access_control_free(OpenAPI_multicast_access_control_t *multicast_access_control)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == multicast_access_control) {
        return;
    }
    if (multicast_access_control->src_ipv4_addr) {
        ogs_free(multicast_access_control->src_ipv4_addr);
        multicast_access_control->src_ipv4_addr = NULL;
    }
    if (multicast_access_control->src_ipv6_addr) {
        ogs_free(multicast_access_control->src_ipv6_addr);
        multicast_access_control->src_ipv6_addr = NULL;
    }
    if (multicast_access_control->multicast_v4_addr) {
        ogs_free(multicast_access_control->multicast_v4_addr);
        multicast_access_control->multicast_v4_addr = NULL;
    }
    if (multicast_access_control->multicast_v6_addr) {
        ogs_free(multicast_access_control->multicast_v6_addr);
        multicast_access_control->multicast_v6_addr = NULL;
    }
    if (multicast_access_control->acc_status) {
        OpenAPI_access_right_status_free(multicast_access_control->acc_status);
        multicast_access_control->acc_status = NULL;
    }
    ogs_free(multicast_access_control);
}

cJSON *OpenAPI_multicast_access_control_convertToJSON(OpenAPI_multicast_access_control_t *multicast_access_control)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (multicast_access_control == NULL) {
        ogs_error("OpenAPI_multicast_access_control_convertToJSON() failed [MulticastAccessControl]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (multicast_access_control->src_ipv4_addr) {
    if (cJSON_AddStringToObject(item, "srcIpv4Addr", multicast_access_control->src_ipv4_addr) == NULL) {
        ogs_error("OpenAPI_multicast_access_control_convertToJSON() failed [src_ipv4_addr]");
        goto end;
    }
    }

    if (multicast_access_control->src_ipv6_addr) {
    if (cJSON_AddStringToObject(item, "srcIpv6Addr", multicast_access_control->src_ipv6_addr) == NULL) {
        ogs_error("OpenAPI_multicast_access_control_convertToJSON() failed [src_ipv6_addr]");
        goto end;
    }
    }

    if (multicast_access_control->multicast_v4_addr) {
    if (cJSON_AddStringToObject(item, "multicastV4Addr", multicast_access_control->multicast_v4_addr) == NULL) {
        ogs_error("OpenAPI_multicast_access_control_convertToJSON() failed [multicast_v4_addr]");
        goto end;
    }
    }

    if (multicast_access_control->multicast_v6_addr) {
    if (cJSON_AddStringToObject(item, "multicastV6Addr", multicast_access_control->multicast_v6_addr) == NULL) {
        ogs_error("OpenAPI_multicast_access_control_convertToJSON() failed [multicast_v6_addr]");
        goto end;
    }
    }

    if (!multicast_access_control->acc_status) {
        ogs_error("OpenAPI_multicast_access_control_convertToJSON() failed [acc_status]");
        return NULL;
    }
    cJSON *acc_status_local_JSON = OpenAPI_access_right_status_convertToJSON(multicast_access_control->acc_status);
    if (acc_status_local_JSON == NULL) {
        ogs_error("OpenAPI_multicast_access_control_convertToJSON() failed [acc_status]");
        goto end;
    }
    cJSON_AddItemToObject(item, "accStatus", acc_status_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_multicast_access_control_convertToJSON() failed [acc_status]");
        goto end;
    }

end:
    return item;
}

OpenAPI_multicast_access_control_t *OpenAPI_multicast_access_control_parseFromJSON(cJSON *multicast_access_controlJSON)
{
    OpenAPI_multicast_access_control_t *multicast_access_control_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *src_ipv4_addr = NULL;
    cJSON *src_ipv6_addr = NULL;
    cJSON *multicast_v4_addr = NULL;
    cJSON *multicast_v6_addr = NULL;
    cJSON *acc_status = NULL;
    OpenAPI_access_right_status_t *acc_status_local_nonprim = NULL;
    src_ipv4_addr = cJSON_GetObjectItemCaseSensitive(multicast_access_controlJSON, "srcIpv4Addr");
    if (src_ipv4_addr) {
    if (!cJSON_IsString(src_ipv4_addr) && !cJSON_IsNull(src_ipv4_addr)) {
        ogs_error("OpenAPI_multicast_access_control_parseFromJSON() failed [src_ipv4_addr]");
        goto end;
    }
    }

    src_ipv6_addr = cJSON_GetObjectItemCaseSensitive(multicast_access_controlJSON, "srcIpv6Addr");
    if (src_ipv6_addr) {
    if (!cJSON_IsString(src_ipv6_addr) && !cJSON_IsNull(src_ipv6_addr)) {
        ogs_error("OpenAPI_multicast_access_control_parseFromJSON() failed [src_ipv6_addr]");
        goto end;
    }
    }

    multicast_v4_addr = cJSON_GetObjectItemCaseSensitive(multicast_access_controlJSON, "multicastV4Addr");
    if (multicast_v4_addr) {
    if (!cJSON_IsString(multicast_v4_addr) && !cJSON_IsNull(multicast_v4_addr)) {
        ogs_error("OpenAPI_multicast_access_control_parseFromJSON() failed [multicast_v4_addr]");
        goto end;
    }
    }

    multicast_v6_addr = cJSON_GetObjectItemCaseSensitive(multicast_access_controlJSON, "multicastV6Addr");
    if (multicast_v6_addr) {
    if (!cJSON_IsString(multicast_v6_addr) && !cJSON_IsNull(multicast_v6_addr)) {
        ogs_error("OpenAPI_multicast_access_control_parseFromJSON() failed [multicast_v6_addr]");
        goto end;
    }
    }

    acc_status = cJSON_GetObjectItemCaseSensitive(multicast_access_controlJSON, "accStatus");
    if (!acc_status) {
        ogs_error("OpenAPI_multicast_access_control_parseFromJSON() failed [acc_status]");
        goto end;
    }
    acc_status_local_nonprim = OpenAPI_access_right_status_parseFromJSON(acc_status);
    if (!acc_status_local_nonprim) {
        ogs_error("OpenAPI_access_right_status_parseFromJSON failed [acc_status]");
        goto end;
    }

    multicast_access_control_local_var = OpenAPI_multicast_access_control_create (
        src_ipv4_addr && !cJSON_IsNull(src_ipv4_addr) ? ogs_strdup(src_ipv4_addr->valuestring) : NULL,
        src_ipv6_addr && !cJSON_IsNull(src_ipv6_addr) ? ogs_strdup(src_ipv6_addr->valuestring) : NULL,
        multicast_v4_addr && !cJSON_IsNull(multicast_v4_addr) ? ogs_strdup(multicast_v4_addr->valuestring) : NULL,
        multicast_v6_addr && !cJSON_IsNull(multicast_v6_addr) ? ogs_strdup(multicast_v6_addr->valuestring) : NULL,
        acc_status_local_nonprim
    );

    return multicast_access_control_local_var;
end:
    if (acc_status_local_nonprim) {
        OpenAPI_access_right_status_free(acc_status_local_nonprim);
        acc_status_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_multicast_access_control_t *OpenAPI_multicast_access_control_copy(OpenAPI_multicast_access_control_t *dst, OpenAPI_multicast_access_control_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_multicast_access_control_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_multicast_access_control_convertToJSON() failed");
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

    OpenAPI_multicast_access_control_free(dst);
    dst = OpenAPI_multicast_access_control_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

