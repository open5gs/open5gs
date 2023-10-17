/*
 * ssm_1.h
 *
 * Source specific IP multicast address
 */

#ifndef _OpenAPI_ssm_1_H_
#define _OpenAPI_ssm_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ip_addr_1.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ssm_1_s OpenAPI_ssm_1_t;
typedef struct OpenAPI_ssm_1_s {
    struct OpenAPI_ip_addr_1_s *source_ip_addr;
    struct OpenAPI_ip_addr_1_s *dest_ip_addr;
} OpenAPI_ssm_1_t;

OpenAPI_ssm_1_t *OpenAPI_ssm_1_create(
    OpenAPI_ip_addr_1_t *source_ip_addr,
    OpenAPI_ip_addr_1_t *dest_ip_addr
);
void OpenAPI_ssm_1_free(OpenAPI_ssm_1_t *ssm_1);
OpenAPI_ssm_1_t *OpenAPI_ssm_1_parseFromJSON(cJSON *ssm_1JSON);
cJSON *OpenAPI_ssm_1_convertToJSON(OpenAPI_ssm_1_t *ssm_1);
OpenAPI_ssm_1_t *OpenAPI_ssm_1_copy(OpenAPI_ssm_1_t *dst, OpenAPI_ssm_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ssm_1_H_ */

