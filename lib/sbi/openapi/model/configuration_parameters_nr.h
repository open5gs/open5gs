/*
 * configuration_parameters_nr.h
 *
 *
 */

#ifndef _OpenAPI_configuration_parameters_nr_H_
#define _OpenAPI_configuration_parameters_nr_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "pc5_qos_mapping.h"
#include "service_id_to_frequency.h"
#include "service_id_to_layer2_id.h"
#include "slrb_configurations.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_configuration_parameters_nr_s OpenAPI_configuration_parameters_nr_t;
typedef struct OpenAPI_configuration_parameters_nr_s {
    OpenAPI_list_t *ser_to_bro_layer2_ids;
    OpenAPI_list_t *ser_to_gro_layer2_ids;
    OpenAPI_list_t *ser_to_def_layer2_ids;
    OpenAPI_list_t *ser_id_to_frequs;
    OpenAPI_list_t *pc5_qos_mappings;
    OpenAPI_list_t *slrb_configs;
} OpenAPI_configuration_parameters_nr_t;

OpenAPI_configuration_parameters_nr_t *OpenAPI_configuration_parameters_nr_create(
    OpenAPI_list_t *ser_to_bro_layer2_ids,
    OpenAPI_list_t *ser_to_gro_layer2_ids,
    OpenAPI_list_t *ser_to_def_layer2_ids,
    OpenAPI_list_t *ser_id_to_frequs,
    OpenAPI_list_t *pc5_qos_mappings,
    OpenAPI_list_t *slrb_configs
    );
void OpenAPI_configuration_parameters_nr_free(OpenAPI_configuration_parameters_nr_t *configuration_parameters_nr);
OpenAPI_configuration_parameters_nr_t *OpenAPI_configuration_parameters_nr_parseFromJSON(cJSON *configuration_parameters_nrJSON);
cJSON *OpenAPI_configuration_parameters_nr_convertToJSON(OpenAPI_configuration_parameters_nr_t *configuration_parameters_nr);
OpenAPI_configuration_parameters_nr_t *OpenAPI_configuration_parameters_nr_copy(OpenAPI_configuration_parameters_nr_t *dst, OpenAPI_configuration_parameters_nr_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_configuration_parameters_nr_H_ */

