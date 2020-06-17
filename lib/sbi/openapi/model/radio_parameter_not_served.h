/*
 * radio_parameter_not_served.h
 *
 *
 */

#ifndef _OpenAPI_radio_parameter_not_served_H_
#define _OpenAPI_radio_parameter_not_served_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "operator_managed.h"
#include "v2x_rat_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_radio_parameter_not_served_s OpenAPI_radio_parameter_not_served_t;
typedef struct OpenAPI_radio_parameter_not_served_s {
    char *radio_params;
    struct OpenAPI_v2x_rat_type_s *rat;
    char *geographical_area;
    struct OpenAPI_operator_managed_s *oper_managed;
} OpenAPI_radio_parameter_not_served_t;

OpenAPI_radio_parameter_not_served_t *OpenAPI_radio_parameter_not_served_create(
    char *radio_params,
    OpenAPI_v2x_rat_type_t *rat,
    char *geographical_area,
    OpenAPI_operator_managed_t *oper_managed
    );
void OpenAPI_radio_parameter_not_served_free(OpenAPI_radio_parameter_not_served_t *radio_parameter_not_served);
OpenAPI_radio_parameter_not_served_t *OpenAPI_radio_parameter_not_served_parseFromJSON(cJSON *radio_parameter_not_servedJSON);
cJSON *OpenAPI_radio_parameter_not_served_convertToJSON(OpenAPI_radio_parameter_not_served_t *radio_parameter_not_served);
OpenAPI_radio_parameter_not_served_t *OpenAPI_radio_parameter_not_served_copy(OpenAPI_radio_parameter_not_served_t *dst, OpenAPI_radio_parameter_not_served_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_radio_parameter_not_served_H_ */

