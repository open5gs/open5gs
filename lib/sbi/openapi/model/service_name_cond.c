
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "service_name_cond.h"

OpenAPI_service_name_cond_t *OpenAPI_service_name_cond_create(
    char *service_name
    )
{
    OpenAPI_service_name_cond_t *service_name_cond_local_var = OpenAPI_malloc(sizeof(OpenAPI_service_name_cond_t));
    if (!service_name_cond_local_var) {
        return NULL;
    }
    service_name_cond_local_var->service_name = service_name;

    return service_name_cond_local_var;
}

void OpenAPI_service_name_cond_free(OpenAPI_service_name_cond_t *service_name_cond)
{
    if (NULL == service_name_cond) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(service_name_cond->service_name);
    ogs_free(service_name_cond);
}

cJSON *OpenAPI_service_name_cond_convertToJSON(OpenAPI_service_name_cond_t *service_name_cond)
{
    cJSON *item = NULL;

    if (service_name_cond == NULL) {
        ogs_error("OpenAPI_service_name_cond_convertToJSON() failed [ServiceNameCond]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!service_name_cond->service_name) {
        ogs_error("OpenAPI_service_name_cond_convertToJSON() failed [service_name]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "serviceName", service_name_cond->service_name) == NULL) {
        ogs_error("OpenAPI_service_name_cond_convertToJSON() failed [service_name]");
        goto end;
    }

end:
    return item;
}

OpenAPI_service_name_cond_t *OpenAPI_service_name_cond_parseFromJSON(cJSON *service_name_condJSON)
{
    OpenAPI_service_name_cond_t *service_name_cond_local_var = NULL;
    cJSON *service_name = cJSON_GetObjectItemCaseSensitive(service_name_condJSON, "serviceName");
    if (!service_name) {
        ogs_error("OpenAPI_service_name_cond_parseFromJSON() failed [service_name]");
        goto end;
    }


    if (!cJSON_IsString(service_name)) {
        ogs_error("OpenAPI_service_name_cond_parseFromJSON() failed [service_name]");
        goto end;
    }

    service_name_cond_local_var = OpenAPI_service_name_cond_create (
        ogs_strdup(service_name->valuestring)
        );

    return service_name_cond_local_var;
end:
    return NULL;
}

