/*
 * mb_upf_info.h
 *
 * Information of an MB-UPF NF Instance
 */

#ifndef _OpenAPI_mb_upf_info_H_
#define _OpenAPI_mb_upf_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "interface_upf_info_item.h"
#include "snssai_upf_info_item.h"
#include "tai.h"
#include "tai_range.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_mb_upf_info_s OpenAPI_mb_upf_info_t;
typedef struct OpenAPI_mb_upf_info_s {
    OpenAPI_list_t *s_nssai_mb_upf_info_list;
    OpenAPI_list_t *mb_smf_serving_area;
    OpenAPI_list_t *interface_mb_upf_info_list;
    OpenAPI_list_t *tai_list;
    OpenAPI_list_t *tai_range_list;
    bool is_priority;
    int priority;
    char *supported_pfcp_features;
} OpenAPI_mb_upf_info_t;

OpenAPI_mb_upf_info_t *OpenAPI_mb_upf_info_create(
    OpenAPI_list_t *s_nssai_mb_upf_info_list,
    OpenAPI_list_t *mb_smf_serving_area,
    OpenAPI_list_t *interface_mb_upf_info_list,
    OpenAPI_list_t *tai_list,
    OpenAPI_list_t *tai_range_list,
    bool is_priority,
    int priority,
    char *supported_pfcp_features
);
void OpenAPI_mb_upf_info_free(OpenAPI_mb_upf_info_t *mb_upf_info);
OpenAPI_mb_upf_info_t *OpenAPI_mb_upf_info_parseFromJSON(cJSON *mb_upf_infoJSON);
cJSON *OpenAPI_mb_upf_info_convertToJSON(OpenAPI_mb_upf_info_t *mb_upf_info);
OpenAPI_mb_upf_info_t *OpenAPI_mb_upf_info_copy(OpenAPI_mb_upf_info_t *dst, OpenAPI_mb_upf_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_mb_upf_info_H_ */

