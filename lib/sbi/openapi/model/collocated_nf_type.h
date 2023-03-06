/*
 * collocated_nf_type.h
 *
 * 
 */

#ifndef _OpenAPI_collocated_nf_type_H_
#define _OpenAPI_collocated_nf_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_collocated_nf_type_NULL = 0, OpenAPI_collocated_nf_type_UPF, OpenAPI_collocated_nf_type_SMF, OpenAPI_collocated_nf_type_MB_UPF, OpenAPI_collocated_nf_type_MB_SMF } OpenAPI_collocated_nf_type_e;

char* OpenAPI_collocated_nf_type_ToString(OpenAPI_collocated_nf_type_e collocated_nf_type);

OpenAPI_collocated_nf_type_e OpenAPI_collocated_nf_type_FromString(char* collocated_nf_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_collocated_nf_type_H_ */

