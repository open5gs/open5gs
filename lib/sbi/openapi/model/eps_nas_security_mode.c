
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "eps_nas_security_mode.h"

OpenAPI_eps_nas_security_mode_t *OpenAPI_eps_nas_security_mode_create(
    OpenAPI_eps_nas_integrity_algorithm_e integrity_algorithm,
    OpenAPI_eps_nas_ciphering_algorithm_e ciphering_algorithm
)
{
    OpenAPI_eps_nas_security_mode_t *eps_nas_security_mode_local_var = ogs_malloc(sizeof(OpenAPI_eps_nas_security_mode_t));
    ogs_assert(eps_nas_security_mode_local_var);

    eps_nas_security_mode_local_var->integrity_algorithm = integrity_algorithm;
    eps_nas_security_mode_local_var->ciphering_algorithm = ciphering_algorithm;

    return eps_nas_security_mode_local_var;
}

void OpenAPI_eps_nas_security_mode_free(OpenAPI_eps_nas_security_mode_t *eps_nas_security_mode)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == eps_nas_security_mode) {
        return;
    }
    ogs_free(eps_nas_security_mode);
}

cJSON *OpenAPI_eps_nas_security_mode_convertToJSON(OpenAPI_eps_nas_security_mode_t *eps_nas_security_mode)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (eps_nas_security_mode == NULL) {
        ogs_error("OpenAPI_eps_nas_security_mode_convertToJSON() failed [EpsNasSecurityMode]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (eps_nas_security_mode->integrity_algorithm == OpenAPI_eps_nas_integrity_algorithm_NULL) {
        ogs_error("OpenAPI_eps_nas_security_mode_convertToJSON() failed [integrity_algorithm]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "integrityAlgorithm", OpenAPI_eps_nas_integrity_algorithm_ToString(eps_nas_security_mode->integrity_algorithm)) == NULL) {
        ogs_error("OpenAPI_eps_nas_security_mode_convertToJSON() failed [integrity_algorithm]");
        goto end;
    }

    if (eps_nas_security_mode->ciphering_algorithm == OpenAPI_eps_nas_ciphering_algorithm_NULL) {
        ogs_error("OpenAPI_eps_nas_security_mode_convertToJSON() failed [ciphering_algorithm]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "cipheringAlgorithm", OpenAPI_eps_nas_ciphering_algorithm_ToString(eps_nas_security_mode->ciphering_algorithm)) == NULL) {
        ogs_error("OpenAPI_eps_nas_security_mode_convertToJSON() failed [ciphering_algorithm]");
        goto end;
    }

end:
    return item;
}

OpenAPI_eps_nas_security_mode_t *OpenAPI_eps_nas_security_mode_parseFromJSON(cJSON *eps_nas_security_modeJSON)
{
    OpenAPI_eps_nas_security_mode_t *eps_nas_security_mode_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *integrity_algorithm = NULL;
    OpenAPI_eps_nas_integrity_algorithm_e integrity_algorithmVariable = 0;
    cJSON *ciphering_algorithm = NULL;
    OpenAPI_eps_nas_ciphering_algorithm_e ciphering_algorithmVariable = 0;
    integrity_algorithm = cJSON_GetObjectItemCaseSensitive(eps_nas_security_modeJSON, "integrityAlgorithm");
    if (!integrity_algorithm) {
        ogs_error("OpenAPI_eps_nas_security_mode_parseFromJSON() failed [integrity_algorithm]");
        goto end;
    }
    if (!cJSON_IsString(integrity_algorithm)) {
        ogs_error("OpenAPI_eps_nas_security_mode_parseFromJSON() failed [integrity_algorithm]");
        goto end;
    }
    integrity_algorithmVariable = OpenAPI_eps_nas_integrity_algorithm_FromString(integrity_algorithm->valuestring);

    ciphering_algorithm = cJSON_GetObjectItemCaseSensitive(eps_nas_security_modeJSON, "cipheringAlgorithm");
    if (!ciphering_algorithm) {
        ogs_error("OpenAPI_eps_nas_security_mode_parseFromJSON() failed [ciphering_algorithm]");
        goto end;
    }
    if (!cJSON_IsString(ciphering_algorithm)) {
        ogs_error("OpenAPI_eps_nas_security_mode_parseFromJSON() failed [ciphering_algorithm]");
        goto end;
    }
    ciphering_algorithmVariable = OpenAPI_eps_nas_ciphering_algorithm_FromString(ciphering_algorithm->valuestring);

    eps_nas_security_mode_local_var = OpenAPI_eps_nas_security_mode_create (
        integrity_algorithmVariable,
        ciphering_algorithmVariable
    );

    return eps_nas_security_mode_local_var;
end:
    return NULL;
}

OpenAPI_eps_nas_security_mode_t *OpenAPI_eps_nas_security_mode_copy(OpenAPI_eps_nas_security_mode_t *dst, OpenAPI_eps_nas_security_mode_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_eps_nas_security_mode_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_eps_nas_security_mode_convertToJSON() failed");
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

    OpenAPI_eps_nas_security_mode_free(dst);
    dst = OpenAPI_eps_nas_security_mode_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

