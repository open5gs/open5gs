/*
 * snssai_dnn_item.h
 *
 * Combination of S-NSSAIs and DNNs
 */

#ifndef _OpenAPI_snssai_dnn_item_H_
#define _OpenAPI_snssai_dnn_item_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_snssai_dnn_item_s OpenAPI_snssai_dnn_item_t;
#include "ext_snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_snssai_dnn_item_s {
    OpenAPI_list_t *snssai_list;
    OpenAPI_list_t *dnn_list;
};

OpenAPI_snssai_dnn_item_t *OpenAPI_snssai_dnn_item_create(
    OpenAPI_list_t *snssai_list,
    OpenAPI_list_t *dnn_list
);
void OpenAPI_snssai_dnn_item_free(OpenAPI_snssai_dnn_item_t *snssai_dnn_item);
OpenAPI_snssai_dnn_item_t *OpenAPI_snssai_dnn_item_parseFromJSON(cJSON *snssai_dnn_itemJSON);
cJSON *OpenAPI_snssai_dnn_item_convertToJSON(OpenAPI_snssai_dnn_item_t *snssai_dnn_item);
OpenAPI_snssai_dnn_item_t *OpenAPI_snssai_dnn_item_copy(OpenAPI_snssai_dnn_item_t *dst, OpenAPI_snssai_dnn_item_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_snssai_dnn_item_H_ */

