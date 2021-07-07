/*
 * w_agf_info.h
 *
 * Information of the W-AGF end-points
 */

#ifndef _OpenAPI_w_agf_info_H_
#define _OpenAPI_w_agf_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_w_agf_info_s OpenAPI_w_agf_info_t;
typedef struct OpenAPI_w_agf_info_s {
    OpenAPI_list_t *ipv4_endpoint_addresses;
    OpenAPI_list_t *ipv6_endpoint_addresses;
    char *endpoint_fqdn;
} OpenAPI_w_agf_info_t;

OpenAPI_w_agf_info_t *OpenAPI_w_agf_info_create(
    OpenAPI_list_t *ipv4_endpoint_addresses,
    OpenAPI_list_t *ipv6_endpoint_addresses,
    char *endpoint_fqdn
);
void OpenAPI_w_agf_info_free(OpenAPI_w_agf_info_t *w_agf_info);
OpenAPI_w_agf_info_t *OpenAPI_w_agf_info_parseFromJSON(cJSON *w_agf_infoJSON);
cJSON *OpenAPI_w_agf_info_convertToJSON(OpenAPI_w_agf_info_t *w_agf_info);
OpenAPI_w_agf_info_t *OpenAPI_w_agf_info_copy(OpenAPI_w_agf_info_t *dst, OpenAPI_w_agf_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_w_agf_info_H_ */

