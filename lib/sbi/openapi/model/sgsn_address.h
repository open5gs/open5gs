/*
 * sgsn_address.h
 *
 * describes the address of the SGSN
 */

#ifndef _OpenAPI_sgsn_address_H_
#define _OpenAPI_sgsn_address_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_sgsn_address_s OpenAPI_sgsn_address_t;
typedef struct OpenAPI_sgsn_address_s {
    char *sgsn_ipv4_addr;
    char *sgsn_ipv6_addr;
} OpenAPI_sgsn_address_t;

OpenAPI_sgsn_address_t *OpenAPI_sgsn_address_create(
    char *sgsn_ipv4_addr,
    char *sgsn_ipv6_addr
);
void OpenAPI_sgsn_address_free(OpenAPI_sgsn_address_t *sgsn_address);
OpenAPI_sgsn_address_t *OpenAPI_sgsn_address_parseFromJSON(cJSON *sgsn_addressJSON);
cJSON *OpenAPI_sgsn_address_convertToJSON(OpenAPI_sgsn_address_t *sgsn_address);
OpenAPI_sgsn_address_t *OpenAPI_sgsn_address_copy(OpenAPI_sgsn_address_t *dst, OpenAPI_sgsn_address_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sgsn_address_H_ */

