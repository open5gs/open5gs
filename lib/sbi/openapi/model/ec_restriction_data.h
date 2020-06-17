/*
 * ec_restriction_data.h
 *
 *
 */

#ifndef _OpenAPI_ec_restriction_data_H_
#define _OpenAPI_ec_restriction_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ec_restriction_data_s OpenAPI_ec_restriction_data_t;
typedef struct OpenAPI_ec_restriction_data_s {
    int ec_mode_a_restricted;
    int ec_mode_b_restricted;
} OpenAPI_ec_restriction_data_t;

OpenAPI_ec_restriction_data_t *OpenAPI_ec_restriction_data_create(
    int ec_mode_a_restricted,
    int ec_mode_b_restricted
    );
void OpenAPI_ec_restriction_data_free(OpenAPI_ec_restriction_data_t *ec_restriction_data);
OpenAPI_ec_restriction_data_t *OpenAPI_ec_restriction_data_parseFromJSON(cJSON *ec_restriction_dataJSON);
cJSON *OpenAPI_ec_restriction_data_convertToJSON(OpenAPI_ec_restriction_data_t *ec_restriction_data);
OpenAPI_ec_restriction_data_t *OpenAPI_ec_restriction_data_copy(OpenAPI_ec_restriction_data_t *dst, OpenAPI_ec_restriction_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ec_restriction_data_H_ */

