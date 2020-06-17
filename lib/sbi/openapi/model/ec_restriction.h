/*
 * ec_restriction.h
 *
 *
 */

#ifndef _OpenAPI_ec_restriction_H_
#define _OpenAPI_ec_restriction_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "plmn_ec_info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ec_restriction_s OpenAPI_ec_restriction_t;
typedef struct OpenAPI_ec_restriction_s {
    char *af_instance_id;
    int reference_id;
    OpenAPI_list_t *plmn_ec_infos;
} OpenAPI_ec_restriction_t;

OpenAPI_ec_restriction_t *OpenAPI_ec_restriction_create(
    char *af_instance_id,
    int reference_id,
    OpenAPI_list_t *plmn_ec_infos
    );
void OpenAPI_ec_restriction_free(OpenAPI_ec_restriction_t *ec_restriction);
OpenAPI_ec_restriction_t *OpenAPI_ec_restriction_parseFromJSON(cJSON *ec_restrictionJSON);
cJSON *OpenAPI_ec_restriction_convertToJSON(OpenAPI_ec_restriction_t *ec_restriction);
OpenAPI_ec_restriction_t *OpenAPI_ec_restriction_copy(OpenAPI_ec_restriction_t *dst, OpenAPI_ec_restriction_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ec_restriction_H_ */

