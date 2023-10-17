/*
 * acs_info_rm.h
 *
 * This data type is defined in the same way as the &#39;AcsInfo&#39; data type, but with the  OpenAPI &#39;nullable: true&#39; property. 
 */

#ifndef _OpenAPI_acs_info_rm_H_
#define _OpenAPI_acs_info_rm_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "acs_info.h"
#include "null_value.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_acs_info_rm_s OpenAPI_acs_info_rm_t;
typedef struct OpenAPI_acs_info_rm_s {
    char *acs_url;
    char *acs_ipv4_addr;
    char *acs_ipv6_addr;
} OpenAPI_acs_info_rm_t;

OpenAPI_acs_info_rm_t *OpenAPI_acs_info_rm_create(
    char *acs_url,
    char *acs_ipv4_addr,
    char *acs_ipv6_addr
);
void OpenAPI_acs_info_rm_free(OpenAPI_acs_info_rm_t *acs_info_rm);
OpenAPI_acs_info_rm_t *OpenAPI_acs_info_rm_parseFromJSON(cJSON *acs_info_rmJSON);
cJSON *OpenAPI_acs_info_rm_convertToJSON(OpenAPI_acs_info_rm_t *acs_info_rm);
OpenAPI_acs_info_rm_t *OpenAPI_acs_info_rm_copy(OpenAPI_acs_info_rm_t *dst, OpenAPI_acs_info_rm_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_acs_info_rm_H_ */

