
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "additional_snssai_data.h"

OpenAPI_additional_snssai_data_t *OpenAPI_additional_snssai_data_create(
    bool is_required_authn_authz,
    int required_authn_authz
)
{
    OpenAPI_additional_snssai_data_t *additional_snssai_data_local_var = ogs_malloc(sizeof(OpenAPI_additional_snssai_data_t));
    ogs_assert(additional_snssai_data_local_var);

    additional_snssai_data_local_var->is_required_authn_authz = is_required_authn_authz;
    additional_snssai_data_local_var->required_authn_authz = required_authn_authz;

    return additional_snssai_data_local_var;
}

void OpenAPI_additional_snssai_data_free(OpenAPI_additional_snssai_data_t *additional_snssai_data)
{
    if (NULL == additional_snssai_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(additional_snssai_data);
}

cJSON *OpenAPI_additional_snssai_data_convertToJSON(OpenAPI_additional_snssai_data_t *additional_snssai_data)
{
    cJSON *item = NULL;

    if (additional_snssai_data == NULL) {
        ogs_error("OpenAPI_additional_snssai_data_convertToJSON() failed [AdditionalSnssaiData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (additional_snssai_data->is_required_authn_authz) {
    if (cJSON_AddBoolToObject(item, "requiredAuthnAuthz", additional_snssai_data->required_authn_authz) == NULL) {
        ogs_error("OpenAPI_additional_snssai_data_convertToJSON() failed [required_authn_authz]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_additional_snssai_data_t *OpenAPI_additional_snssai_data_parseFromJSON(cJSON *additional_snssai_dataJSON)
{
    OpenAPI_additional_snssai_data_t *additional_snssai_data_local_var = NULL;
    cJSON *required_authn_authz = cJSON_GetObjectItemCaseSensitive(additional_snssai_dataJSON, "requiredAuthnAuthz");

    if (required_authn_authz) {
    if (!cJSON_IsBool(required_authn_authz)) {
        ogs_error("OpenAPI_additional_snssai_data_parseFromJSON() failed [required_authn_authz]");
        goto end;
    }
    }

    additional_snssai_data_local_var = OpenAPI_additional_snssai_data_create (
        required_authn_authz ? true : false,
        required_authn_authz ? required_authn_authz->valueint : 0
    );

    return additional_snssai_data_local_var;
end:
    return NULL;
}

OpenAPI_additional_snssai_data_t *OpenAPI_additional_snssai_data_copy(OpenAPI_additional_snssai_data_t *dst, OpenAPI_additional_snssai_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_additional_snssai_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_additional_snssai_data_convertToJSON() failed");
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

    OpenAPI_additional_snssai_data_free(dst);
    dst = OpenAPI_additional_snssai_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

