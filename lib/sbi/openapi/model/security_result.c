
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "security_result.h"

OpenAPI_security_result_t *OpenAPI_security_result_create(
    OpenAPI_protection_result_e integrity_protection_result,
    OpenAPI_protection_result_e confidentiality_protection_result
)
{
    OpenAPI_security_result_t *security_result_local_var = ogs_malloc(sizeof(OpenAPI_security_result_t));
    ogs_assert(security_result_local_var);

    security_result_local_var->integrity_protection_result = integrity_protection_result;
    security_result_local_var->confidentiality_protection_result = confidentiality_protection_result;

    return security_result_local_var;
}

void OpenAPI_security_result_free(OpenAPI_security_result_t *security_result)
{
    if (NULL == security_result) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(security_result);
}

cJSON *OpenAPI_security_result_convertToJSON(OpenAPI_security_result_t *security_result)
{
    cJSON *item = NULL;

    if (security_result == NULL) {
        ogs_error("OpenAPI_security_result_convertToJSON() failed [SecurityResult]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (security_result->integrity_protection_result) {
    if (cJSON_AddStringToObject(item, "integrityProtectionResult", OpenAPI_protection_result_ToString(security_result->integrity_protection_result)) == NULL) {
        ogs_error("OpenAPI_security_result_convertToJSON() failed [integrity_protection_result]");
        goto end;
    }
    }

    if (security_result->confidentiality_protection_result) {
    if (cJSON_AddStringToObject(item, "confidentialityProtectionResult", OpenAPI_protection_result_ToString(security_result->confidentiality_protection_result)) == NULL) {
        ogs_error("OpenAPI_security_result_convertToJSON() failed [confidentiality_protection_result]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_security_result_t *OpenAPI_security_result_parseFromJSON(cJSON *security_resultJSON)
{
    OpenAPI_security_result_t *security_result_local_var = NULL;
    cJSON *integrity_protection_result = cJSON_GetObjectItemCaseSensitive(security_resultJSON, "integrityProtectionResult");

    OpenAPI_protection_result_e integrity_protection_resultVariable;
    if (integrity_protection_result) {
    if (!cJSON_IsString(integrity_protection_result)) {
        ogs_error("OpenAPI_security_result_parseFromJSON() failed [integrity_protection_result]");
        goto end;
    }
    integrity_protection_resultVariable = OpenAPI_protection_result_FromString(integrity_protection_result->valuestring);
    }

    cJSON *confidentiality_protection_result = cJSON_GetObjectItemCaseSensitive(security_resultJSON, "confidentialityProtectionResult");

    OpenAPI_protection_result_e confidentiality_protection_resultVariable;
    if (confidentiality_protection_result) {
    if (!cJSON_IsString(confidentiality_protection_result)) {
        ogs_error("OpenAPI_security_result_parseFromJSON() failed [confidentiality_protection_result]");
        goto end;
    }
    confidentiality_protection_resultVariable = OpenAPI_protection_result_FromString(confidentiality_protection_result->valuestring);
    }

    security_result_local_var = OpenAPI_security_result_create (
        integrity_protection_result ? integrity_protection_resultVariable : 0,
        confidentiality_protection_result ? confidentiality_protection_resultVariable : 0
    );

    return security_result_local_var;
end:
    return NULL;
}

OpenAPI_security_result_t *OpenAPI_security_result_copy(OpenAPI_security_result_t *dst, OpenAPI_security_result_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_security_result_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_security_result_convertToJSON() failed");
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

    OpenAPI_security_result_free(dst);
    dst = OpenAPI_security_result_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

