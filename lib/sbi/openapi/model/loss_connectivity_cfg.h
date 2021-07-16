/*
 * loss_connectivity_cfg.h
 *
 * 
 */

#ifndef _OpenAPI_loss_connectivity_cfg_H_
#define _OpenAPI_loss_connectivity_cfg_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_loss_connectivity_cfg_s OpenAPI_loss_connectivity_cfg_t;
typedef struct OpenAPI_loss_connectivity_cfg_s {
    bool is_max_detection_time;
    int max_detection_time;
} OpenAPI_loss_connectivity_cfg_t;

OpenAPI_loss_connectivity_cfg_t *OpenAPI_loss_connectivity_cfg_create(
    bool is_max_detection_time,
    int max_detection_time
);
void OpenAPI_loss_connectivity_cfg_free(OpenAPI_loss_connectivity_cfg_t *loss_connectivity_cfg);
OpenAPI_loss_connectivity_cfg_t *OpenAPI_loss_connectivity_cfg_parseFromJSON(cJSON *loss_connectivity_cfgJSON);
cJSON *OpenAPI_loss_connectivity_cfg_convertToJSON(OpenAPI_loss_connectivity_cfg_t *loss_connectivity_cfg);
OpenAPI_loss_connectivity_cfg_t *OpenAPI_loss_connectivity_cfg_copy(OpenAPI_loss_connectivity_cfg_t *dst, OpenAPI_loss_connectivity_cfg_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_loss_connectivity_cfg_H_ */

