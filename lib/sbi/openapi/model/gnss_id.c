
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "gnss_id.h"

OpenAPI_gnss_id_t *OpenAPI_gnss_id_create(
)
{
    OpenAPI_gnss_id_t *gnss_id_local_var = ogs_malloc(sizeof(OpenAPI_gnss_id_t));
    ogs_assert(gnss_id_local_var);


    return gnss_id_local_var;
}

void OpenAPI_gnss_id_free(OpenAPI_gnss_id_t *gnss_id)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == gnss_id) {
        return;
    }
    ogs_free(gnss_id);
}

cJSON *OpenAPI_gnss_id_convertToJSON(OpenAPI_gnss_id_t *gnss_id)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (gnss_id == NULL) {
        ogs_error("OpenAPI_gnss_id_convertToJSON() failed [GnssId]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_gnss_id_t *OpenAPI_gnss_id_parseFromJSON(cJSON *gnss_idJSON)
{
    OpenAPI_gnss_id_t *gnss_id_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    gnss_id_local_var = OpenAPI_gnss_id_create (
    );

    return gnss_id_local_var;
end:
    return NULL;
}

OpenAPI_gnss_id_t *OpenAPI_gnss_id_copy(OpenAPI_gnss_id_t *dst, OpenAPI_gnss_id_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_gnss_id_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_gnss_id_convertToJSON() failed");
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

    OpenAPI_gnss_id_free(dst);
    dst = OpenAPI_gnss_id_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

