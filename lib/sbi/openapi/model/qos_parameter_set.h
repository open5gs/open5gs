/*
 * qos_parameter_set.h
 *
 * Contains the QoS requirements expressed as one or more individual QoS parameters. 
 */

#ifndef _OpenAPI_qos_parameter_set_H_
#define _OpenAPI_qos_parameter_set_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_qos_parameter_set_s OpenAPI_qos_parameter_set_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_qos_parameter_set_s {
    bool is_ext_max_burst_size;
    int ext_max_burst_size;
    char *gfbr_dl;
    char *gfbr_ul;
    char *max_bit_rate_dl;
    char *max_bit_rate_ul;
    bool is_max_burst_size;
    int max_burst_size;
    bool is_pdb;
    int pdb;
    char *per;
    bool is_prior_level;
    int prior_level;
};

OpenAPI_qos_parameter_set_t *OpenAPI_qos_parameter_set_create(
    bool is_ext_max_burst_size,
    int ext_max_burst_size,
    char *gfbr_dl,
    char *gfbr_ul,
    char *max_bit_rate_dl,
    char *max_bit_rate_ul,
    bool is_max_burst_size,
    int max_burst_size,
    bool is_pdb,
    int pdb,
    char *per,
    bool is_prior_level,
    int prior_level
);
void OpenAPI_qos_parameter_set_free(OpenAPI_qos_parameter_set_t *qos_parameter_set);
OpenAPI_qos_parameter_set_t *OpenAPI_qos_parameter_set_parseFromJSON(cJSON *qos_parameter_setJSON);
cJSON *OpenAPI_qos_parameter_set_convertToJSON(OpenAPI_qos_parameter_set_t *qos_parameter_set);
OpenAPI_qos_parameter_set_t *OpenAPI_qos_parameter_set_copy(OpenAPI_qos_parameter_set_t *dst, OpenAPI_qos_parameter_set_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_qos_parameter_set_H_ */

