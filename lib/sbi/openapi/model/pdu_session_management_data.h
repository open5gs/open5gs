/*
 * pdu_session_management_data.h
 *
 * Represents Session management data for a UE and a PDU session.
 */

#ifndef _OpenAPI_pdu_session_management_data_H_
#define _OpenAPI_pdu_session_management_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "pdu_session_status.h"
#include "pdu_session_type.h"
#include "route_to_location.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pdu_session_management_data_s OpenAPI_pdu_session_management_data_t;
typedef struct OpenAPI_pdu_session_management_data_s {
    OpenAPI_pdu_session_status_e pdu_session_status;
    char *pdu_session_status_ts;
    char *dnai;
    char *dnai_ts;
    OpenAPI_list_t *n6_traffic_routing_info;
    char *n6_traffic_routing_info_ts;
    char *ipv4_addr;
    OpenAPI_list_t *ipv6_prefix;
    OpenAPI_list_t *ipv6_addrs;
    OpenAPI_pdu_session_type_e pdu_sess_type;
    char *ip_addr_ts;
    char *dnn;
    bool is_pdu_session_id;
    int pdu_session_id;
    char *supp_feat;
    OpenAPI_list_t *reset_ids;
} OpenAPI_pdu_session_management_data_t;

OpenAPI_pdu_session_management_data_t *OpenAPI_pdu_session_management_data_create(
    OpenAPI_pdu_session_status_e pdu_session_status,
    char *pdu_session_status_ts,
    char *dnai,
    char *dnai_ts,
    OpenAPI_list_t *n6_traffic_routing_info,
    char *n6_traffic_routing_info_ts,
    char *ipv4_addr,
    OpenAPI_list_t *ipv6_prefix,
    OpenAPI_list_t *ipv6_addrs,
    OpenAPI_pdu_session_type_e pdu_sess_type,
    char *ip_addr_ts,
    char *dnn,
    bool is_pdu_session_id,
    int pdu_session_id,
    char *supp_feat,
    OpenAPI_list_t *reset_ids
);
void OpenAPI_pdu_session_management_data_free(OpenAPI_pdu_session_management_data_t *pdu_session_management_data);
OpenAPI_pdu_session_management_data_t *OpenAPI_pdu_session_management_data_parseFromJSON(cJSON *pdu_session_management_dataJSON);
cJSON *OpenAPI_pdu_session_management_data_convertToJSON(OpenAPI_pdu_session_management_data_t *pdu_session_management_data);
OpenAPI_pdu_session_management_data_t *OpenAPI_pdu_session_management_data_copy(OpenAPI_pdu_session_management_data_t *dst, OpenAPI_pdu_session_management_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pdu_session_management_data_H_ */

