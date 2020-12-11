/*
 * port_management_container.h
 *
 *
 */

#ifndef _OpenAPI_port_management_container_H_
#define _OpenAPI_port_management_container_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "tsn_port_identifier.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_port_management_container_s OpenAPI_port_management_container_t;
typedef struct OpenAPI_port_management_container_s {
    char port_man_cont;
    struct OpenAPI_tsn_port_identifier_s *port_identifier;
} OpenAPI_port_management_container_t;

OpenAPI_port_management_container_t *OpenAPI_port_management_container_create(
    char port_man_cont,
    OpenAPI_tsn_port_identifier_t *port_identifier
    );
void OpenAPI_port_management_container_free(OpenAPI_port_management_container_t *port_management_container);
OpenAPI_port_management_container_t *OpenAPI_port_management_container_parseFromJSON(cJSON *port_management_containerJSON);
cJSON *OpenAPI_port_management_container_convertToJSON(OpenAPI_port_management_container_t *port_management_container);
OpenAPI_port_management_container_t *OpenAPI_port_management_container_copy(OpenAPI_port_management_container_t *dst, OpenAPI_port_management_container_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_port_management_container_H_ */

