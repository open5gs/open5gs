/*
 * tsc_assistance_information.h
 *
 * TSC Assistance Information
 */

#ifndef _OpenAPI_tsc_assistance_information_H_
#define _OpenAPI_tsc_assistance_information_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_tsc_assistance_information_s OpenAPI_tsc_assistance_information_t;
#include "n6_jitter_information.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_tsc_assistance_information_s {
    bool is_periodicity;
    int periodicity;
    struct OpenAPI_n6_jitter_information_s *n6_jitter_information;
};

OpenAPI_tsc_assistance_information_t *OpenAPI_tsc_assistance_information_create(
    bool is_periodicity,
    int periodicity,
    OpenAPI_n6_jitter_information_t *n6_jitter_information
);
void OpenAPI_tsc_assistance_information_free(OpenAPI_tsc_assistance_information_t *tsc_assistance_information);
OpenAPI_tsc_assistance_information_t *OpenAPI_tsc_assistance_information_parseFromJSON(cJSON *tsc_assistance_informationJSON);
cJSON *OpenAPI_tsc_assistance_information_convertToJSON(OpenAPI_tsc_assistance_information_t *tsc_assistance_information);
OpenAPI_tsc_assistance_information_t *OpenAPI_tsc_assistance_information_copy(OpenAPI_tsc_assistance_information_t *dst, OpenAPI_tsc_assistance_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_tsc_assistance_information_H_ */

