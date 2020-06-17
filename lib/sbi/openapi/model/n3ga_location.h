/*
 * n3ga_location.h
 *
 *
 */

#ifndef _OpenAPI_n3ga_location_H_
#define _OpenAPI_n3ga_location_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "hfc_node_id.h"
#include "tai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_n3ga_location_s OpenAPI_n3ga_location_t;
typedef struct OpenAPI_n3ga_location_s {
    struct OpenAPI_tai_s *n3gpp_tai;
    char *n3_iwf_id;
    char *ue_ipv4_addr;
    char *ue_ipv6_addr;
    int port_number;
    char *ss_id;
    char *bss_id;
    char civic_address;
    struct OpenAPI_hfc_node_id_s *hfc_node_id;
    char gli;
} OpenAPI_n3ga_location_t;

OpenAPI_n3ga_location_t *OpenAPI_n3ga_location_create(
    OpenAPI_tai_t *n3gpp_tai,
    char *n3_iwf_id,
    char *ue_ipv4_addr,
    char *ue_ipv6_addr,
    int port_number,
    char *ss_id,
    char *bss_id,
    char civic_address,
    OpenAPI_hfc_node_id_t *hfc_node_id,
    char gli
    );
void OpenAPI_n3ga_location_free(OpenAPI_n3ga_location_t *n3ga_location);
OpenAPI_n3ga_location_t *OpenAPI_n3ga_location_parseFromJSON(cJSON *n3ga_locationJSON);
cJSON *OpenAPI_n3ga_location_convertToJSON(OpenAPI_n3ga_location_t *n3ga_location);
OpenAPI_n3ga_location_t *OpenAPI_n3ga_location_copy(OpenAPI_n3ga_location_t *dst, OpenAPI_n3ga_location_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_n3ga_location_H_ */

