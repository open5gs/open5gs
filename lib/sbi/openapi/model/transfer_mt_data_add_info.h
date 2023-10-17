/*
 * transfer_mt_data_add_info.h
 *
 * Transfer MT Data Error Response Additional Information
 */

#ifndef _OpenAPI_transfer_mt_data_add_info_H_
#define _OpenAPI_transfer_mt_data_add_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_transfer_mt_data_add_info_s OpenAPI_transfer_mt_data_add_info_t;
typedef struct OpenAPI_transfer_mt_data_add_info_s {
    bool is_max_waiting_time;
    int max_waiting_time;
} OpenAPI_transfer_mt_data_add_info_t;

OpenAPI_transfer_mt_data_add_info_t *OpenAPI_transfer_mt_data_add_info_create(
    bool is_max_waiting_time,
    int max_waiting_time
);
void OpenAPI_transfer_mt_data_add_info_free(OpenAPI_transfer_mt_data_add_info_t *transfer_mt_data_add_info);
OpenAPI_transfer_mt_data_add_info_t *OpenAPI_transfer_mt_data_add_info_parseFromJSON(cJSON *transfer_mt_data_add_infoJSON);
cJSON *OpenAPI_transfer_mt_data_add_info_convertToJSON(OpenAPI_transfer_mt_data_add_info_t *transfer_mt_data_add_info);
OpenAPI_transfer_mt_data_add_info_t *OpenAPI_transfer_mt_data_add_info_copy(OpenAPI_transfer_mt_data_add_info_t *dst, OpenAPI_transfer_mt_data_add_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_transfer_mt_data_add_info_H_ */

