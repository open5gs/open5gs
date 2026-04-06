/*
 * eps_interworking_info.h
 *
 * This information element contains the associations between APN/DNN and PGW-C+SMF selected by the AMF for EPS interworking. 
 */

#ifndef _OpenAPI_eps_interworking_info_H_
#define _OpenAPI_eps_interworking_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_eps_interworking_info_s OpenAPI_eps_interworking_info_t;
#include "eps_iwk_pgw.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_eps_interworking_info_s {
    OpenAPI_list_t* eps_iwk_pgws;
    char *registration_time;
};

OpenAPI_eps_interworking_info_t *OpenAPI_eps_interworking_info_create(
    OpenAPI_list_t* eps_iwk_pgws,
    char *registration_time
);
void OpenAPI_eps_interworking_info_free(OpenAPI_eps_interworking_info_t *eps_interworking_info);
OpenAPI_eps_interworking_info_t *OpenAPI_eps_interworking_info_parseFromJSON(cJSON *eps_interworking_infoJSON);
cJSON *OpenAPI_eps_interworking_info_convertToJSON(OpenAPI_eps_interworking_info_t *eps_interworking_info);
OpenAPI_eps_interworking_info_t *OpenAPI_eps_interworking_info_copy(OpenAPI_eps_interworking_info_t *dst, OpenAPI_eps_interworking_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_eps_interworking_info_H_ */

