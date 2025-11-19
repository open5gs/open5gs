//
// Created by Fatemeh Shafiei Ardestani on 2024-07-07.
//

#ifndef REST_API_C_DECODER_H
#define REST_API_C_DECODER_H
#include "./types.h"
#include <jansson.h>
#include "cvector.h"

UpfEventSubscription *parse_subscription_request(const char *body);


#endif //REST_API_C_DECODER_H
