/*
 * rat_type.h
 *
 *
 */

#ifndef _OpenAPI_rat_type_H_
#define _OpenAPI_rat_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_rat_type_s OpenAPI_rat_type_t;
typedef struct OpenAPI_rat_type_s {
} OpenAPI_rat_type_t;

OpenAPI_rat_type_t *OpenAPI_rat_type_create(
    );
void OpenAPI_rat_type_free(OpenAPI_rat_type_t *rat_type);
OpenAPI_rat_type_t *OpenAPI_rat_type_parseFromJSON(cJSON *rat_typeJSON);
cJSON *OpenAPI_rat_type_convertToJSON(OpenAPI_rat_type_t *rat_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_rat_type_H_ */

