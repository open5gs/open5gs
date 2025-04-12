//
// Created by Fatemeh Shafiei Ardestani on 2024-12-10.
//

#ifndef OPEN5GS_SUBSCRIBER_CLIENT_H
#define OPEN5GS_SUBSCRIBER_CLIENT_H
#include "ogs-gtp.h"
#include <microhttpd.h>
#include <jansson.h>
#include <curl/curl.h>

char * make_subscription_request(int period);
void send_subscription_request(void);
#endif //OPEN5GS_SUBSCRIBER_CLIENT_H
