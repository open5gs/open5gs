/*
 * eps_iwk_pgw.h
 *
 * This datatype signifies the PGW FQDN, SMF instance ID and the PLMNId location.
 */

#ifndef _OpenAPI_eps_iwk_pgw_H_
#define _OpenAPI_eps_iwk_pgw_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_eps_iwk_pgw_s OpenAPI_eps_iwk_pgw_t;
#include "plmn_id.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_eps_iwk_pgw_s {
    char *pgw_fqdn;
    char *smf_instance_id;
    struct OpenAPI_plmn_id_s *plmn_id;
};

OpenAPI_eps_iwk_pgw_t *OpenAPI_eps_iwk_pgw_create(
    char *pgw_fqdn,
    char *smf_instance_id,
    OpenAPI_plmn_id_t *plmn_id
);
void OpenAPI_eps_iwk_pgw_free(OpenAPI_eps_iwk_pgw_t *eps_iwk_pgw);
OpenAPI_eps_iwk_pgw_t *OpenAPI_eps_iwk_pgw_parseFromJSON(cJSON *eps_iwk_pgwJSON);
cJSON *OpenAPI_eps_iwk_pgw_convertToJSON(OpenAPI_eps_iwk_pgw_t *eps_iwk_pgw);
OpenAPI_eps_iwk_pgw_t *OpenAPI_eps_iwk_pgw_copy(OpenAPI_eps_iwk_pgw_t *dst, OpenAPI_eps_iwk_pgw_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_eps_iwk_pgw_H_ */

