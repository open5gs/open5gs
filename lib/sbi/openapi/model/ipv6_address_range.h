/*
 * ipv6_address_range.h
 *
 * Range of IPv6 addresses
 */

#ifndef _OpenAPI_ipv6_address_range_H_
#define _OpenAPI_ipv6_address_range_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_ipv6_address_range_s OpenAPI_ipv6_address_range_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_ipv6_address_range_s {
    char *start;
    char *end;
};

OpenAPI_ipv6_address_range_t *OpenAPI_ipv6_address_range_create(
    char *start,
    char *end
);
void OpenAPI_ipv6_address_range_free(OpenAPI_ipv6_address_range_t *ipv6_address_range);
OpenAPI_ipv6_address_range_t *OpenAPI_ipv6_address_range_parseFromJSON(cJSON *ipv6_address_rangeJSON);
cJSON *OpenAPI_ipv6_address_range_convertToJSON(OpenAPI_ipv6_address_range_t *ipv6_address_range);
OpenAPI_ipv6_address_range_t *OpenAPI_ipv6_address_range_copy(OpenAPI_ipv6_address_range_t *dst, OpenAPI_ipv6_address_range_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ipv6_address_range_H_ */

