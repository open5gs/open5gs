/*
 * nsag_info.h
 *
 * Contains the association of NSAGs and S-NSSAI(s) along with the TA(s) within which the association is valid.
 */

#ifndef _OpenAPI_nsag_info_H_
#define _OpenAPI_nsag_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "snssai.h"
#include "tai.h"
#include "tai_range.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_nsag_info_s OpenAPI_nsag_info_t;
typedef struct OpenAPI_nsag_info_s {
    OpenAPI_list_t *nsag_ids;
    OpenAPI_list_t *snssai_list;
    OpenAPI_list_t *tai_list;
    OpenAPI_list_t *tai_range_list;
} OpenAPI_nsag_info_t;

OpenAPI_nsag_info_t *OpenAPI_nsag_info_create(
    OpenAPI_list_t *nsag_ids,
    OpenAPI_list_t *snssai_list,
    OpenAPI_list_t *tai_list,
    OpenAPI_list_t *tai_range_list
);
void OpenAPI_nsag_info_free(OpenAPI_nsag_info_t *nsag_info);
OpenAPI_nsag_info_t *OpenAPI_nsag_info_parseFromJSON(cJSON *nsag_infoJSON);
cJSON *OpenAPI_nsag_info_convertToJSON(OpenAPI_nsag_info_t *nsag_info);
OpenAPI_nsag_info_t *OpenAPI_nsag_info_copy(OpenAPI_nsag_info_t *dst, OpenAPI_nsag_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nsag_info_H_ */

