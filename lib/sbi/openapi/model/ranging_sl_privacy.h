/*
 * ranging_sl_privacy.h
 *
 * Contains Ranging and Sidelink Privacy Data
 */

#ifndef _OpenAPI_ranging_sl_privacy_H_
#define _OpenAPI_ranging_sl_privacy_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_ranging_sl_privacy_s OpenAPI_ranging_sl_privacy_t;
#include "rslppi.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_ranging_sl_privacy_s {
    char *af_instance_id;
    bool is_reference_id;
    int reference_id;
    struct OpenAPI_rslppi_s *rslppi;
};

OpenAPI_ranging_sl_privacy_t *OpenAPI_ranging_sl_privacy_create(
    char *af_instance_id,
    bool is_reference_id,
    int reference_id,
    OpenAPI_rslppi_t *rslppi
);
void OpenAPI_ranging_sl_privacy_free(OpenAPI_ranging_sl_privacy_t *ranging_sl_privacy);
OpenAPI_ranging_sl_privacy_t *OpenAPI_ranging_sl_privacy_parseFromJSON(cJSON *ranging_sl_privacyJSON);
cJSON *OpenAPI_ranging_sl_privacy_convertToJSON(OpenAPI_ranging_sl_privacy_t *ranging_sl_privacy);
OpenAPI_ranging_sl_privacy_t *OpenAPI_ranging_sl_privacy_copy(OpenAPI_ranging_sl_privacy_t *dst, OpenAPI_ranging_sl_privacy_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ranging_sl_privacy_H_ */

