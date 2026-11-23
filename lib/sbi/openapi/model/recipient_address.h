/*
 * recipient_address.h
 *
 * 
 */

#ifndef _OpenAPI_recipient_address_H_
#define _OpenAPI_recipient_address_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_recipient_address_s OpenAPI_recipient_address_t;
#include "sm_address_info.h"
#include "sm_addressee_type.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_recipient_address_s {
    struct OpenAPI_sm_address_info_s *recipient_address_info;
    OpenAPI_sm_addressee_type_e s_maddressee_type;
};

OpenAPI_recipient_address_t *OpenAPI_recipient_address_create(
    OpenAPI_sm_address_info_t *recipient_address_info,
    OpenAPI_sm_addressee_type_e s_maddressee_type
);
void OpenAPI_recipient_address_free(OpenAPI_recipient_address_t *recipient_address);
OpenAPI_recipient_address_t *OpenAPI_recipient_address_parseFromJSON(cJSON *recipient_addressJSON);
cJSON *OpenAPI_recipient_address_convertToJSON(OpenAPI_recipient_address_t *recipient_address);
OpenAPI_recipient_address_t *OpenAPI_recipient_address_copy(OpenAPI_recipient_address_t *dst, OpenAPI_recipient_address_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_recipient_address_H_ */

