/*
 * sip_event_type.h
 *
 * 
 */

#ifndef _OpenAPI_sip_event_type_H_
#define _OpenAPI_sip_event_type_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_sip_event_type_s OpenAPI_sip_event_type_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_sip_event_type_s {
    char *s_ip_method;
    char *event_header;
    bool is_expires_header;
    int expires_header;
};

OpenAPI_sip_event_type_t *OpenAPI_sip_event_type_create(
    char *s_ip_method,
    char *event_header,
    bool is_expires_header,
    int expires_header
);
void OpenAPI_sip_event_type_free(OpenAPI_sip_event_type_t *sip_event_type);
OpenAPI_sip_event_type_t *OpenAPI_sip_event_type_parseFromJSON(cJSON *sip_event_typeJSON);
cJSON *OpenAPI_sip_event_type_convertToJSON(OpenAPI_sip_event_type_t *sip_event_type);
OpenAPI_sip_event_type_t *OpenAPI_sip_event_type_copy(OpenAPI_sip_event_type_t *dst, OpenAPI_sip_event_type_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sip_event_type_H_ */

