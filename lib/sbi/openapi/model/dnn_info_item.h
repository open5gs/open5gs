/*
 * dnn_info_item.h
 *
 * Set of parameters supported by NF for a given DNN
 */

#ifndef _OpenAPI_dnn_info_item_H_
#define _OpenAPI_dnn_info_item_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_dnn_info_item_s OpenAPI_dnn_info_item_t;
typedef struct OpenAPI_dnn_info_item_s {
    char *dnn;
} OpenAPI_dnn_info_item_t;

OpenAPI_dnn_info_item_t *OpenAPI_dnn_info_item_create(
    char *dnn
);
void OpenAPI_dnn_info_item_free(OpenAPI_dnn_info_item_t *dnn_info_item);
OpenAPI_dnn_info_item_t *OpenAPI_dnn_info_item_parseFromJSON(cJSON *dnn_info_itemJSON);
cJSON *OpenAPI_dnn_info_item_convertToJSON(OpenAPI_dnn_info_item_t *dnn_info_item);
OpenAPI_dnn_info_item_t *OpenAPI_dnn_info_item_copy(OpenAPI_dnn_info_item_t *dst, OpenAPI_dnn_info_item_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_dnn_info_item_H_ */

