/*
 * charging_characteristics_selection_mode.h
 *
 * 
 */

#ifndef _OpenAPI_charging_characteristics_selection_mode_H_
#define _OpenAPI_charging_characteristics_selection_mode_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_charging_characteristics_selection_mode_NULL = 0, OpenAPI_charging_characteristics_selection_mode_HOME_DEFAULT, OpenAPI_charging_characteristics_selection_mode_ROAMING_DEFAULT, OpenAPI_charging_characteristics_selection_mode_VISITING_DEFAULT } OpenAPI_charging_characteristics_selection_mode_e;

char* OpenAPI_charging_characteristics_selection_mode_ToString(OpenAPI_charging_characteristics_selection_mode_e charging_characteristics_selection_mode);

OpenAPI_charging_characteristics_selection_mode_e OpenAPI_charging_characteristics_selection_mode_FromString(char* charging_characteristics_selection_mode);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_charging_characteristics_selection_mode_H_ */

