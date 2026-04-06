
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "deregistration_resp_data.h"

OpenAPI_deregistration_resp_data_t *OpenAPI_deregistration_resp_data_create(
    bool is_smf_event_removed_ind,
    int smf_event_removed_ind
)
{
    OpenAPI_deregistration_resp_data_t *deregistration_resp_data_local_var = ogs_malloc(sizeof(OpenAPI_deregistration_resp_data_t));
    ogs_assert(deregistration_resp_data_local_var);

    deregistration_resp_data_local_var->is_smf_event_removed_ind = is_smf_event_removed_ind;
    deregistration_resp_data_local_var->smf_event_removed_ind = smf_event_removed_ind;

    return deregistration_resp_data_local_var;
}

void OpenAPI_deregistration_resp_data_free(OpenAPI_deregistration_resp_data_t *deregistration_resp_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == deregistration_resp_data) {
        return;
    }
    ogs_free(deregistration_resp_data);
}

cJSON *OpenAPI_deregistration_resp_data_convertToJSON(OpenAPI_deregistration_resp_data_t *deregistration_resp_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (deregistration_resp_data == NULL) {
        ogs_error("OpenAPI_deregistration_resp_data_convertToJSON() failed [DeregistrationRespData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (deregistration_resp_data->is_smf_event_removed_ind) {
    if (cJSON_AddBoolToObject(item, "smfEventRemovedInd", deregistration_resp_data->smf_event_removed_ind) == NULL) {
        ogs_error("OpenAPI_deregistration_resp_data_convertToJSON() failed [smf_event_removed_ind]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_deregistration_resp_data_t *OpenAPI_deregistration_resp_data_parseFromJSON(cJSON *deregistration_resp_dataJSON)
{
    OpenAPI_deregistration_resp_data_t *deregistration_resp_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *smf_event_removed_ind = NULL;
    smf_event_removed_ind = cJSON_GetObjectItemCaseSensitive(deregistration_resp_dataJSON, "smfEventRemovedInd");
    if (smf_event_removed_ind) {
    if (!cJSON_IsBool(smf_event_removed_ind)) {
        ogs_error("OpenAPI_deregistration_resp_data_parseFromJSON() failed [smf_event_removed_ind]");
        goto end;
    }
    }

    deregistration_resp_data_local_var = OpenAPI_deregistration_resp_data_create (
        smf_event_removed_ind ? true : false,
        smf_event_removed_ind ? smf_event_removed_ind->valueint : 0
    );

    return deregistration_resp_data_local_var;
end:
    return NULL;
}

OpenAPI_deregistration_resp_data_t *OpenAPI_deregistration_resp_data_copy(OpenAPI_deregistration_resp_data_t *dst, OpenAPI_deregistration_resp_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_deregistration_resp_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_deregistration_resp_data_convertToJSON() failed");
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

    OpenAPI_deregistration_resp_data_free(dst);
    dst = OpenAPI_deregistration_resp_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

