
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "apn_rate_status.h"

OpenAPI_apn_rate_status_t *OpenAPI_apn_rate_status_create(
    bool is_remain_packets_ul,
    int remain_packets_ul,
    bool is_remain_packets_dl,
    int remain_packets_dl,
    char *validity_time,
    bool is_remain_ex_reports_ul,
    int remain_ex_reports_ul,
    bool is_remain_ex_reports_dl,
    int remain_ex_reports_dl
)
{
    OpenAPI_apn_rate_status_t *apn_rate_status_local_var = ogs_malloc(sizeof(OpenAPI_apn_rate_status_t));
    ogs_assert(apn_rate_status_local_var);

    apn_rate_status_local_var->is_remain_packets_ul = is_remain_packets_ul;
    apn_rate_status_local_var->remain_packets_ul = remain_packets_ul;
    apn_rate_status_local_var->is_remain_packets_dl = is_remain_packets_dl;
    apn_rate_status_local_var->remain_packets_dl = remain_packets_dl;
    apn_rate_status_local_var->validity_time = validity_time;
    apn_rate_status_local_var->is_remain_ex_reports_ul = is_remain_ex_reports_ul;
    apn_rate_status_local_var->remain_ex_reports_ul = remain_ex_reports_ul;
    apn_rate_status_local_var->is_remain_ex_reports_dl = is_remain_ex_reports_dl;
    apn_rate_status_local_var->remain_ex_reports_dl = remain_ex_reports_dl;

    return apn_rate_status_local_var;
}

void OpenAPI_apn_rate_status_free(OpenAPI_apn_rate_status_t *apn_rate_status)
{
    if (NULL == apn_rate_status) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(apn_rate_status->validity_time);
    ogs_free(apn_rate_status);
}

cJSON *OpenAPI_apn_rate_status_convertToJSON(OpenAPI_apn_rate_status_t *apn_rate_status)
{
    cJSON *item = NULL;

    if (apn_rate_status == NULL) {
        ogs_error("OpenAPI_apn_rate_status_convertToJSON() failed [ApnRateStatus]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (apn_rate_status->is_remain_packets_ul) {
    if (cJSON_AddNumberToObject(item, "remainPacketsUl", apn_rate_status->remain_packets_ul) == NULL) {
        ogs_error("OpenAPI_apn_rate_status_convertToJSON() failed [remain_packets_ul]");
        goto end;
    }
    }

    if (apn_rate_status->is_remain_packets_dl) {
    if (cJSON_AddNumberToObject(item, "remainPacketsDl", apn_rate_status->remain_packets_dl) == NULL) {
        ogs_error("OpenAPI_apn_rate_status_convertToJSON() failed [remain_packets_dl]");
        goto end;
    }
    }

    if (apn_rate_status->validity_time) {
    if (cJSON_AddStringToObject(item, "validityTime", apn_rate_status->validity_time) == NULL) {
        ogs_error("OpenAPI_apn_rate_status_convertToJSON() failed [validity_time]");
        goto end;
    }
    }

    if (apn_rate_status->is_remain_ex_reports_ul) {
    if (cJSON_AddNumberToObject(item, "remainExReportsUl", apn_rate_status->remain_ex_reports_ul) == NULL) {
        ogs_error("OpenAPI_apn_rate_status_convertToJSON() failed [remain_ex_reports_ul]");
        goto end;
    }
    }

    if (apn_rate_status->is_remain_ex_reports_dl) {
    if (cJSON_AddNumberToObject(item, "remainExReportsDl", apn_rate_status->remain_ex_reports_dl) == NULL) {
        ogs_error("OpenAPI_apn_rate_status_convertToJSON() failed [remain_ex_reports_dl]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_apn_rate_status_t *OpenAPI_apn_rate_status_parseFromJSON(cJSON *apn_rate_statusJSON)
{
    OpenAPI_apn_rate_status_t *apn_rate_status_local_var = NULL;
    cJSON *remain_packets_ul = cJSON_GetObjectItemCaseSensitive(apn_rate_statusJSON, "remainPacketsUl");

    if (remain_packets_ul) {
    if (!cJSON_IsNumber(remain_packets_ul)) {
        ogs_error("OpenAPI_apn_rate_status_parseFromJSON() failed [remain_packets_ul]");
        goto end;
    }
    }

    cJSON *remain_packets_dl = cJSON_GetObjectItemCaseSensitive(apn_rate_statusJSON, "remainPacketsDl");

    if (remain_packets_dl) {
    if (!cJSON_IsNumber(remain_packets_dl)) {
        ogs_error("OpenAPI_apn_rate_status_parseFromJSON() failed [remain_packets_dl]");
        goto end;
    }
    }

    cJSON *validity_time = cJSON_GetObjectItemCaseSensitive(apn_rate_statusJSON, "validityTime");

    if (validity_time) {
    if (!cJSON_IsString(validity_time)) {
        ogs_error("OpenAPI_apn_rate_status_parseFromJSON() failed [validity_time]");
        goto end;
    }
    }

    cJSON *remain_ex_reports_ul = cJSON_GetObjectItemCaseSensitive(apn_rate_statusJSON, "remainExReportsUl");

    if (remain_ex_reports_ul) {
    if (!cJSON_IsNumber(remain_ex_reports_ul)) {
        ogs_error("OpenAPI_apn_rate_status_parseFromJSON() failed [remain_ex_reports_ul]");
        goto end;
    }
    }

    cJSON *remain_ex_reports_dl = cJSON_GetObjectItemCaseSensitive(apn_rate_statusJSON, "remainExReportsDl");

    if (remain_ex_reports_dl) {
    if (!cJSON_IsNumber(remain_ex_reports_dl)) {
        ogs_error("OpenAPI_apn_rate_status_parseFromJSON() failed [remain_ex_reports_dl]");
        goto end;
    }
    }

    apn_rate_status_local_var = OpenAPI_apn_rate_status_create (
        remain_packets_ul ? true : false,
        remain_packets_ul ? remain_packets_ul->valuedouble : 0,
        remain_packets_dl ? true : false,
        remain_packets_dl ? remain_packets_dl->valuedouble : 0,
        validity_time ? ogs_strdup(validity_time->valuestring) : NULL,
        remain_ex_reports_ul ? true : false,
        remain_ex_reports_ul ? remain_ex_reports_ul->valuedouble : 0,
        remain_ex_reports_dl ? true : false,
        remain_ex_reports_dl ? remain_ex_reports_dl->valuedouble : 0
    );

    return apn_rate_status_local_var;
end:
    return NULL;
}

OpenAPI_apn_rate_status_t *OpenAPI_apn_rate_status_copy(OpenAPI_apn_rate_status_t *dst, OpenAPI_apn_rate_status_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_apn_rate_status_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_apn_rate_status_convertToJSON() failed");
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

    OpenAPI_apn_rate_status_free(dst);
    dst = OpenAPI_apn_rate_status_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

