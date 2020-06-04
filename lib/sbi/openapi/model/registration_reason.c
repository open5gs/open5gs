
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "registration_reason.h"

OpenAPI_registration_reason_t *OpenAPI_registration_reason_create(
    )
{
    OpenAPI_registration_reason_t *registration_reason_local_var = OpenAPI_malloc(sizeof(OpenAPI_registration_reason_t));
    if (!registration_reason_local_var) {
        return NULL;
    }

    return registration_reason_local_var;
}

void OpenAPI_registration_reason_free(OpenAPI_registration_reason_t *registration_reason)
{
    if (NULL == registration_reason) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(registration_reason);
}

cJSON *OpenAPI_registration_reason_convertToJSON(OpenAPI_registration_reason_t *registration_reason)
{
    cJSON *item = NULL;

    if (registration_reason == NULL) {
        ogs_error("OpenAPI_registration_reason_convertToJSON() failed [RegistrationReason]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_registration_reason_t *OpenAPI_registration_reason_parseFromJSON(cJSON *registration_reasonJSON)
{
    OpenAPI_registration_reason_t *registration_reason_local_var = NULL;
    registration_reason_local_var = OpenAPI_registration_reason_create (
        );

    return registration_reason_local_var;
end:
    return NULL;
}

