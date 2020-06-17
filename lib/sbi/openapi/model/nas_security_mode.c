
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nas_security_mode.h"

OpenAPI_nas_security_mode_t *OpenAPI_nas_security_mode_create(
    OpenAPI_integrity_algorithm_e integrity_algorithm,
    OpenAPI_ciphering_algorithm_e ciphering_algorithm
    )
{
    OpenAPI_nas_security_mode_t *nas_security_mode_local_var = OpenAPI_malloc(sizeof(OpenAPI_nas_security_mode_t));
    if (!nas_security_mode_local_var) {
        return NULL;
    }
    nas_security_mode_local_var->integrity_algorithm = integrity_algorithm;
    nas_security_mode_local_var->ciphering_algorithm = ciphering_algorithm;

    return nas_security_mode_local_var;
}

void OpenAPI_nas_security_mode_free(OpenAPI_nas_security_mode_t *nas_security_mode)
{
    if (NULL == nas_security_mode) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(nas_security_mode);
}

cJSON *OpenAPI_nas_security_mode_convertToJSON(OpenAPI_nas_security_mode_t *nas_security_mode)
{
    cJSON *item = NULL;

    if (nas_security_mode == NULL) {
        ogs_error("OpenAPI_nas_security_mode_convertToJSON() failed [NasSecurityMode]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!nas_security_mode->integrity_algorithm) {
        ogs_error("OpenAPI_nas_security_mode_convertToJSON() failed [integrity_algorithm]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "integrityAlgorithm", OpenAPI_integrity_algorithm_ToString(nas_security_mode->integrity_algorithm)) == NULL) {
        ogs_error("OpenAPI_nas_security_mode_convertToJSON() failed [integrity_algorithm]");
        goto end;
    }

    if (!nas_security_mode->ciphering_algorithm) {
        ogs_error("OpenAPI_nas_security_mode_convertToJSON() failed [ciphering_algorithm]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "cipheringAlgorithm", OpenAPI_ciphering_algorithm_ToString(nas_security_mode->ciphering_algorithm)) == NULL) {
        ogs_error("OpenAPI_nas_security_mode_convertToJSON() failed [ciphering_algorithm]");
        goto end;
    }

end:
    return item;
}

OpenAPI_nas_security_mode_t *OpenAPI_nas_security_mode_parseFromJSON(cJSON *nas_security_modeJSON)
{
    OpenAPI_nas_security_mode_t *nas_security_mode_local_var = NULL;
    cJSON *integrity_algorithm = cJSON_GetObjectItemCaseSensitive(nas_security_modeJSON, "integrityAlgorithm");
    if (!integrity_algorithm) {
        ogs_error("OpenAPI_nas_security_mode_parseFromJSON() failed [integrity_algorithm]");
        goto end;
    }

    OpenAPI_integrity_algorithm_e integrity_algorithmVariable;

    if (!cJSON_IsString(integrity_algorithm)) {
        ogs_error("OpenAPI_nas_security_mode_parseFromJSON() failed [integrity_algorithm]");
        goto end;
    }
    integrity_algorithmVariable = OpenAPI_integrity_algorithm_FromString(integrity_algorithm->valuestring);

    cJSON *ciphering_algorithm = cJSON_GetObjectItemCaseSensitive(nas_security_modeJSON, "cipheringAlgorithm");
    if (!ciphering_algorithm) {
        ogs_error("OpenAPI_nas_security_mode_parseFromJSON() failed [ciphering_algorithm]");
        goto end;
    }

    OpenAPI_ciphering_algorithm_e ciphering_algorithmVariable;

    if (!cJSON_IsString(ciphering_algorithm)) {
        ogs_error("OpenAPI_nas_security_mode_parseFromJSON() failed [ciphering_algorithm]");
        goto end;
    }
    ciphering_algorithmVariable = OpenAPI_ciphering_algorithm_FromString(ciphering_algorithm->valuestring);

    nas_security_mode_local_var = OpenAPI_nas_security_mode_create (
        integrity_algorithmVariable,
        ciphering_algorithmVariable
        );

    return nas_security_mode_local_var;
end:
    return NULL;
}

OpenAPI_nas_security_mode_t *OpenAPI_nas_security_mode_copy(OpenAPI_nas_security_mode_t *dst, OpenAPI_nas_security_mode_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nas_security_mode_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nas_security_mode_convertToJSON() failed");
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

    OpenAPI_nas_security_mode_free(dst);
    dst = OpenAPI_nas_security_mode_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

