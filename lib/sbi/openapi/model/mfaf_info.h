/*
 * mfaf_info.h
 *
 * Information of a MFAF NF Instance
 */

#ifndef _OpenAPI_mfaf_info_H_
#define _OpenAPI_mfaf_info_H_

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

typedef struct OpenAPI_mfaf_info_s OpenAPI_mfaf_info_t;
typedef struct OpenAPI_mfaf_info_s {
    OpenAPI_list_t *serving_nf_type_list;
    OpenAPI_list_t *serving_nf_set_id_list;
    OpenAPI_list_t *tai_list;
    OpenAPI_list_t *tai_range_list;
} OpenAPI_mfaf_info_t;

OpenAPI_mfaf_info_t *OpenAPI_mfaf_info_create(
    OpenAPI_list_t *serving_nf_type_list,
    OpenAPI_list_t *serving_nf_set_id_list,
    OpenAPI_list_t *tai_list,
    OpenAPI_list_t *tai_range_list
);
void OpenAPI_mfaf_info_free(OpenAPI_mfaf_info_t *mfaf_info);
OpenAPI_mfaf_info_t *OpenAPI_mfaf_info_parseFromJSON(cJSON *mfaf_infoJSON);
cJSON *OpenAPI_mfaf_info_convertToJSON(OpenAPI_mfaf_info_t *mfaf_info);
OpenAPI_mfaf_info_t *OpenAPI_mfaf_info_copy(OpenAPI_mfaf_info_t *dst, OpenAPI_mfaf_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_mfaf_info_H_ */

