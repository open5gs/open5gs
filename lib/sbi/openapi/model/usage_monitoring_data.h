/*
 * usage_monitoring_data.h
 *
 *
 */

#ifndef _OpenAPI_usage_monitoring_data_H_
#define _OpenAPI_usage_monitoring_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_usage_monitoring_data_s OpenAPI_usage_monitoring_data_t;
typedef struct OpenAPI_usage_monitoring_data_s {
    char *um_id;
    long volume_threshold;
    long volume_threshold_uplink;
    long volume_threshold_downlink;
    int time_threshold;
    char *monitoring_time;
    long next_vol_threshold;
    long next_vol_threshold_uplink;
    long next_vol_threshold_downlink;
    int next_time_threshold;
    int inactivity_time;
    OpenAPI_list_t *ex_usage_pcc_rule_ids;
} OpenAPI_usage_monitoring_data_t;

OpenAPI_usage_monitoring_data_t *OpenAPI_usage_monitoring_data_create(
    char *um_id,
    long volume_threshold,
    long volume_threshold_uplink,
    long volume_threshold_downlink,
    int time_threshold,
    char *monitoring_time,
    long next_vol_threshold,
    long next_vol_threshold_uplink,
    long next_vol_threshold_downlink,
    int next_time_threshold,
    int inactivity_time,
    OpenAPI_list_t *ex_usage_pcc_rule_ids
    );
void OpenAPI_usage_monitoring_data_free(OpenAPI_usage_monitoring_data_t *usage_monitoring_data);
OpenAPI_usage_monitoring_data_t *OpenAPI_usage_monitoring_data_parseFromJSON(cJSON *usage_monitoring_dataJSON);
cJSON *OpenAPI_usage_monitoring_data_convertToJSON(OpenAPI_usage_monitoring_data_t *usage_monitoring_data);
OpenAPI_usage_monitoring_data_t *OpenAPI_usage_monitoring_data_copy(OpenAPI_usage_monitoring_data_t *dst, OpenAPI_usage_monitoring_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_usage_monitoring_data_H_ */

