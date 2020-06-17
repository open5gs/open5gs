/*
 * rat_selector.h
 *
 *
 */

#ifndef _OpenAPI_rat_selector_H_
#define _OpenAPI_rat_selector_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_rat_selector_NULL = 0, OpenAPI_rat_selector_E_UTRA, OpenAPI_rat_selector_NR } OpenAPI_rat_selector_e;

char* OpenAPI_rat_selector_ToString(OpenAPI_rat_selector_e rat_selector);

OpenAPI_rat_selector_e OpenAPI_rat_selector_FromString(char* rat_selector);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_rat_selector_H_ */

