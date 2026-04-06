/*
 * accuracy_info.h
 *
 * The analytics accuracy information.
 */

#ifndef _OpenAPI_accuracy_info_H_
#define _OpenAPI_accuracy_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_accuracy_info_s OpenAPI_accuracy_info_t;
#include "analytics_accuracy_indication.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_accuracy_info_s {
    int accuracy_val;
    bool is_accu_sample_nbr;
    int accu_sample_nbr;
    OpenAPI_analytics_accuracy_indication_e ana_accu_ind;
};

OpenAPI_accuracy_info_t *OpenAPI_accuracy_info_create(
    int accuracy_val,
    bool is_accu_sample_nbr,
    int accu_sample_nbr,
    OpenAPI_analytics_accuracy_indication_e ana_accu_ind
);
void OpenAPI_accuracy_info_free(OpenAPI_accuracy_info_t *accuracy_info);
OpenAPI_accuracy_info_t *OpenAPI_accuracy_info_parseFromJSON(cJSON *accuracy_infoJSON);
cJSON *OpenAPI_accuracy_info_convertToJSON(OpenAPI_accuracy_info_t *accuracy_info);
OpenAPI_accuracy_info_t *OpenAPI_accuracy_info_copy(OpenAPI_accuracy_info_t *dst, OpenAPI_accuracy_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_accuracy_info_H_ */

