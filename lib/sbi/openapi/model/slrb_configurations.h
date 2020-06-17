/*
 * slrb_configurations.h
 *
 *
 */

#ifndef _OpenAPI_slrb_configurations_H_
#define _OpenAPI_slrb_configurations_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "pc5_qo_s_para.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_slrb_configurations_s OpenAPI_slrb_configurations_t;
typedef struct OpenAPI_slrb_configurations_s {
    struct OpenAPI_pc5_qo_s_para_s *pc5_qos_para;
    char *slrb;
} OpenAPI_slrb_configurations_t;

OpenAPI_slrb_configurations_t *OpenAPI_slrb_configurations_create(
    OpenAPI_pc5_qo_s_para_t *pc5_qos_para,
    char *slrb
    );
void OpenAPI_slrb_configurations_free(OpenAPI_slrb_configurations_t *slrb_configurations);
OpenAPI_slrb_configurations_t *OpenAPI_slrb_configurations_parseFromJSON(cJSON *slrb_configurationsJSON);
cJSON *OpenAPI_slrb_configurations_convertToJSON(OpenAPI_slrb_configurations_t *slrb_configurations);
OpenAPI_slrb_configurations_t *OpenAPI_slrb_configurations_copy(OpenAPI_slrb_configurations_t *dst, OpenAPI_slrb_configurations_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_slrb_configurations_H_ */

