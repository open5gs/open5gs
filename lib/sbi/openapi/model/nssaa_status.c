
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nssaa_status.h"

OpenAPI_nssaa_status_t *OpenAPI_nssaa_status_create(
    OpenAPI_snssai_t *snssai,
    OpenAPI_auth_status_e status
)
{
    OpenAPI_nssaa_status_t *nssaa_status_local_var = ogs_malloc(sizeof(OpenAPI_nssaa_status_t));
    ogs_assert(nssaa_status_local_var);

    nssaa_status_local_var->snssai = snssai;
    nssaa_status_local_var->status = status;

    return nssaa_status_local_var;
}

void OpenAPI_nssaa_status_free(OpenAPI_nssaa_status_t *nssaa_status)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == nssaa_status) {
        return;
    }
    if (nssaa_status->snssai) {
        OpenAPI_snssai_free(nssaa_status->snssai);
        nssaa_status->snssai = NULL;
    }
    ogs_free(nssaa_status);
}

cJSON *OpenAPI_nssaa_status_convertToJSON(OpenAPI_nssaa_status_t *nssaa_status)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (nssaa_status == NULL) {
        ogs_error("OpenAPI_nssaa_status_convertToJSON() failed [NssaaStatus]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!nssaa_status->snssai) {
        ogs_error("OpenAPI_nssaa_status_convertToJSON() failed [snssai]");
        return NULL;
    }
    cJSON *snssai_local_JSON = OpenAPI_snssai_convertToJSON(nssaa_status->snssai);
    if (snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_nssaa_status_convertToJSON() failed [snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "snssai", snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_nssaa_status_convertToJSON() failed [snssai]");
        goto end;
    }

    if (nssaa_status->status == OpenAPI_auth_status_NULL) {
        ogs_error("OpenAPI_nssaa_status_convertToJSON() failed [status]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "status", OpenAPI_auth_status_ToString(nssaa_status->status)) == NULL) {
        ogs_error("OpenAPI_nssaa_status_convertToJSON() failed [status]");
        goto end;
    }

end:
    return item;
}

OpenAPI_nssaa_status_t *OpenAPI_nssaa_status_parseFromJSON(cJSON *nssaa_statusJSON)
{
    OpenAPI_nssaa_status_t *nssaa_status_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *snssai = NULL;
    OpenAPI_snssai_t *snssai_local_nonprim = NULL;
    cJSON *status = NULL;
    OpenAPI_auth_status_e statusVariable = 0;
    snssai = cJSON_GetObjectItemCaseSensitive(nssaa_statusJSON, "snssai");
    if (!snssai) {
        ogs_error("OpenAPI_nssaa_status_parseFromJSON() failed [snssai]");
        goto end;
    }
    snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(snssai);
    if (!snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [snssai]");
        goto end;
    }

    status = cJSON_GetObjectItemCaseSensitive(nssaa_statusJSON, "status");
    if (!status) {
        ogs_error("OpenAPI_nssaa_status_parseFromJSON() failed [status]");
        goto end;
    }
    if (!cJSON_IsString(status)) {
        ogs_error("OpenAPI_nssaa_status_parseFromJSON() failed [status]");
        goto end;
    }
    statusVariable = OpenAPI_auth_status_FromString(status->valuestring);

    nssaa_status_local_var = OpenAPI_nssaa_status_create (
        snssai_local_nonprim,
        statusVariable
    );

    return nssaa_status_local_var;
end:
    if (snssai_local_nonprim) {
        OpenAPI_snssai_free(snssai_local_nonprim);
        snssai_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_nssaa_status_t *OpenAPI_nssaa_status_copy(OpenAPI_nssaa_status_t *dst, OpenAPI_nssaa_status_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nssaa_status_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nssaa_status_convertToJSON() failed");
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

    OpenAPI_nssaa_status_free(dst);
    dst = OpenAPI_nssaa_status_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

