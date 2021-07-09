/*
 * inter_freq_target_info_1.h
 *
 * 
 */

#ifndef _OpenAPI_inter_freq_target_info_1_H_
#define _OpenAPI_inter_freq_target_info_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_inter_freq_target_info_1_s OpenAPI_inter_freq_target_info_1_t;
typedef struct OpenAPI_inter_freq_target_info_1_s {
    int dl_carrier_freq;
    OpenAPI_list_t *cell_id_list;
} OpenAPI_inter_freq_target_info_1_t;

OpenAPI_inter_freq_target_info_1_t *OpenAPI_inter_freq_target_info_1_create(
    int dl_carrier_freq,
    OpenAPI_list_t *cell_id_list
);
void OpenAPI_inter_freq_target_info_1_free(OpenAPI_inter_freq_target_info_1_t *inter_freq_target_info_1);
OpenAPI_inter_freq_target_info_1_t *OpenAPI_inter_freq_target_info_1_parseFromJSON(cJSON *inter_freq_target_info_1JSON);
cJSON *OpenAPI_inter_freq_target_info_1_convertToJSON(OpenAPI_inter_freq_target_info_1_t *inter_freq_target_info_1);
OpenAPI_inter_freq_target_info_1_t *OpenAPI_inter_freq_target_info_1_copy(OpenAPI_inter_freq_target_info_1_t *dst, OpenAPI_inter_freq_target_info_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_inter_freq_target_info_1_H_ */

