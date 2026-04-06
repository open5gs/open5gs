/*
 * ranging_sl_external_unrelated_class.h
 *
 * Describes Call/Session unrelated Class subscriptions for ranging and sidelink positioning  for identified value added LCS Clients or AFs. 
 */

#ifndef _OpenAPI_ranging_sl_external_unrelated_class_H_
#define _OpenAPI_ranging_sl_external_unrelated_class_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_ranging_sl_external_unrelated_class_s OpenAPI_ranging_sl_external_unrelated_class_t;
#include "ranging_sl_af_external.h"
#include "ranging_sl_lcs_client_external.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_ranging_sl_external_unrelated_class_s {
    OpenAPI_list_t *ranging_sl_lcs_client_externals;
    OpenAPI_list_t *ranging_sl_af_externals;
};

OpenAPI_ranging_sl_external_unrelated_class_t *OpenAPI_ranging_sl_external_unrelated_class_create(
    OpenAPI_list_t *ranging_sl_lcs_client_externals,
    OpenAPI_list_t *ranging_sl_af_externals
);
void OpenAPI_ranging_sl_external_unrelated_class_free(OpenAPI_ranging_sl_external_unrelated_class_t *ranging_sl_external_unrelated_class);
OpenAPI_ranging_sl_external_unrelated_class_t *OpenAPI_ranging_sl_external_unrelated_class_parseFromJSON(cJSON *ranging_sl_external_unrelated_classJSON);
cJSON *OpenAPI_ranging_sl_external_unrelated_class_convertToJSON(OpenAPI_ranging_sl_external_unrelated_class_t *ranging_sl_external_unrelated_class);
OpenAPI_ranging_sl_external_unrelated_class_t *OpenAPI_ranging_sl_external_unrelated_class_copy(OpenAPI_ranging_sl_external_unrelated_class_t *dst, OpenAPI_ranging_sl_external_unrelated_class_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ranging_sl_external_unrelated_class_H_ */

