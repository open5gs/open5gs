/*
 * v2x_rat_type.h
 *
 * Possible values are - PC5_LTE: Indicates the LTE RAT type over PC5 interface. - PC5_NR: Indicates the NR RAT type over NR interface.
 */

#ifndef _OpenAPI_v2x_rat_type_H_
#define _OpenAPI_v2x_rat_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_v2x_rat_type_s OpenAPI_v2x_rat_type_t;
typedef struct OpenAPI_v2x_rat_type_s {
} OpenAPI_v2x_rat_type_t;

OpenAPI_v2x_rat_type_t *OpenAPI_v2x_rat_type_create(
    );
void OpenAPI_v2x_rat_type_free(OpenAPI_v2x_rat_type_t *v2x_rat_type);
OpenAPI_v2x_rat_type_t *OpenAPI_v2x_rat_type_parseFromJSON(cJSON *v2x_rat_typeJSON);
cJSON *OpenAPI_v2x_rat_type_convertToJSON(OpenAPI_v2x_rat_type_t *v2x_rat_type);
OpenAPI_v2x_rat_type_t *OpenAPI_v2x_rat_type_copy(OpenAPI_v2x_rat_type_t *dst, OpenAPI_v2x_rat_type_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_v2x_rat_type_H_ */

