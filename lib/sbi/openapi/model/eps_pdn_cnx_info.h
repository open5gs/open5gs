/*
 * eps_pdn_cnx_info.h
 *
 *
 */

#ifndef _OpenAPI_eps_pdn_cnx_info_H_
#define _OpenAPI_eps_pdn_cnx_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_eps_pdn_cnx_info_s OpenAPI_eps_pdn_cnx_info_t;
typedef struct OpenAPI_eps_pdn_cnx_info_s {
    char pgw_s8c_fteid;
    char pgw_node_name;
    int linked_bearer_id;
} OpenAPI_eps_pdn_cnx_info_t;

OpenAPI_eps_pdn_cnx_info_t *OpenAPI_eps_pdn_cnx_info_create(
    char pgw_s8c_fteid,
    char pgw_node_name,
    int linked_bearer_id
    );
void OpenAPI_eps_pdn_cnx_info_free(OpenAPI_eps_pdn_cnx_info_t *eps_pdn_cnx_info);
OpenAPI_eps_pdn_cnx_info_t *OpenAPI_eps_pdn_cnx_info_parseFromJSON(cJSON *eps_pdn_cnx_infoJSON);
cJSON *OpenAPI_eps_pdn_cnx_info_convertToJSON(OpenAPI_eps_pdn_cnx_info_t *eps_pdn_cnx_info);
OpenAPI_eps_pdn_cnx_info_t *OpenAPI_eps_pdn_cnx_info_copy(OpenAPI_eps_pdn_cnx_info_t *dst, OpenAPI_eps_pdn_cnx_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_eps_pdn_cnx_info_H_ */

