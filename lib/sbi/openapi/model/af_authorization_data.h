/*
 * af_authorization_data.h
 *
 * Contains the AF Authorization Data.
 */

#ifndef _OpenAPI_af_authorization_data_H_
#define _OpenAPI_af_authorization_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_af_authorization_data_s OpenAPI_af_authorization_data_t;
#include "individual_af_authorization_data.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_af_authorization_data_s {
    OpenAPI_list_t* af_auth_data;
};

OpenAPI_af_authorization_data_t *OpenAPI_af_authorization_data_create(
    OpenAPI_list_t* af_auth_data
);
void OpenAPI_af_authorization_data_free(OpenAPI_af_authorization_data_t *af_authorization_data);
OpenAPI_af_authorization_data_t *OpenAPI_af_authorization_data_parseFromJSON(cJSON *af_authorization_dataJSON);
cJSON *OpenAPI_af_authorization_data_convertToJSON(OpenAPI_af_authorization_data_t *af_authorization_data);
OpenAPI_af_authorization_data_t *OpenAPI_af_authorization_data_copy(OpenAPI_af_authorization_data_t *dst, OpenAPI_af_authorization_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_af_authorization_data_H_ */

