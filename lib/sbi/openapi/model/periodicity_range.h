/*
 * periodicity_range.h
 *
 * Contains the acceptable range (which is formulated as lower bound and upper bound of the periodicity of the start twobursts in reference to the external GM) or acceptable periodicity value(s) (which is formulated as a list of values for the periodicity). 
 */

#ifndef _OpenAPI_periodicity_range_H_
#define _OpenAPI_periodicity_range_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_periodicity_range_s OpenAPI_periodicity_range_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_periodicity_range_s {
    bool is_lower_bound;
    int lower_bound;
    bool is_upper_bound;
    int upper_bound;
    OpenAPI_list_t *periodic_vals;
    OpenAPI_list_t *add_periodic_vals;
};

OpenAPI_periodicity_range_t *OpenAPI_periodicity_range_create(
    bool is_lower_bound,
    int lower_bound,
    bool is_upper_bound,
    int upper_bound,
    OpenAPI_list_t *periodic_vals,
    OpenAPI_list_t *add_periodic_vals
);
void OpenAPI_periodicity_range_free(OpenAPI_periodicity_range_t *periodicity_range);
OpenAPI_periodicity_range_t *OpenAPI_periodicity_range_parseFromJSON(cJSON *periodicity_rangeJSON);
cJSON *OpenAPI_periodicity_range_convertToJSON(OpenAPI_periodicity_range_t *periodicity_range);
OpenAPI_periodicity_range_t *OpenAPI_periodicity_range_copy(OpenAPI_periodicity_range_t *dst, OpenAPI_periodicity_range_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_periodicity_range_H_ */

