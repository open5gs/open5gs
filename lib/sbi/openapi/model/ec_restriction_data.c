
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ec_restriction_data.h"

OpenAPI_ec_restriction_data_t *OpenAPI_ec_restriction_data_create(
    int ec_mode_a_restricted,
    int ec_mode_b_restricted
    )
{
    OpenAPI_ec_restriction_data_t *ec_restriction_data_local_var = OpenAPI_malloc(sizeof(OpenAPI_ec_restriction_data_t));
    if (!ec_restriction_data_local_var) {
        return NULL;
    }
    ec_restriction_data_local_var->ec_mode_a_restricted = ec_mode_a_restricted;
    ec_restriction_data_local_var->ec_mode_b_restricted = ec_mode_b_restricted;

    return ec_restriction_data_local_var;
}

void OpenAPI_ec_restriction_data_free(OpenAPI_ec_restriction_data_t *ec_restriction_data)
{
    if (NULL == ec_restriction_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(ec_restriction_data);
}

cJSON *OpenAPI_ec_restriction_data_convertToJSON(OpenAPI_ec_restriction_data_t *ec_restriction_data)
{
    cJSON *item = NULL;

    if (ec_restriction_data == NULL) {
        ogs_error("OpenAPI_ec_restriction_data_convertToJSON() failed [EcRestrictionData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ec_restriction_data->ec_mode_a_restricted >= 0) {
        if (cJSON_AddBoolToObject(item, "ecModeARestricted", ec_restriction_data->ec_mode_a_restricted) == NULL) {
            ogs_error("OpenAPI_ec_restriction_data_convertToJSON() failed [ec_mode_a_restricted]");
            goto end;
        }
    }

    if (ec_restriction_data->ec_mode_b_restricted >= 0) {
        if (cJSON_AddBoolToObject(item, "ecModeBRestricted", ec_restriction_data->ec_mode_b_restricted) == NULL) {
            ogs_error("OpenAPI_ec_restriction_data_convertToJSON() failed [ec_mode_b_restricted]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_ec_restriction_data_t *OpenAPI_ec_restriction_data_parseFromJSON(cJSON *ec_restriction_dataJSON)
{
    OpenAPI_ec_restriction_data_t *ec_restriction_data_local_var = NULL;
    cJSON *ec_mode_a_restricted = cJSON_GetObjectItemCaseSensitive(ec_restriction_dataJSON, "ecModeARestricted");

    if (ec_mode_a_restricted) {
        if (!cJSON_IsBool(ec_mode_a_restricted)) {
            ogs_error("OpenAPI_ec_restriction_data_parseFromJSON() failed [ec_mode_a_restricted]");
            goto end;
        }
    }

    cJSON *ec_mode_b_restricted = cJSON_GetObjectItemCaseSensitive(ec_restriction_dataJSON, "ecModeBRestricted");

    if (ec_mode_b_restricted) {
        if (!cJSON_IsBool(ec_mode_b_restricted)) {
            ogs_error("OpenAPI_ec_restriction_data_parseFromJSON() failed [ec_mode_b_restricted]");
            goto end;
        }
    }

    ec_restriction_data_local_var = OpenAPI_ec_restriction_data_create (
        ec_mode_a_restricted ? ec_mode_a_restricted->valueint : 0,
        ec_mode_b_restricted ? ec_mode_b_restricted->valueint : 0
        );

    return ec_restriction_data_local_var;
end:
    return NULL;
}

