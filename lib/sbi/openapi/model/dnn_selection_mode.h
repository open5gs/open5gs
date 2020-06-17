/*
 * dnn_selection_mode.h
 *
 * Possible values are - VERIFIED - UE_DNN_NOT_VERIFIED - NW_DNN_NOT_VERIFIED
 */

#ifndef _OpenAPI_dnn_selection_mode_H_
#define _OpenAPI_dnn_selection_mode_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_dnn_selection_mode_s OpenAPI_dnn_selection_mode_t;
typedef struct OpenAPI_dnn_selection_mode_s {
} OpenAPI_dnn_selection_mode_t;

OpenAPI_dnn_selection_mode_t *OpenAPI_dnn_selection_mode_create(
    );
void OpenAPI_dnn_selection_mode_free(OpenAPI_dnn_selection_mode_t *dnn_selection_mode);
OpenAPI_dnn_selection_mode_t *OpenAPI_dnn_selection_mode_parseFromJSON(cJSON *dnn_selection_modeJSON);
cJSON *OpenAPI_dnn_selection_mode_convertToJSON(OpenAPI_dnn_selection_mode_t *dnn_selection_mode);
OpenAPI_dnn_selection_mode_t *OpenAPI_dnn_selection_mode_copy(OpenAPI_dnn_selection_mode_t *dst, OpenAPI_dnn_selection_mode_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_dnn_selection_mode_H_ */

