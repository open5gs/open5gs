/*
 * tngf_info.h
 *
 * Infomation of the TNGF endpoints
 */

#ifndef _OpenAPI_tngf_info_H_
#define _OpenAPI_tngf_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_tngf_info_s OpenAPI_tngf_info_t;
typedef struct OpenAPI_tngf_info_s {
    OpenAPI_list_t *ipv4_endpoint_addresses;
    OpenAPI_list_t *ipv6_endpoint_addresses;
    char *endpoint_fqdn;
} OpenAPI_tngf_info_t;

OpenAPI_tngf_info_t *OpenAPI_tngf_info_create(
    OpenAPI_list_t *ipv4_endpoint_addresses,
    OpenAPI_list_t *ipv6_endpoint_addresses,
    char *endpoint_fqdn
    );
void OpenAPI_tngf_info_free(OpenAPI_tngf_info_t *tngf_info);
OpenAPI_tngf_info_t *OpenAPI_tngf_info_parseFromJSON(cJSON *tngf_infoJSON);
cJSON *OpenAPI_tngf_info_convertToJSON(OpenAPI_tngf_info_t *tngf_info);
OpenAPI_tngf_info_t *OpenAPI_tngf_info_copy(OpenAPI_tngf_info_t *dst, OpenAPI_tngf_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_tngf_info_H_ */

