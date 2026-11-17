/*
 * transmitter_info.h
 *
 * 
 */

#ifndef _OpenAPI_transmitter_info_H_
#define _OpenAPI_transmitter_info_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_transmitter_info_s OpenAPI_transmitter_info_t;
#include "ip_addr.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_transmitter_info_s {
    struct OpenAPI_ip_addr_s *prose_source_ip_address;
    char *prose_source_l2_id;
};

OpenAPI_transmitter_info_t *OpenAPI_transmitter_info_create(
    OpenAPI_ip_addr_t *prose_source_ip_address,
    char *prose_source_l2_id
);
void OpenAPI_transmitter_info_free(OpenAPI_transmitter_info_t *transmitter_info);
OpenAPI_transmitter_info_t *OpenAPI_transmitter_info_parseFromJSON(cJSON *transmitter_infoJSON);
cJSON *OpenAPI_transmitter_info_convertToJSON(OpenAPI_transmitter_info_t *transmitter_info);
OpenAPI_transmitter_info_t *OpenAPI_transmitter_info_copy(OpenAPI_transmitter_info_t *dst, OpenAPI_transmitter_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_transmitter_info_H_ */

