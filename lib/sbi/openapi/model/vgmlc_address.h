/*
 * vgmlc_address.h
 *
 *
 */

#ifndef _OpenAPI_vgmlc_address_H_
#define _OpenAPI_vgmlc_address_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_vgmlc_address_s OpenAPI_vgmlc_address_t;
typedef struct OpenAPI_vgmlc_address_s {
    char *vgmlc_address_ipv4;
    char *vgmlc_address_ipv6;
    char *vgmlc_fqdn;
} OpenAPI_vgmlc_address_t;

OpenAPI_vgmlc_address_t *OpenAPI_vgmlc_address_create(
    char *vgmlc_address_ipv4,
    char *vgmlc_address_ipv6,
    char *vgmlc_fqdn
    );
void OpenAPI_vgmlc_address_free(OpenAPI_vgmlc_address_t *vgmlc_address);
OpenAPI_vgmlc_address_t *OpenAPI_vgmlc_address_parseFromJSON(cJSON *vgmlc_addressJSON);
cJSON *OpenAPI_vgmlc_address_convertToJSON(OpenAPI_vgmlc_address_t *vgmlc_address);
OpenAPI_vgmlc_address_t *OpenAPI_vgmlc_address_copy(OpenAPI_vgmlc_address_t *dst, OpenAPI_vgmlc_address_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_vgmlc_address_H_ */

