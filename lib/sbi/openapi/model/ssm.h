/*
 * ssm.h
 *
 * Source specific IP multicast address
 */

#ifndef _OpenAPI_ssm_H_
#define _OpenAPI_ssm_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ip_addr.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ssm_s OpenAPI_ssm_t;
typedef struct OpenAPI_ssm_s {
    struct OpenAPI_ip_addr_s *source_ip_addr;
    struct OpenAPI_ip_addr_s *dest_ip_addr;
} OpenAPI_ssm_t;

OpenAPI_ssm_t *OpenAPI_ssm_create(
    OpenAPI_ip_addr_t *source_ip_addr,
    OpenAPI_ip_addr_t *dest_ip_addr
);
void OpenAPI_ssm_free(OpenAPI_ssm_t *ssm);
OpenAPI_ssm_t *OpenAPI_ssm_parseFromJSON(cJSON *ssmJSON);
cJSON *OpenAPI_ssm_convertToJSON(OpenAPI_ssm_t *ssm);
OpenAPI_ssm_t *OpenAPI_ssm_copy(OpenAPI_ssm_t *dst, OpenAPI_ssm_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ssm_H_ */

