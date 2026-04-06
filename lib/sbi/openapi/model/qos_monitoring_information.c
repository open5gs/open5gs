
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "qos_monitoring_information.h"

OpenAPI_qos_monitoring_information_t *OpenAPI_qos_monitoring_information_create(
    bool is_rep_thresh_dl,
    int rep_thresh_dl,
    bool is_rep_thresh_ul,
    int rep_thresh_ul,
    bool is_rep_thresh_rp,
    int rep_thresh_rp,
    char *rep_thresh_dat_rate_ul,
    char *rep_thresh_dat_rate_dl,
    bool is_con_thresh_dl,
    int con_thresh_dl,
    bool is_con_thresh_ul,
    int con_thresh_ul,
    OpenAPI_list_t *avl_bitrate_ul_thrs,
    OpenAPI_list_t *avl_bitrate_dl_thrs
)
{
    OpenAPI_qos_monitoring_information_t *qos_monitoring_information_local_var = ogs_malloc(sizeof(OpenAPI_qos_monitoring_information_t));
    ogs_assert(qos_monitoring_information_local_var);

    qos_monitoring_information_local_var->is_rep_thresh_dl = is_rep_thresh_dl;
    qos_monitoring_information_local_var->rep_thresh_dl = rep_thresh_dl;
    qos_monitoring_information_local_var->is_rep_thresh_ul = is_rep_thresh_ul;
    qos_monitoring_information_local_var->rep_thresh_ul = rep_thresh_ul;
    qos_monitoring_information_local_var->is_rep_thresh_rp = is_rep_thresh_rp;
    qos_monitoring_information_local_var->rep_thresh_rp = rep_thresh_rp;
    qos_monitoring_information_local_var->rep_thresh_dat_rate_ul = rep_thresh_dat_rate_ul;
    qos_monitoring_information_local_var->rep_thresh_dat_rate_dl = rep_thresh_dat_rate_dl;
    qos_monitoring_information_local_var->is_con_thresh_dl = is_con_thresh_dl;
    qos_monitoring_information_local_var->con_thresh_dl = con_thresh_dl;
    qos_monitoring_information_local_var->is_con_thresh_ul = is_con_thresh_ul;
    qos_monitoring_information_local_var->con_thresh_ul = con_thresh_ul;
    qos_monitoring_information_local_var->avl_bitrate_ul_thrs = avl_bitrate_ul_thrs;
    qos_monitoring_information_local_var->avl_bitrate_dl_thrs = avl_bitrate_dl_thrs;

    return qos_monitoring_information_local_var;
}

