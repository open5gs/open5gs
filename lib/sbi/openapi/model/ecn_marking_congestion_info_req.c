
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ecn_marking_congestion_info_req.h"

OpenAPI_ecn_marking_congestion_info_req_t *OpenAPI_ecn_marking_congestion_info_req_create(
    OpenAPI_ecn_marking_req_e ecn_marking_ran_req,
    OpenAPI_ecn_marking_req_e ecn_marking_upf_req,
    OpenAPI_congestion_info_req_e congestion_info_req
)
{
    OpenAPI_ecn_marking_congestion_info_req_t *ecn_marking_congestion_info_req_local_var = ogs_malloc(sizeof(OpenAPI_ecn_marking_congestion_info_req_t));
    ogs_assert(ecn_marking_congestion_info_req_local_var);

    ecn_marking_congestion_info_req_local_var->ecn_marking_ran_req = ecn_marking_ran_req;
    ecn_marking_congestion_info_req_local_var->ecn_marking_upf_req = ecn_marking_upf_req;
    ecn_marking_congestion_info_req_local_var->congestion_info_req = congestion_info_req;

    return ecn_marking_congestion_info_req_local_var;
}

void OpenAPI_ecn_marking_congestion_info_req_free(OpenAPI_ecn_marking_congestion_info_req_t *ecn_marking_congestion_info_req)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ecn_marking_congestion_info_req) {
        return;
    }
    ogs_free(ecn_marking_congestion_info_req);
}

cJSON *OpenAPI_ecn_marking_congestion_info_req_convertToJSON(OpenAPI_ecn_marking_congestion_info_req_t *ecn_marking_congestion_info_req)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ecn_marking_congestion_info_req == NULL) {
        ogs_error("OpenAPI_ecn_marking_congestion_info_req_convertToJSON() failed [EcnMarkingCongestionInfoReq]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ecn_marking_congestion_info_req->ecn_marking_ran_req != OpenAPI_ecn_marking_req_NULL) {
    if (cJSON_AddStringToObject(item, "ecnMarkingRanReq", OpenAPI_ecn_marking_req_ToString(ecn_marking_congestion_info_req->ecn_marking_ran_req)) == NULL) {
        ogs_error("OpenAPI_ecn_marking_congestion_info_req_convertToJSON() failed [ecn_marking_ran_req]");
        goto end;
    }
    }

    if (ecn_marking_congestion_info_req->ecn_marking_upf_req != OpenAPI_ecn_marking_req_NULL) {
    if (cJSON_AddStringToObject(item, "ecnMarkingUpfReq", OpenAPI_ecn_marking_req_ToString(ecn_marking_congestion_info_req->ecn_marking_upf_req)) == NULL) {
        ogs_error("OpenAPI_ecn_marking_congestion_info_req_convertToJSON() failed [ecn_marking_upf_req]");
        goto end;
    }
    }

    if (ecn_marking_congestion_info_req->congestion_info_req != OpenAPI_congestion_info_req_NULL) {
    if (cJSON_AddStringToObject(item, "congestionInfoReq", OpenAPI_congestion_info_req_ToString(ecn_marking_congestion_info_req->congestion_info_req)) == NULL) {
        ogs_error("OpenAPI_ecn_marking_congestion_info_req_convertToJSON() failed [congestion_info_req]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ecn_marking_congestion_info_req_t *OpenAPI_ecn_marking_congestion_info_req_parseFromJSON(cJSON *ecn_marking_congestion_info_reqJSON)
{
    OpenAPI_ecn_marking_congestion_info_req_t *ecn_marking_congestion_info_req_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ecn_marking_ran_req = NULL;
    OpenAPI_ecn_marking_req_e ecn_marking_ran_reqVariable = 0;
    cJSON *ecn_marking_upf_req = NULL;
    OpenAPI_ecn_marking_req_e ecn_marking_upf_reqVariable = 0;
    cJSON *congestion_info_req = NULL;
    OpenAPI_congestion_info_req_e congestion_info_reqVariable = 0;
    ecn_marking_ran_req = cJSON_GetObjectItemCaseSensitive(ecn_marking_congestion_info_reqJSON, "ecnMarkingRanReq");
    if (ecn_marking_ran_req) {
    if (!cJSON_IsString(ecn_marking_ran_req)) {
        ogs_error("OpenAPI_ecn_marking_congestion_info_req_parseFromJSON() failed [ecn_marking_ran_req]");
        goto end;
    }
    ecn_marking_ran_reqVariable = OpenAPI_ecn_marking_req_FromString(ecn_marking_ran_req->valuestring);
    }

    ecn_marking_upf_req = cJSON_GetObjectItemCaseSensitive(ecn_marking_congestion_info_reqJSON, "ecnMarkingUpfReq");
    if (ecn_marking_upf_req) {
    if (!cJSON_IsString(ecn_marking_upf_req)) {
        ogs_error("OpenAPI_ecn_marking_congestion_info_req_parseFromJSON() failed [ecn_marking_upf_req]");
        goto end;
    }
    ecn_marking_upf_reqVariable = OpenAPI_ecn_marking_req_FromString(ecn_marking_upf_req->valuestring);
    }

    congestion_info_req = cJSON_GetObjectItemCaseSensitive(ecn_marking_congestion_info_reqJSON, "congestionInfoReq");
    if (congestion_info_req) {
    if (!cJSON_IsString(congestion_info_req)) {
        ogs_error("OpenAPI_ecn_marking_congestion_info_req_parseFromJSON() failed [congestion_info_req]");
        goto end;
    }
    congestion_info_reqVariable = OpenAPI_congestion_info_req_FromString(congestion_info_req->valuestring);
    }

    ecn_marking_congestion_info_req_local_var = OpenAPI_ecn_marking_congestion_info_req_create (
        ecn_marking_ran_req ? ecn_marking_ran_reqVariable : 0,
        ecn_marking_upf_req ? ecn_marking_upf_reqVariable : 0,
        congestion_info_req ? congestion_info_reqVariable : 0
    );

    return ecn_marking_congestion_info_req_local_var;
end:
    return NULL;
}

OpenAPI_ecn_marking_congestion_info_req_t *OpenAPI_ecn_marking_congestion_info_req_copy(OpenAPI_ecn_marking_congestion_info_req_t *dst, OpenAPI_ecn_marking_congestion_info_req_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ecn_marking_congestion_info_req_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ecn_marking_congestion_info_req_convertToJSON() failed");
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

    OpenAPI_ecn_marking_congestion_info_req_free(dst);
    dst = OpenAPI_ecn_marking_congestion_info_req_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

