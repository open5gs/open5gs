
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

