/*
 * accu_usage_report.h
 *
 * Contains the accumulated usage report information.
 */

#ifndef _OpenAPI_accu_usage_report_H_
#define _OpenAPI_accu_usage_report_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_accu_usage_report_s OpenAPI_accu_usage_report_t;
typedef struct OpenAPI_accu_usage_report_s {
    char *ref_um_ids;
    bool is_vol_usage;
    long vol_usage;
    bool is_vol_usage_uplink;
    long vol_usage_uplink;
    bool is_vol_usage_downlink;
    long vol_usage_downlink;
    bool is_time_usage;
    int time_usage;
    bool is_next_vol_usage;
    long next_vol_usage;
    bool is_next_vol_usage_uplink;
    long next_vol_usage_uplink;
    bool is_next_vol_usage_downlink;
    long next_vol_usage_downlink;
    bool is_next_time_usage;
    int next_time_usage;
} OpenAPI_accu_usage_report_t;

OpenAPI_accu_usage_report_t *OpenAPI_accu_usage_report_create(
    char *ref_um_ids,
    bool is_vol_usage,
    long vol_usage,
    bool is_vol_usage_uplink,
    long vol_usage_uplink,
    bool is_vol_usage_downlink,
    long vol_usage_downlink,
    bool is_time_usage,
    int time_usage,
    bool is_next_vol_usage,
    long next_vol_usage,
    bool is_next_vol_usage_uplink,
    long next_vol_usage_uplink,
    bool is_next_vol_usage_downlink,
    long next_vol_usage_downlink,
    bool is_next_time_usage,
    int next_time_usage
);
void OpenAPI_accu_usage_report_free(OpenAPI_accu_usage_report_t *accu_usage_report);
OpenAPI_accu_usage_report_t *OpenAPI_accu_usage_report_parseFromJSON(cJSON *accu_usage_reportJSON);
cJSON *OpenAPI_accu_usage_report_convertToJSON(OpenAPI_accu_usage_report_t *accu_usage_report);
OpenAPI_accu_usage_report_t *OpenAPI_accu_usage_report_copy(OpenAPI_accu_usage_report_t *dst, OpenAPI_accu_usage_report_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_accu_usage_report_H_ */

