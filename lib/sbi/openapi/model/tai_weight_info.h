/*
 * tai_weight_info.h
 *
 * Weight assigned to TAI List(s) and/or TAI range List(s). A higher value indicates a higher priority. 
 */

#ifndef _OpenAPI_tai_weight_info_H_
#define _OpenAPI_tai_weight_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_tai_weight_info_s OpenAPI_tai_weight_info_t;
#include "tai.h"
#include "tai_range.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_tai_weight_info_s {
    OpenAPI_list_t *tai_list;
    OpenAPI_list_t *tai_range_list;
    int weight;
};

OpenAPI_tai_weight_info_t *OpenAPI_tai_weight_info_create(
    OpenAPI_list_t *tai_list,
    OpenAPI_list_t *tai_range_list,
    int weight
);
void OpenAPI_tai_weight_info_free(OpenAPI_tai_weight_info_t *tai_weight_info);
OpenAPI_tai_weight_info_t *OpenAPI_tai_weight_info_parseFromJSON(cJSON *tai_weight_infoJSON);
cJSON *OpenAPI_tai_weight_info_convertToJSON(OpenAPI_tai_weight_info_t *tai_weight_info);
OpenAPI_tai_weight_info_t *OpenAPI_tai_weight_info_copy(OpenAPI_tai_weight_info_t *dst, OpenAPI_tai_weight_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_tai_weight_info_H_ */

