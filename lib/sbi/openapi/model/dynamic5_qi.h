/*
 * dynamic5_qi.h
 *
 *
 */

#ifndef _OpenAPI_dynamic5_qi_H_
#define _OpenAPI_dynamic5_qi_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "qos_resource_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_dynamic5_qi_s OpenAPI_dynamic5_qi_t;
typedef struct OpenAPI_dynamic5_qi_s {
    struct OpenAPI_qos_resource_type_s *resource_type;
    int priority_level;
    int packet_delay_budget;
    char *packet_err_rate;
    int aver_window;
    int max_data_burst_vol;
    int ext_max_data_burst_vol;
} OpenAPI_dynamic5_qi_t;

OpenAPI_dynamic5_qi_t *OpenAPI_dynamic5_qi_create(
    OpenAPI_qos_resource_type_t *resource_type,
    int priority_level,
    int packet_delay_budget,
    char *packet_err_rate,
    int aver_window,
    int max_data_burst_vol,
    int ext_max_data_burst_vol
    );
void OpenAPI_dynamic5_qi_free(OpenAPI_dynamic5_qi_t *dynamic5_qi);
OpenAPI_dynamic5_qi_t *OpenAPI_dynamic5_qi_parseFromJSON(cJSON *dynamic5_qiJSON);
cJSON *OpenAPI_dynamic5_qi_convertToJSON(OpenAPI_dynamic5_qi_t *dynamic5_qi);
OpenAPI_dynamic5_qi_t *OpenAPI_dynamic5_qi_copy(OpenAPI_dynamic5_qi_t *dst, OpenAPI_dynamic5_qi_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_dynamic5_qi_H_ */

