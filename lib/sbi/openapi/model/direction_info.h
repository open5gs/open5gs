/*
 * direction_info.h
 *
 * Represents the UE direction information.
 */

#ifndef _OpenAPI_direction_info_H_
#define _OpenAPI_direction_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_direction_info_s OpenAPI_direction_info_t;
#include "direction.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_direction_info_s {
    char *supi;
    char *gpsi;
    bool is_num_of_ue;
    int num_of_ue;
    bool is_avr_speed;
    float avr_speed;
    bool is_ratio;
    int ratio;
    OpenAPI_direction_e direction;
};

OpenAPI_direction_info_t *OpenAPI_direction_info_create(
    char *supi,
    char *gpsi,
    bool is_num_of_ue,
    int num_of_ue,
    bool is_avr_speed,
    float avr_speed,
    bool is_ratio,
    int ratio,
    OpenAPI_direction_e direction
);
void OpenAPI_direction_info_free(OpenAPI_direction_info_t *direction_info);
OpenAPI_direction_info_t *OpenAPI_direction_info_parseFromJSON(cJSON *direction_infoJSON);
cJSON *OpenAPI_direction_info_convertToJSON(OpenAPI_direction_info_t *direction_info);
OpenAPI_direction_info_t *OpenAPI_direction_info_copy(OpenAPI_direction_info_t *dst, OpenAPI_direction_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_direction_info_H_ */

