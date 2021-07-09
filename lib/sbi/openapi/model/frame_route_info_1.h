/*
 * frame_route_info_1.h
 *
 * 
 */

#ifndef _OpenAPI_frame_route_info_1_H_
#define _OpenAPI_frame_route_info_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_frame_route_info_1_s OpenAPI_frame_route_info_1_t;
typedef struct OpenAPI_frame_route_info_1_s {
    char *ipv4_mask;
    char *ipv6_prefix;
} OpenAPI_frame_route_info_1_t;

OpenAPI_frame_route_info_1_t *OpenAPI_frame_route_info_1_create(
    char *ipv4_mask,
    char *ipv6_prefix
);
void OpenAPI_frame_route_info_1_free(OpenAPI_frame_route_info_1_t *frame_route_info_1);
OpenAPI_frame_route_info_1_t *OpenAPI_frame_route_info_1_parseFromJSON(cJSON *frame_route_info_1JSON);
cJSON *OpenAPI_frame_route_info_1_convertToJSON(OpenAPI_frame_route_info_1_t *frame_route_info_1);
OpenAPI_frame_route_info_1_t *OpenAPI_frame_route_info_1_copy(OpenAPI_frame_route_info_1_t *dst, OpenAPI_frame_route_info_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_frame_route_info_1_H_ */

