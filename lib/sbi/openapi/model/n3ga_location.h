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
#include "line_type.h"
#include "tai.h"
#include "tnap_id.h"
#include "twap_id.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_n3ga_location_s OpenAPI_n3ga_location_t;
typedef struct OpenAPI_n3ga_location_s {
    struct OpenAPI_tai_s *n3gpp_tai;
    char *n3_iwf_id;
    char *ue_ipv4_addr;
    char *ue_ipv6_addr;
    bool is_port_number;
    int port_number;
    struct OpenAPI_tnap_id_s *tnap_id;
    struct OpenAPI_twap_id_s *twap_id;
    struct OpenAPI_hfc_node_id_s *hfc_node_id;
    char *gli;
    OpenAPI_line_type_e w5gban_line_type;
    char *gci;
} OpenAPI_n3ga_location_t;

OpenAPI_n3ga_location_t *OpenAPI_n3ga_location_create(
    OpenAPI_tai_t *n3gpp_tai,
    char *n3_iwf_id,
    char *ue_ipv4_addr,
    char *ue_ipv6_addr,
    bool is_port_number,
    int port_number,
    OpenAPI_tnap_id_t *tnap_id,
    OpenAPI_twap_id_t *twap_id,
    OpenAPI_hfc_node_id_t *hfc_node_id,
    char *gli,
    OpenAPI_line_type_e w5gban_line_type,
    char *gci
);
void OpenAPI_n3ga_location_free(OpenAPI_n3ga_location_t *n3ga_location);
OpenAPI_n3ga_location_t *OpenAPI_n3ga_location_parseFromJSON(cJSON *n3ga_locationJSON);
cJSON *OpenAPI_n3ga_location_convertToJSON(OpenAPI_n3ga_location_t *n3ga_location);
OpenAPI_n3ga_location_t *OpenAPI_n3ga_location_copy(OpenAPI_n3ga_location_t *dst, OpenAPI_n3ga_location_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_n3ga_location_H_ */

