/*
 * alt_qos_param_set.h
 *
 * Contains the alternative QoS requirements expressed as the list of individual QoS parameter sets. 
 */

#ifndef _OpenAPI_alt_qos_param_set_H_
#define _OpenAPI_alt_qos_param_set_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_alt_qos_param_set_s OpenAPI_alt_qos_param_set_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_alt_qos_param_set_s {
    char *gfbr_dl;
    char *gfbr_ul;
    bool is_pdb;
    int pdb;
    char *per;
};

OpenAPI_alt_qos_param_set_t *OpenAPI_alt_qos_param_set_create(
    char *gfbr_dl,
    char *gfbr_ul,
    bool is_pdb,
    int pdb,
    char *per
);
void OpenAPI_alt_qos_param_set_free(OpenAPI_alt_qos_param_set_t *alt_qos_param_set);
OpenAPI_alt_qos_param_set_t *OpenAPI_alt_qos_param_set_parseFromJSON(cJSON *alt_qos_param_setJSON);
cJSON *OpenAPI_alt_qos_param_set_convertToJSON(OpenAPI_alt_qos_param_set_t *alt_qos_param_set);
OpenAPI_alt_qos_param_set_t *OpenAPI_alt_qos_param_set_copy(OpenAPI_alt_qos_param_set_t *dst, OpenAPI_alt_qos_param_set_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_alt_qos_param_set_H_ */

