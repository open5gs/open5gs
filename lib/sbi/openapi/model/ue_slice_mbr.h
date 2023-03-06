/*
 * ue_slice_mbr.h
 *
 * Contains a UE-Slice-MBR and the related information.
 */

#ifndef _OpenAPI_ue_slice_mbr_H_
#define _OpenAPI_ue_slice_mbr_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "slice_mbr.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ue_slice_mbr_s OpenAPI_ue_slice_mbr_t;
typedef struct OpenAPI_ue_slice_mbr_s {
    OpenAPI_list_t* slice_mbr;
    struct OpenAPI_snssai_s *serving_snssai;
    struct OpenAPI_snssai_s *mapped_home_snssai;
} OpenAPI_ue_slice_mbr_t;

OpenAPI_ue_slice_mbr_t *OpenAPI_ue_slice_mbr_create(
    OpenAPI_list_t* slice_mbr,
    OpenAPI_snssai_t *serving_snssai,
    OpenAPI_snssai_t *mapped_home_snssai
);
void OpenAPI_ue_slice_mbr_free(OpenAPI_ue_slice_mbr_t *ue_slice_mbr);
OpenAPI_ue_slice_mbr_t *OpenAPI_ue_slice_mbr_parseFromJSON(cJSON *ue_slice_mbrJSON);
cJSON *OpenAPI_ue_slice_mbr_convertToJSON(OpenAPI_ue_slice_mbr_t *ue_slice_mbr);
OpenAPI_ue_slice_mbr_t *OpenAPI_ue_slice_mbr_copy(OpenAPI_ue_slice_mbr_t *dst, OpenAPI_ue_slice_mbr_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_slice_mbr_H_ */

