/*
 * inter_freq_target_info.h
 *
 * Indicates the Inter Frequency Target information.
 */

#ifndef _OpenAPI_inter_freq_target_info_H_
#define _OpenAPI_inter_freq_target_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_inter_freq_target_info_s OpenAPI_inter_freq_target_info_t;
typedef struct OpenAPI_inter_freq_target_info_s {
    int dl_carrier_freq;
    OpenAPI_list_t *cell_id_list;
} OpenAPI_inter_freq_target_info_t;

OpenAPI_inter_freq_target_info_t *OpenAPI_inter_freq_target_info_create(
    int dl_carrier_freq,
    OpenAPI_list_t *cell_id_list
);
void OpenAPI_inter_freq_target_info_free(OpenAPI_inter_freq_target_info_t *inter_freq_target_info);
OpenAPI_inter_freq_target_info_t *OpenAPI_inter_freq_target_info_parseFromJSON(cJSON *inter_freq_target_infoJSON);
cJSON *OpenAPI_inter_freq_target_info_convertToJSON(OpenAPI_inter_freq_target_info_t *inter_freq_target_info);
OpenAPI_inter_freq_target_info_t *OpenAPI_inter_freq_target_info_copy(OpenAPI_inter_freq_target_info_t *dst, OpenAPI_inter_freq_target_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_inter_freq_target_info_H_ */

