
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ims_vo_ps.h"

OpenAPI_ims_vo_ps_t *OpenAPI_ims_vo_ps_create(
    )
{
    OpenAPI_ims_vo_ps_t *ims_vo_ps_local_var = OpenAPI_malloc(sizeof(OpenAPI_ims_vo_ps_t));
    if (!ims_vo_ps_local_var) {
        return NULL;
    }

    return ims_vo_ps_local_var;
}

void OpenAPI_ims_vo_ps_free(OpenAPI_ims_vo_ps_t *ims_vo_ps)
{
    if (NULL == ims_vo_ps) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(ims_vo_ps);
}

cJSON *OpenAPI_ims_vo_ps_convertToJSON(OpenAPI_ims_vo_ps_t *ims_vo_ps)
{
    cJSON *item = NULL;

    if (ims_vo_ps == NULL) {
        ogs_error("OpenAPI_ims_vo_ps_convertToJSON() failed [ImsVoPs]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_ims_vo_ps_t *OpenAPI_ims_vo_ps_parseFromJSON(cJSON *ims_vo_psJSON)
{
    OpenAPI_ims_vo_ps_t *ims_vo_ps_local_var = NULL;
    ims_vo_ps_local_var = OpenAPI_ims_vo_ps_create (
        );

    return ims_vo_ps_local_var;
end:
    return NULL;
}

OpenAPI_ims_vo_ps_t *OpenAPI_ims_vo_ps_copy(OpenAPI_ims_vo_ps_t *dst, OpenAPI_ims_vo_ps_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ims_vo_ps_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ims_vo_ps_convertToJSON() failed");
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

    OpenAPI_ims_vo_ps_free(dst);
    dst = OpenAPI_ims_vo_ps_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

