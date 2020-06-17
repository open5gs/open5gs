
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "n1_n2_msg_txfr_err_detail.h"

OpenAPI_n1_n2_msg_txfr_err_detail_t *OpenAPI_n1_n2_msg_txfr_err_detail_create(
    int retry_after,
    OpenAPI_arp_t *highest_prio_arp,
    int max_waiting_time
    )
{
    OpenAPI_n1_n2_msg_txfr_err_detail_t *n1_n2_msg_txfr_err_detail_local_var = OpenAPI_malloc(sizeof(OpenAPI_n1_n2_msg_txfr_err_detail_t));
    if (!n1_n2_msg_txfr_err_detail_local_var) {
        return NULL;
    }
    n1_n2_msg_txfr_err_detail_local_var->retry_after = retry_after;
    n1_n2_msg_txfr_err_detail_local_var->highest_prio_arp = highest_prio_arp;
    n1_n2_msg_txfr_err_detail_local_var->max_waiting_time = max_waiting_time;

    return n1_n2_msg_txfr_err_detail_local_var;
}

void OpenAPI_n1_n2_msg_txfr_err_detail_free(OpenAPI_n1_n2_msg_txfr_err_detail_t *n1_n2_msg_txfr_err_detail)
{
    if (NULL == n1_n2_msg_txfr_err_detail) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_arp_free(n1_n2_msg_txfr_err_detail->highest_prio_arp);
    ogs_free(n1_n2_msg_txfr_err_detail);
}

cJSON *OpenAPI_n1_n2_msg_txfr_err_detail_convertToJSON(OpenAPI_n1_n2_msg_txfr_err_detail_t *n1_n2_msg_txfr_err_detail)
{
    cJSON *item = NULL;

    if (n1_n2_msg_txfr_err_detail == NULL) {
        ogs_error("OpenAPI_n1_n2_msg_txfr_err_detail_convertToJSON() failed [N1N2MsgTxfrErrDetail]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (n1_n2_msg_txfr_err_detail->retry_after) {
        if (cJSON_AddNumberToObject(item, "retryAfter", n1_n2_msg_txfr_err_detail->retry_after) == NULL) {
            ogs_error("OpenAPI_n1_n2_msg_txfr_err_detail_convertToJSON() failed [retry_after]");
            goto end;
        }
    }

    if (n1_n2_msg_txfr_err_detail->highest_prio_arp) {
        cJSON *highest_prio_arp_local_JSON = OpenAPI_arp_convertToJSON(n1_n2_msg_txfr_err_detail->highest_prio_arp);
        if (highest_prio_arp_local_JSON == NULL) {
            ogs_error("OpenAPI_n1_n2_msg_txfr_err_detail_convertToJSON() failed [highest_prio_arp]");
            goto end;
        }
        cJSON_AddItemToObject(item, "highestPrioArp", highest_prio_arp_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_n1_n2_msg_txfr_err_detail_convertToJSON() failed [highest_prio_arp]");
            goto end;
        }
    }

    if (n1_n2_msg_txfr_err_detail->max_waiting_time) {
        if (cJSON_AddNumberToObject(item, "maxWaitingTime", n1_n2_msg_txfr_err_detail->max_waiting_time) == NULL) {
            ogs_error("OpenAPI_n1_n2_msg_txfr_err_detail_convertToJSON() failed [max_waiting_time]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_n1_n2_msg_txfr_err_detail_t *OpenAPI_n1_n2_msg_txfr_err_detail_parseFromJSON(cJSON *n1_n2_msg_txfr_err_detailJSON)
{
    OpenAPI_n1_n2_msg_txfr_err_detail_t *n1_n2_msg_txfr_err_detail_local_var = NULL;
    cJSON *retry_after = cJSON_GetObjectItemCaseSensitive(n1_n2_msg_txfr_err_detailJSON, "retryAfter");

    if (retry_after) {
        if (!cJSON_IsNumber(retry_after)) {
            ogs_error("OpenAPI_n1_n2_msg_txfr_err_detail_parseFromJSON() failed [retry_after]");
            goto end;
        }
    }

    cJSON *highest_prio_arp = cJSON_GetObjectItemCaseSensitive(n1_n2_msg_txfr_err_detailJSON, "highestPrioArp");

    OpenAPI_arp_t *highest_prio_arp_local_nonprim = NULL;
    if (highest_prio_arp) {
        highest_prio_arp_local_nonprim = OpenAPI_arp_parseFromJSON(highest_prio_arp);
    }

    cJSON *max_waiting_time = cJSON_GetObjectItemCaseSensitive(n1_n2_msg_txfr_err_detailJSON, "maxWaitingTime");

    if (max_waiting_time) {
        if (!cJSON_IsNumber(max_waiting_time)) {
            ogs_error("OpenAPI_n1_n2_msg_txfr_err_detail_parseFromJSON() failed [max_waiting_time]");
            goto end;
        }
    }

    n1_n2_msg_txfr_err_detail_local_var = OpenAPI_n1_n2_msg_txfr_err_detail_create (
        retry_after ? retry_after->valuedouble : 0,
        highest_prio_arp ? highest_prio_arp_local_nonprim : NULL,
        max_waiting_time ? max_waiting_time->valuedouble : 0
        );

    return n1_n2_msg_txfr_err_detail_local_var;
end:
    return NULL;
}

OpenAPI_n1_n2_msg_txfr_err_detail_t *OpenAPI_n1_n2_msg_txfr_err_detail_copy(OpenAPI_n1_n2_msg_txfr_err_detail_t *dst, OpenAPI_n1_n2_msg_txfr_err_detail_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_n1_n2_msg_txfr_err_detail_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_n1_n2_msg_txfr_err_detail_convertToJSON() failed");
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

    OpenAPI_n1_n2_msg_txfr_err_detail_free(dst);
    dst = OpenAPI_n1_n2_msg_txfr_err_detail_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

