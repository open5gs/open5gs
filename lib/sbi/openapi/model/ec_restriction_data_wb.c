
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ec_restriction_data_wb.h"

OpenAPI_ec_restriction_data_wb_t *OpenAPI_ec_restriction_data_wb_create(
    bool is_ec_mode_a_restricted,
    int ec_mode_a_restricted,
    int ec_mode_b_restricted
)
{
    OpenAPI_ec_restriction_data_wb_t *ec_restriction_data_wb_local_var = ogs_malloc(sizeof(OpenAPI_ec_restriction_data_wb_t));
    ogs_assert(ec_restriction_data_wb_local_var);

    ec_restriction_data_wb_local_var->is_ec_mode_a_restricted = is_ec_mode_a_restricted;
    ec_restriction_data_wb_local_var->ec_mode_a_restricted = ec_mode_a_restricted;
    ec_restriction_data_wb_local_var->ec_mode_b_restricted = ec_mode_b_restricted;

    return ec_restriction_data_wb_local_var;
}

void OpenAPI_ec_restriction_data_wb_free(OpenAPI_ec_restriction_data_wb_t *ec_restriction_data_wb)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ec_restriction_data_wb) {
        return;
    }
    ogs_free(ec_restriction_data_wb);
}

cJSON *OpenAPI_ec_restriction_data_wb_convertToJSON(OpenAPI_ec_restriction_data_wb_t *ec_restriction_data_wb)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ec_restriction_data_wb == NULL) {
        ogs_error("OpenAPI_ec_restriction_data_wb_convertToJSON() failed [EcRestrictionDataWb]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ec_restriction_data_wb->is_ec_mode_a_restricted) {
    if (cJSON_AddBoolToObject(item, "ecModeARestricted", ec_restriction_data_wb->ec_mode_a_restricted) == NULL) {
        ogs_error("OpenAPI_ec_restriction_data_wb_convertToJSON() failed [ec_mode_a_restricted]");
        goto end;
    }
    }

    if (cJSON_AddBoolToObject(item, "ecModeBRestricted", ec_restriction_data_wb->ec_mode_b_restricted) == NULL) {
        ogs_error("OpenAPI_ec_restriction_data_wb_convertToJSON() failed [ec_mode_b_restricted]");
        goto end;
    }

end:
    return item;
}

OpenAPI_ec_restriction_data_wb_t *OpenAPI_ec_restriction_data_wb_parseFromJSON(cJSON *ec_restriction_data_wbJSON)
{
    OpenAPI_ec_restriction_data_wb_t *ec_restriction_data_wb_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ec_mode_a_restricted = NULL;
    cJSON *ec_mode_b_restricted = NULL;
    ec_mode_a_restricted = cJSON_GetObjectItemCaseSensitive(ec_restriction_data_wbJSON, "ecModeARestricted");
    if (ec_mode_a_restricted) {
    if (!cJSON_IsBool(ec_mode_a_restricted)) {
        ogs_error("OpenAPI_ec_restriction_data_wb_parseFromJSON() failed [ec_mode_a_restricted]");
        goto end;
    }
    }

    ec_mode_b_restricted = cJSON_GetObjectItemCaseSensitive(ec_restriction_data_wbJSON, "ecModeBRestricted");
    if (!ec_mode_b_restricted) {
        ogs_error("OpenAPI_ec_restriction_data_wb_parseFromJSON() failed [ec_mode_b_restricted]");
        goto end;
    }
    if (!cJSON_IsBool(ec_mode_b_restricted)) {
        ogs_error("OpenAPI_ec_restriction_data_wb_parseFromJSON() failed [ec_mode_b_restricted]");
        goto end;
    }

    ec_restriction_data_wb_local_var = OpenAPI_ec_restriction_data_wb_create (
        ec_mode_a_restricted ? true : false,
        ec_mode_a_restricted ? ec_mode_a_restricted->valueint : 0,
        
        ec_mode_b_restricted->valueint
    );

    return ec_restriction_data_wb_local_var;
end:
    return NULL;
}

OpenAPI_ec_restriction_data_wb_t *OpenAPI_ec_restriction_data_wb_copy(OpenAPI_ec_restriction_data_wb_t *dst, OpenAPI_ec_restriction_data_wb_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ec_restriction_data_wb_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ec_restriction_data_wb_convertToJSON() failed");
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

    OpenAPI_ec_restriction_data_wb_free(dst);
    dst = OpenAPI_ec_restriction_data_wb_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

