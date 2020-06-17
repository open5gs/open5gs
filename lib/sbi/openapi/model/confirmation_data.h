/*
 * confirmation_data.h
 *
 *
 */

#ifndef _OpenAPI_confirmation_data_H_
#define _OpenAPI_confirmation_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_confirmation_data_s OpenAPI_confirmation_data_t;
typedef struct OpenAPI_confirmation_data_s {
    char *res_star;
} OpenAPI_confirmation_data_t;

OpenAPI_confirmation_data_t *OpenAPI_confirmation_data_create(
    char *res_star
    );
void OpenAPI_confirmation_data_free(OpenAPI_confirmation_data_t *confirmation_data);
OpenAPI_confirmation_data_t *OpenAPI_confirmation_data_parseFromJSON(cJSON *confirmation_dataJSON);
cJSON *OpenAPI_confirmation_data_convertToJSON(OpenAPI_confirmation_data_t *confirmation_data);
OpenAPI_confirmation_data_t *OpenAPI_confirmation_data_copy(OpenAPI_confirmation_data_t *dst, OpenAPI_confirmation_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_confirmation_data_H_ */

