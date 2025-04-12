////
//// Created by Fatemeh Shafiei Ardestani on 2024-08-18.
////
//
#ifndef UPG_VPP_EE_CLIENT_H
#define UPG_VPP_EE_CLIENT_H

#include <microhttpd.h>
#include <jansson.h>
#include "types.h"
#include "btree.h"
#include "send_data.h"
#include "ee_event.h"
#include "stb_ds.h"

__attribute__((__noreturn__)) void* periodic_sending(void*);
#endif //UPG_VPP_EE_CLIENT_H
