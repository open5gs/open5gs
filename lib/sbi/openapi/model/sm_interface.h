/*
 * sm_interface.h
 *
 * 
 */

#ifndef _OpenAPI_sm_interface_H_
#define _OpenAPI_sm_interface_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_sm_interface_s OpenAPI_sm_interface_t;
#include "interface_type.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_sm_interface_s {
    char *interface_id;
    char *interface_text;
    char *interface_port;
    OpenAPI_interface_type_e interface_type;
};

OpenAPI_sm_interface_t *OpenAPI_sm_interface_create(
    char *interface_id,
    char *interface_text,
    char *interface_port,
    OpenAPI_interface_type_e interface_type
);
void OpenAPI_sm_interface_free(OpenAPI_sm_interface_t *sm_interface);
OpenAPI_sm_interface_t *OpenAPI_sm_interface_parseFromJSON(cJSON *sm_interfaceJSON);
cJSON *OpenAPI_sm_interface_convertToJSON(OpenAPI_sm_interface_t *sm_interface);
OpenAPI_sm_interface_t *OpenAPI_sm_interface_copy(OpenAPI_sm_interface_t *dst, OpenAPI_sm_interface_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sm_interface_H_ */

