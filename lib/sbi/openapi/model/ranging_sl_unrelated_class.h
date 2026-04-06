/*
 * ranging_sl_unrelated_class.h
 *
 * Describes Call/Session unrelated Classes for ranging and sidelink positioning.
 */

#ifndef _OpenAPI_ranging_sl_unrelated_class_H_
#define _OpenAPI_ranging_sl_unrelated_class_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_ranging_sl_unrelated_class_s OpenAPI_ranging_sl_unrelated_class_t;
#include "ranging_sl_app_id_unrelated_class.h"
#include "ranging_sl_default_unrelated_class.h"
#include "ranging_sl_external_unrelated_class.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_ranging_sl_unrelated_class_s {
    struct OpenAPI_ranging_sl_default_unrelated_class_s *ranging_sl_default_unrelated_class;
    struct OpenAPI_ranging_sl_external_unrelated_class_s *ranging_sl_external_unrelated_class;
    OpenAPI_list_t *ranging_sl_app_id_unrelated_classes;
};

OpenAPI_ranging_sl_unrelated_class_t *OpenAPI_ranging_sl_unrelated_class_create(
    OpenAPI_ranging_sl_default_unrelated_class_t *ranging_sl_default_unrelated_class,
    OpenAPI_ranging_sl_external_unrelated_class_t *ranging_sl_external_unrelated_class,
    OpenAPI_list_t *ranging_sl_app_id_unrelated_classes
);
void OpenAPI_ranging_sl_unrelated_class_free(OpenAPI_ranging_sl_unrelated_class_t *ranging_sl_unrelated_class);
OpenAPI_ranging_sl_unrelated_class_t *OpenAPI_ranging_sl_unrelated_class_parseFromJSON(cJSON *ranging_sl_unrelated_classJSON);
cJSON *OpenAPI_ranging_sl_unrelated_class_convertToJSON(OpenAPI_ranging_sl_unrelated_class_t *ranging_sl_unrelated_class);
OpenAPI_ranging_sl_unrelated_class_t *OpenAPI_ranging_sl_unrelated_class_copy(OpenAPI_ranging_sl_unrelated_class_t *dst, OpenAPI_ranging_sl_unrelated_class_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ranging_sl_unrelated_class_H_ */

