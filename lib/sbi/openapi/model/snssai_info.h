/*
 * snssai_info.h
 *
 *
 */

#ifndef _OpenAPI_snssai_info_H_
#define _OpenAPI_snssai_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "dnn_info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_snssai_info_s OpenAPI_snssai_info_t;
typedef struct OpenAPI_snssai_info_s {
    OpenAPI_list_t *dnn_infos;
} OpenAPI_snssai_info_t;

OpenAPI_snssai_info_t *OpenAPI_snssai_info_create(
    OpenAPI_list_t *dnn_infos
    );
void OpenAPI_snssai_info_free(OpenAPI_snssai_info_t *snssai_info);
OpenAPI_snssai_info_t *OpenAPI_snssai_info_parseFromJSON(cJSON *snssai_infoJSON);
cJSON *OpenAPI_snssai_info_convertToJSON(OpenAPI_snssai_info_t *snssai_info);
OpenAPI_snssai_info_t *OpenAPI_snssai_info_copy(OpenAPI_snssai_info_t *dst, OpenAPI_snssai_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_snssai_info_H_ */

