/*
 * slice_mbr.h
 *
 * MBR related to slice
 */

#ifndef _OpenAPI_slice_mbr_H_
#define _OpenAPI_slice_mbr_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_slice_mbr_s OpenAPI_slice_mbr_t;
typedef struct OpenAPI_slice_mbr_s {
    char *uplink;
    char *downlink;
} OpenAPI_slice_mbr_t;

OpenAPI_slice_mbr_t *OpenAPI_slice_mbr_create(
    char *uplink,
    char *downlink
);
void OpenAPI_slice_mbr_free(OpenAPI_slice_mbr_t *slice_mbr);
OpenAPI_slice_mbr_t *OpenAPI_slice_mbr_parseFromJSON(cJSON *slice_mbrJSON);
cJSON *OpenAPI_slice_mbr_convertToJSON(OpenAPI_slice_mbr_t *slice_mbr);
OpenAPI_slice_mbr_t *OpenAPI_slice_mbr_copy(OpenAPI_slice_mbr_t *dst, OpenAPI_slice_mbr_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_slice_mbr_H_ */

