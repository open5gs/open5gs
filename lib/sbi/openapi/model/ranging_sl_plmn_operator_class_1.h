/*
 * ranging_sl_plmn_operator_class_1.h
 *
 * Describes PLMN Operator Class for ranging and sidelink positioning.
 */

#ifndef _OpenAPI_ranging_sl_plmn_operator_class_1_H_
#define _OpenAPI_ranging_sl_plmn_operator_class_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_ranging_sl_plmn_operator_class_1_s OpenAPI_ranging_sl_plmn_operator_class_1_t;
#include "lcs_client_class.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_ranging_sl_plmn_operator_class_1_s {
    OpenAPI_lcs_client_class_e ranging_sl_lcs_client_class;
    OpenAPI_list_t *lcs_client_ids;
};

OpenAPI_ranging_sl_plmn_operator_class_1_t *OpenAPI_ranging_sl_plmn_operator_class_1_create(
    OpenAPI_lcs_client_class_e ranging_sl_lcs_client_class,
    OpenAPI_list_t *lcs_client_ids
);
void OpenAPI_ranging_sl_plmn_operator_class_1_free(OpenAPI_ranging_sl_plmn_operator_class_1_t *ranging_sl_plmn_operator_class_1);
OpenAPI_ranging_sl_plmn_operator_class_1_t *OpenAPI_ranging_sl_plmn_operator_class_1_parseFromJSON(cJSON *ranging_sl_plmn_operator_class_1JSON);
cJSON *OpenAPI_ranging_sl_plmn_operator_class_1_convertToJSON(OpenAPI_ranging_sl_plmn_operator_class_1_t *ranging_sl_plmn_operator_class_1);
OpenAPI_ranging_sl_plmn_operator_class_1_t *OpenAPI_ranging_sl_plmn_operator_class_1_copy(OpenAPI_ranging_sl_plmn_operator_class_1_t *dst, OpenAPI_ranging_sl_plmn_operator_class_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ranging_sl_plmn_operator_class_1_H_ */

