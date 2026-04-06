/*
 * qos_requirements.h
 *
 * Represents QoS requirements.
 */

#ifndef _OpenAPI_qos_requirements_H_
#define _OpenAPI_qos_requirements_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_qos_requirements_s OpenAPI_qos_requirements_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_qos_requirements_s {
    char *mar_bw_ul;
    char *mar_bw_dl;
    char *mir_bw_ul;
    char *mir_bw_dl;
    bool is_max_burst_size;
    int max_burst_size;
    bool is_ext_max_burst_size;
    int ext_max_burst_size;
    bool is_pdb;
    int pdb;
    char *per;
    bool is_prior_level;
    int prior_level;
};

OpenAPI_qos_requirements_t *OpenAPI_qos_requirements_create(
    char *mar_bw_ul,
    char *mar_bw_dl,
    char *mir_bw_ul,
    char *mir_bw_dl,
    bool is_max_burst_size,
    int max_burst_size,
    bool is_ext_max_burst_size,
    int ext_max_burst_size,
    bool is_pdb,
    int pdb,
    char *per,
    bool is_prior_level,
    int prior_level
);
void OpenAPI_qos_requirements_free(OpenAPI_qos_requirements_t *qos_requirements);
OpenAPI_qos_requirements_t *OpenAPI_qos_requirements_parseFromJSON(cJSON *qos_requirementsJSON);
cJSON *OpenAPI_qos_requirements_convertToJSON(OpenAPI_qos_requirements_t *qos_requirements);
OpenAPI_qos_requirements_t *OpenAPI_qos_requirements_copy(OpenAPI_qos_requirements_t *dst, OpenAPI_qos_requirements_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_qos_requirements_H_ */

