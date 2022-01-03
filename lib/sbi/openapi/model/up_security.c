
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "up_security.h"

OpenAPI_up_security_t *OpenAPI_up_security_create(
    OpenAPI_up_integrity_e up_integr,
    OpenAPI_up_confidentiality_e up_confid
)
{
    OpenAPI_up_security_t *up_security_local_var = ogs_malloc(sizeof(OpenAPI_up_security_t));
    ogs_assert(up_security_local_var);

    up_security_local_var->up_integr = up_integr;
    up_security_local_var->up_confid = up_confid;

    return up_security_local_var;
}

void OpenAPI_up_security_free(OpenAPI_up_security_t *up_security)
{
    if (NULL == up_security) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(up_security);
}

cJSON *OpenAPI_up_security_convertToJSON(OpenAPI_up_security_t *up_security)
{
    cJSON *item = NULL;

    if (up_security == NULL) {
        ogs_error("OpenAPI_up_security_convertToJSON() failed [UpSecurity]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddStringToObject(item, "upIntegr", OpenAPI_up_integrity_ToString(up_security->up_integr)) == NULL) {
        ogs_error("OpenAPI_up_security_convertToJSON() failed [up_integr]");
        goto end;
    }

    if (cJSON_AddStringToObject(item, "upConfid", OpenAPI_up_confidentiality_ToString(up_security->up_confid)) == NULL) {
        ogs_error("OpenAPI_up_security_convertToJSON() failed [up_confid]");
        goto end;
    }

end:
    return item;
}

OpenAPI_up_security_t *OpenAPI_up_security_parseFromJSON(cJSON *up_securityJSON)
{
    OpenAPI_up_security_t *up_security_local_var = NULL;
    cJSON *up_integr = cJSON_GetObjectItemCaseSensitive(up_securityJSON, "upIntegr");
    if (!up_integr) {
        ogs_error("OpenAPI_up_security_parseFromJSON() failed [up_integr]");
        goto end;
    }

    OpenAPI_up_integrity_e up_integrVariable;
    if (!cJSON_IsString(up_integr)) {
        ogs_error("OpenAPI_up_security_parseFromJSON() failed [up_integr]");
        goto end;
    }
    up_integrVariable = OpenAPI_up_integrity_FromString(up_integr->valuestring);

    cJSON *up_confid = cJSON_GetObjectItemCaseSensitive(up_securityJSON, "upConfid");
    if (!up_confid) {
        ogs_error("OpenAPI_up_security_parseFromJSON() failed [up_confid]");
        goto end;
    }

    OpenAPI_up_confidentiality_e up_confidVariable;
    if (!cJSON_IsString(up_confid)) {
        ogs_error("OpenAPI_up_security_parseFromJSON() failed [up_confid]");
        goto end;
    }
    up_confidVariable = OpenAPI_up_confidentiality_FromString(up_confid->valuestring);

    up_security_local_var = OpenAPI_up_security_create (
        up_integrVariable,
        up_confidVariable
    );

    return up_security_local_var;
end:
    return NULL;
}

OpenAPI_up_security_t *OpenAPI_up_security_copy(OpenAPI_up_security_t *dst, OpenAPI_up_security_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_up_security_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_up_security_convertToJSON() failed");
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

    OpenAPI_up_security_free(dst);
    dst = OpenAPI_up_security_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

