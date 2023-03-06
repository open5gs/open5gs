/*
 * pcscf_address.h
 *
 * Contains the addressing information (IP addresses and/or FQDN) of the P-CSCF
 */

#ifndef _OpenAPI_pcscf_address_H_
#define _OpenAPI_pcscf_address_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pcscf_address_s OpenAPI_pcscf_address_t;
typedef struct OpenAPI_pcscf_address_s {
    OpenAPI_list_t *ipv4_addrs;
    OpenAPI_list_t *ipv6_addrs;
    char *fqdn;
} OpenAPI_pcscf_address_t;

OpenAPI_pcscf_address_t *OpenAPI_pcscf_address_create(
    OpenAPI_list_t *ipv4_addrs,
    OpenAPI_list_t *ipv6_addrs,
    char *fqdn
);
void OpenAPI_pcscf_address_free(OpenAPI_pcscf_address_t *pcscf_address);
OpenAPI_pcscf_address_t *OpenAPI_pcscf_address_parseFromJSON(cJSON *pcscf_addressJSON);
cJSON *OpenAPI_pcscf_address_convertToJSON(OpenAPI_pcscf_address_t *pcscf_address);
OpenAPI_pcscf_address_t *OpenAPI_pcscf_address_copy(OpenAPI_pcscf_address_t *dst, OpenAPI_pcscf_address_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pcscf_address_H_ */

