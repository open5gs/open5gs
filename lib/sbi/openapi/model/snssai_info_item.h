/*
 * snssai_info_item.h
 *
 * Parameters supported by an NF for a given S-NSSAI Set of parameters supported by NF for a given S-NSSAI 
 */

#ifndef _OpenAPI_snssai_info_item_H_
#define _OpenAPI_snssai_info_item_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "dnn_info_item.h"
#include "ext_snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_snssai_info_item_s OpenAPI_snssai_info_item_t;
typedef struct OpenAPI_snssai_info_item_s {
    struct OpenAPI_ext_snssai_s *s_nssai;
    OpenAPI_list_t *dnn_info_list;
} OpenAPI_snssai_info_item_t;

OpenAPI_snssai_info_item_t *OpenAPI_snssai_info_item_create(
    OpenAPI_ext_snssai_t *s_nssai,
    OpenAPI_list_t *dnn_info_list
);
void OpenAPI_snssai_info_item_free(OpenAPI_snssai_info_item_t *snssai_info_item);
OpenAPI_snssai_info_item_t *OpenAPI_snssai_info_item_parseFromJSON(cJSON *snssai_info_itemJSON);
cJSON *OpenAPI_snssai_info_item_convertToJSON(OpenAPI_snssai_info_item_t *snssai_info_item);
OpenAPI_snssai_info_item_t *OpenAPI_snssai_info_item_copy(OpenAPI_snssai_info_item_t *dst, OpenAPI_snssai_info_item_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_snssai_info_item_H_ */

