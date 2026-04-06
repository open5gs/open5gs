/*
 * td_traffic.h
 *
 * Represents traffic that matches or unmatches Traffic Descriptor of URSP rule.
 */

#ifndef _OpenAPI_td_traffic_H_
#define _OpenAPI_td_traffic_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_td_traffic_s OpenAPI_td_traffic_t;
#include "pdu_ses_traffic_req.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_td_traffic_s {
    OpenAPI_list_t *pdu_ses_traf_reqs;
    bool is_ul_vol;
    long ul_vol;
    bool is_dl_vol;
    long dl_vol;
    bool is_all_vol;
    long all_vol;
    bool is_ul_num_of_pkt;
    int ul_num_of_pkt;
    bool is_dl_num_of_pkt;
    int dl_num_of_pkt;
    bool is_all_num_of_pkt;
    int all_num_of_pkt;
};

OpenAPI_td_traffic_t *OpenAPI_td_traffic_create(
    OpenAPI_list_t *pdu_ses_traf_reqs,
    bool is_ul_vol,
    long ul_vol,
    bool is_dl_vol,
    long dl_vol,
    bool is_all_vol,
    long all_vol,
    bool is_ul_num_of_pkt,
    int ul_num_of_pkt,
    bool is_dl_num_of_pkt,
    int dl_num_of_pkt,
    bool is_all_num_of_pkt,
    int all_num_of_pkt
);
void OpenAPI_td_traffic_free(OpenAPI_td_traffic_t *td_traffic);
OpenAPI_td_traffic_t *OpenAPI_td_traffic_parseFromJSON(cJSON *td_trafficJSON);
cJSON *OpenAPI_td_traffic_convertToJSON(OpenAPI_td_traffic_t *td_traffic);
OpenAPI_td_traffic_t *OpenAPI_td_traffic_copy(OpenAPI_td_traffic_t *dst, OpenAPI_td_traffic_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_td_traffic_H_ */

