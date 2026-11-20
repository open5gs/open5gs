/*
 * mm_originator_info.h
 *
 * 
 */

#ifndef _OpenAPI_mm_originator_info_H_
#define _OpenAPI_mm_originator_info_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_mm_originator_info_s OpenAPI_mm_originator_info_t;
#include "sm_address_info.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_mm_originator_info_s {
    char *originator_supi;
    char *originator_gpsi;
    OpenAPI_list_t *originator_other_address;
};

OpenAPI_mm_originator_info_t *OpenAPI_mm_originator_info_create(
    char *originator_supi,
    char *originator_gpsi,
    OpenAPI_list_t *originator_other_address
);
void OpenAPI_mm_originator_info_free(OpenAPI_mm_originator_info_t *mm_originator_info);
OpenAPI_mm_originator_info_t *OpenAPI_mm_originator_info_parseFromJSON(cJSON *mm_originator_infoJSON);
cJSON *OpenAPI_mm_originator_info_convertToJSON(OpenAPI_mm_originator_info_t *mm_originator_info);
OpenAPI_mm_originator_info_t *OpenAPI_mm_originator_info_copy(OpenAPI_mm_originator_info_t *dst, OpenAPI_mm_originator_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_mm_originator_info_H_ */

