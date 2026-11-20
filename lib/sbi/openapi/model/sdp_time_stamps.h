/*
 * sdp_time_stamps.h
 *
 * 
 */

#ifndef _OpenAPI_sdp_time_stamps_H_
#define _OpenAPI_sdp_time_stamps_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_sdp_time_stamps_s OpenAPI_sdp_time_stamps_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_sdp_time_stamps_s {
    char *s_dp_offer_timestamp;
    char *s_dp_answer_timestamp;
};

OpenAPI_sdp_time_stamps_t *OpenAPI_sdp_time_stamps_create(
    char *s_dp_offer_timestamp,
    char *s_dp_answer_timestamp
);
void OpenAPI_sdp_time_stamps_free(OpenAPI_sdp_time_stamps_t *sdp_time_stamps);
OpenAPI_sdp_time_stamps_t *OpenAPI_sdp_time_stamps_parseFromJSON(cJSON *sdp_time_stampsJSON);
cJSON *OpenAPI_sdp_time_stamps_convertToJSON(OpenAPI_sdp_time_stamps_t *sdp_time_stamps);
OpenAPI_sdp_time_stamps_t *OpenAPI_sdp_time_stamps_copy(OpenAPI_sdp_time_stamps_t *dst, OpenAPI_sdp_time_stamps_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sdp_time_stamps_H_ */

