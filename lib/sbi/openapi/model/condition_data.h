/*
 * condition_data.h
 *
 * Contains conditions of applicability for a rule.
 */

#ifndef _OpenAPI_condition_data_H_
#define _OpenAPI_condition_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "access_type.h"
#include "rat_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_condition_data_s OpenAPI_condition_data_t;
typedef struct OpenAPI_condition_data_s {
    char *cond_id;
    bool is_activation_time_null;
    char *activation_time;
    bool is_deactivation_time_null;
    char *deactivation_time;
    OpenAPI_access_type_e access_type;
    OpenAPI_rat_type_e rat_type;
} OpenAPI_condition_data_t;

OpenAPI_condition_data_t *OpenAPI_condition_data_create(
    char *cond_id,
    bool is_activation_time_null,
    char *activation_time,
    bool is_deactivation_time_null,
    char *deactivation_time,
    OpenAPI_access_type_e access_type,
    OpenAPI_rat_type_e rat_type
);
void OpenAPI_condition_data_free(OpenAPI_condition_data_t *condition_data);
OpenAPI_condition_data_t *OpenAPI_condition_data_parseFromJSON(cJSON *condition_dataJSON);
cJSON *OpenAPI_condition_data_convertToJSON(OpenAPI_condition_data_t *condition_data);
OpenAPI_condition_data_t *OpenAPI_condition_data_copy(OpenAPI_condition_data_t *dst, OpenAPI_condition_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_condition_data_H_ */

