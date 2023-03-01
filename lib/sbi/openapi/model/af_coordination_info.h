/*
 * af_coordination_info.h
 *
 * AF Coordination Information
 */

#ifndef _OpenAPI_af_coordination_info_H_
#define _OpenAPI_af_coordination_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "notification_info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_af_coordination_info_s OpenAPI_af_coordination_info_t;
typedef struct OpenAPI_af_coordination_info_s {
    char *source_dnai;
    char *source_ue_ipv4_addr;
    char *source_ue_ipv6_prefix;
    OpenAPI_list_t *notification_info_list;
} OpenAPI_af_coordination_info_t;

OpenAPI_af_coordination_info_t *OpenAPI_af_coordination_info_create(
    char *source_dnai,
    char *source_ue_ipv4_addr,
    char *source_ue_ipv6_prefix,
    OpenAPI_list_t *notification_info_list
);
void OpenAPI_af_coordination_info_free(OpenAPI_af_coordination_info_t *af_coordination_info);
OpenAPI_af_coordination_info_t *OpenAPI_af_coordination_info_parseFromJSON(cJSON *af_coordination_infoJSON);
cJSON *OpenAPI_af_coordination_info_convertToJSON(OpenAPI_af_coordination_info_t *af_coordination_info);
OpenAPI_af_coordination_info_t *OpenAPI_af_coordination_info_copy(OpenAPI_af_coordination_info_t *dst, OpenAPI_af_coordination_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_af_coordination_info_H_ */

