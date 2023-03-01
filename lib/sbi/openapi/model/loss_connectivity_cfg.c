
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "loss_connectivity_cfg.h"

OpenAPI_loss_connectivity_cfg_t *OpenAPI_loss_connectivity_cfg_create(
    bool is_max_detection_time,
    int max_detection_time
)
{
    OpenAPI_loss_connectivity_cfg_t *loss_connectivity_cfg_local_var = ogs_malloc(sizeof(OpenAPI_loss_connectivity_cfg_t));
    ogs_assert(loss_connectivity_cfg_local_var);

    loss_connectivity_cfg_local_var->is_max_detection_time = is_max_detection_time;
    loss_connectivity_cfg_local_var->max_detection_time = max_detection_time;

    return loss_connectivity_cfg_local_var;
}

void OpenAPI_loss_connectivity_cfg_free(OpenAPI_loss_connectivity_cfg_t *loss_connectivity_cfg)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == loss_connectivity_cfg) {
        return;
    }
    ogs_free(loss_connectivity_cfg);
}

cJSON *OpenAPI_loss_connectivity_cfg_convertToJSON(OpenAPI_loss_connectivity_cfg_t *loss_connectivity_cfg)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (loss_connectivity_cfg == NULL) {
        ogs_error("OpenAPI_loss_connectivity_cfg_convertToJSON() failed [LossConnectivityCfg]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (loss_connectivity_cfg->is_max_detection_time) {
    if (cJSON_AddNumberToObject(item, "maxDetectionTime", loss_connectivity_cfg->max_detection_time) == NULL) {
        ogs_error("OpenAPI_loss_connectivity_cfg_convertToJSON() failed [max_detection_time]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_loss_connectivity_cfg_t *OpenAPI_loss_connectivity_cfg_parseFromJSON(cJSON *loss_connectivity_cfgJSON)
{
    OpenAPI_loss_connectivity_cfg_t *loss_connectivity_cfg_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *max_detection_time = NULL;
    max_detection_time = cJSON_GetObjectItemCaseSensitive(loss_connectivity_cfgJSON, "maxDetectionTime");
    if (max_detection_time) {
    if (!cJSON_IsNumber(max_detection_time)) {
        ogs_error("OpenAPI_loss_connectivity_cfg_parseFromJSON() failed [max_detection_time]");
        goto end;
    }
    }

    loss_connectivity_cfg_local_var = OpenAPI_loss_connectivity_cfg_create (
        max_detection_time ? true : false,
        max_detection_time ? max_detection_time->valuedouble : 0
    );

    return loss_connectivity_cfg_local_var;
end:
    return NULL;
}

OpenAPI_loss_connectivity_cfg_t *OpenAPI_loss_connectivity_cfg_copy(OpenAPI_loss_connectivity_cfg_t *dst, OpenAPI_loss_connectivity_cfg_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_loss_connectivity_cfg_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_loss_connectivity_cfg_convertToJSON() failed");
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

    OpenAPI_loss_connectivity_cfg_free(dst);
    dst = OpenAPI_loss_connectivity_cfg_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

