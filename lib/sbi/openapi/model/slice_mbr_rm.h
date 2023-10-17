/*
 * slice_mbr_rm.h
 *
 * SliceMbr with nullable: true
 */

#ifndef _OpenAPI_slice_mbr_rm_H_
#define _OpenAPI_slice_mbr_rm_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "null_value.h"
#include "slice_mbr.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_slice_mbr_rm_s OpenAPI_slice_mbr_rm_t;
typedef struct OpenAPI_slice_mbr_rm_s {
    char *uplink;
    char *downlink;
} OpenAPI_slice_mbr_rm_t;

OpenAPI_slice_mbr_rm_t *OpenAPI_slice_mbr_rm_create(
    char *uplink,
    char *downlink
);
void OpenAPI_slice_mbr_rm_free(OpenAPI_slice_mbr_rm_t *slice_mbr_rm);
OpenAPI_slice_mbr_rm_t *OpenAPI_slice_mbr_rm_parseFromJSON(cJSON *slice_mbr_rmJSON);
cJSON *OpenAPI_slice_mbr_rm_convertToJSON(OpenAPI_slice_mbr_rm_t *slice_mbr_rm);
OpenAPI_slice_mbr_rm_t *OpenAPI_slice_mbr_rm_copy(OpenAPI_slice_mbr_rm_t *dst, OpenAPI_slice_mbr_rm_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_slice_mbr_rm_H_ */

