/*
 * sepp_info.h
 *
 * Information of a SEPP Instance
 */

#ifndef _OpenAPI_sepp_info_H_
#define _OpenAPI_sepp_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "plmn_id.h"
#include "plmn_id_nid.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_sepp_info_s OpenAPI_sepp_info_t;
typedef struct OpenAPI_sepp_info_s {
    char *sepp_prefix;
    OpenAPI_list_t* sepp_ports;
    OpenAPI_list_t *remote_plmn_list;
    OpenAPI_list_t *remote_snpn_list;
} OpenAPI_sepp_info_t;

OpenAPI_sepp_info_t *OpenAPI_sepp_info_create(
    char *sepp_prefix,
    OpenAPI_list_t* sepp_ports,
    OpenAPI_list_t *remote_plmn_list,
    OpenAPI_list_t *remote_snpn_list
);
void OpenAPI_sepp_info_free(OpenAPI_sepp_info_t *sepp_info);
OpenAPI_sepp_info_t *OpenAPI_sepp_info_parseFromJSON(cJSON *sepp_infoJSON);
cJSON *OpenAPI_sepp_info_convertToJSON(OpenAPI_sepp_info_t *sepp_info);
OpenAPI_sepp_info_t *OpenAPI_sepp_info_copy(OpenAPI_sepp_info_t *dst, OpenAPI_sepp_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sepp_info_H_ */

