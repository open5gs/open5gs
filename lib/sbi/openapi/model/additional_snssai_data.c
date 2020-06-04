
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "additional_snssai_data.h"

OpenAPI_additional_snssai_data_t *OpenAPI_additional_snssai_data_create(
    int required_authn_authz
    )
{
    OpenAPI_additional_snssai_data_t *additional_snssai_data_local_var = OpenAPI_malloc(sizeof(OpenAPI_additional_snssai_data_t));
    if (!additional_snssai_data_local_var) {
        return NULL;
    }
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
    if (additional_snssai_data->required_authn_authz >= 0) {
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
        required_authn_authz ? required_authn_authz->valueint : 0
        );

    return additional_snssai_data_local_var;
end:
    return NULL;
}

