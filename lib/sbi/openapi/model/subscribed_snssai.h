/*
 * subscribed_snssai.h
 *
 * 
 */

#ifndef _OpenAPI_subscribed_snssai_H_
#define _OpenAPI_subscribed_snssai_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_subscribed_snssai_s OpenAPI_subscribed_snssai_t;
typedef struct OpenAPI_subscribed_snssai_s {
    struct OpenAPI_snssai_s *subscribed_snssai;
    bool is_default_indication;
    int default_indication;
} OpenAPI_subscribed_snssai_t;

OpenAPI_subscribed_snssai_t *OpenAPI_subscribed_snssai_create(
    OpenAPI_snssai_t *subscribed_snssai,
    bool is_default_indication,
    int default_indication
);
void OpenAPI_subscribed_snssai_free(OpenAPI_subscribed_snssai_t *subscribed_snssai);
OpenAPI_subscribed_snssai_t *OpenAPI_subscribed_snssai_parseFromJSON(cJSON *subscribed_snssaiJSON);
cJSON *OpenAPI_subscribed_snssai_convertToJSON(OpenAPI_subscribed_snssai_t *subscribed_snssai);
OpenAPI_subscribed_snssai_t *OpenAPI_subscribed_snssai_copy(OpenAPI_subscribed_snssai_t *dst, OpenAPI_subscribed_snssai_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_subscribed_snssai_H_ */

