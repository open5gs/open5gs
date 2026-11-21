/*
 * ims_address.h
 *
 * 
 */

#ifndef _OpenAPI_ims_address_H_
#define _OpenAPI_ims_address_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_ims_address_s OpenAPI_ims_address_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_ims_address_s {
    char *ipv4_addr;
    char *ipv6_addr;
    char *e164;
};

OpenAPI_ims_address_t *OpenAPI_ims_address_create(
    char *ipv4_addr,
    char *ipv6_addr,
    char *e164
);
void OpenAPI_ims_address_free(OpenAPI_ims_address_t *ims_address);
OpenAPI_ims_address_t *OpenAPI_ims_address_parseFromJSON(cJSON *ims_addressJSON);
cJSON *OpenAPI_ims_address_convertToJSON(OpenAPI_ims_address_t *ims_address);
OpenAPI_ims_address_t *OpenAPI_ims_address_copy(OpenAPI_ims_address_t *dst, OpenAPI_ims_address_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ims_address_H_ */

