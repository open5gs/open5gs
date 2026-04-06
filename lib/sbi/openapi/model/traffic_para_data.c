
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "traffic_para_data.h"

OpenAPI_traffic_para_data_t *OpenAPI_traffic_para_data_create(
    bool is_period_ul,
    int period_ul,
    bool is_period_dl,
    int period_dl,
    OpenAPI_list_t *req_traffic_paras,
    OpenAPI_list_t *rep_freqs,
    bool is_dl_n6_jitter_thr,
    int dl_n6_jitter_thr,
    bool is_rep_period_null,
    bool is_rep_period,
    int rep_period
)
{
    OpenAPI_traffic_para_data_t *traffic_para_data_local_var = ogs_malloc(sizeof(OpenAPI_traffic_para_data_t));
    ogs_assert(traffic_para_data_local_var);

    traffic_para_data_local_var->is_period_ul = is_period_ul;
    traffic_para_data_local_var->period_ul = period_ul;
    traffic_para_data_local_var->is_period_dl = is_period_dl;
    traffic_para_data_local_var->period_dl = period_dl;
    traffic_para_data_local_var->req_traffic_paras = req_traffic_paras;
    traffic_para_data_local_var->rep_freqs = rep_freqs;
    traffic_para_data_local_var->is_dl_n6_jitter_thr = is_dl_n6_jitter_thr;
    traffic_para_data_local_var->dl_n6_jitter_thr = dl_n6_jitter_thr;
    traffic_para_data_local_var->is_rep_period_null = is_rep_period_null;
    traffic_para_data_local_var->is_rep_period = is_rep_period;
    traffic_para_data_local_var->rep_period = rep_period;

    return traffic_para_data_local_var;
}

void OpenAPI_traffic_para_data_free(OpenAPI_traffic_para_data_t *traffic_para_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == traffic_para_data) {
        return;
    }
    if (traffic_para_data->req_traffic_paras) {
        OpenAPI_list_free(traffic_para_data->req_traffic_paras);
        traffic_para_data->req_traffic_paras = NULL;
    }
    if (traffic_para_data->rep_freqs) {
        OpenAPI_list_free(traffic_para_data->rep_freqs);
        traffic_para_data->rep_freqs = NULL;
    }
    ogs_free(traffic_para_data);
}

