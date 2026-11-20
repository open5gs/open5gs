/*
 * roaming_qbc_information.h
 *
 * 
 */

#ifndef _OpenAPI_roaming_qbc_information_H_
#define _OpenAPI_roaming_qbc_information_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_roaming_qbc_information_s OpenAPI_roaming_qbc_information_t;
#include "multiple_qf_icontainer.h"
#include "roaming_charging_profile.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_roaming_qbc_information_s {
    OpenAPI_list_t *multiple_qf_icontainer;
    char *u_pfid;
    struct OpenAPI_roaming_charging_profile_s *roaming_charging_profile;
};

OpenAPI_roaming_qbc_information_t *OpenAPI_roaming_qbc_information_create(
    OpenAPI_list_t *multiple_qf_icontainer,
    char *u_pfid,
    OpenAPI_roaming_charging_profile_t *roaming_charging_profile
);
void OpenAPI_roaming_qbc_information_free(OpenAPI_roaming_qbc_information_t *roaming_qbc_information);
OpenAPI_roaming_qbc_information_t *OpenAPI_roaming_qbc_information_parseFromJSON(cJSON *roaming_qbc_informationJSON);
cJSON *OpenAPI_roaming_qbc_information_convertToJSON(OpenAPI_roaming_qbc_information_t *roaming_qbc_information);
OpenAPI_roaming_qbc_information_t *OpenAPI_roaming_qbc_information_copy(OpenAPI_roaming_qbc_information_t *dst, OpenAPI_roaming_qbc_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_roaming_qbc_information_H_ */

