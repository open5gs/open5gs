
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mov_behav_req.h"

OpenAPI_mov_behav_req_t *OpenAPI_mov_behav_req_create(
    OpenAPI_loc_info_granularity_e location_gran_req,
    OpenAPI_threshold_level_t *report_thresholds
)
{
    OpenAPI_mov_behav_req_t *mov_behav_req_local_var = ogs_malloc(sizeof(OpenAPI_mov_behav_req_t));
    ogs_assert(mov_behav_req_local_var);

    mov_behav_req_local_var->location_gran_req = location_gran_req;
    mov_behav_req_local_var->report_thresholds = report_thresholds;

    return mov_behav_req_local_var;
}

void OpenAPI_mov_behav_req_free(OpenAPI_mov_behav_req_t *mov_behav_req)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == mov_behav_req) {
        return;
    }
    if (mov_behav_req->report_thresholds) {
        OpenAPI_threshold_level_free(mov_behav_req->report_thresholds);
        mov_behav_req->report_thresholds = NULL;
    }
    ogs_free(mov_behav_req);
}

cJSON *OpenAPI_mov_behav_req_convertToJSON(OpenAPI_mov_behav_req_t *mov_behav_req)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (mov_behav_req == NULL) {
        ogs_error("OpenAPI_mov_behav_req_convertToJSON() failed [MovBehavReq]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (mov_behav_req->location_gran_req != OpenAPI_loc_info_granularity_NULL) {
    if (cJSON_AddStringToObject(item, "locationGranReq", OpenAPI_loc_info_granularity_ToString(mov_behav_req->location_gran_req)) == NULL) {
        ogs_error("OpenAPI_mov_behav_req_convertToJSON() failed [location_gran_req]");
        goto end;
    }
    }

    if (mov_behav_req->report_thresholds) {
    cJSON *report_thresholds_local_JSON = OpenAPI_threshold_level_convertToJSON(mov_behav_req->report_thresholds);
    if (report_thresholds_local_JSON == NULL) {
        ogs_error("OpenAPI_mov_behav_req_convertToJSON() failed [report_thresholds]");
        goto end;
    }
    cJSON_AddItemToObject(item, "reportThresholds", report_thresholds_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_mov_behav_req_convertToJSON() failed [report_thresholds]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_mov_behav_req_t *OpenAPI_mov_behav_req_parseFromJSON(cJSON *mov_behav_reqJSON)
{
    OpenAPI_mov_behav_req_t *mov_behav_req_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *location_gran_req = NULL;
    OpenAPI_loc_info_granularity_e location_gran_reqVariable = 0;
    cJSON *report_thresholds = NULL;
    OpenAPI_threshold_level_t *report_thresholds_local_nonprim = NULL;
    location_gran_req = cJSON_GetObjectItemCaseSensitive(mov_behav_reqJSON, "locationGranReq");
    if (location_gran_req) {
    if (!cJSON_IsString(location_gran_req)) {
        ogs_error("OpenAPI_mov_behav_req_parseFromJSON() failed [location_gran_req]");
        goto end;
    }
    location_gran_reqVariable = OpenAPI_loc_info_granularity_FromString(location_gran_req->valuestring);
    }

    report_thresholds = cJSON_GetObjectItemCaseSensitive(mov_behav_reqJSON, "reportThresholds");
    if (report_thresholds) {
    report_thresholds_local_nonprim = OpenAPI_threshold_level_parseFromJSON(report_thresholds);
    if (!report_thresholds_local_nonprim) {
        ogs_error("OpenAPI_threshold_level_parseFromJSON failed [report_thresholds]");
        goto end;
    }
    }

    mov_behav_req_local_var = OpenAPI_mov_behav_req_create (
        location_gran_req ? location_gran_reqVariable : 0,
        report_thresholds ? report_thresholds_local_nonprim : NULL
    );

    return mov_behav_req_local_var;
end:
    if (report_thresholds_local_nonprim) {
        OpenAPI_threshold_level_free(report_thresholds_local_nonprim);
        report_thresholds_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_mov_behav_req_t *OpenAPI_mov_behav_req_copy(OpenAPI_mov_behav_req_t *dst, OpenAPI_mov_behav_req_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_mov_behav_req_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_mov_behav_req_convertToJSON() failed");
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

    OpenAPI_mov_behav_req_free(dst);
    dst = OpenAPI_mov_behav_req_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