cJSON *OpenAPI_traffic_para_data_convertToJSON(OpenAPI_traffic_para_data_t *traffic_para_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (traffic_para_data == NULL) {
        ogs_error("OpenAPI_traffic_para_data_convertToJSON() failed [TrafficParaData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (traffic_para_data->is_period_ul) {
    if (cJSON_AddNumberToObject(item, "periodUl", traffic_para_data->period_ul) == NULL) {
        ogs_error("OpenAPI_traffic_para_data_convertToJSON() failed [period_ul]");
        goto end;
    }
    }

    if (traffic_para_data->is_period_dl) {
    if (cJSON_AddNumberToObject(item, "periodDl", traffic_para_data->period_dl) == NULL) {
        ogs_error("OpenAPI_traffic_para_data_convertToJSON() failed [period_dl]");
        goto end;
    }
    }

    if (traffic_para_data->req_traffic_paras != OpenAPI_traffic_parameter_meas_NULL) {
    cJSON *req_traffic_parasList = cJSON_AddArrayToObject(item, "reqTrafficParas");
    if (req_traffic_parasList == NULL) {
        ogs_error("OpenAPI_traffic_para_data_convertToJSON() failed [req_traffic_paras]");
        goto end;
    }
    OpenAPI_list_for_each(traffic_para_data->req_traffic_paras, node) {
        if (cJSON_AddStringToObject(req_traffic_parasList, "", OpenAPI_traffic_parameter_meas_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_traffic_para_data_convertToJSON() failed [req_traffic_paras]");
            goto end;
        }
    }
    }

    if (traffic_para_data->rep_freqs != OpenAPI_reporting_frequency_NULL) {
    cJSON *rep_freqsList = cJSON_AddArrayToObject(item, "repFreqs");
    if (rep_freqsList == NULL) {
        ogs_error("OpenAPI_traffic_para_data_convertToJSON() failed [rep_freqs]");
        goto end;
    }
    OpenAPI_list_for_each(traffic_para_data->rep_freqs, node) {
        if (cJSON_AddStringToObject(rep_freqsList, "", OpenAPI_reporting_frequency_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_traffic_para_data_convertToJSON() failed [rep_freqs]");
            goto end;
        }
    }
    }

    if (traffic_para_data->is_dl_n6_jitter_thr) {
    if (cJSON_AddNumberToObject(item, "dlN6JitterThr", traffic_para_data->dl_n6_jitter_thr) == NULL) {
        ogs_error("OpenAPI_traffic_para_data_convertToJSON() failed [dl_n6_jitter_thr]");
        goto end;
    }
    }

    if (traffic_para_data->is_rep_period) {
    if (cJSON_AddNumberToObject(item, "repPeriod", traffic_para_data->rep_period) == NULL) {
        ogs_error("OpenAPI_traffic_para_data_convertToJSON() failed [rep_period]");
        goto end;
    }
    } else if (traffic_para_data->is_rep_period_null) {
        if (cJSON_AddNullToObject(item, "repPeriod") == NULL) {
            ogs_error("OpenAPI_traffic_para_data_convertToJSON() failed [rep_period]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_traffic_para_data_t *OpenAPI_traffic_para_data_parseFromJSON(cJSON *traffic_para_dataJSON)
{
    OpenAPI_traffic_para_data_t *traffic_para_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *period_ul = NULL;
    cJSON *period_dl = NULL;
    cJSON *req_traffic_paras = NULL;
    OpenAPI_list_t *req_traffic_parasList = NULL;
    cJSON *rep_freqs = NULL;
    OpenAPI_list_t *rep_freqsList = NULL;
    cJSON *dl_n6_jitter_thr = NULL;
    cJSON *rep_period = NULL;
    period_ul = cJSON_GetObjectItemCaseSensitive(traffic_para_dataJSON, "periodUl");
    if (period_ul) {
    if (!cJSON_IsNumber(period_ul)) {
        ogs_error("OpenAPI_traffic_para_data_parseFromJSON() failed [period_ul]");
        goto end;
    }
    }

    period_dl = cJSON_GetObjectItemCaseSensitive(traffic_para_dataJSON, "periodDl");
    if (period_dl) {
    if (!cJSON_IsNumber(period_dl)) {
        ogs_error("OpenAPI_traffic_para_data_parseFromJSON() failed [period_dl]");
        goto end;
    }
    }

    req_traffic_paras = cJSON_GetObjectItemCaseSensitive(traffic_para_dataJSON, "reqTrafficParas");
    if (req_traffic_paras) {
        cJSON *req_traffic_paras_local = NULL;
        if (!cJSON_IsArray(req_traffic_paras)) {
            ogs_error("OpenAPI_traffic_para_data_parseFromJSON() failed [req_traffic_paras]");
            goto end;
        }

        req_traffic_parasList = OpenAPI_list_create();

        cJSON_ArrayForEach(req_traffic_paras_local, req_traffic_paras) {
            OpenAPI_traffic_parameter_meas_e localEnum = OpenAPI_traffic_parameter_meas_NULL;
            if (!cJSON_IsString(req_traffic_paras_local)) {
                ogs_error("OpenAPI_traffic_para_data_parseFromJSON() failed [req_traffic_paras]");
                goto end;
            }
            localEnum = OpenAPI_traffic_parameter_meas_FromString(req_traffic_paras_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"req_traffic_paras\" is not supported. Ignoring it ...",
                         req_traffic_paras_local->valuestring);
            } else {
                OpenAPI_list_add(req_traffic_parasList, (void *)localEnum);
            }
        }
        if (req_traffic_parasList->count == 0) {
            ogs_error("OpenAPI_traffic_para_data_parseFromJSON() failed: Expected req_traffic_parasList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    rep_freqs = cJSON_GetObjectItemCaseSensitive(traffic_para_dataJSON, "repFreqs");
    if (rep_freqs) {
        cJSON *rep_freqs_local = NULL;
        if (!cJSON_IsArray(rep_freqs)) {
            ogs_error("OpenAPI_traffic_para_data_parseFromJSON() failed [rep_freqs]");
            goto end;
        }

        rep_freqsList = OpenAPI_list_create();

        cJSON_ArrayForEach(rep_freqs_local, rep_freqs) {
            OpenAPI_reporting_frequency_e localEnum = OpenAPI_reporting_frequency_NULL;
            if (!cJSON_IsString(rep_freqs_local)) {
                ogs_error("OpenAPI_traffic_para_data_parseFromJSON() failed [rep_freqs]");
                goto end;
            }
            localEnum = OpenAPI_reporting_frequency_FromString(rep_freqs_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"rep_freqs\" is not supported. Ignoring it ...",
                         rep_freqs_local->valuestring);
            } else {
                OpenAPI_list_add(rep_freqsList, (void *)localEnum);
            }
        }
        if (rep_freqsList->count == 0) {
            ogs_error("OpenAPI_traffic_para_data_parseFromJSON() failed: Expected rep_freqsList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    dl_n6_jitter_thr = cJSON_GetObjectItemCaseSensitive(traffic_para_dataJSON, "dlN6JitterThr");
    if (dl_n6_jitter_thr) {
    if (!cJSON_IsNumber(dl_n6_jitter_thr)) {
        ogs_error("OpenAPI_traffic_para_data_parseFromJSON() failed [dl_n6_jitter_thr]");
        goto end;
    }
    }

    rep_period = cJSON_GetObjectItemCaseSensitive(traffic_para_dataJSON, "repPeriod");
    if (rep_period) {
    if (!cJSON_IsNull(rep_period)) {
    if (!cJSON_IsNumber(rep_period)) {
        ogs_error("OpenAPI_traffic_para_data_parseFromJSON() failed [rep_period]");
        goto end;
    }
    }
    }

    traffic_para_data_local_var = OpenAPI_traffic_para_data_create (
        period_ul ? true : false,
        period_ul ? period_ul->valuedouble : 0,
        period_dl ? true : false,
        period_dl ? period_dl->valuedouble : 0,
        req_traffic_paras ? req_traffic_parasList : NULL,
        rep_freqs ? rep_freqsList : NULL,
        dl_n6_jitter_thr ? true : false,
        dl_n6_jitter_thr ? dl_n6_jitter_thr->valuedouble : 0,
        rep_period && cJSON_IsNull(rep_period) ? true : false,
        rep_period ? true : false,
        rep_period ? rep_period->valuedouble : 0
    );

    return traffic_para_data_local_var;
end:
    if (req_traffic_parasList) {
        OpenAPI_list_free(req_traffic_parasList);
        req_traffic_parasList = NULL;
    }
    if (rep_freqsList) {
        OpenAPI_list_free(rep_freqsList);
        rep_freqsList = NULL;
    }
    return NULL;
}

OpenAPI_traffic_para_data_t *OpenAPI_traffic_para_data_copy(OpenAPI_traffic_para_data_t *dst, OpenAPI_traffic_para_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_traffic_para_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_traffic_para_data_convertToJSON() failed");
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

    OpenAPI_traffic_para_data_free(dst);
    dst = OpenAPI_traffic_para_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

