/*
 * signal_storm_info.h
 *
 * The signalling storm analytics information.
 */

#ifndef _OpenAPI_signal_storm_info_H_
#define _OpenAPI_signal_storm_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_signal_storm_info_s OpenAPI_signal_storm_info_t;
#include "signal_info.h"
#include "target_cause_id.h"
#include "timer_info.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_signal_storm_info_s {
    OpenAPI_list_t *tgt_nf_instance_ids;
    OpenAPI_list_t *tgt_nf_set_ids;
    OpenAPI_list_t *src_int_group_ids;
    OpenAPI_list_t *src_exter_group_ids;
    OpenAPI_list_t *src_supis;
    OpenAPI_list_t *src_gpsis;
    OpenAPI_list_t *sig_storm_causes;
    OpenAPI_list_t *src_nf_instance_ids;
    OpenAPI_list_t *src_nf_set_ids;
    OpenAPI_list_t *sig_info;
    OpenAPI_list_t *timer_info;
    bool is_priority;
    int priority;
    bool is_capacity;
    int capacity;
    bool is_confidence;
    int confidence;
};

OpenAPI_signal_storm_info_t *OpenAPI_signal_storm_info_create(
    OpenAPI_list_t *tgt_nf_instance_ids,
    OpenAPI_list_t *tgt_nf_set_ids,
    OpenAPI_list_t *src_int_group_ids,
    OpenAPI_list_t *src_exter_group_ids,
    OpenAPI_list_t *src_supis,
    OpenAPI_list_t *src_gpsis,
    OpenAPI_list_t *sig_storm_causes,
    OpenAPI_list_t *src_nf_instance_ids,
    OpenAPI_list_t *src_nf_set_ids,
    OpenAPI_list_t *sig_info,
    OpenAPI_list_t *timer_info,
    bool is_priority,
    int priority,
    bool is_capacity,
    int capacity,
    bool is_confidence,
    int confidence
);
void OpenAPI_signal_storm_info_free(OpenAPI_signal_storm_info_t *signal_storm_info);
OpenAPI_signal_storm_info_t *OpenAPI_signal_storm_info_parseFromJSON(cJSON *signal_storm_infoJSON);
cJSON *OpenAPI_signal_storm_info_convertToJSON(OpenAPI_signal_storm_info_t *signal_storm_info);
OpenAPI_signal_storm_info_t *OpenAPI_signal_storm_info_copy(OpenAPI_signal_storm_info_t *dst, OpenAPI_signal_storm_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_signal_storm_info_H_ */

