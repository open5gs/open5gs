/*
 * slice_mbr_1.h
 *
 * MBR related to slice
 */

#ifndef _OpenAPI_slice_mbr_1_H_
#define _OpenAPI_slice_mbr_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_slice_mbr_1_s OpenAPI_slice_mbr_1_t;
typedef struct OpenAPI_slice_mbr_1_s {
    char *uplink;
    char *downlink;
} OpenAPI_slice_mbr_1_t;

OpenAPI_slice_mbr_1_t *OpenAPI_slice_mbr_1_create(
    char *uplink,
    char *downlink
);
void OpenAPI_slice_mbr_1_free(OpenAPI_slice_mbr_1_t *slice_mbr_1);
OpenAPI_slice_mbr_1_t *OpenAPI_slice_mbr_1_parseFromJSON(cJSON *slice_mbr_1JSON);
cJSON *OpenAPI_slice_mbr_1_convertToJSON(OpenAPI_slice_mbr_1_t *slice_mbr_1);
OpenAPI_slice_mbr_1_t *OpenAPI_slice_mbr_1_copy(OpenAPI_slice_mbr_1_t *dst, OpenAPI_slice_mbr_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_slice_mbr_1_H_ */

