
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "redundant_transmission_exp_info.h"

OpenAPI_redundant_transmission_exp_info_t *OpenAPI_redundant_transmission_exp_info_create(
    OpenAPI_network_area_info_t *spatial_valid_con,
    char *dnn,
    OpenAPI_list_t *red_trans_exps
)
{
    OpenAPI_redundant_transmission_exp_info_t *redundant_transmission_exp_info_local_var = ogs_malloc(sizeof(OpenAPI_redundant_transmission_exp_info_t));
    ogs_assert(redundant_transmission_exp_info_local_var);

    redundant_transmission_exp_info_local_var->spatial_valid_con = spatial_valid_con;
    redundant_transmission_exp_info_local_var->dnn = dnn;
    redundant_transmission_exp_info_local_var->red_trans_exps = red_trans_exps;

    return redundant_transmission_exp_info_local_var;
}

void OpenAPI_redundant_transmission_exp_info_free(OpenAPI_redundant_transmission_exp_info_t *redundant_transmission_exp_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == redundant_transmission_exp_info) {
        return;
    }
    if (redundant_transmission_exp_info->spatial_valid_con) {
        OpenAPI_network_area_info_free(redundant_transmission_exp_info->spatial_valid_con);
        redundant_transmission_exp_info->spatial_valid_con = NULL;
    }
    if (redundant_transmission_exp_info->dnn) {
        ogs_free(redundant_transmission_exp_info->dnn);
        redundant_transmission_exp_info->dnn = NULL;
    }
    if (redundant_transmission_exp_info->red_trans_exps) {
        OpenAPI_list_for_each(redundant_transmission_exp_info->red_trans_exps, node) {
            OpenAPI_redundant_transmission_exp_per_ts_free(node->data);
        }
        OpenAPI_list_free(redundant_transmission_exp_info->red_trans_exps);
        redundant_transmission_exp_info->red_trans_exps = NULL;
    }
    ogs_free(redundant_transmission_exp_info);
}

cJSON *OpenAPI_redundant_transmission_exp_info_convertToJSON(OpenAPI_redundant_transmission_exp_info_t *redundant_transmission_exp_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (redundant_transmission_exp_info == NULL) {
        ogs_error("OpenAPI_redundant_transmission_exp_info_convertToJSON() failed [RedundantTransmissionExpInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (redundant_transmission_exp_info->spatial_valid_con) {
    cJSON *spatial_valid_con_local_JSON = OpenAPI_network_area_info_convertToJSON(redundant_transmission_exp_info->spatial_valid_con);
    if (spatial_valid_con_local_JSON == NULL) {
        ogs_error("OpenAPI_redundant_transmission_exp_info_convertToJSON() failed [spatial_valid_con]");
        goto end;
    }
    cJSON_AddItemToObject(item, "spatialValidCon", spatial_valid_con_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_redundant_transmission_exp_info_convertToJSON() failed [spatial_valid_con]");
        goto end;
    }
    }

    if (redundant_transmission_exp_info->dnn) {
    if (cJSON_AddStringToObject(item, "dnn", redundant_transmission_exp_info->dnn) == NULL) {
        ogs_error("OpenAPI_redundant_transmission_exp_info_convertToJSON() failed [dnn]");
        goto end;
    }
    }

    if (!redundant_transmission_exp_info->red_trans_exps) {
        ogs_error("OpenAPI_redundant_transmission_exp_info_convertToJSON() failed [red_trans_exps]");
        return NULL;
    }
    cJSON *red_trans_expsList = cJSON_AddArrayToObject(item, "redTransExps");
    if (red_trans_expsList == NULL) {
        ogs_error("OpenAPI_redundant_transmission_exp_info_convertToJSON() failed [red_trans_exps]");
        goto end;
    }
    OpenAPI_list_for_each(redundant_transmission_exp_info->red_trans_exps, node) {
        cJSON *itemLocal = OpenAPI_redundant_transmission_exp_per_ts_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_redundant_transmission_exp_info_convertToJSON() failed [red_trans_exps]");
            goto end;
        }
        cJSON_AddItemToArray(red_trans_expsList, itemLocal);
    }

end:
    return item;
}