void OpenAPI_qos_monitoring_information_free(OpenAPI_qos_monitoring_information_t *qos_monitoring_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == qos_monitoring_information) {
        return;
    }
    if (qos_monitoring_information->rep_thresh_dat_rate_ul) {
        ogs_free(qos_monitoring_information->rep_thresh_dat_rate_ul);
        qos_monitoring_information->rep_thresh_dat_rate_ul = NULL;
    }
    if (qos_monitoring_information->rep_thresh_dat_rate_dl) {
        ogs_free(qos_monitoring_information->rep_thresh_dat_rate_dl);
        qos_monitoring_information->rep_thresh_dat_rate_dl = NULL;
    }
    if (qos_monitoring_information->avl_bitrate_ul_thrs) {
        OpenAPI_list_for_each(qos_monitoring_information->avl_bitrate_ul_thrs, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(qos_monitoring_information->avl_bitrate_ul_thrs);
        qos_monitoring_information->avl_bitrate_ul_thrs = NULL;
    }
    if (qos_monitoring_information->avl_bitrate_dl_thrs) {
        OpenAPI_list_for_each(qos_monitoring_information->avl_bitrate_dl_thrs, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(qos_monitoring_information->avl_bitrate_dl_thrs);
        qos_monitoring_information->avl_bitrate_dl_thrs = NULL;
    }
    ogs_free(qos_monitoring_information);
}

cJSON *OpenAPI_qos_monitoring_information_convertToJSON(OpenAPI_qos_monitoring_information_t *qos_monitoring_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (qos_monitoring_information == NULL) {
        ogs_error("OpenAPI_qos_monitoring_information_convertToJSON() failed [QosMonitoringInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (qos_monitoring_information->is_rep_thresh_dl) {
    if (cJSON_AddNumberToObject(item, "repThreshDl", qos_monitoring_information->rep_thresh_dl) == NULL) {
        ogs_error("OpenAPI_qos_monitoring_information_convertToJSON() failed [rep_thresh_dl]");
        goto end;
    }
    }

    if (qos_monitoring_information->is_rep_thresh_ul) {
    if (cJSON_AddNumberToObject(item, "repThreshUl", qos_monitoring_information->rep_thresh_ul) == NULL) {
        ogs_error("OpenAPI_qos_monitoring_information_convertToJSON() failed [rep_thresh_ul]");
        goto end;
    }
    }

    if (qos_monitoring_information->is_rep_thresh_rp) {
    if (cJSON_AddNumberToObject(item, "repThreshRp", qos_monitoring_information->rep_thresh_rp) == NULL) {
        ogs_error("OpenAPI_qos_monitoring_information_convertToJSON() failed [rep_thresh_rp]");
        goto end;
    }
    }

    if (qos_monitoring_information->rep_thresh_dat_rate_ul) {
    if (cJSON_AddStringToObject(item, "repThreshDatRateUl", qos_monitoring_information->rep_thresh_dat_rate_ul) == NULL) {
        ogs_error("OpenAPI_qos_monitoring_information_convertToJSON() failed [rep_thresh_dat_rate_ul]");
        goto end;
    }
    }

    if (qos_monitoring_information->rep_thresh_dat_rate_dl) {
    if (cJSON_AddStringToObject(item, "repThreshDatRateDl", qos_monitoring_information->rep_thresh_dat_rate_dl) == NULL) {
        ogs_error("OpenAPI_qos_monitoring_information_convertToJSON() failed [rep_thresh_dat_rate_dl]");
        goto end;
    }
    }

    if (qos_monitoring_information->is_con_thresh_dl) {
    if (cJSON_AddNumberToObject(item, "conThreshDl", qos_monitoring_information->con_thresh_dl) == NULL) {
        ogs_error("OpenAPI_qos_monitoring_information_convertToJSON() failed [con_thresh_dl]");
        goto end;
    }
    }

    if (qos_monitoring_information->is_con_thresh_ul) {
    if (cJSON_AddNumberToObject(item, "conThreshUl", qos_monitoring_information->con_thresh_ul) == NULL) {
        ogs_error("OpenAPI_qos_monitoring_information_convertToJSON() failed [con_thresh_ul]");
        goto end;
    }
    }

    if (qos_monitoring_information->avl_bitrate_ul_thrs) {
    cJSON *avl_bitrate_ul_thrsList = cJSON_AddArrayToObject(item, "avlBitrateUlThrs");
    if (avl_bitrate_ul_thrsList == NULL) {
        ogs_error("OpenAPI_qos_monitoring_information_convertToJSON() failed [avl_bitrate_ul_thrs]");
        goto end;
    }
    OpenAPI_list_for_each(qos_monitoring_information->avl_bitrate_ul_thrs, node) {
        if (cJSON_AddStringToObject(avl_bitrate_ul_thrsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_qos_monitoring_information_convertToJSON() failed [avl_bitrate_ul_thrs]");
            goto end;
        }
    }
    }

    if (qos_monitoring_information->avl_bitrate_dl_thrs) {
    cJSON *avl_bitrate_dl_thrsList = cJSON_AddArrayToObject(item, "avlBitrateDlThrs");
    if (avl_bitrate_dl_thrsList == NULL) {
        ogs_error("OpenAPI_qos_monitoring_information_convertToJSON() failed [avl_bitrate_dl_thrs]");
        goto end;
    }
    OpenAPI_list_for_each(qos_monitoring_information->avl_bitrate_dl_thrs, node) {
        if (cJSON_AddStringToObject(avl_bitrate_dl_thrsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_qos_monitoring_information_convertToJSON() failed [avl_bitrate_dl_thrs]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_qos_monitoring_information_t *OpenAPI_qos_monitoring_information_parseFromJSON(cJSON *qos_monitoring_informationJSON)
{
    OpenAPI_qos_monitoring_information_t *qos_monitoring_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *rep_thresh_dl = NULL;
    cJSON *rep_thresh_ul = NULL;
    cJSON *rep_thresh_rp = NULL;
    cJSON *rep_thresh_dat_rate_ul = NULL;
    cJSON *rep_thresh_dat_rate_dl = NULL;
    cJSON *con_thresh_dl = NULL;
    cJSON *con_thresh_ul = NULL;
    cJSON *avl_bitrate_ul_thrs = NULL;
    OpenAPI_list_t *avl_bitrate_ul_thrsList = NULL;
    cJSON *avl_bitrate_dl_thrs = NULL;
    OpenAPI_list_t *avl_bitrate_dl_thrsList = NULL;
    rep_thresh_dl = cJSON_GetObjectItemCaseSensitive(qos_monitoring_informationJSON, "repThreshDl");
    if (rep_thresh_dl) {
    if (!cJSON_IsNumber(rep_thresh_dl)) {
        ogs_error("OpenAPI_qos_monitoring_information_parseFromJSON() failed [rep_thresh_dl]");
        goto end;
    }
    }

    rep_thresh_ul = cJSON_GetObjectItemCaseSensitive(qos_monitoring_informationJSON, "repThreshUl");
    if (rep_thresh_ul) {
    if (!cJSON_IsNumber(rep_thresh_ul)) {
        ogs_error("OpenAPI_qos_monitoring_information_parseFromJSON() failed [rep_thresh_ul]");
        goto end;
    }
    }

    rep_thresh_rp = cJSON_GetObjectItemCaseSensitive(qos_monitoring_informationJSON, "repThreshRp");
    if (rep_thresh_rp) {
    if (!cJSON_IsNumber(rep_thresh_rp)) {
        ogs_error("OpenAPI_qos_monitoring_information_parseFromJSON() failed [rep_thresh_rp]");
        goto end;
    }
    }

    rep_thresh_dat_rate_ul = cJSON_GetObjectItemCaseSensitive(qos_monitoring_informationJSON, "repThreshDatRateUl");
    if (rep_thresh_dat_rate_ul) {
    if (!cJSON_IsString(rep_thresh_dat_rate_ul) && !cJSON_IsNull(rep_thresh_dat_rate_ul)) {
        ogs_error("OpenAPI_qos_monitoring_information_parseFromJSON() failed [rep_thresh_dat_rate_ul]");
        goto end;
    }
    }

    rep_thresh_dat_rate_dl = cJSON_GetObjectItemCaseSensitive(qos_monitoring_informationJSON, "repThreshDatRateDl");
    if (rep_thresh_dat_rate_dl) {
    if (!cJSON_IsString(rep_thresh_dat_rate_dl) && !cJSON_IsNull(rep_thresh_dat_rate_dl)) {
        ogs_error("OpenAPI_qos_monitoring_information_parseFromJSON() failed [rep_thresh_dat_rate_dl]");
        goto end;
    }
    }

    con_thresh_dl = cJSON_GetObjectItemCaseSensitive(qos_monitoring_informationJSON, "conThreshDl");
    if (con_thresh_dl) {
    if (!cJSON_IsNumber(con_thresh_dl)) {
        ogs_error("OpenAPI_qos_monitoring_information_parseFromJSON() failed [con_thresh_dl]");
        goto end;
    }
    }

    con_thresh_ul = cJSON_GetObjectItemCaseSensitive(qos_monitoring_informationJSON, "conThreshUl");
    if (con_thresh_ul) {
    if (!cJSON_IsNumber(con_thresh_ul)) {
        ogs_error("OpenAPI_qos_monitoring_information_parseFromJSON() failed [con_thresh_ul]");
        goto end;
    }
    }

    avl_bitrate_ul_thrs = cJSON_GetObjectItemCaseSensitive(qos_monitoring_informationJSON, "avlBitrateUlThrs");
    if (avl_bitrate_ul_thrs) {
        cJSON *avl_bitrate_ul_thrs_local = NULL;
        if (!cJSON_IsArray(avl_bitrate_ul_thrs)) {
            ogs_error("OpenAPI_qos_monitoring_information_parseFromJSON() failed [avl_bitrate_ul_thrs]");
            goto end;
        }

        avl_bitrate_ul_thrsList = OpenAPI_list_create();

        cJSON_ArrayForEach(avl_bitrate_ul_thrs_local, avl_bitrate_ul_thrs) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(avl_bitrate_ul_thrs_local)) {
                ogs_error("OpenAPI_qos_monitoring_information_parseFromJSON() failed [avl_bitrate_ul_thrs]");
                goto end;
            }
            OpenAPI_list_add(avl_bitrate_ul_thrsList, ogs_strdup(avl_bitrate_ul_thrs_local->valuestring));
        }
    }

    avl_bitrate_dl_thrs = cJSON_GetObjectItemCaseSensitive(qos_monitoring_informationJSON, "avlBitrateDlThrs");
    if (avl_bitrate_dl_thrs) {
        cJSON *avl_bitrate_dl_thrs_local = NULL;
        if (!cJSON_IsArray(avl_bitrate_dl_thrs)) {
            ogs_error("OpenAPI_qos_monitoring_information_parseFromJSON() failed [avl_bitrate_dl_thrs]");
            goto end;
        }

        avl_bitrate_dl_thrsList = OpenAPI_list_create();

        cJSON_ArrayForEach(avl_bitrate_dl_thrs_local, avl_bitrate_dl_thrs) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(avl_bitrate_dl_thrs_local)) {
                ogs_error("OpenAPI_qos_monitoring_information_parseFromJSON() failed [avl_bitrate_dl_thrs]");
                goto end;
            }
            OpenAPI_list_add(avl_bitrate_dl_thrsList, ogs_strdup(avl_bitrate_dl_thrs_local->valuestring));
        }
    }

    qos_monitoring_information_local_var = OpenAPI_qos_monitoring_information_create (
        rep_thresh_dl ? true : false,
        rep_thresh_dl ? rep_thresh_dl->valuedouble : 0,
        rep_thresh_ul ? true : false,
        rep_thresh_ul ? rep_thresh_ul->valuedouble : 0,
        rep_thresh_rp ? true : false,
        rep_thresh_rp ? rep_thresh_rp->valuedouble : 0,
        rep_thresh_dat_rate_ul && !cJSON_IsNull(rep_thresh_dat_rate_ul) ? ogs_strdup(rep_thresh_dat_rate_ul->valuestring) : NULL,
        rep_thresh_dat_rate_dl && !cJSON_IsNull(rep_thresh_dat_rate_dl) ? ogs_strdup(rep_thresh_dat_rate_dl->valuestring) : NULL,
        con_thresh_dl ? true : false,
        con_thresh_dl ? con_thresh_dl->valuedouble : 0,
        con_thresh_ul ? true : false,
        con_thresh_ul ? con_thresh_ul->valuedouble : 0,
        avl_bitrate_ul_thrs ? avl_bitrate_ul_thrsList : NULL,
        avl_bitrate_dl_thrs ? avl_bitrate_dl_thrsList : NULL
    );

    return qos_monitoring_information_local_var;
end:
    if (avl_bitrate_ul_thrsList) {
        OpenAPI_list_for_each(avl_bitrate_ul_thrsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(avl_bitrate_ul_thrsList);
        avl_bitrate_ul_thrsList = NULL;
    }
    if (avl_bitrate_dl_thrsList) {
        OpenAPI_list_for_each(avl_bitrate_dl_thrsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(avl_bitrate_dl_thrsList);
        avl_bitrate_dl_thrsList = NULL;
    }
    return NULL;
}

OpenAPI_qos_monitoring_information_t *OpenAPI_qos_monitoring_information_copy(OpenAPI_qos_monitoring_information_t *dst, OpenAPI_qos_monitoring_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_qos_monitoring_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_qos_monitoring_information_convertToJSON() failed");
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

    OpenAPI_qos_monitoring_information_free(dst);
    dst = OpenAPI_qos_monitoring_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

