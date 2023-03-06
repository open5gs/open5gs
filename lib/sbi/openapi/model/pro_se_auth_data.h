/*
 * pro_se_auth_data.h
 *
 * 
 */

#ifndef _OpenAPI_pro_se_auth_data_H_
#define _OpenAPI_pro_se_auth_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pro_se_auth_data_s OpenAPI_pro_se_auth_data_t;
typedef struct OpenAPI_pro_se_auth_data_s {
} OpenAPI_pro_se_auth_data_t;

OpenAPI_pro_se_auth_data_t *OpenAPI_pro_se_auth_data_create(
);
void OpenAPI_pro_se_auth_data_free(OpenAPI_pro_se_auth_data_t *pro_se_auth_data);
OpenAPI_pro_se_auth_data_t *OpenAPI_pro_se_auth_data_parseFromJSON(cJSON *pro_se_auth_dataJSON);
cJSON *OpenAPI_pro_se_auth_data_convertToJSON(OpenAPI_pro_se_auth_data_t *pro_se_auth_data);
OpenAPI_pro_se_auth_data_t *OpenAPI_pro_se_auth_data_copy(OpenAPI_pro_se_auth_data_t *dst, OpenAPI_pro_se_auth_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pro_se_auth_data_H_ */

