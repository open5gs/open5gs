/*
 * acs_info.h
 *
 * 
 */

#ifndef _OpenAPI_acs_info_H_
#define _OpenAPI_acs_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_acs_info_s OpenAPI_acs_info_t;
typedef struct OpenAPI_acs_info_s {
    char *acs_url;
    char *acs_ipv4_addr;
    char *acs_ipv6_addr;
} OpenAPI_acs_info_t;

OpenAPI_acs_info_t *OpenAPI_acs_info_create(
    char *acs_url,
    char *acs_ipv4_addr,
    char *acs_ipv6_addr
);
void OpenAPI_acs_info_free(OpenAPI_acs_info_t *acs_info);
OpenAPI_acs_info_t *OpenAPI_acs_info_parseFromJSON(cJSON *acs_infoJSON);
cJSON *OpenAPI_acs_info_convertToJSON(OpenAPI_acs_info_t *acs_info);
OpenAPI_acs_info_t *OpenAPI_acs_info_copy(OpenAPI_acs_info_t *dst, OpenAPI_acs_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_acs_info_H_ */

