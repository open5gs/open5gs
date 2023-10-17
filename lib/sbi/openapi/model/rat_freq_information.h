/*
 * rat_freq_information.h
 *
 * Represents the RAT type and/or Frequency information.
 */

#ifndef _OpenAPI_rat_freq_information_H_
#define _OpenAPI_rat_freq_information_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "matching_direction.h"
#include "rat_type.h"
#include "threshold_level.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_rat_freq_information_s OpenAPI_rat_freq_information_t;
typedef struct OpenAPI_rat_freq_information_s {
    bool is_all_freq;
    int all_freq;
    bool is_all_rat;
    int all_rat;
    bool is_freq;
    int freq;
    OpenAPI_rat_type_e rat_type;
    struct OpenAPI_threshold_level_s *svc_exp_threshold;
    struct OpenAPI_matching_direction_s *matching_dir;
} OpenAPI_rat_freq_information_t;

OpenAPI_rat_freq_information_t *OpenAPI_rat_freq_information_create(
    bool is_all_freq,
    int all_freq,
    bool is_all_rat,
    int all_rat,
    bool is_freq,
    int freq,
    OpenAPI_rat_type_e rat_type,
    OpenAPI_threshold_level_t *svc_exp_threshold,
    OpenAPI_matching_direction_t *matching_dir
);
void OpenAPI_rat_freq_information_free(OpenAPI_rat_freq_information_t *rat_freq_information);
OpenAPI_rat_freq_information_t *OpenAPI_rat_freq_information_parseFromJSON(cJSON *rat_freq_informationJSON);
cJSON *OpenAPI_rat_freq_information_convertToJSON(OpenAPI_rat_freq_information_t *rat_freq_information);
OpenAPI_rat_freq_information_t *OpenAPI_rat_freq_information_copy(OpenAPI_rat_freq_information_t *dst, OpenAPI_rat_freq_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_rat_freq_information_H_ */

