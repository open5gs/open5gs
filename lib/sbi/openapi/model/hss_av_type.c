
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "hss_av_type.h"

OpenAPI_hss_av_type_t *OpenAPI_hss_av_type_create(
    )
{
    OpenAPI_hss_av_type_t *hss_av_type_local_var = OpenAPI_malloc(sizeof(OpenAPI_hss_av_type_t));
    if (!hss_av_type_local_var) {
        return NULL;
    }

    return hss_av_type_local_var;
}

void OpenAPI_hss_av_type_free(OpenAPI_hss_av_type_t *hss_av_type)
{
    if (NULL == hss_av_type) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(hss_av_type);
}

cJSON *OpenAPI_hss_av_type_convertToJSON(OpenAPI_hss_av_type_t *hss_av_type)
{
    cJSON *item = NULL;

    if (hss_av_type == NULL) {
        ogs_error("OpenAPI_hss_av_type_convertToJSON() failed [HssAvType]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_hss_av_type_t *OpenAPI_hss_av_type_parseFromJSON(cJSON *hss_av_typeJSON)
{
    OpenAPI_hss_av_type_t *hss_av_type_local_var = NULL;
    hss_av_type_local_var = OpenAPI_hss_av_type_create (
        );

    return hss_av_type_local_var;
end:
    return NULL;
}

