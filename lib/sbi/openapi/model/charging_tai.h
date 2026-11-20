/*
 * charging_tai.h
 *
 * 
 */

#ifndef _OpenAPI_charging_tai_H_
#define _OpenAPI_charging_tai_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_charging_tai_s OpenAPI_charging_tai_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_charging_tai_s {
    char *mcc;
    char *mnc;
    char *tac;
};

OpenAPI_charging_tai_t *OpenAPI_charging_tai_create(
    char *mcc,
    char *mnc,
    char *tac
);
void OpenAPI_charging_tai_free(OpenAPI_charging_tai_t *charging_tai);
OpenAPI_charging_tai_t *OpenAPI_charging_tai_parseFromJSON(cJSON *charging_taiJSON);
cJSON *OpenAPI_charging_tai_convertToJSON(OpenAPI_charging_tai_t *charging_tai);
OpenAPI_charging_tai_t *OpenAPI_charging_tai_copy(OpenAPI_charging_tai_t *dst, OpenAPI_charging_tai_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_charging_tai_H_ */

