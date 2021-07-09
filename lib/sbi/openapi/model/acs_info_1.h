/*
 * acs_info_1.h
 *
 * 
 */

#ifndef _OpenAPI_acs_info_1_H_
#define _OpenAPI_acs_info_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_acs_info_1_s OpenAPI_acs_info_1_t;
typedef struct OpenAPI_acs_info_1_s {
    char *acs_url;
    char *acs_ipv4_addr;
    char *acs_ipv6_addr;
} OpenAPI_acs_info_1_t;

OpenAPI_acs_info_1_t *OpenAPI_acs_info_1_create(
    char *acs_url,
    char *acs_ipv4_addr,
    char *acs_ipv6_addr
);
void OpenAPI_acs_info_1_free(OpenAPI_acs_info_1_t *acs_info_1);
OpenAPI_acs_info_1_t *OpenAPI_acs_info_1_parseFromJSON(cJSON *acs_info_1JSON);
cJSON *OpenAPI_acs_info_1_convertToJSON(OpenAPI_acs_info_1_t *acs_info_1);
OpenAPI_acs_info_1_t *OpenAPI_acs_info_1_copy(OpenAPI_acs_info_1_t *dst, OpenAPI_acs_info_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_acs_info_1_H_ */

