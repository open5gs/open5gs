/*
 * early_media_description.h
 *
 * 
 */

#ifndef _OpenAPI_early_media_description_H_
#define _OpenAPI_early_media_description_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_early_media_description_s OpenAPI_early_media_description_t;
#include "sdp_media_component.h"
#include "sdp_time_stamps.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_early_media_description_s {
    struct OpenAPI_sdp_time_stamps_s *s_dp_time_stamps;
    OpenAPI_list_t *s_dp_media_component;
    OpenAPI_list_t *s_dp_session_description;
};

OpenAPI_early_media_description_t *OpenAPI_early_media_description_create(
    OpenAPI_sdp_time_stamps_t *s_dp_time_stamps,
    OpenAPI_list_t *s_dp_media_component,
    OpenAPI_list_t *s_dp_session_description
);
void OpenAPI_early_media_description_free(OpenAPI_early_media_description_t *early_media_description);
OpenAPI_early_media_description_t *OpenAPI_early_media_description_parseFromJSON(cJSON *early_media_descriptionJSON);
cJSON *OpenAPI_early_media_description_convertToJSON(OpenAPI_early_media_description_t *early_media_description);
OpenAPI_early_media_description_t *OpenAPI_early_media_description_copy(OpenAPI_early_media_description_t *dst, OpenAPI_early_media_description_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_early_media_description_H_ */

