/*
 * nni_information.h
 *
 * 
 */

#ifndef _OpenAPI_nni_information_H_
#define _OpenAPI_nni_information_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_nni_information_s OpenAPI_nni_information_t;
#include "ims_address.h"
#include "nni_relationship_mode.h"
#include "nni_session_direction.h"
#include "nni_type.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_nni_information_s {
    OpenAPI_nni_session_direction_e session_direction;
    OpenAPI_nni_type_e n_ni_type;
    OpenAPI_nni_relationship_mode_e relationship_mode;
    bool is_neighbour_node_address_null;
    struct OpenAPI_ims_address_s *neighbour_node_address;
};

OpenAPI_nni_information_t *OpenAPI_nni_information_create(
    OpenAPI_nni_session_direction_e session_direction,
    OpenAPI_nni_type_e n_ni_type,
    OpenAPI_nni_relationship_mode_e relationship_mode,
    bool is_neighbour_node_address_null,
    OpenAPI_ims_address_t *neighbour_node_address
);
void OpenAPI_nni_information_free(OpenAPI_nni_information_t *nni_information);
OpenAPI_nni_information_t *OpenAPI_nni_information_parseFromJSON(cJSON *nni_informationJSON);
cJSON *OpenAPI_nni_information_convertToJSON(OpenAPI_nni_information_t *nni_information);
OpenAPI_nni_information_t *OpenAPI_nni_information_copy(OpenAPI_nni_information_t *dst, OpenAPI_nni_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nni_information_H_ */

