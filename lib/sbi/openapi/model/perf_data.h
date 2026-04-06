/*
 * perf_data.h
 *
 * Represents DN performance data.
 */

#ifndef _OpenAPI_perf_data_H_
#define _OpenAPI_perf_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_perf_data_s OpenAPI_perf_data_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_perf_data_s {
    char *avg_traffic_rate;
    char *max_traffic_rate;
    char *min_traffic_rate;
    char *agg_traffic_rate;
    bool is_var_traffic_rate;
    float var_traffic_rate;
    OpenAPI_list_t *traf_rate_ue_ids;
    bool is_ave_packet_delay;
    int ave_packet_delay;
    bool is_max_packet_delay;
    int max_packet_delay;
    bool is_var_packet_delay;
    float var_packet_delay;
    OpenAPI_list_t *pack_delay_ue_ids;
    bool is_avg_packet_loss_rate;
    int avg_packet_loss_rate;
    bool is_max_packet_loss_rate;
    int max_packet_loss_rate;
    bool is_var_packet_loss_rate;
    float var_packet_loss_rate;
    OpenAPI_list_t *pack_loss_ue_ids;
    bool is_num_of_ue;
    int num_of_ue;
};

OpenAPI_perf_data_t *OpenAPI_perf_data_create(
    char *avg_traffic_rate,
    char *max_traffic_rate,
    char *min_traffic_rate,
    char *agg_traffic_rate,
    bool is_var_traffic_rate,
    float var_traffic_rate,
    OpenAPI_list_t *traf_rate_ue_ids,
    bool is_ave_packet_delay,
    int ave_packet_delay,
    bool is_max_packet_delay,
    int max_packet_delay,
    bool is_var_packet_delay,
    float var_packet_delay,
    OpenAPI_list_t *pack_delay_ue_ids,
    bool is_avg_packet_loss_rate,
    int avg_packet_loss_rate,
    bool is_max_packet_loss_rate,
    int max_packet_loss_rate,
    bool is_var_packet_loss_rate,
    float var_packet_loss_rate,
    OpenAPI_list_t *pack_loss_ue_ids,
    bool is_num_of_ue,
    int num_of_ue
);
void OpenAPI_perf_data_free(OpenAPI_perf_data_t *perf_data);
OpenAPI_perf_data_t *OpenAPI_perf_data_parseFromJSON(cJSON *perf_dataJSON);
cJSON *OpenAPI_perf_data_convertToJSON(OpenAPI_perf_data_t *perf_data);
OpenAPI_perf_data_t *OpenAPI_perf_data_copy(OpenAPI_perf_data_t *dst, OpenAPI_perf_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_perf_data_H_ */

