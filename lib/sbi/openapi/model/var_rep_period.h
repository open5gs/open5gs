/*
 * var_rep_period.h
 *
 * Indicates the Variable reporting periodicity for event reporting 
 */

#ifndef _OpenAPI_var_rep_period_H_
#define _OpenAPI_var_rep_period_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_var_rep_period_s OpenAPI_var_rep_period_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_var_rep_period_s {
    int rep_period;
    bool is_perc_value_nf_load;
    int perc_value_nf_load;
};

OpenAPI_var_rep_period_t *OpenAPI_var_rep_period_create(
    int rep_period,
    bool is_perc_value_nf_load,
    int perc_value_nf_load
);
void OpenAPI_var_rep_period_free(OpenAPI_var_rep_period_t *var_rep_period);
OpenAPI_var_rep_period_t *OpenAPI_var_rep_period_parseFromJSON(cJSON *var_rep_periodJSON);
cJSON *OpenAPI_var_rep_period_convertToJSON(OpenAPI_var_rep_period_t *var_rep_period);
OpenAPI_var_rep_period_t *OpenAPI_var_rep_period_copy(OpenAPI_var_rep_period_t *dst, OpenAPI_var_rep_period_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_var_rep_period_H_ */

