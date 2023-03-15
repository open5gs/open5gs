
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "plmn_ec_info.h"

OpenAPI_plmn_ec_info_t *OpenAPI_plmn_ec_info_create(
    OpenAPI_plmn_id_t *plmn_id,
    OpenAPI_ec_restriction_data_wb_t *ec_restriction_data_wb,
    bool is_ec_restriction_data_nb,
    int ec_restriction_data_nb
)
{
    OpenAPI_plmn_ec_info_t *plmn_ec_info_local_var = ogs_malloc(sizeof(OpenAPI_plmn_ec_info_t));
    ogs_assert(plmn_ec_info_local_var);

    plmn_ec_info_local_var->plmn_id = plmn_id;
    plmn_ec_info_local_var->ec_restriction_data_wb = ec_restriction_data_wb;
    plmn_ec_info_local_var->is_ec_restriction_data_nb = is_ec_restriction_data_nb;
    plmn_ec_info_local_var->ec_restriction_data_nb = ec_restriction_data_nb;

    return plmn_ec_info_local_var;
}

void OpenAPI_plmn_ec_info_free(OpenAPI_plmn_ec_info_t *plmn_ec_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == plmn_ec_info) {
        return;
    }
    if (plmn_ec_info->plmn_id) {
        OpenAPI_plmn_id_free(plmn_ec_info->plmn_id);
        plmn_ec_info->plmn_id = NULL;
    }
    if (plmn_ec_info->ec_restriction_data_wb) {
        OpenAPI_ec_restriction_data_wb_free(plmn_ec_info->ec_restriction_data_wb);
        plmn_ec_info->ec_restriction_data_wb = NULL;
    }
    ogs_free(plmn_ec_info);
}

cJSON *OpenAPI_plmn_ec_info_convertToJSON(OpenAPI_plmn_ec_info_t *plmn_ec_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (plmn_ec_info == NULL) {
        ogs_error("OpenAPI_plmn_ec_info_convertToJSON() failed [PlmnEcInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!plmn_ec_info->plmn_id) {
        ogs_error("OpenAPI_plmn_ec_info_convertToJSON() failed [plmn_id]");
        return NULL;
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

    if (plmn_ec_info->ec_restriction_data_wb) {
    cJSON *ec_restriction_data_wb_local_JSON = OpenAPI_ec_restriction_data_wb_convertToJSON(plmn_ec_info->ec_restriction_data_wb);
    if (ec_restriction_data_wb_local_JSON == NULL) {
        ogs_error("OpenAPI_plmn_ec_info_convertToJSON() failed [ec_restriction_data_wb]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ecRestrictionDataWb", ec_restriction_data_wb_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_plmn_ec_info_convertToJSON() failed [ec_restriction_data_wb]");
        goto end;
    }
    }

    if (plmn_ec_info->is_ec_restriction_data_nb) {
    if (cJSON_AddBoolToObject(item, "ecRestrictionDataNb", plmn_ec_info->ec_restriction_data_nb) == NULL) {
        ogs_error("OpenAPI_plmn_ec_info_convertToJSON() failed [ec_restriction_data_nb]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_plmn_ec_info_t *OpenAPI_plmn_ec_info_parseFromJSON(cJSON *plmn_ec_infoJSON)
{
    OpenAPI_plmn_ec_info_t *plmn_ec_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *plmn_id = NULL;
    OpenAPI_plmn_id_t *plmn_id_local_nonprim = NULL;
    cJSON *ec_restriction_data_wb = NULL;
    OpenAPI_ec_restriction_data_wb_t *ec_restriction_data_wb_local_nonprim = NULL;
    cJSON *ec_restriction_data_nb = NULL;
    plmn_id = cJSON_GetObjectItemCaseSensitive(plmn_ec_infoJSON, "plmnId");
    if (!plmn_id) {
        ogs_error("OpenAPI_plmn_ec_info_parseFromJSON() failed [plmn_id]");
        goto end;
    }
    plmn_id_local_nonprim = OpenAPI_plmn_id_parseFromJSON(plmn_id);
    if (!plmn_id_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_parseFromJSON failed [plmn_id]");
        goto end;
    }

    ec_restriction_data_wb = cJSON_GetObjectItemCaseSensitive(plmn_ec_infoJSON, "ecRestrictionDataWb");
    if (ec_restriction_data_wb) {
    ec_restriction_data_wb_local_nonprim = OpenAPI_ec_restriction_data_wb_parseFromJSON(ec_restriction_data_wb);
    if (!ec_restriction_data_wb_local_nonprim) {
        ogs_error("OpenAPI_ec_restriction_data_wb_parseFromJSON failed [ec_restriction_data_wb]");
        goto end;
    }
    }

    ec_restriction_data_nb = cJSON_GetObjectItemCaseSensitive(plmn_ec_infoJSON, "ecRestrictionDataNb");
    if (ec_restriction_data_nb) {
    if (!cJSON_IsBool(ec_restriction_data_nb)) {
        ogs_error("OpenAPI_plmn_ec_info_parseFromJSON() failed [ec_restriction_data_nb]");
        goto end;
    }
    }

    plmn_ec_info_local_var = OpenAPI_plmn_ec_info_create (
        plmn_id_local_nonprim,
        ec_restriction_data_wb ? ec_restriction_data_wb_local_nonprim : NULL,
        ec_restriction_data_nb ? true : false,
        ec_restriction_data_nb ? ec_restriction_data_nb->valueint : 0
    );

    return plmn_ec_info_local_var;
end:
    if (plmn_id_local_nonprim) {
        OpenAPI_plmn_id_free(plmn_id_local_nonprim);
        plmn_id_local_nonprim = NULL;
    }
    if (ec_restriction_data_wb_local_nonprim) {
        OpenAPI_ec_restriction_data_wb_free(ec_restriction_data_wb_local_nonprim);
        ec_restriction_data_wb_local_nonprim = NULL;
    }
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

