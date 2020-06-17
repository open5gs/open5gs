/*
 * twif_info.h
 *
 *
 */

#ifndef _OpenAPI_twif_info_H_
#define _OpenAPI_twif_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_twif_info_s OpenAPI_twif_info_t;
typedef struct OpenAPI_twif_info_s {
    OpenAPI_list_t *ipv4_endpoint_addresses;
    OpenAPI_list_t *ipv6_endpoint_addresses;
    char *endpoint_fqdn;
} OpenAPI_twif_info_t;

OpenAPI_twif_info_t *OpenAPI_twif_info_create(
    OpenAPI_list_t *ipv4_endpoint_addresses,
    OpenAPI_list_t *ipv6_endpoint_addresses,
    char *endpoint_fqdn
    );
void OpenAPI_twif_info_free(OpenAPI_twif_info_t *twif_info);
OpenAPI_twif_info_t *OpenAPI_twif_info_parseFromJSON(cJSON *twif_infoJSON);
cJSON *OpenAPI_twif_info_convertToJSON(OpenAPI_twif_info_t *twif_info);
OpenAPI_twif_info_t *OpenAPI_twif_info_copy(OpenAPI_twif_info_t *dst, OpenAPI_twif_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_twif_info_H_ */

