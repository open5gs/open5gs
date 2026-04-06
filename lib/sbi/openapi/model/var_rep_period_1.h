/*
 * var_rep_period_1.h
 *
 * Indicates the Variable reporting periodicity for event reporting 
 */

#ifndef _OpenAPI_var_rep_period_1_H_
#define _OpenAPI_var_rep_period_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_var_rep_period_1_s OpenAPI_var_rep_period_1_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_var_rep_period_1_s {
    int rep_period;
    bool is_perc_value_nf_load;
    int perc_value_nf_load;
};

OpenAPI_var_rep_period_1_t *OpenAPI_var_rep_period_1_create(
    int rep_period,
    bool is_perc_value_nf_load,
    int perc_value_nf_load
);
void OpenAPI_var_rep_period_1_free(OpenAPI_var_rep_period_1_t *var_rep_period_1);
OpenAPI_var_rep_period_1_t *OpenAPI_var_rep_period_1_parseFromJSON(cJSON *var_rep_period_1JSON);
cJSON *OpenAPI_var_rep_period_1_convertToJSON(OpenAPI_var_rep_period_1_t *var_rep_period_1);
OpenAPI_var_rep_period_1_t *OpenAPI_var_rep_period_1_copy(OpenAPI_var_rep_period_1_t *dst, OpenAPI_var_rep_period_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_var_rep_period_1_H_ */

