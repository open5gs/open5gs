#ifndef __GX_LIB_H__
#define __GX_LIB_H__

#include "freeDiameter/freeDiameter-host.h"
#include "freeDiameter/libfdcore.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

extern struct dict_object *gx_application;

extern struct dict_object *gx_cmd_ccr;
extern struct dict_object *gx_cmd_cca;

int gx_dict_init(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ! __GX_LIB_H__ */
