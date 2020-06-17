
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "eps_interworking_indication.h"

OpenAPI_eps_interworking_indication_t *OpenAPI_eps_interworking_indication_create(
    )
{
    OpenAPI_eps_interworking_indication_t *eps_interworking_indication_local_var = OpenAPI_malloc(sizeof(OpenAPI_eps_interworking_indication_t));
    if (!eps_interworking_indication_local_var) {
        return NULL;
    }

    return eps_interworking_indication_local_var;
}

void OpenAPI_eps_interworking_indication_free(OpenAPI_eps_interworking_indication_t *eps_interworking_indication)
{
    if (NULL == eps_interworking_indication) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(eps_interworking_indication);
}

cJSON *OpenAPI_eps_interworking_indication_convertToJSON(OpenAPI_eps_interworking_indication_t *eps_interworking_indication)
{
    cJSON *item = NULL;

    if (eps_interworking_indication == NULL) {
        ogs_error("OpenAPI_eps_interworking_indication_convertToJSON() failed [EpsInterworkingIndication]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_eps_interworking_indication_t *OpenAPI_eps_interworking_indication_parseFromJSON(cJSON *eps_interworking_indicationJSON)
{
    OpenAPI_eps_interworking_indication_t *eps_interworking_indication_local_var = NULL;
    eps_interworking_indication_local_var = OpenAPI_eps_interworking_indication_create (
        );

    return eps_interworking_indication_local_var;
end:
    return NULL;
}

OpenAPI_eps_interworking_indication_t *OpenAPI_eps_interworking_indication_copy(OpenAPI_eps_interworking_indication_t *dst, OpenAPI_eps_interworking_indication_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_eps_interworking_indication_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_eps_interworking_indication_convertToJSON() failed");
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

    OpenAPI_eps_interworking_indication_free(dst);
    dst = OpenAPI_eps_interworking_indication_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

