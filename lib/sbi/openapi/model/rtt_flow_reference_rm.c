
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "rtt_flow_reference_rm.h"

OpenAPI_rtt_flow_reference_rm_t *OpenAPI_rtt_flow_reference_rm_create(
    OpenAPI_flow_direction_e flow_dir,
    int shared_key
)
{
    OpenAPI_rtt_flow_reference_rm_t *rtt_flow_reference_rm_local_var = ogs_malloc(sizeof(OpenAPI_rtt_flow_reference_rm_t));
    ogs_assert(rtt_flow_reference_rm_local_var);

    rtt_flow_reference_rm_local_var->flow_dir = flow_dir;
    rtt_flow_reference_rm_local_var->shared_key = shared_key;

    return rtt_flow_reference_rm_local_var;
}

void OpenAPI_rtt_flow_reference_rm_free(OpenAPI_rtt_flow_reference_rm_t *rtt_flow_reference_rm)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == rtt_flow_reference_rm) {
        return;
    }
    ogs_free(rtt_flow_reference_rm);
}

cJSON *OpenAPI_rtt_flow_reference_rm_convertToJSON(OpenAPI_rtt_flow_reference_rm_t *rtt_flow_reference_rm)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (rtt_flow_reference_rm == NULL) {
        ogs_error("OpenAPI_rtt_flow_reference_rm_convertToJSON() failed [RttFlowReferenceRm]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (rtt_flow_reference_rm->flow_dir != OpenAPI_flow_direction_NULL) {
    if (cJSON_AddStringToObject(item, "flowDir", OpenAPI_flow_direction_ToString(rtt_flow_reference_rm->flow_dir)) == NULL) {
        ogs_error("OpenAPI_rtt_flow_reference_rm_convertToJSON() failed [flow_dir]");
        goto end;
    }
    }

    if (cJSON_AddNumberToObject(item, "sharedKey", rtt_flow_reference_rm->shared_key) == NULL) {
        ogs_error("OpenAPI_rtt_flow_reference_rm_convertToJSON() failed [shared_key]");
        goto end;
    }

end:
    return item;
}

OpenAPI_rtt_flow_reference_rm_t *OpenAPI_rtt_flow_reference_rm_parseFromJSON(cJSON *rtt_flow_reference_rmJSON)
{
    OpenAPI_rtt_flow_reference_rm_t *rtt_flow_reference_rm_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *flow_dir = NULL;
    OpenAPI_flow_direction_e flow_dirVariable = 0;
    cJSON *shared_key = NULL;
    flow_dir = cJSON_GetObjectItemCaseSensitive(rtt_flow_reference_rmJSON, "flowDir");
    if (flow_dir) {
    if (!cJSON_IsString(flow_dir)) {
        ogs_error("OpenAPI_rtt_flow_reference_rm_parseFromJSON() failed [flow_dir]");
        goto end;
    }
    flow_dirVariable = OpenAPI_flow_direction_FromString(flow_dir->valuestring);
    }

    shared_key = cJSON_GetObjectItemCaseSensitive(rtt_flow_reference_rmJSON, "sharedKey");
    if (!shared_key) {
        ogs_error("OpenAPI_rtt_flow_reference_rm_parseFromJSON() failed [shared_key]");
        goto end;
    }
    if (!cJSON_IsNumber(shared_key)) {
        ogs_error("OpenAPI_rtt_flow_reference_rm_parseFromJSON() failed [shared_key]");
        goto end;
    }

    rtt_flow_reference_rm_local_var = OpenAPI_rtt_flow_reference_rm_create (
        flow_dir ? flow_dirVariable : 0,
        
        shared_key->valuedouble
    );

    return rtt_flow_reference_rm_local_var;
end:
    return NULL;
}

OpenAPI_rtt_flow_reference_rm_t *OpenAPI_rtt_flow_reference_rm_copy(OpenAPI_rtt_flow_reference_rm_t *dst, OpenAPI_rtt_flow_reference_rm_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_rtt_flow_reference_rm_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_rtt_flow_reference_rm_convertToJSON() failed");
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

    OpenAPI_rtt_flow_reference_rm_free(dst);
    dst = OpenAPI_rtt_flow_reference_rm_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

