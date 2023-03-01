/*
 * ip_sm_gw_guidance.h
 *
 * Contains guidance information (e.g. minimum and recommended delivery times) of the IP-SM-GW 
 */

#ifndef _OpenAPI_ip_sm_gw_guidance_H_
#define _OpenAPI_ip_sm_gw_guidance_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ip_sm_gw_guidance_s OpenAPI_ip_sm_gw_guidance_t;
typedef struct OpenAPI_ip_sm_gw_guidance_s {
    int min_delivery_time;
    int recomm_delivery_time;
} OpenAPI_ip_sm_gw_guidance_t;

OpenAPI_ip_sm_gw_guidance_t *OpenAPI_ip_sm_gw_guidance_create(
    int min_delivery_time,
    int recomm_delivery_time
);
void OpenAPI_ip_sm_gw_guidance_free(OpenAPI_ip_sm_gw_guidance_t *ip_sm_gw_guidance);
OpenAPI_ip_sm_gw_guidance_t *OpenAPI_ip_sm_gw_guidance_parseFromJSON(cJSON *ip_sm_gw_guidanceJSON);
cJSON *OpenAPI_ip_sm_gw_guidance_convertToJSON(OpenAPI_ip_sm_gw_guidance_t *ip_sm_gw_guidance);
OpenAPI_ip_sm_gw_guidance_t *OpenAPI_ip_sm_gw_guidance_copy(OpenAPI_ip_sm_gw_guidance_t *dst, OpenAPI_ip_sm_gw_guidance_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ip_sm_gw_guidance_H_ */

