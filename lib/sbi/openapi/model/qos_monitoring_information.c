
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
    int rep_thresh_rp
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

    return qos_monitoring_information_local_var;
}

void OpenAPI_qos_monitoring_information_free(OpenAPI_qos_monitoring_information_t *qos_monitoring_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == qos_monitoring_information) {
        return;
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

    qos_monitoring_information_local_var = OpenAPI_qos_monitoring_information_create (
        rep_thresh_dl ? true : false,
        rep_thresh_dl ? rep_thresh_dl->valuedouble : 0,
        rep_thresh_ul ? true : false,
        rep_thresh_ul ? rep_thresh_ul->valuedouble : 0,
        rep_thresh_rp ? true : false,
        rep_thresh_rp ? rep_thresh_rp->valuedouble : 0
    );

    return qos_monitoring_information_local_var;
end:
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

