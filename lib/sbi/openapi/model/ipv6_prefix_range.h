/*
 * ipv6_prefix_range.h
 *
 * Range of IPv6 prefixes
 */

#ifndef _OpenAPI_ipv6_prefix_range_H_
#define _OpenAPI_ipv6_prefix_range_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ipv6_prefix_range_s OpenAPI_ipv6_prefix_range_t;
typedef struct OpenAPI_ipv6_prefix_range_s {
    char *start;
    char *end;
} OpenAPI_ipv6_prefix_range_t;

OpenAPI_ipv6_prefix_range_t *OpenAPI_ipv6_prefix_range_create(
    char *start,
    char *end
);
void OpenAPI_ipv6_prefix_range_free(OpenAPI_ipv6_prefix_range_t *ipv6_prefix_range);
OpenAPI_ipv6_prefix_range_t *OpenAPI_ipv6_prefix_range_parseFromJSON(cJSON *ipv6_prefix_rangeJSON);
cJSON *OpenAPI_ipv6_prefix_range_convertToJSON(OpenAPI_ipv6_prefix_range_t *ipv6_prefix_range);
OpenAPI_ipv6_prefix_range_t *OpenAPI_ipv6_prefix_range_copy(OpenAPI_ipv6_prefix_range_t *dst, OpenAPI_ipv6_prefix_range_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ipv6_prefix_range_H_ */

