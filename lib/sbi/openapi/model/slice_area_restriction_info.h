/*
 * slice_area_restriction_info.h
 *
 * Slice Area Restriction Information
 */

#ifndef _OpenAPI_slice_area_restriction_info_H_
#define _OpenAPI_slice_area_restriction_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_slice_area_restriction_info_s OpenAPI_slice_area_restriction_info_t;
#include "presence_state.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_slice_area_restriction_info_s {
    struct OpenAPI_snssai_s *s_nssai;
    OpenAPI_presence_state_e presence;
};

OpenAPI_slice_area_restriction_info_t *OpenAPI_slice_area_restriction_info_create(
    OpenAPI_snssai_t *s_nssai,
    OpenAPI_presence_state_e presence
);
void OpenAPI_slice_area_restriction_info_free(OpenAPI_slice_area_restriction_info_t *slice_area_restriction_info);
OpenAPI_slice_area_restriction_info_t *OpenAPI_slice_area_restriction_info_parseFromJSON(cJSON *slice_area_restriction_infoJSON);
cJSON *OpenAPI_slice_area_restriction_info_convertToJSON(OpenAPI_slice_area_restriction_info_t *slice_area_restriction_info);
OpenAPI_slice_area_restriction_info_t *OpenAPI_slice_area_restriction_info_copy(OpenAPI_slice_area_restriction_info_t *dst, OpenAPI_slice_area_restriction_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_slice_area_restriction_info_H_ */

