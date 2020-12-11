/*
 * tsn_port_identifier.h
 *
 *
 */

#ifndef _OpenAPI_tsn_port_identifier_H_
#define _OpenAPI_tsn_port_identifier_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_tsn_port_identifier_s OpenAPI_tsn_port_identifier_t;
typedef struct OpenAPI_tsn_port_identifier_s {
    char *port_mac;
    int port_number;
} OpenAPI_tsn_port_identifier_t;

OpenAPI_tsn_port_identifier_t *OpenAPI_tsn_port_identifier_create(
    char *port_mac,
    int port_number
    );
void OpenAPI_tsn_port_identifier_free(OpenAPI_tsn_port_identifier_t *tsn_port_identifier);
OpenAPI_tsn_port_identifier_t *OpenAPI_tsn_port_identifier_parseFromJSON(cJSON *tsn_port_identifierJSON);
cJSON *OpenAPI_tsn_port_identifier_convertToJSON(OpenAPI_tsn_port_identifier_t *tsn_port_identifier);
OpenAPI_tsn_port_identifier_t *OpenAPI_tsn_port_identifier_copy(OpenAPI_tsn_port_identifier_t *dst, OpenAPI_tsn_port_identifier_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_tsn_port_identifier_H_ */

