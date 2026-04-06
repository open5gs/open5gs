/*
 * pgw_info_1.h
 *
 * Contains PGW Information
 */

#ifndef _OpenAPI_pgw_info_1_H_
#define _OpenAPI_pgw_info_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_pgw_info_1_s OpenAPI_pgw_info_1_t;
#include "ip_address_1.h"
#include "plmn_id.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_pgw_info_1_s {
    char *dnn;
    char *pgw_fqdn;
    struct OpenAPI_ip_address_1_s *pgw_ip_addr;
    struct OpenAPI_plmn_id_s *plmn_id;
    bool is_epdg_ind;
    int epdg_ind;
    char *pcf_id;
    char *registration_time;
    bool is_wildcard_ind;
    int wildcard_ind;
};

OpenAPI_pgw_info_1_t *OpenAPI_pgw_info_1_create(
    char *dnn,
    char *pgw_fqdn,
    OpenAPI_ip_address_1_t *pgw_ip_addr,
    OpenAPI_plmn_id_t *plmn_id,
    bool is_epdg_ind,
    int epdg_ind,
    char *pcf_id,
    char *registration_time,
    bool is_wildcard_ind,
    int wildcard_ind
);
void OpenAPI_pgw_info_1_free(OpenAPI_pgw_info_1_t *pgw_info_1);
OpenAPI_pgw_info_1_t *OpenAPI_pgw_info_1_parseFromJSON(cJSON *pgw_info_1JSON);
cJSON *OpenAPI_pgw_info_1_convertToJSON(OpenAPI_pgw_info_1_t *pgw_info_1);
OpenAPI_pgw_info_1_t *OpenAPI_pgw_info_1_copy(OpenAPI_pgw_info_1_t *dst, OpenAPI_pgw_info_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pgw_info_1_H_ */

