/*
 * pcf_addressing_info.h
 *
 * Contains PCF address information.
 */

#ifndef _OpenAPI_pcf_addressing_info_H_
#define _OpenAPI_pcf_addressing_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ip_end_point.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pcf_addressing_info_s OpenAPI_pcf_addressing_info_t;
typedef struct OpenAPI_pcf_addressing_info_s {
    char *pcf_fqdn;
    OpenAPI_list_t *pcf_ip_end_points;
    char *binding_info;
} OpenAPI_pcf_addressing_info_t;

OpenAPI_pcf_addressing_info_t *OpenAPI_pcf_addressing_info_create(
    char *pcf_fqdn,
    OpenAPI_list_t *pcf_ip_end_points,
    char *binding_info
);
void OpenAPI_pcf_addressing_info_free(OpenAPI_pcf_addressing_info_t *pcf_addressing_info);
OpenAPI_pcf_addressing_info_t *OpenAPI_pcf_addressing_info_parseFromJSON(cJSON *pcf_addressing_infoJSON);
cJSON *OpenAPI_pcf_addressing_info_convertToJSON(OpenAPI_pcf_addressing_info_t *pcf_addressing_info);
OpenAPI_pcf_addressing_info_t *OpenAPI_pcf_addressing_info_copy(OpenAPI_pcf_addressing_info_t *dst, OpenAPI_pcf_addressing_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pcf_addressing_info_H_ */

