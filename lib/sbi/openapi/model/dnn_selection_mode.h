/*
 * dnn_selection_mode.h
 *
 * 
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

typedef enum { OpenAPI_dnn_selection_mode_NULL = 0, OpenAPI_dnn_selection_mode_VERIFIED, OpenAPI_dnn_selection_mode_UE_DNN_NOT_VERIFIED, OpenAPI_dnn_selection_mode_NW_DNN_NOT_VERIFIED } OpenAPI_dnn_selection_mode_e;

char* OpenAPI_dnn_selection_mode_ToString(OpenAPI_dnn_selection_mode_e dnn_selection_mode);

OpenAPI_dnn_selection_mode_e OpenAPI_dnn_selection_mode_FromString(char* dnn_selection_mode);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_dnn_selection_mode_H_ */

