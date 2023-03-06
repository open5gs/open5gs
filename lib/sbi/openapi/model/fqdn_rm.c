
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "fqdn_rm.h"

OpenAPI_fqdn_rm_t *OpenAPI_fqdn_rm_create(
)
{
    OpenAPI_fqdn_rm_t *fqdn_rm_local_var = ogs_malloc(sizeof(OpenAPI_fqdn_rm_t));
    ogs_assert(fqdn_rm_local_var);


    return fqdn_rm_local_var;
}

void OpenAPI_fqdn_rm_free(OpenAPI_fqdn_rm_t *fqdn_rm)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == fqdn_rm) {
        return;
    }
    ogs_free(fqdn_rm);
}

cJSON *OpenAPI_fqdn_rm_convertToJSON(OpenAPI_fqdn_rm_t *fqdn_rm)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (fqdn_rm == NULL) {
        ogs_error("OpenAPI_fqdn_rm_convertToJSON() failed [FqdnRm]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_fqdn_rm_t *OpenAPI_fqdn_rm_parseFromJSON(cJSON *fqdn_rmJSON)
{
    OpenAPI_fqdn_rm_t *fqdn_rm_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    fqdn_rm_local_var = OpenAPI_fqdn_rm_create (
    );

    return fqdn_rm_local_var;
end:
    return NULL;
}

OpenAPI_fqdn_rm_t *OpenAPI_fqdn_rm_copy(OpenAPI_fqdn_rm_t *dst, OpenAPI_fqdn_rm_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_fqdn_rm_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_fqdn_rm_convertToJSON() failed");
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

    OpenAPI_fqdn_rm_free(dst);
    dst = OpenAPI_fqdn_rm_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

