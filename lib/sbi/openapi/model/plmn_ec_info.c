
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "plmn_ec_info.h"

OpenAPI_plmn_ec_info_t *OpenAPI_plmn_ec_info_create(
    OpenAPI_plmn_id_t *plmn_id,
    int ec_mode_a_restricted,
    int ec_mode_b_restricted
    )
{
    OpenAPI_plmn_ec_info_t *plmn_ec_info_local_var = OpenAPI_malloc(sizeof(OpenAPI_plmn_ec_info_t));
    if (!plmn_ec_info_local_var) {
        return NULL;
    }
    plmn_ec_info_local_var->plmn_id = plmn_id;
    plmn_ec_info_local_var->ec_mode_a_restricted = ec_mode_a_restricted;
    plmn_ec_info_local_var->ec_mode_b_restricted = ec_mode_b_restricted;

    return plmn_ec_info_local_var;
}

void OpenAPI_plmn_ec_info_free(OpenAPI_plmn_ec_info_t *plmn_ec_info)
{
    if (NULL == plmn_ec_info) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_plmn_id_free(plmn_ec_info->plmn_id);
    ogs_free(plmn_ec_info);
}

cJSON *OpenAPI_plmn_ec_info_convertToJSON(OpenAPI_plmn_ec_info_t *plmn_ec_info)
{
    cJSON *item = NULL;

    if (plmn_ec_info == NULL) {
        ogs_error("OpenAPI_plmn_ec_info_convertToJSON() failed [PlmnEcInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!plmn_ec_info->plmn_id) {
        ogs_error("OpenAPI_plmn_ec_info_convertToJSON() failed [plmn_id]");
        goto end;
    }
    cJSON *plmn_id_local_JSON = OpenAPI_plmn_id_convertToJSON(plmn_ec_info->plmn_id);
    if (plmn_id_local_JSON == NULL) {
        ogs_error("OpenAPI_plmn_ec_info_convertToJSON() failed [plmn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "plmnId", plmn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_plmn_ec_info_convertToJSON() failed [plmn_id]");
        goto end;
    }

    if (plmn_ec_info->ec_mode_a_restricted) {
        if (cJSON_AddBoolToObject(item, "ecModeARestricted", plmn_ec_info->ec_mode_a_restricted) == NULL) {
            ogs_error("OpenAPI_plmn_ec_info_convertToJSON() failed [ec_mode_a_restricted]");
            goto end;
        }
    }

    if (plmn_ec_info->ec_mode_b_restricted) {
        if (cJSON_AddBoolToObject(item, "ecModeBRestricted", plmn_ec_info->ec_mode_b_restricted) == NULL) {
            ogs_error("OpenAPI_plmn_ec_info_convertToJSON() failed [ec_mode_b_restricted]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_plmn_ec_info_t *OpenAPI_plmn_ec_info_parseFromJSON(cJSON *plmn_ec_infoJSON)
{
    OpenAPI_plmn_ec_info_t *plmn_ec_info_local_var = NULL;
    cJSON *plmn_id = cJSON_GetObjectItemCaseSensitive(plmn_ec_infoJSON, "plmnId");
    if (!plmn_id) {
        ogs_error("OpenAPI_plmn_ec_info_parseFromJSON() failed [plmn_id]");
        goto end;
    }

    OpenAPI_plmn_id_t *plmn_id_local_nonprim = NULL;

    plmn_id_local_nonprim = OpenAPI_plmn_id_parseFromJSON(plmn_id);

    cJSON *ec_mode_a_restricted = cJSON_GetObjectItemCaseSensitive(plmn_ec_infoJSON, "ecModeARestricted");

    if (ec_mode_a_restricted) {
        if (!cJSON_IsBool(ec_mode_a_restricted)) {
            ogs_error("OpenAPI_plmn_ec_info_parseFromJSON() failed [ec_mode_a_restricted]");
            goto end;
        }
    }

    cJSON *ec_mode_b_restricted = cJSON_GetObjectItemCaseSensitive(plmn_ec_infoJSON, "ecModeBRestricted");

    if (ec_mode_b_restricted) {
        if (!cJSON_IsBool(ec_mode_b_restricted)) {
            ogs_error("OpenAPI_plmn_ec_info_parseFromJSON() failed [ec_mode_b_restricted]");
            goto end;
        }
    }

    plmn_ec_info_local_var = OpenAPI_plmn_ec_info_create (
        plmn_id_local_nonprim,
        ec_mode_a_restricted ? ec_mode_a_restricted->valueint : 0,
        ec_mode_b_restricted ? ec_mode_b_restricted->valueint : 0
        );

    return plmn_ec_info_local_var;
end:
    return NULL;
}

OpenAPI_plmn_ec_info_t *OpenAPI_plmn_ec_info_copy(OpenAPI_plmn_ec_info_t *dst, OpenAPI_plmn_ec_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_plmn_ec_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_plmn_ec_info_convertToJSON() failed");
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

    OpenAPI_plmn_ec_info_free(dst);
    dst = OpenAPI_plmn_ec_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

