/*
 * plmn_ec_info.h
 *
 * Contains serving PLMNs where Enhanced Coverage shall be allowed and the detailed enhanced coverage restriction configuration under per the PLMN. 
 */

#ifndef _OpenAPI_plmn_ec_info_H_
#define _OpenAPI_plmn_ec_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_plmn_ec_info_s OpenAPI_plmn_ec_info_t;
#include "ec_restriction_data_wb.h"
#include "plmn_id.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_plmn_ec_info_s {
    struct OpenAPI_plmn_id_s *plmn_id;
    bool is_ec_restriction_data_wb_null;
    struct OpenAPI_ec_restriction_data_wb_s *ec_restriction_data_wb;
    bool is_ec_restriction_data_nb;
    int ec_restriction_data_nb;
};

OpenAPI_plmn_ec_info_t *OpenAPI_plmn_ec_info_create(
    OpenAPI_plmn_id_t *plmn_id,
    bool is_ec_restriction_data_wb_null,
    OpenAPI_ec_restriction_data_wb_t *ec_restriction_data_wb,
    bool is_ec_restriction_data_nb,
    int ec_restriction_data_nb
);
void OpenAPI_plmn_ec_info_free(OpenAPI_plmn_ec_info_t *plmn_ec_info);
OpenAPI_plmn_ec_info_t *OpenAPI_plmn_ec_info_parseFromJSON(cJSON *plmn_ec_infoJSON);
cJSON *OpenAPI_plmn_ec_info_convertToJSON(OpenAPI_plmn_ec_info_t *plmn_ec_info);
OpenAPI_plmn_ec_info_t *OpenAPI_plmn_ec_info_copy(OpenAPI_plmn_ec_info_t *dst, OpenAPI_plmn_ec_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_plmn_ec_info_H_ */

