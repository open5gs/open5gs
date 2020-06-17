/*
 * configuration_parameters_eutra.h
 *
 *
 */

#ifndef _OpenAPI_configuration_parameters_eutra_H_
#define _OpenAPI_configuration_parameters_eutra_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "pppp_to_pdb.h"
#include "service_id_to_frequency.h"
#include "service_id_to_layer2_id.h"
#include "service_id_to_pppr.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_configuration_parameters_eutra_s OpenAPI_configuration_parameters_eutra_t;
typedef struct OpenAPI_configuration_parameters_eutra_s {
    OpenAPI_list_t *ser_to_layer2_ids;
    OpenAPI_list_t *pppp_to_pdbs;
    OpenAPI_list_t *ser_id_to_frequs;
    OpenAPI_list_t *ser_id_to_ppprs;
} OpenAPI_configuration_parameters_eutra_t;

OpenAPI_configuration_parameters_eutra_t *OpenAPI_configuration_parameters_eutra_create(
    OpenAPI_list_t *ser_to_layer2_ids,
    OpenAPI_list_t *pppp_to_pdbs,
    OpenAPI_list_t *ser_id_to_frequs,
    OpenAPI_list_t *ser_id_to_ppprs
    );
void OpenAPI_configuration_parameters_eutra_free(OpenAPI_configuration_parameters_eutra_t *configuration_parameters_eutra);
OpenAPI_configuration_parameters_eutra_t *OpenAPI_configuration_parameters_eutra_parseFromJSON(cJSON *configuration_parameters_eutraJSON);
cJSON *OpenAPI_configuration_parameters_eutra_convertToJSON(OpenAPI_configuration_parameters_eutra_t *configuration_parameters_eutra);
OpenAPI_configuration_parameters_eutra_t *OpenAPI_configuration_parameters_eutra_copy(OpenAPI_configuration_parameters_eutra_t *dst, OpenAPI_configuration_parameters_eutra_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_configuration_parameters_eutra_H_ */

