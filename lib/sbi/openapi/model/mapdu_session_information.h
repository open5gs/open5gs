/*
 * mapdu_session_information.h
 *
 * 
 */

#ifndef _OpenAPI_mapdu_session_information_H_
#define _OpenAPI_mapdu_session_information_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_mapdu_session_information_s OpenAPI_mapdu_session_information_t;
#include "atsss_capability.h"
#include "ma_pdu_indication.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_mapdu_session_information_s {
    OpenAPI_ma_pdu_indication_e m_apdu_session_indicator;
    struct OpenAPI_atsss_capability_s *a_tsss_capability;
};

OpenAPI_mapdu_session_information_t *OpenAPI_mapdu_session_information_create(
    OpenAPI_ma_pdu_indication_e m_apdu_session_indicator,
    OpenAPI_atsss_capability_t *a_tsss_capability
);
void OpenAPI_mapdu_session_information_free(OpenAPI_mapdu_session_information_t *mapdu_session_information);
OpenAPI_mapdu_session_information_t *OpenAPI_mapdu_session_information_parseFromJSON(cJSON *mapdu_session_informationJSON);
cJSON *OpenAPI_mapdu_session_information_convertToJSON(OpenAPI_mapdu_session_information_t *mapdu_session_information);
OpenAPI_mapdu_session_information_t *OpenAPI_mapdu_session_information_copy(OpenAPI_mapdu_session_information_t *dst, OpenAPI_mapdu_session_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_mapdu_session_information_H_ */

