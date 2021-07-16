/*
 * pws_information.h
 *
 * 
 */

#ifndef _OpenAPI_pws_information_H_
#define _OpenAPI_pws_information_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "global_ran_node_id.h"
#include "n2_info_content.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pws_information_s OpenAPI_pws_information_t;
typedef struct OpenAPI_pws_information_s {
    int message_identifier;
    int serial_number;
    struct OpenAPI_n2_info_content_s *pws_container;
    OpenAPI_list_t *bc_empty_area_list;
    bool is_send_ran_response;
    int send_ran_response;
    char *omc_id;
} OpenAPI_pws_information_t;

OpenAPI_pws_information_t *OpenAPI_pws_information_create(
    int message_identifier,
    int serial_number,
    OpenAPI_n2_info_content_t *pws_container,
    OpenAPI_list_t *bc_empty_area_list,
    bool is_send_ran_response,
    int send_ran_response,
    char *omc_id
);
void OpenAPI_pws_information_free(OpenAPI_pws_information_t *pws_information);
OpenAPI_pws_information_t *OpenAPI_pws_information_parseFromJSON(cJSON *pws_informationJSON);
cJSON *OpenAPI_pws_information_convertToJSON(OpenAPI_pws_information_t *pws_information);
OpenAPI_pws_information_t *OpenAPI_pws_information_copy(OpenAPI_pws_information_t *dst, OpenAPI_pws_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pws_information_H_ */

