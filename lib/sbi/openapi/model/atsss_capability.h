/*
 * atsss_capability.h
 *
 * 
 */

#ifndef _OpenAPI_atsss_capability_H_
#define _OpenAPI_atsss_capability_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_atsss_capability_s OpenAPI_atsss_capability_t;
typedef struct OpenAPI_atsss_capability_s {
    bool is_atsss_ll;
    int atsss_ll;
    bool is_mptcp;
    int mptcp;
    bool is_rtt_without_pmf;
    int rtt_without_pmf;
} OpenAPI_atsss_capability_t;

OpenAPI_atsss_capability_t *OpenAPI_atsss_capability_create(
    bool is_atsss_ll,
    int atsss_ll,
    bool is_mptcp,
    int mptcp,
    bool is_rtt_without_pmf,
    int rtt_without_pmf
);
void OpenAPI_atsss_capability_free(OpenAPI_atsss_capability_t *atsss_capability);
OpenAPI_atsss_capability_t *OpenAPI_atsss_capability_parseFromJSON(cJSON *atsss_capabilityJSON);
cJSON *OpenAPI_atsss_capability_convertToJSON(OpenAPI_atsss_capability_t *atsss_capability);
OpenAPI_atsss_capability_t *OpenAPI_atsss_capability_copy(OpenAPI_atsss_capability_t *dst, OpenAPI_atsss_capability_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_atsss_capability_H_ */