OpenAPI_redundant_transmission_exp_info_t *OpenAPI_redundant_transmission_exp_info_parseFromJSON(cJSON *redundant_transmission_exp_infoJSON)
{
    OpenAPI_redundant_transmission_exp_info_t *redundant_transmission_exp_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *spatial_valid_con = NULL;
    OpenAPI_network_area_info_t *spatial_valid_con_local_nonprim = NULL;
    cJSON *dnn = NULL;
    cJSON *red_trans_exps = NULL;
    OpenAPI_list_t *red_trans_expsList = NULL;
    spatial_valid_con = cJSON_GetObjectItemCaseSensitive(redundant_transmission_exp_infoJSON, "spatialValidCon");
    if (spatial_valid_con) {
    spatial_valid_con_local_nonprim = OpenAPI_network_area_info_parseFromJSON(spatial_valid_con);
    if (!spatial_valid_con_local_nonprim) {
        ogs_error("OpenAPI_network_area_info_parseFromJSON failed [spatial_valid_con]");
        goto end;
    }
    }

    dnn = cJSON_GetObjectItemCaseSensitive(redundant_transmission_exp_infoJSON, "dnn");
    if (dnn) {
    if (!cJSON_IsString(dnn) && !cJSON_IsNull(dnn)) {
        ogs_error("OpenAPI_redundant_transmission_exp_info_parseFromJSON() failed [dnn]");
        goto end;
    }
    }

    red_trans_exps = cJSON_GetObjectItemCaseSensitive(redundant_transmission_exp_infoJSON, "redTransExps");
    if (!red_trans_exps) {
        ogs_error("OpenAPI_redundant_transmission_exp_info_parseFromJSON() failed [red_trans_exps]");
        goto end;
    }
        cJSON *red_trans_exps_local = NULL;
        if (!cJSON_IsArray(red_trans_exps)) {
            ogs_error("OpenAPI_redundant_transmission_exp_info_parseFromJSON() failed [red_trans_exps]");
            goto end;
        }

        red_trans_expsList = OpenAPI_list_create();

        cJSON_ArrayForEach(red_trans_exps_local, red_trans_exps) {
            if (!cJSON_IsObject(red_trans_exps_local)) {
                ogs_error("OpenAPI_redundant_transmission_exp_info_parseFromJSON() failed [red_trans_exps]");
                goto end;
            }
            OpenAPI_redundant_transmission_exp_per_ts_t *red_trans_expsItem = OpenAPI_redundant_transmission_exp_per_ts_parseFromJSON(red_trans_exps_local);
            if (!red_trans_expsItem) {
                ogs_error("No red_trans_expsItem");
                goto end;
            }
            OpenAPI_list_add(red_trans_expsList, red_trans_expsItem);
        }

    redundant_transmission_exp_info_local_var = OpenAPI_redundant_transmission_exp_info_create (
        spatial_valid_con ? spatial_valid_con_local_nonprim : NULL,
        dnn && !cJSON_IsNull(dnn) ? ogs_strdup(dnn->valuestring) : NULL,
        red_trans_expsList
    );

    return redundant_transmission_exp_info_local_var;
end:
    if (spatial_valid_con_local_nonprim) {
        OpenAPI_network_area_info_free(spatial_valid_con_local_nonprim);
        spatial_valid_con_local_nonprim = NULL;
    }
    if (red_trans_expsList) {
        OpenAPI_list_for_each(red_trans_expsList, node) {
            OpenAPI_redundant_transmission_exp_per_ts_free(node->data);
        }
        OpenAPI_list_free(red_trans_expsList);
        red_trans_expsList = NULL;
    }
    return NULL;
}

OpenAPI_redundant_transmission_exp_info_t *OpenAPI_redundant_transmission_exp_info_copy(OpenAPI_redundant_transmission_exp_info_t *dst, OpenAPI_redundant_transmission_exp_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_redundant_transmission_exp_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_redundant_transmission_exp_info_convertToJSON() failed");
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

    OpenAPI_redundant_transmission_exp_info_free(dst);
    dst = OpenAPI_redundant_transmission_exp_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

