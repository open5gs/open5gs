/*
 * alternative_service_requirements_data.h
 *
 * Contains an alternative QoS related parameter set.
 */

#ifndef _OpenAPI_alternative_service_requirements_data_H_
#define _OpenAPI_alternative_service_requirements_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_alternative_service_requirements_data_s OpenAPI_alternative_service_requirements_data_t;
typedef struct OpenAPI_alternative_service_requirements_data_s {
    char *alt_qos_param_set_ref;
    char *gbr_ul;
    char *gbr_dl;
    bool is_pdb;
    int pdb;
} OpenAPI_alternative_service_requirements_data_t;

OpenAPI_alternative_service_requirements_data_t *OpenAPI_alternative_service_requirements_data_create(
    char *alt_qos_param_set_ref,
    char *gbr_ul,
    char *gbr_dl,
    bool is_pdb,
    int pdb
);
void OpenAPI_alternative_service_requirements_data_free(OpenAPI_alternative_service_requirements_data_t *alternative_service_requirements_data);
OpenAPI_alternative_service_requirements_data_t *OpenAPI_alternative_service_requirements_data_parseFromJSON(cJSON *alternative_service_requirements_dataJSON);
cJSON *OpenAPI_alternative_service_requirements_data_convertToJSON(OpenAPI_alternative_service_requirements_data_t *alternative_service_requirements_data);
OpenAPI_alternative_service_requirements_data_t *OpenAPI_alternative_service_requirements_data_copy(OpenAPI_alternative_service_requirements_data_t *dst, OpenAPI_alternative_service_requirements_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_alternative_service_requirements_data_H_ */

