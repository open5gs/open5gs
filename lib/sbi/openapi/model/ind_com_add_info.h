/*
 * ind_com_add_info.h
 *
 * Additional Information for Indirect Communication between a source and a target domain 
 */

#ifndef _OpenAPI_ind_com_add_info_H_
#define _OpenAPI_ind_com_add_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_ind_com_add_info_s OpenAPI_ind_com_add_info_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_ind_com_add_info_s {
    bool is_all_nf_types_ind;
    int all_nf_types_ind;
    char *scp_api_root;
};

OpenAPI_ind_com_add_info_t *OpenAPI_ind_com_add_info_create(
    bool is_all_nf_types_ind,
    int all_nf_types_ind,
    char *scp_api_root
);
void OpenAPI_ind_com_add_info_free(OpenAPI_ind_com_add_info_t *ind_com_add_info);
OpenAPI_ind_com_add_info_t *OpenAPI_ind_com_add_info_parseFromJSON(cJSON *ind_com_add_infoJSON);
cJSON *OpenAPI_ind_com_add_info_convertToJSON(OpenAPI_ind_com_add_info_t *ind_com_add_info);
OpenAPI_ind_com_add_info_t *OpenAPI_ind_com_add_info_copy(OpenAPI_ind_com_add_info_t *dst, OpenAPI_ind_com_add_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ind_com_add_info_H_ */

