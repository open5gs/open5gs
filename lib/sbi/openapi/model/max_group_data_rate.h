/*
 * max_group_data_rate.h
 *
 * The limits of the total bit rate across all sessions of the 5G VN group (uplink and downlink) 
 */

#ifndef _OpenAPI_max_group_data_rate_H_
#define _OpenAPI_max_group_data_rate_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_max_group_data_rate_s OpenAPI_max_group_data_rate_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_max_group_data_rate_s {
    char *uplink;
    char *downlink;
};

OpenAPI_max_group_data_rate_t *OpenAPI_max_group_data_rate_create(
    char *uplink,
    char *downlink
);
void OpenAPI_max_group_data_rate_free(OpenAPI_max_group_data_rate_t *max_group_data_rate);
OpenAPI_max_group_data_rate_t *OpenAPI_max_group_data_rate_parseFromJSON(cJSON *max_group_data_rateJSON);
cJSON *OpenAPI_max_group_data_rate_convertToJSON(OpenAPI_max_group_data_rate_t *max_group_data_rate);
OpenAPI_max_group_data_rate_t *OpenAPI_max_group_data_rate_copy(OpenAPI_max_group_data_rate_t *dst, OpenAPI_max_group_data_rate_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_max_group_data_rate_H_ */

