
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sms_support.h"

OpenAPI_sms_support_t *OpenAPI_sms_support_create(
    )
{
    OpenAPI_sms_support_t *sms_support_local_var = OpenAPI_malloc(sizeof(OpenAPI_sms_support_t));
    if (!sms_support_local_var) {
        return NULL;
    }

    return sms_support_local_var;
}

void OpenAPI_sms_support_free(OpenAPI_sms_support_t *sms_support)
{
    if (NULL == sms_support) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(sms_support);
}

cJSON *OpenAPI_sms_support_convertToJSON(OpenAPI_sms_support_t *sms_support)
{
    cJSON *item = NULL;

    if (sms_support == NULL) {
        ogs_error("OpenAPI_sms_support_convertToJSON() failed [SmsSupport]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_sms_support_t *OpenAPI_sms_support_parseFromJSON(cJSON *sms_supportJSON)
{
    OpenAPI_sms_support_t *sms_support_local_var = NULL;
    sms_support_local_var = OpenAPI_sms_support_create (
        );

    return sms_support_local_var;
end:
    return NULL;
}

