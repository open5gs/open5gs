/*
 * ipx_provider_sec_info.h
 *
 * Defines the security information list of an IPX
 */

#ifndef _OpenAPI_ipx_provider_sec_info_H_
#define _OpenAPI_ipx_provider_sec_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ipx_provider_sec_info_s OpenAPI_ipx_provider_sec_info_t;
typedef struct OpenAPI_ipx_provider_sec_info_s {
    char *ipx_provider_id;
    OpenAPI_list_t *raw_public_key_list;
    OpenAPI_list_t *certificate_list;
} OpenAPI_ipx_provider_sec_info_t;

OpenAPI_ipx_provider_sec_info_t *OpenAPI_ipx_provider_sec_info_create(
    char *ipx_provider_id,
    OpenAPI_list_t *raw_public_key_list,
    OpenAPI_list_t *certificate_list
);
void OpenAPI_ipx_provider_sec_info_free(OpenAPI_ipx_provider_sec_info_t *ipx_provider_sec_info);
OpenAPI_ipx_provider_sec_info_t *OpenAPI_ipx_provider_sec_info_parseFromJSON(cJSON *ipx_provider_sec_infoJSON);
cJSON *OpenAPI_ipx_provider_sec_info_convertToJSON(OpenAPI_ipx_provider_sec_info_t *ipx_provider_sec_info);
OpenAPI_ipx_provider_sec_info_t *OpenAPI_ipx_provider_sec_info_copy(OpenAPI_ipx_provider_sec_info_t *dst, OpenAPI_ipx_provider_sec_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ipx_provider_sec_info_H_ */

