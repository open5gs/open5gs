/*
 * mo_exp_data_counter.h
 *
 *
 */

#ifndef _OpenAPI_mo_exp_data_counter_H_
#define _OpenAPI_mo_exp_data_counter_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_mo_exp_data_counter_s OpenAPI_mo_exp_data_counter_t;
typedef struct OpenAPI_mo_exp_data_counter_s {
    int counter;
    char *time_stamp;
} OpenAPI_mo_exp_data_counter_t;

OpenAPI_mo_exp_data_counter_t *OpenAPI_mo_exp_data_counter_create(
    int counter,
    char *time_stamp
    );
void OpenAPI_mo_exp_data_counter_free(OpenAPI_mo_exp_data_counter_t *mo_exp_data_counter);
OpenAPI_mo_exp_data_counter_t *OpenAPI_mo_exp_data_counter_parseFromJSON(cJSON *mo_exp_data_counterJSON);
cJSON *OpenAPI_mo_exp_data_counter_convertToJSON(OpenAPI_mo_exp_data_counter_t *mo_exp_data_counter);
OpenAPI_mo_exp_data_counter_t *OpenAPI_mo_exp_data_counter_copy(OpenAPI_mo_exp_data_counter_t *dst, OpenAPI_mo_exp_data_counter_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_mo_exp_data_counter_H_ */

