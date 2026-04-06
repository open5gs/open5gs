/*
 * port_range.h
 *
 * Range of port numbers
 */

#ifndef _OpenAPI_port_range_H_
#define _OpenAPI_port_range_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_port_range_s OpenAPI_port_range_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_port_range_s {
    int start;
    int end;
};

OpenAPI_port_range_t *OpenAPI_port_range_create(
    int start,
    int end
);
void OpenAPI_port_range_free(OpenAPI_port_range_t *port_range);
OpenAPI_port_range_t *OpenAPI_port_range_parseFromJSON(cJSON *port_rangeJSON);
cJSON *OpenAPI_port_range_convertToJSON(OpenAPI_port_range_t *port_range);
OpenAPI_port_range_t *OpenAPI_port_range_copy(OpenAPI_port_range_t *dst, OpenAPI_port_range_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_port_range_H_ */

