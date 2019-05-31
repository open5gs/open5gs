#ifndef __PGW_PATH_H__
#define __PGW_PATH_H__

#include "ogs-tun.h"
#include "gtp/gtp_xact.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int pgw_gtp_open();
void pgw_gtp_close();

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __PGW_PATH_H__ */
