/*
 * pgw_info.h
 *
 * 
 */

#ifndef _OpenAPI_pgw_info_H_
#define _OpenAPI_pgw_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "plmn_id.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pgw_info_s OpenAPI_pgw_info_t;
typedef struct OpenAPI_pgw_info_s {
    char *dnn;
    char *pgw_fqdn;
    struct OpenAPI_plmn_id_s *plmn_id;
    bool is_epdg_ind;
    int epdg_ind;
} OpenAPI_pgw_info_t;

OpenAPI_pgw_info_t *OpenAPI_pgw_info_create(
    char *dnn,
    char *pgw_fqdn,
    OpenAPI_plmn_id_t *plmn_id,
    bool is_epdg_ind,
    int epdg_ind
);
void OpenAPI_pgw_info_free(OpenAPI_pgw_info_t *pgw_info);
OpenAPI_pgw_info_t *OpenAPI_pgw_info_parseFromJSON(cJSON *pgw_infoJSON);
cJSON *OpenAPI_pgw_info_convertToJSON(OpenAPI_pgw_info_t *pgw_info);
OpenAPI_pgw_info_t *OpenAPI_pgw_info_copy(OpenAPI_pgw_info_t *dst, OpenAPI_pgw_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pgw_info_H_ */

