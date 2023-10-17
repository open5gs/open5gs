/*
 * ip_sm_gw_info.h
 *
 * Contains the IP-SM-GW Information
 */

#ifndef _OpenAPI_ip_sm_gw_info_H_
#define _OpenAPI_ip_sm_gw_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ip_sm_gw_guidance.h"
#include "ip_sm_gw_registration.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ip_sm_gw_info_s OpenAPI_ip_sm_gw_info_t;
typedef struct OpenAPI_ip_sm_gw_info_s {
    struct OpenAPI_ip_sm_gw_registration_s *ip_sm_gw_registration;
    struct OpenAPI_ip_sm_gw_guidance_s *ip_sm_gw_guidance;
} OpenAPI_ip_sm_gw_info_t;

OpenAPI_ip_sm_gw_info_t *OpenAPI_ip_sm_gw_info_create(
    OpenAPI_ip_sm_gw_registration_t *ip_sm_gw_registration,
    OpenAPI_ip_sm_gw_guidance_t *ip_sm_gw_guidance
);
void OpenAPI_ip_sm_gw_info_free(OpenAPI_ip_sm_gw_info_t *ip_sm_gw_info);
OpenAPI_ip_sm_gw_info_t *OpenAPI_ip_sm_gw_info_parseFromJSON(cJSON *ip_sm_gw_infoJSON);
cJSON *OpenAPI_ip_sm_gw_info_convertToJSON(OpenAPI_ip_sm_gw_info_t *ip_sm_gw_info);
OpenAPI_ip_sm_gw_info_t *OpenAPI_ip_sm_gw_info_copy(OpenAPI_ip_sm_gw_info_t *dst, OpenAPI_ip_sm_gw_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ip_sm_gw_info_H_ */

