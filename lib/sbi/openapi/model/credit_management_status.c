
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "credit_management_status.h"

OpenAPI_credit_management_status_t *OpenAPI_credit_management_status_create(
    )
{
    OpenAPI_credit_management_status_t *credit_management_status_local_var = OpenAPI_malloc(sizeof(OpenAPI_credit_management_status_t));
    if (!credit_management_status_local_var) {
        return NULL;
    }

    return credit_management_status_local_var;
}

void OpenAPI_credit_management_status_free(OpenAPI_credit_management_status_t *credit_management_status)
{
    if (NULL == credit_management_status) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(credit_management_status);
}

cJSON *OpenAPI_credit_management_status_convertToJSON(OpenAPI_credit_management_status_t *credit_management_status)
{
    cJSON *item = NULL;

    if (credit_management_status == NULL) {
        ogs_error("OpenAPI_credit_management_status_convertToJSON() failed [CreditManagementStatus]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_credit_management_status_t *OpenAPI_credit_management_status_parseFromJSON(cJSON *credit_management_statusJSON)
{
    OpenAPI_credit_management_status_t *credit_management_status_local_var = NULL;
    credit_management_status_local_var = OpenAPI_credit_management_status_create (
        );

    return credit_management_status_local_var;
end:
    return NULL;
}

OpenAPI_credit_management_status_t *OpenAPI_credit_management_status_copy(OpenAPI_credit_management_status_t *dst, OpenAPI_credit_management_status_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_credit_management_status_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_credit_management_status_convertToJSON() failed");
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

    OpenAPI_credit_management_status_free(dst);
    dst = OpenAPI_credit_management_status_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

