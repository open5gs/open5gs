/*
 * model_5_g_multicast_service.h
 *
 * 
 */

#ifndef _OpenAPI_model_5_g_multicast_service_H_
#define _OpenAPI_model_5_g_multicast_service_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_model_5_g_multicast_service_s OpenAPI_model_5_g_multicast_service_t;
#include "mbs_session_id.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_model_5_g_multicast_service_s {
    OpenAPI_list_t *m_bs_session_id_list;
};

OpenAPI_model_5_g_multicast_service_t *OpenAPI_model_5_g_multicast_service_create(
    OpenAPI_list_t *m_bs_session_id_list
);
void OpenAPI_model_5_g_multicast_service_free(OpenAPI_model_5_g_multicast_service_t *model_5_g_multicast_service);
OpenAPI_model_5_g_multicast_service_t *OpenAPI_model_5_g_multicast_service_parseFromJSON(cJSON *model_5_g_multicast_serviceJSON);
cJSON *OpenAPI_model_5_g_multicast_service_convertToJSON(OpenAPI_model_5_g_multicast_service_t *model_5_g_multicast_service);
OpenAPI_model_5_g_multicast_service_t *OpenAPI_model_5_g_multicast_service_copy(OpenAPI_model_5_g_multicast_service_t *dst, OpenAPI_model_5_g_multicast_service_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_model_5_g_multicast_service_H_ */

