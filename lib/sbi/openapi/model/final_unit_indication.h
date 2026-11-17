/*
 * final_unit_indication.h
 *
 * 
 */

#ifndef _OpenAPI_final_unit_indication_H_
#define _OpenAPI_final_unit_indication_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_final_unit_indication_s OpenAPI_final_unit_indication_t;
#include "final_unit_action.h"
#include "redirect_server.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_final_unit_indication_s {
    OpenAPI_final_unit_action_e final_unit_action;
    char *restriction_filter_rule;
    OpenAPI_list_t *restriction_filter_rule_list;
    char *filter_id;
    OpenAPI_list_t *filter_id_list;
    struct OpenAPI_redirect_server_s *redirect_server;
};

OpenAPI_final_unit_indication_t *OpenAPI_final_unit_indication_create(
    OpenAPI_final_unit_action_e final_unit_action,
    char *restriction_filter_rule,
    OpenAPI_list_t *restriction_filter_rule_list,
    char *filter_id,
    OpenAPI_list_t *filter_id_list,
    OpenAPI_redirect_server_t *redirect_server
);
void OpenAPI_final_unit_indication_free(OpenAPI_final_unit_indication_t *final_unit_indication);
OpenAPI_final_unit_indication_t *OpenAPI_final_unit_indication_parseFromJSON(cJSON *final_unit_indicationJSON);
cJSON *OpenAPI_final_unit_indication_convertToJSON(OpenAPI_final_unit_indication_t *final_unit_indication);
OpenAPI_final_unit_indication_t *OpenAPI_final_unit_indication_copy(OpenAPI_final_unit_indication_t *dst, OpenAPI_final_unit_indication_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_final_unit_indication_H_ */

