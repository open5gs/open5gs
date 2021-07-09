/*
 * n2_info_container.h
 *
 * 
 */

#ifndef _OpenAPI_n2_info_container_H_
#define _OpenAPI_n2_info_container_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "n2_information_class.h"
#include "n2_ran_information.h"
#include "n2_sm_information.h"
#include "nrppa_information.h"
#include "pws_information.h"
#include "v2x_information.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_n2_info_container_s OpenAPI_n2_info_container_t;
typedef struct OpenAPI_n2_info_container_s {
    OpenAPI_n2_information_class_e n2_information_class;
    struct OpenAPI_n2_sm_information_s *sm_info;
    struct OpenAPI_n2_ran_information_s *ran_info;
    struct OpenAPI_nrppa_information_s *nrppa_info;
    struct OpenAPI_pws_information_s *pws_info;
    struct OpenAPI_v2x_information_s *v2x_info;
} OpenAPI_n2_info_container_t;

OpenAPI_n2_info_container_t *OpenAPI_n2_info_container_create(
    OpenAPI_n2_information_class_e n2_information_class,
    OpenAPI_n2_sm_information_t *sm_info,
    OpenAPI_n2_ran_information_t *ran_info,
    OpenAPI_nrppa_information_t *nrppa_info,
    OpenAPI_pws_information_t *pws_info,
    OpenAPI_v2x_information_t *v2x_info
);
void OpenAPI_n2_info_container_free(OpenAPI_n2_info_container_t *n2_info_container);
OpenAPI_n2_info_container_t *OpenAPI_n2_info_container_parseFromJSON(cJSON *n2_info_containerJSON);
cJSON *OpenAPI_n2_info_container_convertToJSON(OpenAPI_n2_info_container_t *n2_info_container);
OpenAPI_n2_info_container_t *OpenAPI_n2_info_container_copy(OpenAPI_n2_info_container_t *dst, OpenAPI_n2_info_container_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_n2_info_container_H_ */

