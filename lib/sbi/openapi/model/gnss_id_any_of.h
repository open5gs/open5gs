/*
 * gnss_id_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_gnss_id_any_of_H_
#define _OpenAPI_gnss_id_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_gnss_id_any_of_NULL = 0, OpenAPI_gnss_id_any_of_GPS, OpenAPI_gnss_id_any_of_GALILEO, OpenAPI_gnss_id_any_of_SBAS, OpenAPI_gnss_id_any_of_MODERNIZED_GPS, OpenAPI_gnss_id_any_of_QZSS, OpenAPI_gnss_id_any_of_GLONASS, OpenAPI_gnss_id_any_of_BDS, OpenAPI_gnss_id_any_of_NAVIC } OpenAPI_gnss_id_any_of_e;

char* OpenAPI_gnss_id_any_of_ToString(OpenAPI_gnss_id_any_of_e gnss_id_any_of);

OpenAPI_gnss_id_any_of_e OpenAPI_gnss_id_any_of_FromString(char* gnss_id_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_gnss_id_any_of_H_ */

