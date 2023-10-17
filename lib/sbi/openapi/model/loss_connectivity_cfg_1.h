/*
 * loss_connectivity_cfg_1.h
 *
 * 
 */

#ifndef _OpenAPI_loss_connectivity_cfg_1_H_
#define _OpenAPI_loss_connectivity_cfg_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_loss_connectivity_cfg_1_s OpenAPI_loss_connectivity_cfg_1_t;
typedef struct OpenAPI_loss_connectivity_cfg_1_s {
    bool is_max_detection_time;
    int max_detection_time;
} OpenAPI_loss_connectivity_cfg_1_t;

OpenAPI_loss_connectivity_cfg_1_t *OpenAPI_loss_connectivity_cfg_1_create(
    bool is_max_detection_time,
    int max_detection_time
);
void OpenAPI_loss_connectivity_cfg_1_free(OpenAPI_loss_connectivity_cfg_1_t *loss_connectivity_cfg_1);
OpenAPI_loss_connectivity_cfg_1_t *OpenAPI_loss_connectivity_cfg_1_parseFromJSON(cJSON *loss_connectivity_cfg_1JSON);
cJSON *OpenAPI_loss_connectivity_cfg_1_convertToJSON(OpenAPI_loss_connectivity_cfg_1_t *loss_connectivity_cfg_1);
OpenAPI_loss_connectivity_cfg_1_t *OpenAPI_loss_connectivity_cfg_1_copy(OpenAPI_loss_connectivity_cfg_1_t *dst, OpenAPI_loss_connectivity_cfg_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_loss_connectivity_cfg_1_H_ */

