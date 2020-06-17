
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "up_security.h"

OpenAPI_up_security_t *OpenAPI_up_security_create(
    OpenAPI_up_integrity_t *up_integr,
    OpenAPI_up_confidentiality_t *up_confid
    )
{
    OpenAPI_up_security_t *up_security_local_var = OpenAPI_malloc(sizeof(OpenAPI_up_security_t));
    if (!up_security_local_var) {
        return NULL;
    }
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
    OpenAPI_up_integrity_free(up_security->up_integr);
    OpenAPI_up_confidentiality_free(up_security->up_confid);
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
    if (!up_security->up_integr) {
        ogs_error("OpenAPI_up_security_convertToJSON() failed [up_integr]");
        goto end;
    }
    cJSON *up_integr_local_JSON = OpenAPI_up_integrity_convertToJSON(up_security->up_integr);
    if (up_integr_local_JSON == NULL) {
        ogs_error("OpenAPI_up_security_convertToJSON() failed [up_integr]");
        goto end;
    }
    cJSON_AddItemToObject(item, "upIntegr", up_integr_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_up_security_convertToJSON() failed [up_integr]");
        goto end;
    }

    if (!up_security->up_confid) {
        ogs_error("OpenAPI_up_security_convertToJSON() failed [up_confid]");
        goto end;
    }
    cJSON *up_confid_local_JSON = OpenAPI_up_confidentiality_convertToJSON(up_security->up_confid);
    if (up_confid_local_JSON == NULL) {
        ogs_error("OpenAPI_up_security_convertToJSON() failed [up_confid]");
        goto end;
    }
    cJSON_AddItemToObject(item, "upConfid", up_confid_local_JSON);
    if (item->child == NULL) {
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

    OpenAPI_up_integrity_t *up_integr_local_nonprim = NULL;

    up_integr_local_nonprim = OpenAPI_up_integrity_parseFromJSON(up_integr);

    cJSON *up_confid = cJSON_GetObjectItemCaseSensitive(up_securityJSON, "upConfid");
    if (!up_confid) {
        ogs_error("OpenAPI_up_security_parseFromJSON() failed [up_confid]");
        goto end;
    }

    OpenAPI_up_confidentiality_t *up_confid_local_nonprim = NULL;

    up_confid_local_nonprim = OpenAPI_up_confidentiality_parseFromJSON(up_confid);

    up_security_local_var = OpenAPI_up_security_create (
        up_integr_local_nonprim,
        up_confid_local_nonprim
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

