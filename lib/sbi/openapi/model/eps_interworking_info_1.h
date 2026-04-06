/*
 * eps_interworking_info_1.h
 *
 * This information element contains the associations between APN/DNN and PGW-C+SMF selected by the AMF for EPS interworking. 
 */

#ifndef _OpenAPI_eps_interworking_info_1_H_
#define _OpenAPI_eps_interworking_info_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_eps_interworking_info_1_s OpenAPI_eps_interworking_info_1_t;
#include "eps_iwk_pgw_1.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_eps_interworking_info_1_s {
    OpenAPI_list_t* eps_iwk_pgws;
    char *registration_time;
};

OpenAPI_eps_interworking_info_1_t *OpenAPI_eps_interworking_info_1_create(
    OpenAPI_list_t* eps_iwk_pgws,
    char *registration_time
);
void OpenAPI_eps_interworking_info_1_free(OpenAPI_eps_interworking_info_1_t *eps_interworking_info_1);
OpenAPI_eps_interworking_info_1_t *OpenAPI_eps_interworking_info_1_parseFromJSON(cJSON *eps_interworking_info_1JSON);
cJSON *OpenAPI_eps_interworking_info_1_convertToJSON(OpenAPI_eps_interworking_info_1_t *eps_interworking_info_1);
OpenAPI_eps_interworking_info_1_t *OpenAPI_eps_interworking_info_1_copy(OpenAPI_eps_interworking_info_1_t *dst, OpenAPI_eps_interworking_info_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_eps_interworking_info_1_H_ */

