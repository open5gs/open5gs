/*
 * acknowledge_info.h
 *
 *
 */

#ifndef _OpenAPI_acknowledge_info_H_
#define _OpenAPI_acknowledge_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_acknowledge_info_s OpenAPI_acknowledge_info_t;
typedef struct OpenAPI_acknowledge_info_s {
    char *sor_mac_iue;
    char *upu_mac_iue;
    char *secured_packet;
    char *provisioning_time;
} OpenAPI_acknowledge_info_t;

OpenAPI_acknowledge_info_t *OpenAPI_acknowledge_info_create(
    char *sor_mac_iue,
    char *upu_mac_iue,
    char *secured_packet,
    char *provisioning_time
    );
void OpenAPI_acknowledge_info_free(OpenAPI_acknowledge_info_t *acknowledge_info);
OpenAPI_acknowledge_info_t *OpenAPI_acknowledge_info_parseFromJSON(cJSON *acknowledge_infoJSON);
cJSON *OpenAPI_acknowledge_info_convertToJSON(OpenAPI_acknowledge_info_t *acknowledge_info);
OpenAPI_acknowledge_info_t *OpenAPI_acknowledge_info_copy(OpenAPI_acknowledge_info_t *dst, OpenAPI_acknowledge_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_acknowledge_info_H_ */

