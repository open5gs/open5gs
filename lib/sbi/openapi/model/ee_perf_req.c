
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ee_perf_req.h"

char *OpenAPI_kpi_typeee_perf_req_ToString(OpenAPI_ee_perf_req_kpi_type_e kpi_type)
{
    const char *kpi_typeArray[] =  { "NULL", "NUMOFBITS", "NUMOFBITS_RANBASED" };
    size_t sizeofArray = sizeof(kpi_typeArray) / sizeof(kpi_typeArray[0]);
    if (kpi_type < sizeofArray)
        return (char *)kpi_typeArray[kpi_type];
    else
        return (char *)"Unknown";
}

OpenAPI_ee_perf_req_kpi_type_e OpenAPI_kpi_typeee_perf_req_FromString(char* kpi_type)
{
    int stringToReturn = 0;
    const char *kpi_typeArray[] =  { "NULL", "NUMOFBITS", "NUMOFBITS_RANBASED" };
    size_t sizeofArray = sizeof(kpi_typeArray) / sizeof(kpi_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(kpi_type, kpi_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}
OpenAPI_ee_perf_req_t *OpenAPI_ee_perf_req_create(
    OpenAPI_ee_perf_req_kpi_type_e kpi_type,
    bool is_req,
    double req
)
{
    OpenAPI_ee_perf_req_t *ee_perf_req_local_var = ogs_malloc(sizeof(OpenAPI_ee_perf_req_t));
    ogs_assert(ee_perf_req_local_var);

    ee_perf_req_local_var->kpi_type = kpi_type;
    ee_perf_req_local_var->is_req = is_req;
    ee_perf_req_local_var->req = req;

    return ee_perf_req_local_var;
}

void OpenAPI_ee_perf_req_free(OpenAPI_ee_perf_req_t *ee_perf_req)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ee_perf_req) {
        return;
    }
    ogs_free(ee_perf_req);
}

cJSON *OpenAPI_ee_perf_req_convertToJSON(OpenAPI_ee_perf_req_t *ee_perf_req)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ee_perf_req == NULL) {
        ogs_error("OpenAPI_ee_perf_req_convertToJSON() failed [EEPerfReq]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ee_perf_req->kpi_type != OpenAPI_ee_perf_req_KPITYPE_NULL) {
    if (cJSON_AddStringToObject(item, "kpiType", OpenAPI_kpi_typeee_perf_req_ToString(ee_perf_req->kpi_type)) == NULL) {
        ogs_error("OpenAPI_ee_perf_req_convertToJSON() failed [kpi_type]");
        goto end;
    }
    }

    if (ee_perf_req->is_req) {
    if (cJSON_AddNumberToObject(item, "req", ee_perf_req->req) == NULL) {
        ogs_error("OpenAPI_ee_perf_req_convertToJSON() failed [req]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ee_perf_req_t *OpenAPI_ee_perf_req_parseFromJSON(cJSON *ee_perf_reqJSON)
{
    OpenAPI_ee_perf_req_t *ee_perf_req_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *kpi_type = NULL;
    OpenAPI_ee_perf_req_kpi_type_e kpi_typeVariable = 0;
    cJSON *req = NULL;
    kpi_type = cJSON_GetObjectItemCaseSensitive(ee_perf_reqJSON, "kpiType");
    if (kpi_type) {
    if (!cJSON_IsString(kpi_type)) {
        ogs_error("OpenAPI_ee_perf_req_parseFromJSON() failed [kpi_type]");
        goto end;
    }
    kpi_typeVariable = OpenAPI_kpi_typeee_perf_req_FromString(kpi_type->valuestring);
    }

    req = cJSON_GetObjectItemCaseSensitive(ee_perf_reqJSON, "req");
    if (req) {
    if (!cJSON_IsNumber(req)) {
        ogs_error("OpenAPI_ee_perf_req_parseFromJSON() failed [req]");
        goto end;
    }
    }

    ee_perf_req_local_var = OpenAPI_ee_perf_req_create (
        kpi_type ? kpi_typeVariable : 0,
        req ? true : false,
        req ? req->valuedouble : 0
    );

    return ee_perf_req_local_var;
end:
    return NULL;
}

OpenAPI_ee_perf_req_t *OpenAPI_ee_perf_req_copy(OpenAPI_ee_perf_req_t *dst, OpenAPI_ee_perf_req_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ee_perf_req_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ee_perf_req_convertToJSON() failed");
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

    OpenAPI_ee_perf_req_free(dst);
    dst = OpenAPI_ee_perf_req_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

