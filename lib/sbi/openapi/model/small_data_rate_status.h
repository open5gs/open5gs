/*
 * small_data_rate_status.h
 *
 *
 */

#ifndef _OpenAPI_small_data_rate_status_H_
#define _OpenAPI_small_data_rate_status_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_small_data_rate_status_s OpenAPI_small_data_rate_status_t;
typedef struct OpenAPI_small_data_rate_status_s {
    int remain_packets_ul;
    int remain_packets_dl;
    char *validity_time;
    int remain_ex_reports_ul;
    int remain_ex_reports_dl;
} OpenAPI_small_data_rate_status_t;

OpenAPI_small_data_rate_status_t *OpenAPI_small_data_rate_status_create(
    int remain_packets_ul,
    int remain_packets_dl,
    char *validity_time,
    int remain_ex_reports_ul,
    int remain_ex_reports_dl
    );
void OpenAPI_small_data_rate_status_free(OpenAPI_small_data_rate_status_t *small_data_rate_status);
OpenAPI_small_data_rate_status_t *OpenAPI_small_data_rate_status_parseFromJSON(cJSON *small_data_rate_statusJSON);
cJSON *OpenAPI_small_data_rate_status_convertToJSON(OpenAPI_small_data_rate_status_t *small_data_rate_status);
OpenAPI_small_data_rate_status_t *OpenAPI_small_data_rate_status_copy(OpenAPI_small_data_rate_status_t *dst, OpenAPI_small_data_rate_status_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_small_data_rate_status_H_ */

