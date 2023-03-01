/*
 * svc_experience.h
 *
 * Contains a mean opinion score with the customized range.
 */

#ifndef _OpenAPI_svc_experience_H_
#define _OpenAPI_svc_experience_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_svc_experience_s OpenAPI_svc_experience_t;
typedef struct OpenAPI_svc_experience_s {
    bool is_mos;
    float mos;
    bool is_upper_range;
    float upper_range;
    bool is_lower_range;
    float lower_range;
} OpenAPI_svc_experience_t;

OpenAPI_svc_experience_t *OpenAPI_svc_experience_create(
    bool is_mos,
    float mos,
    bool is_upper_range,
    float upper_range,
    bool is_lower_range,
    float lower_range
);
void OpenAPI_svc_experience_free(OpenAPI_svc_experience_t *svc_experience);
OpenAPI_svc_experience_t *OpenAPI_svc_experience_parseFromJSON(cJSON *svc_experienceJSON);
cJSON *OpenAPI_svc_experience_convertToJSON(OpenAPI_svc_experience_t *svc_experience);
OpenAPI_svc_experience_t *OpenAPI_svc_experience_copy(OpenAPI_svc_experience_t *dst, OpenAPI_svc_experience_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_svc_experience_H_ */

