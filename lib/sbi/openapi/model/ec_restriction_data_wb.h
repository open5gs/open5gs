/*
 * ec_restriction_data_wb.h
 *
 * 
 */

#ifndef _OpenAPI_ec_restriction_data_wb_H_
#define _OpenAPI_ec_restriction_data_wb_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ec_restriction_data_wb_s OpenAPI_ec_restriction_data_wb_t;
typedef struct OpenAPI_ec_restriction_data_wb_s {
    bool is_ec_mode_a_restricted;
    int ec_mode_a_restricted;
    int ec_mode_b_restricted;
} OpenAPI_ec_restriction_data_wb_t;

OpenAPI_ec_restriction_data_wb_t *OpenAPI_ec_restriction_data_wb_create(
    bool is_ec_mode_a_restricted,
    int ec_mode_a_restricted,
    int ec_mode_b_restricted
);
void OpenAPI_ec_restriction_data_wb_free(OpenAPI_ec_restriction_data_wb_t *ec_restriction_data_wb);
OpenAPI_ec_restriction_data_wb_t *OpenAPI_ec_restriction_data_wb_parseFromJSON(cJSON *ec_restriction_data_wbJSON);
cJSON *OpenAPI_ec_restriction_data_wb_convertToJSON(OpenAPI_ec_restriction_data_wb_t *ec_restriction_data_wb);
OpenAPI_ec_restriction_data_wb_t *OpenAPI_ec_restriction_data_wb_copy(OpenAPI_ec_restriction_data_wb_t *dst, OpenAPI_ec_restriction_data_wb_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ec_restriction_data_wb_H_ */

