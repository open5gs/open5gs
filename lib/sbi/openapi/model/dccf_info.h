/*
 * dccf_info.h
 *
 * Information of a DCCF NF Instance
 */

#ifndef _OpenAPI_dccf_info_H_
#define _OpenAPI_dccf_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "nf_type.h"
#include "tai.h"
#include "tai_range.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_dccf_info_s OpenAPI_dccf_info_t;
typedef struct OpenAPI_dccf_info_s {
    OpenAPI_list_t *serving_nf_type_list;
    OpenAPI_list_t *serving_nf_set_id_list;
    OpenAPI_list_t *tai_list;
    OpenAPI_list_t *tai_range_list;
} OpenAPI_dccf_info_t;

OpenAPI_dccf_info_t *OpenAPI_dccf_info_create(
    OpenAPI_list_t *serving_nf_type_list,
    OpenAPI_list_t *serving_nf_set_id_list,
    OpenAPI_list_t *tai_list,
    OpenAPI_list_t *tai_range_list
);
void OpenAPI_dccf_info_free(OpenAPI_dccf_info_t *dccf_info);
OpenAPI_dccf_info_t *OpenAPI_dccf_info_parseFromJSON(cJSON *dccf_infoJSON);
cJSON *OpenAPI_dccf_info_convertToJSON(OpenAPI_dccf_info_t *dccf_info);
OpenAPI_dccf_info_t *OpenAPI_dccf_info_copy(OpenAPI_dccf_info_t *dst, OpenAPI_dccf_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_dccf_info_H_ */

