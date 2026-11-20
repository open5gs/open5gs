/*
 * ps_cell_information.h
 *
 * 
 */

#ifndef _OpenAPI_ps_cell_information_H_
#define _OpenAPI_ps_cell_information_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_ps_cell_information_s OpenAPI_ps_cell_information_t;
#include "ecgi.h"
#include "ncgi.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_ps_cell_information_s {
    struct OpenAPI_ncgi_s *nrcgi;
    struct OpenAPI_ecgi_s *ecgi;
};

OpenAPI_ps_cell_information_t *OpenAPI_ps_cell_information_create(
    OpenAPI_ncgi_t *nrcgi,
    OpenAPI_ecgi_t *ecgi
);
void OpenAPI_ps_cell_information_free(OpenAPI_ps_cell_information_t *ps_cell_information);
OpenAPI_ps_cell_information_t *OpenAPI_ps_cell_information_parseFromJSON(cJSON *ps_cell_informationJSON);
cJSON *OpenAPI_ps_cell_information_convertToJSON(OpenAPI_ps_cell_information_t *ps_cell_information);
OpenAPI_ps_cell_information_t *OpenAPI_ps_cell_information_copy(OpenAPI_ps_cell_information_t *dst, OpenAPI_ps_cell_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ps_cell_information_H_ */

