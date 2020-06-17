
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "small_data_rate_status.h"

OpenAPI_small_data_rate_status_t *OpenAPI_small_data_rate_status_create(
    int remain_packets_ul,
    int remain_packets_dl,
    char *validity_time,
    int remain_ex_reports_ul,
    int remain_ex_reports_dl
    )
{
    OpenAPI_small_data_rate_status_t *small_data_rate_status_local_var = OpenAPI_malloc(sizeof(OpenAPI_small_data_rate_status_t));
    if (!small_data_rate_status_local_var) {
        return NULL;
    }
    small_data_rate_status_local_var->remain_packets_ul = remain_packets_ul;
    small_data_rate_status_local_var->remain_packets_dl = remain_packets_dl;
    small_data_rate_status_local_var->validity_time = validity_time;
    small_data_rate_status_local_var->remain_ex_reports_ul = remain_ex_reports_ul;
    small_data_rate_status_local_var->remain_ex_reports_dl = remain_ex_reports_dl;

    return small_data_rate_status_local_var;
}

void OpenAPI_small_data_rate_status_free(OpenAPI_small_data_rate_status_t *small_data_rate_status)
{
    if (NULL == small_data_rate_status) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(small_data_rate_status->validity_time);
    ogs_free(small_data_rate_status);
}

cJSON *OpenAPI_small_data_rate_status_convertToJSON(OpenAPI_small_data_rate_status_t *small_data_rate_status)
{
    cJSON *item = NULL;

    if (small_data_rate_status == NULL) {
        ogs_error("OpenAPI_small_data_rate_status_convertToJSON() failed [SmallDataRateStatus]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (small_data_rate_status->remain_packets_ul) {
        if (cJSON_AddNumberToObject(item, "remainPacketsUl", small_data_rate_status->remain_packets_ul) == NULL) {
            ogs_error("OpenAPI_small_data_rate_status_convertToJSON() failed [remain_packets_ul]");
            goto end;
        }
    }

    if (small_data_rate_status->remain_packets_dl) {
        if (cJSON_AddNumberToObject(item, "remainPacketsDl", small_data_rate_status->remain_packets_dl) == NULL) {
            ogs_error("OpenAPI_small_data_rate_status_convertToJSON() failed [remain_packets_dl]");
            goto end;
        }
    }

    if (small_data_rate_status->validity_time) {
        if (cJSON_AddStringToObject(item, "validityTime", small_data_rate_status->validity_time) == NULL) {
            ogs_error("OpenAPI_small_data_rate_status_convertToJSON() failed [validity_time]");
            goto end;
        }
    }

    if (small_data_rate_status->remain_ex_reports_ul) {
        if (cJSON_AddNumberToObject(item, "remainExReportsUl", small_data_rate_status->remain_ex_reports_ul) == NULL) {
            ogs_error("OpenAPI_small_data_rate_status_convertToJSON() failed [remain_ex_reports_ul]");
            goto end;
        }
    }

    if (small_data_rate_status->remain_ex_reports_dl) {
        if (cJSON_AddNumberToObject(item, "remainExReportsDl", small_data_rate_status->remain_ex_reports_dl) == NULL) {
            ogs_error("OpenAPI_small_data_rate_status_convertToJSON() failed [remain_ex_reports_dl]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_small_data_rate_status_t *OpenAPI_small_data_rate_status_parseFromJSON(cJSON *small_data_rate_statusJSON)
{
    OpenAPI_small_data_rate_status_t *small_data_rate_status_local_var = NULL;
    cJSON *remain_packets_ul = cJSON_GetObjectItemCaseSensitive(small_data_rate_statusJSON, "remainPacketsUl");

    if (remain_packets_ul) {
        if (!cJSON_IsNumber(remain_packets_ul)) {
            ogs_error("OpenAPI_small_data_rate_status_parseFromJSON() failed [remain_packets_ul]");
            goto end;
        }
    }

    cJSON *remain_packets_dl = cJSON_GetObjectItemCaseSensitive(small_data_rate_statusJSON, "remainPacketsDl");

    if (remain_packets_dl) {
        if (!cJSON_IsNumber(remain_packets_dl)) {
            ogs_error("OpenAPI_small_data_rate_status_parseFromJSON() failed [remain_packets_dl]");
            goto end;
        }
    }

    cJSON *validity_time = cJSON_GetObjectItemCaseSensitive(small_data_rate_statusJSON, "validityTime");

    if (validity_time) {
        if (!cJSON_IsString(validity_time)) {
            ogs_error("OpenAPI_small_data_rate_status_parseFromJSON() failed [validity_time]");
            goto end;
        }
    }

    cJSON *remain_ex_reports_ul = cJSON_GetObjectItemCaseSensitive(small_data_rate_statusJSON, "remainExReportsUl");

    if (remain_ex_reports_ul) {
        if (!cJSON_IsNumber(remain_ex_reports_ul)) {
            ogs_error("OpenAPI_small_data_rate_status_parseFromJSON() failed [remain_ex_reports_ul]");
            goto end;
        }
    }

    cJSON *remain_ex_reports_dl = cJSON_GetObjectItemCaseSensitive(small_data_rate_statusJSON, "remainExReportsDl");

    if (remain_ex_reports_dl) {
        if (!cJSON_IsNumber(remain_ex_reports_dl)) {
            ogs_error("OpenAPI_small_data_rate_status_parseFromJSON() failed [remain_ex_reports_dl]");
            goto end;
        }
    }

    small_data_rate_status_local_var = OpenAPI_small_data_rate_status_create (
        remain_packets_ul ? remain_packets_ul->valuedouble : 0,
        remain_packets_dl ? remain_packets_dl->valuedouble : 0,
        validity_time ? ogs_strdup(validity_time->valuestring) : NULL,
        remain_ex_reports_ul ? remain_ex_reports_ul->valuedouble : 0,
        remain_ex_reports_dl ? remain_ex_reports_dl->valuedouble : 0
        );

    return small_data_rate_status_local_var;
end:
    return NULL;
}

OpenAPI_small_data_rate_status_t *OpenAPI_small_data_rate_status_copy(OpenAPI_small_data_rate_status_t *dst, OpenAPI_small_data_rate_status_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_small_data_rate_status_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_small_data_rate_status_convertToJSON() failed");
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

    OpenAPI_small_data_rate_status_free(dst);
    dst = OpenAPI_small_data_rate_status_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

