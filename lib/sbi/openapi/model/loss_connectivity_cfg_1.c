
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "loss_connectivity_cfg_1.h"

OpenAPI_loss_connectivity_cfg_1_t *OpenAPI_loss_connectivity_cfg_1_create(
    bool is_max_detection_time,
    int max_detection_time
)
{
    OpenAPI_loss_connectivity_cfg_1_t *loss_connectivity_cfg_1_local_var = ogs_malloc(sizeof(OpenAPI_loss_connectivity_cfg_1_t));
    ogs_assert(loss_connectivity_cfg_1_local_var);

    loss_connectivity_cfg_1_local_var->is_max_detection_time = is_max_detection_time;
    loss_connectivity_cfg_1_local_var->max_detection_time = max_detection_time;

    return loss_connectivity_cfg_1_local_var;
}

void OpenAPI_loss_connectivity_cfg_1_free(OpenAPI_loss_connectivity_cfg_1_t *loss_connectivity_cfg_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == loss_connectivity_cfg_1) {
        return;
    }
    ogs_free(loss_connectivity_cfg_1);
}

cJSON *OpenAPI_loss_connectivity_cfg_1_convertToJSON(OpenAPI_loss_connectivity_cfg_1_t *loss_connectivity_cfg_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (loss_connectivity_cfg_1 == NULL) {
        ogs_error("OpenAPI_loss_connectivity_cfg_1_convertToJSON() failed [LossConnectivityCfg_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (loss_connectivity_cfg_1->is_max_detection_time) {
    if (cJSON_AddNumberToObject(item, "maxDetectionTime", loss_connectivity_cfg_1->max_detection_time) == NULL) {
        ogs_error("OpenAPI_loss_connectivity_cfg_1_convertToJSON() failed [max_detection_time]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_loss_connectivity_cfg_1_t *OpenAPI_loss_connectivity_cfg_1_parseFromJSON(cJSON *loss_connectivity_cfg_1JSON)
{
    OpenAPI_loss_connectivity_cfg_1_t *loss_connectivity_cfg_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *max_detection_time = NULL;
    max_detection_time = cJSON_GetObjectItemCaseSensitive(loss_connectivity_cfg_1JSON, "maxDetectionTime");
    if (max_detection_time) {
    if (!cJSON_IsNumber(max_detection_time)) {
        ogs_error("OpenAPI_loss_connectivity_cfg_1_parseFromJSON() failed [max_detection_time]");
        goto end;
    }
    }

    loss_connectivity_cfg_1_local_var = OpenAPI_loss_connectivity_cfg_1_create (
        max_detection_time ? true : false,
        max_detection_time ? max_detection_time->valuedouble : 0
    );

    return loss_connectivity_cfg_1_local_var;
end:
    return NULL;
}

OpenAPI_loss_connectivity_cfg_1_t *OpenAPI_loss_connectivity_cfg_1_copy(OpenAPI_loss_connectivity_cfg_1_t *dst, OpenAPI_loss_connectivity_cfg_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_loss_connectivity_cfg_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_loss_connectivity_cfg_1_convertToJSON() failed");
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

    OpenAPI_loss_connectivity_cfg_1_free(dst);
    dst = OpenAPI_loss_connectivity_cfg_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

