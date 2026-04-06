
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "signal_analytics.h"

OpenAPI_signal_analytics_t *OpenAPI_signal_analytics_create(
    bool is_recv_sig_num,
    int recv_sig_num,
    bool is_sig_growth_rate,
    int sig_growth_rate,
    bool is_recv_sig_num_from_ue,
    int recv_sig_num_from_ue
)
{
    OpenAPI_signal_analytics_t *signal_analytics_local_var = ogs_malloc(sizeof(OpenAPI_signal_analytics_t));
    ogs_assert(signal_analytics_local_var);

    signal_analytics_local_var->is_recv_sig_num = is_recv_sig_num;
    signal_analytics_local_var->recv_sig_num = recv_sig_num;
    signal_analytics_local_var->is_sig_growth_rate = is_sig_growth_rate;
    signal_analytics_local_var->sig_growth_rate = sig_growth_rate;
    signal_analytics_local_var->is_recv_sig_num_from_ue = is_recv_sig_num_from_ue;
    signal_analytics_local_var->recv_sig_num_from_ue = recv_sig_num_from_ue;

    return signal_analytics_local_var;
}

void OpenAPI_signal_analytics_free(OpenAPI_signal_analytics_t *signal_analytics)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == signal_analytics) {
        return;
    }
    ogs_free(signal_analytics);
}

cJSON *OpenAPI_signal_analytics_convertToJSON(OpenAPI_signal_analytics_t *signal_analytics)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (signal_analytics == NULL) {
        ogs_error("OpenAPI_signal_analytics_convertToJSON() failed [SignalAnalytics]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (signal_analytics->is_recv_sig_num) {
    if (cJSON_AddNumberToObject(item, "recvSigNum", signal_analytics->recv_sig_num) == NULL) {
        ogs_error("OpenAPI_signal_analytics_convertToJSON() failed [recv_sig_num]");
        goto end;
    }
    }

    if (signal_analytics->is_sig_growth_rate) {
    if (cJSON_AddNumberToObject(item, "sigGrowthRate", signal_analytics->sig_growth_rate) == NULL) {
        ogs_error("OpenAPI_signal_analytics_convertToJSON() failed [sig_growth_rate]");
        goto end;
    }
    }

    if (signal_analytics->is_recv_sig_num_from_ue) {
    if (cJSON_AddNumberToObject(item, "recvSigNumFromUe", signal_analytics->recv_sig_num_from_ue) == NULL) {
        ogs_error("OpenAPI_signal_analytics_convertToJSON() failed [recv_sig_num_from_ue]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_signal_analytics_t *OpenAPI_signal_analytics_parseFromJSON(cJSON *signal_analyticsJSON)
{
    OpenAPI_signal_analytics_t *signal_analytics_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *recv_sig_num = NULL;
    cJSON *sig_growth_rate = NULL;
    cJSON *recv_sig_num_from_ue = NULL;
    recv_sig_num = cJSON_GetObjectItemCaseSensitive(signal_analyticsJSON, "recvSigNum");
    if (recv_sig_num) {
    if (!cJSON_IsNumber(recv_sig_num)) {
        ogs_error("OpenAPI_signal_analytics_parseFromJSON() failed [recv_sig_num]");
        goto end;
    }
    }

    sig_growth_rate = cJSON_GetObjectItemCaseSensitive(signal_analyticsJSON, "sigGrowthRate");
    if (sig_growth_rate) {
    if (!cJSON_IsNumber(sig_growth_rate)) {
        ogs_error("OpenAPI_signal_analytics_parseFromJSON() failed [sig_growth_rate]");
        goto end;
    }
    }

    recv_sig_num_from_ue = cJSON_GetObjectItemCaseSensitive(signal_analyticsJSON, "recvSigNumFromUe");
    if (recv_sig_num_from_ue) {
    if (!cJSON_IsNumber(recv_sig_num_from_ue)) {
        ogs_error("OpenAPI_signal_analytics_parseFromJSON() failed [recv_sig_num_from_ue]");
        goto end;
    }
    }

    signal_analytics_local_var = OpenAPI_signal_analytics_create (
        recv_sig_num ? true : false,
        recv_sig_num ? recv_sig_num->valuedouble : 0,
        sig_growth_rate ? true : false,
        sig_growth_rate ? sig_growth_rate->valuedouble : 0,
        recv_sig_num_from_ue ? true : false,
        recv_sig_num_from_ue ? recv_sig_num_from_ue->valuedouble : 0
    );

    return signal_analytics_local_var;
end:
    return NULL;
}

OpenAPI_signal_analytics_t *OpenAPI_signal_analytics_copy(OpenAPI_signal_analytics_t *dst, OpenAPI_signal_analytics_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_signal_analytics_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_signal_analytics_convertToJSON() failed");
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

    OpenAPI_signal_analytics_free(dst);
    dst = OpenAPI_signal_analytics_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

