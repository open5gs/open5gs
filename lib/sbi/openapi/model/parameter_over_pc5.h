/*
 * parameter_over_pc5.h
 *
 *
 */

#ifndef _OpenAPI_parameter_over_pc5_H_
#define _OpenAPI_parameter_over_pc5_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "configuration_parameters_eutra.h"
#include "configuration_parameters_nr.h"
#include "plmn_rat_served.h"
#include "privacy_parameter.h"
#include "radio_parameter_not_served.h"
#include "service_to_tx.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_parameter_over_pc5_s OpenAPI_parameter_over_pc5_t;
typedef struct OpenAPI_parameter_over_pc5_s {
    char *expiry;
    OpenAPI_list_t *plmm_rat_served;
    int auth_not_served;
    OpenAPI_list_t *radio_params_not_served;
    OpenAPI_list_t *ser_to_tx;
    OpenAPI_list_t *privacy_params;
    struct OpenAPI_configuration_parameters_eutra_s *config_para_eutra;
    struct OpenAPI_configuration_parameters_nr_s *config_para_nr;
} OpenAPI_parameter_over_pc5_t;

OpenAPI_parameter_over_pc5_t *OpenAPI_parameter_over_pc5_create(
    char *expiry,
    OpenAPI_list_t *plmm_rat_served,
    int auth_not_served,
    OpenAPI_list_t *radio_params_not_served,
    OpenAPI_list_t *ser_to_tx,
    OpenAPI_list_t *privacy_params,
    OpenAPI_configuration_parameters_eutra_t *config_para_eutra,
    OpenAPI_configuration_parameters_nr_t *config_para_nr
    );
void OpenAPI_parameter_over_pc5_free(OpenAPI_parameter_over_pc5_t *parameter_over_pc5);
OpenAPI_parameter_over_pc5_t *OpenAPI_parameter_over_pc5_parseFromJSON(cJSON *parameter_over_pc5JSON);
cJSON *OpenAPI_parameter_over_pc5_convertToJSON(OpenAPI_parameter_over_pc5_t *parameter_over_pc5);
OpenAPI_parameter_over_pc5_t *OpenAPI_parameter_over_pc5_copy(OpenAPI_parameter_over_pc5_t *dst, OpenAPI_parameter_over_pc5_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_parameter_over_pc5_H_ */

