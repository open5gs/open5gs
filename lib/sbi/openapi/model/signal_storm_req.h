/*
 * signal_storm_req.h
 *
 * The signalling storm analytics requirement information.
 */

#ifndef _OpenAPI_signal_storm_req_H_
#define _OpenAPI_signal_storm_req_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_signal_storm_req_s OpenAPI_signal_storm_req_t;
#include "target_cause_id.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_signal_storm_req_s {
    OpenAPI_list_t *tgt_nf_instance_ids;
    OpenAPI_list_t *tgt_nf_set_ids;
    OpenAPI_list_t *int_group_ids;
    OpenAPI_list_t *exter_group_ids;
    OpenAPI_list_t *supis;
    OpenAPI_list_t *gpsis;
    OpenAPI_list_t *tgt_cause_ids;
    bool is_thr_period;
    int thr_period;
    bool is_sig_freq_thr;
    int sig_freq_thr;
    bool is_ue_req_thr;
    int ue_req_thr;
    bool is_ue_num_thr;
    int ue_num_thr;
};

OpenAPI_signal_storm_req_t *OpenAPI_signal_storm_req_create(
    OpenAPI_list_t *tgt_nf_instance_ids,
    OpenAPI_list_t *tgt_nf_set_ids,
    OpenAPI_list_t *int_group_ids,
    OpenAPI_list_t *exter_group_ids,
    OpenAPI_list_t *supis,
    OpenAPI_list_t *gpsis,
    OpenAPI_list_t *tgt_cause_ids,
    bool is_thr_period,
    int thr_period,
    bool is_sig_freq_thr,
    int sig_freq_thr,
    bool is_ue_req_thr,
    int ue_req_thr,
    bool is_ue_num_thr,
    int ue_num_thr
);
void OpenAPI_signal_storm_req_free(OpenAPI_signal_storm_req_t *signal_storm_req);
OpenAPI_signal_storm_req_t *OpenAPI_signal_storm_req_parseFromJSON(cJSON *signal_storm_reqJSON);
cJSON *OpenAPI_signal_storm_req_convertToJSON(OpenAPI_signal_storm_req_t *signal_storm_req);
OpenAPI_signal_storm_req_t *OpenAPI_signal_storm_req_copy(OpenAPI_signal_storm_req_t *dst, OpenAPI_signal_storm_req_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_signal_storm_req_H_ */

