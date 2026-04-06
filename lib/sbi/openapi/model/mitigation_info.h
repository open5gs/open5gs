/*
 * mitigation_info.h
 *
 * include the congestion mitigation information.
 */

#ifndef _OpenAPI_mitigation_info_H_
#define _OpenAPI_mitigation_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_mitigation_info_s OpenAPI_mitigation_info_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_mitigation_info_s {
    bool is_perc_value_num_ues;
    int perc_value_num_ues;
    bool is_new_ues_ind;
    int new_ues_ind;
};

OpenAPI_mitigation_info_t *OpenAPI_mitigation_info_create(
    bool is_perc_value_num_ues,
    int perc_value_num_ues,
    bool is_new_ues_ind,
    int new_ues_ind
);
void OpenAPI_mitigation_info_free(OpenAPI_mitigation_info_t *mitigation_info);
OpenAPI_mitigation_info_t *OpenAPI_mitigation_info_parseFromJSON(cJSON *mitigation_infoJSON);
cJSON *OpenAPI_mitigation_info_convertToJSON(OpenAPI_mitigation_info_t *mitigation_info);
OpenAPI_mitigation_info_t *OpenAPI_mitigation_info_copy(OpenAPI_mitigation_info_t *dst, OpenAPI_mitigation_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_mitigation_info_H_ */

