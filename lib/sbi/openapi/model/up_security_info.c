
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "up_security_info.h"

OpenAPI_up_security_info_t *OpenAPI_up_security_info_create(
    OpenAPI_up_security_t *up_security,
    OpenAPI_max_integrity_protected_data_rate_e max_integrity_protected_data_rate_ul,
    OpenAPI_max_integrity_protected_data_rate_e max_integrity_protected_data_rate_dl,
    OpenAPI_security_result_t *security_result
)
{
    OpenAPI_up_security_info_t *up_security_info_local_var = ogs_malloc(sizeof(OpenAPI_up_security_info_t));
    ogs_assert(up_security_info_local_var);

    up_security_info_local_var->up_security = up_security;
    up_security_info_local_var->max_integrity_protected_data_rate_ul = max_integrity_protected_data_rate_ul;
    up_security_info_local_var->max_integrity_protected_data_rate_dl = max_integrity_protected_data_rate_dl;
    up_security_info_local_var->security_result = security_result;

    return up_security_info_local_var;
}

void OpenAPI_up_security_info_free(OpenAPI_up_security_info_t *up_security_info)
{
    if (NULL == up_security_info) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_up_security_free(up_security_info->up_security);
    OpenAPI_security_result_free(up_security_info->security_result);
    ogs_free(up_security_info);
}

cJSON *OpenAPI_up_security_info_convertToJSON(OpenAPI_up_security_info_t *up_security_info)
{
    cJSON *item = NULL;

    if (up_security_info == NULL) {
        ogs_error("OpenAPI_up_security_info_convertToJSON() failed [UpSecurityInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    cJSON *up_security_local_JSON = OpenAPI_up_security_convertToJSON(up_security_info->up_security);
    if (up_security_local_JSON == NULL) {
        ogs_error("OpenAPI_up_security_info_convertToJSON() failed [up_security]");
        goto end;
    }
    cJSON_AddItemToObject(item, "upSecurity", up_security_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_up_security_info_convertToJSON() failed [up_security]");
        goto end;
    }

    if (up_security_info->max_integrity_protected_data_rate_ul) {
    if (cJSON_AddStringToObject(item, "maxIntegrityProtectedDataRateUl", OpenAPI_max_integrity_protected_data_rate_ToString(up_security_info->max_integrity_protected_data_rate_ul)) == NULL) {
        ogs_error("OpenAPI_up_security_info_convertToJSON() failed [max_integrity_protected_data_rate_ul]");
        goto end;
    }
    }

    if (up_security_info->max_integrity_protected_data_rate_dl) {
    if (cJSON_AddStringToObject(item, "maxIntegrityProtectedDataRateDl", OpenAPI_max_integrity_protected_data_rate_ToString(up_security_info->max_integrity_protected_data_rate_dl)) == NULL) {
        ogs_error("OpenAPI_up_security_info_convertToJSON() failed [max_integrity_protected_data_rate_dl]");
        goto end;
    }
    }

    if (up_security_info->security_result) {
    cJSON *security_result_local_JSON = OpenAPI_security_result_convertToJSON(up_security_info->security_result);
    if (security_result_local_JSON == NULL) {
        ogs_error("OpenAPI_up_security_info_convertToJSON() failed [security_result]");
        goto end;
    }
    cJSON_AddItemToObject(item, "securityResult", security_result_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_up_security_info_convertToJSON() failed [security_result]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_up_security_info_t *OpenAPI_up_security_info_parseFromJSON(cJSON *up_security_infoJSON)
{
    OpenAPI_up_security_info_t *up_security_info_local_var = NULL;
    cJSON *up_security = cJSON_GetObjectItemCaseSensitive(up_security_infoJSON, "upSecurity");
    if (!up_security) {
        ogs_error("OpenAPI_up_security_info_parseFromJSON() failed [up_security]");
        goto end;
    }

    OpenAPI_up_security_t *up_security_local_nonprim = NULL;
    up_security_local_nonprim = OpenAPI_up_security_parseFromJSON(up_security);

    cJSON *max_integrity_protected_data_rate_ul = cJSON_GetObjectItemCaseSensitive(up_security_infoJSON, "maxIntegrityProtectedDataRateUl");

    OpenAPI_max_integrity_protected_data_rate_e max_integrity_protected_data_rate_ulVariable;
    if (max_integrity_protected_data_rate_ul) {
    if (!cJSON_IsString(max_integrity_protected_data_rate_ul)) {
        ogs_error("OpenAPI_up_security_info_parseFromJSON() failed [max_integrity_protected_data_rate_ul]");
        goto end;
    }
    max_integrity_protected_data_rate_ulVariable = OpenAPI_max_integrity_protected_data_rate_FromString(max_integrity_protected_data_rate_ul->valuestring);
    }

    cJSON *max_integrity_protected_data_rate_dl = cJSON_GetObjectItemCaseSensitive(up_security_infoJSON, "maxIntegrityProtectedDataRateDl");

    OpenAPI_max_integrity_protected_data_rate_e max_integrity_protected_data_rate_dlVariable;
    if (max_integrity_protected_data_rate_dl) {
    if (!cJSON_IsString(max_integrity_protected_data_rate_dl)) {
        ogs_error("OpenAPI_up_security_info_parseFromJSON() failed [max_integrity_protected_data_rate_dl]");
        goto end;
    }
    max_integrity_protected_data_rate_dlVariable = OpenAPI_max_integrity_protected_data_rate_FromString(max_integrity_protected_data_rate_dl->valuestring);
    }

    cJSON *security_result = cJSON_GetObjectItemCaseSensitive(up_security_infoJSON, "securityResult");

    OpenAPI_security_result_t *security_result_local_nonprim = NULL;
    if (security_result) {
    security_result_local_nonprim = OpenAPI_security_result_parseFromJSON(security_result);
    }

    up_security_info_local_var = OpenAPI_up_security_info_create (
        up_security_local_nonprim,
        max_integrity_protected_data_rate_ul ? max_integrity_protected_data_rate_ulVariable : 0,
        max_integrity_protected_data_rate_dl ? max_integrity_protected_data_rate_dlVariable : 0,
        security_result ? security_result_local_nonprim : NULL
    );

    return up_security_info_local_var;
end:
    return NULL;
}

OpenAPI_up_security_info_t *OpenAPI_up_security_info_copy(OpenAPI_up_security_info_t *dst, OpenAPI_up_security_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_up_security_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_up_security_info_convertToJSON() failed");
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

    OpenAPI_up_security_info_free(dst);
    dst = OpenAPI_up_security_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

