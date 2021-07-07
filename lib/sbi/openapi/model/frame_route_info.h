/*
 * frame_route_info.h
 *
 * 
 */

#ifndef _OpenAPI_frame_route_info_H_
#define _OpenAPI_frame_route_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_frame_route_info_s OpenAPI_frame_route_info_t;
typedef struct OpenAPI_frame_route_info_s {
    char *ipv4_mask;
    char *ipv6_prefix;
} OpenAPI_frame_route_info_t;

OpenAPI_frame_route_info_t *OpenAPI_frame_route_info_create(
    char *ipv4_mask,
    char *ipv6_prefix
);
void OpenAPI_frame_route_info_free(OpenAPI_frame_route_info_t *frame_route_info);
OpenAPI_frame_route_info_t *OpenAPI_frame_route_info_parseFromJSON(cJSON *frame_route_infoJSON);
cJSON *OpenAPI_frame_route_info_convertToJSON(OpenAPI_frame_route_info_t *frame_route_info);
OpenAPI_frame_route_info_t *OpenAPI_frame_route_info_copy(OpenAPI_frame_route_info_t *dst, OpenAPI_frame_route_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_frame_route_info_H_ */

