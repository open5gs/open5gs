
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nwdaf_event.h"

OpenAPI_nwdaf_event_t *OpenAPI_nwdaf_event_create(
)
{
    OpenAPI_nwdaf_event_t *nwdaf_event_local_var = ogs_malloc(sizeof(OpenAPI_nwdaf_event_t));
    ogs_assert(nwdaf_event_local_var);


    return nwdaf_event_local_var;
}

void OpenAPI_nwdaf_event_free(OpenAPI_nwdaf_event_t *nwdaf_event)
{
    if (NULL == nwdaf_event) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(nwdaf_event);
}

cJSON *OpenAPI_nwdaf_event_convertToJSON(OpenAPI_nwdaf_event_t *nwdaf_event)
{
    cJSON *item = NULL;

    if (nwdaf_event == NULL) {
        ogs_error("OpenAPI_nwdaf_event_convertToJSON() failed [NwdafEvent]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_nwdaf_event_t *OpenAPI_nwdaf_event_parseFromJSON(cJSON *nwdaf_eventJSON)
{
    OpenAPI_nwdaf_event_t *nwdaf_event_local_var = NULL;
    nwdaf_event_local_var = OpenAPI_nwdaf_event_create (
    );

    return nwdaf_event_local_var;
end:
    return NULL;
}

OpenAPI_nwdaf_event_t *OpenAPI_nwdaf_event_copy(OpenAPI_nwdaf_event_t *dst, OpenAPI_nwdaf_event_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nwdaf_event_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nwdaf_event_convertToJSON() failed");
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

    OpenAPI_nwdaf_event_free(dst);
    dst = OpenAPI_nwdaf_event_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

