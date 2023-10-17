/*
 * plmn_ec_info_1.h
 *
 * 
 */

#ifndef _OpenAPI_plmn_ec_info_1_H_
#define _OpenAPI_plmn_ec_info_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ec_restriction_data_wb.h"
#include "plmn_id_1.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_plmn_ec_info_1_s OpenAPI_plmn_ec_info_1_t;
typedef struct OpenAPI_plmn_ec_info_1_s {
    struct OpenAPI_plmn_id_1_s *plmn_id;
    struct OpenAPI_ec_restriction_data_wb_s *ec_restriction_data_wb;
    bool is_ec_restriction_data_nb;
    int ec_restriction_data_nb;
} OpenAPI_plmn_ec_info_1_t;

OpenAPI_plmn_ec_info_1_t *OpenAPI_plmn_ec_info_1_create(
    OpenAPI_plmn_id_1_t *plmn_id,
    OpenAPI_ec_restriction_data_wb_t *ec_restriction_data_wb,
    bool is_ec_restriction_data_nb,
    int ec_restriction_data_nb
);
void OpenAPI_plmn_ec_info_1_free(OpenAPI_plmn_ec_info_1_t *plmn_ec_info_1);
OpenAPI_plmn_ec_info_1_t *OpenAPI_plmn_ec_info_1_parseFromJSON(cJSON *plmn_ec_info_1JSON);
cJSON *OpenAPI_plmn_ec_info_1_convertToJSON(OpenAPI_plmn_ec_info_1_t *plmn_ec_info_1);
OpenAPI_plmn_ec_info_1_t *OpenAPI_plmn_ec_info_1_copy(OpenAPI_plmn_ec_info_1_t *dst, OpenAPI_plmn_ec_info_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_plmn_ec_info_1_H_ */

