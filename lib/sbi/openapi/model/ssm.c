
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ssm.h"

OpenAPI_ssm_t *OpenAPI_ssm_create(
    OpenAPI_ip_addr_t *source_ip_addr,
    OpenAPI_ip_addr_t *dest_ip_addr
)
{
    OpenAPI_ssm_t *ssm_local_var = ogs_malloc(sizeof(OpenAPI_ssm_t));
    ogs_assert(ssm_local_var);

    ssm_local_var->source_ip_addr = source_ip_addr;
    ssm_local_var->dest_ip_addr = dest_ip_addr;

    return ssm_local_var;
}

void OpenAPI_ssm_free(OpenAPI_ssm_t *ssm)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ssm) {
        return;
    }
    if (ssm->source_ip_addr) {
        OpenAPI_ip_addr_free(ssm->source_ip_addr);
        ssm->source_ip_addr = NULL;
    }
    if (ssm->dest_ip_addr) {
        OpenAPI_ip_addr_free(ssm->dest_ip_addr);
        ssm->dest_ip_addr = NULL;
    }
    ogs_free(ssm);
}

cJSON *OpenAPI_ssm_convertToJSON(OpenAPI_ssm_t *ssm)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ssm == NULL) {
        ogs_error("OpenAPI_ssm_convertToJSON() failed [Ssm]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!ssm->source_ip_addr) {
        ogs_error("OpenAPI_ssm_convertToJSON() failed [source_ip_addr]");
        return NULL;
    }
    cJSON *source_ip_addr_local_JSON = OpenAPI_ip_addr_convertToJSON(ssm->source_ip_addr);
    if (source_ip_addr_local_JSON == NULL) {
        ogs_error("OpenAPI_ssm_convertToJSON() failed [source_ip_addr]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sourceIpAddr", source_ip_addr_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ssm_convertToJSON() failed [source_ip_addr]");
        goto end;
    }

    if (!ssm->dest_ip_addr) {
        ogs_error("OpenAPI_ssm_convertToJSON() failed [dest_ip_addr]");
        return NULL;
    }
    cJSON *dest_ip_addr_local_JSON = OpenAPI_ip_addr_convertToJSON(ssm->dest_ip_addr);
    if (dest_ip_addr_local_JSON == NULL) {
        ogs_error("OpenAPI_ssm_convertToJSON() failed [dest_ip_addr]");
        goto end;
    }
    cJSON_AddItemToObject(item, "destIpAddr", dest_ip_addr_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ssm_convertToJSON() failed [dest_ip_addr]");
        goto end;
    }

end:
    return item;
}

OpenAPI_ssm_t *OpenAPI_ssm_parseFromJSON(cJSON *ssmJSON)
{
    OpenAPI_ssm_t *ssm_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *source_ip_addr = NULL;
    OpenAPI_ip_addr_t *source_ip_addr_local_nonprim = NULL;
    cJSON *dest_ip_addr = NULL;
    OpenAPI_ip_addr_t *dest_ip_addr_local_nonprim = NULL;
    source_ip_addr = cJSON_GetObjectItemCaseSensitive(ssmJSON, "sourceIpAddr");
    if (!source_ip_addr) {
        ogs_error("OpenAPI_ssm_parseFromJSON() failed [source_ip_addr]");
        goto end;
    }
    source_ip_addr_local_nonprim = OpenAPI_ip_addr_parseFromJSON(source_ip_addr);
    if (!source_ip_addr_local_nonprim) {
        ogs_error("OpenAPI_ip_addr_parseFromJSON failed [source_ip_addr]");
        goto end;
    }

    dest_ip_addr = cJSON_GetObjectItemCaseSensitive(ssmJSON, "destIpAddr");
    if (!dest_ip_addr) {
        ogs_error("OpenAPI_ssm_parseFromJSON() failed [dest_ip_addr]");
        goto end;
    }
    dest_ip_addr_local_nonprim = OpenAPI_ip_addr_parseFromJSON(dest_ip_addr);
    if (!dest_ip_addr_local_nonprim) {
        ogs_error("OpenAPI_ip_addr_parseFromJSON failed [dest_ip_addr]");
        goto end;
    }

    ssm_local_var = OpenAPI_ssm_create (
        source_ip_addr_local_nonprim,
        dest_ip_addr_local_nonprim
    );

    return ssm_local_var;
end:
    if (source_ip_addr_local_nonprim) {
        OpenAPI_ip_addr_free(source_ip_addr_local_nonprim);
        source_ip_addr_local_nonprim = NULL;
    }
    if (dest_ip_addr_local_nonprim) {
        OpenAPI_ip_addr_free(dest_ip_addr_local_nonprim);
        dest_ip_addr_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_ssm_t *OpenAPI_ssm_copy(OpenAPI_ssm_t *dst, OpenAPI_ssm_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ssm_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ssm_convertToJSON() failed");
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

    OpenAPI_ssm_free(dst);
    dst = OpenAPI_ssm_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

