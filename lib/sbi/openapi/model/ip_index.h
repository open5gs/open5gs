/*
 * ip_index.h
 *
 * Represents the IP Index to be sent from UDM to the SMF (its value can be either an integer or a string)
 */

#ifndef _OpenAPI_ip_index_H_
#define _OpenAPI_ip_index_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ip_index_s OpenAPI_ip_index_t;
typedef struct OpenAPI_ip_index_s {
} OpenAPI_ip_index_t;

OpenAPI_ip_index_t *OpenAPI_ip_index_create(
);
void OpenAPI_ip_index_free(OpenAPI_ip_index_t *ip_index);
OpenAPI_ip_index_t *OpenAPI_ip_index_parseFromJSON(cJSON *ip_indexJSON);
cJSON *OpenAPI_ip_index_convertToJSON(OpenAPI_ip_index_t *ip_index);
OpenAPI_ip_index_t *OpenAPI_ip_index_copy(OpenAPI_ip_index_t *dst, OpenAPI_ip_index_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ip_index_H_ */

