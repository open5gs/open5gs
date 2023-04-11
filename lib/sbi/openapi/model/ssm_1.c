
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ssm_1.h"

OpenAPI_ssm_1_t *OpenAPI_ssm_1_create(
    OpenAPI_ip_addr_1_t *source_ip_addr,
    OpenAPI_ip_addr_1_t *dest_ip_addr
)
{
    OpenAPI_ssm_1_t *ssm_1_local_var = ogs_malloc(sizeof(OpenAPI_ssm_1_t));
    ogs_assert(ssm_1_local_var);

    ssm_1_local_var->source_ip_addr = source_ip_addr;
    ssm_1_local_var->dest_ip_addr = dest_ip_addr;

    return ssm_1_local_var;
}

void OpenAPI_ssm_1_free(OpenAPI_ssm_1_t *ssm_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ssm_1) {
        return;
    }
    if (ssm_1->source_ip_addr) {
        OpenAPI_ip_addr_1_free(ssm_1->source_ip_addr);
        ssm_1->source_ip_addr = NULL;
    }
    if (ssm_1->dest_ip_addr) {
        OpenAPI_ip_addr_1_free(ssm_1->dest_ip_addr);
        ssm_1->dest_ip_addr = NULL;
    }
    ogs_free(ssm_1);
}

cJSON *OpenAPI_ssm_1_convertToJSON(OpenAPI_ssm_1_t *ssm_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ssm_1 == NULL) {
        ogs_error("OpenAPI_ssm_1_convertToJSON() failed [Ssm_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!ssm_1->source_ip_addr) {
        ogs_error("OpenAPI_ssm_1_convertToJSON() failed [source_ip_addr]");
        return NULL;
    }
    cJSON *source_ip_addr_local_JSON = OpenAPI_ip_addr_1_convertToJSON(ssm_1->source_ip_addr);
    if (source_ip_addr_local_JSON == NULL) {
        ogs_error("OpenAPI_ssm_1_convertToJSON() failed [source_ip_addr]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sourceIpAddr", source_ip_addr_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ssm_1_convertToJSON() failed [source_ip_addr]");
        goto end;
    }

    if (!ssm_1->dest_ip_addr) {
        ogs_error("OpenAPI_ssm_1_convertToJSON() failed [dest_ip_addr]");
        return NULL;
    }
    cJSON *dest_ip_addr_local_JSON = OpenAPI_ip_addr_1_convertToJSON(ssm_1->dest_ip_addr);
    if (dest_ip_addr_local_JSON == NULL) {
        ogs_error("OpenAPI_ssm_1_convertToJSON() failed [dest_ip_addr]");
        goto end;
    }
    cJSON_AddItemToObject(item, "destIpAddr", dest_ip_addr_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ssm_1_convertToJSON() failed [dest_ip_addr]");
        goto end;
    }

end:
    return item;
}

OpenAPI_ssm_1_t *OpenAPI_ssm_1_parseFromJSON(cJSON *ssm_1JSON)
{
    OpenAPI_ssm_1_t *ssm_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *source_ip_addr = NULL;
    OpenAPI_ip_addr_1_t *source_ip_addr_local_nonprim = NULL;
    cJSON *dest_ip_addr = NULL;
    OpenAPI_ip_addr_1_t *dest_ip_addr_local_nonprim = NULL;
    source_ip_addr = cJSON_GetObjectItemCaseSensitive(ssm_1JSON, "sourceIpAddr");
    if (!source_ip_addr) {
        ogs_error("OpenAPI_ssm_1_parseFromJSON() failed [source_ip_addr]");
        goto end;
    }
    source_ip_addr_local_nonprim = OpenAPI_ip_addr_1_parseFromJSON(source_ip_addr);
    if (!source_ip_addr_local_nonprim) {
        ogs_error("OpenAPI_ip_addr_1_parseFromJSON failed [source_ip_addr]");
        goto end;
    }

    dest_ip_addr = cJSON_GetObjectItemCaseSensitive(ssm_1JSON, "destIpAddr");
    if (!dest_ip_addr) {
        ogs_error("OpenAPI_ssm_1_parseFromJSON() failed [dest_ip_addr]");
        goto end;
    }
    dest_ip_addr_local_nonprim = OpenAPI_ip_addr_1_parseFromJSON(dest_ip_addr);
    if (!dest_ip_addr_local_nonprim) {
        ogs_error("OpenAPI_ip_addr_1_parseFromJSON failed [dest_ip_addr]");
        goto end;
    }

    ssm_1_local_var = OpenAPI_ssm_1_create (
        source_ip_addr_local_nonprim,
        dest_ip_addr_local_nonprim
    );

    return ssm_1_local_var;
end:
    if (source_ip_addr_local_nonprim) {
        OpenAPI_ip_addr_1_free(source_ip_addr_local_nonprim);
        source_ip_addr_local_nonprim = NULL;
    }
    if (dest_ip_addr_local_nonprim) {
        OpenAPI_ip_addr_1_free(dest_ip_addr_local_nonprim);
        dest_ip_addr_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_ssm_1_t *OpenAPI_ssm_1_copy(OpenAPI_ssm_1_t *dst, OpenAPI_ssm_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ssm_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ssm_1_convertToJSON() failed");
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

    OpenAPI_ssm_1_free(dst);
    dst = OpenAPI_ssm_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

