/*
 * ec_restriction_1.h
 *
 * 
 */

#ifndef _OpenAPI_ec_restriction_1_H_
#define _OpenAPI_ec_restriction_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "plmn_ec_info_1.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ec_restriction_1_s OpenAPI_ec_restriction_1_t;
typedef struct OpenAPI_ec_restriction_1_s {
    char *af_instance_id;
    int reference_id;
    OpenAPI_list_t *plmn_ec_infos;
    char *mtc_provider_information;
} OpenAPI_ec_restriction_1_t;

OpenAPI_ec_restriction_1_t *OpenAPI_ec_restriction_1_create(
    char *af_instance_id,
    int reference_id,
    OpenAPI_list_t *plmn_ec_infos,
    char *mtc_provider_information
);
void OpenAPI_ec_restriction_1_free(OpenAPI_ec_restriction_1_t *ec_restriction_1);
OpenAPI_ec_restriction_1_t *OpenAPI_ec_restriction_1_parseFromJSON(cJSON *ec_restriction_1JSON);
cJSON *OpenAPI_ec_restriction_1_convertToJSON(OpenAPI_ec_restriction_1_t *ec_restriction_1);
OpenAPI_ec_restriction_1_t *OpenAPI_ec_restriction_1_copy(OpenAPI_ec_restriction_1_t *dst, OpenAPI_ec_restriction_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ec_restriction_1_H_ */

