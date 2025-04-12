//
// Created by Fatemeh Shafiei Ardestani on 2024-12-10.
//

#ifndef OPEN5GS_SUBSCRIBER_SERVER_H
#define OPEN5GS_SUBSCRIBER_SERVER_H
#include "ogs-gtp.h"
#include <microhttpd.h>
#include <jansson.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>


struct PostData {
    char *data;
    size_t size;
    size_t capacity;
};
typedef struct {
    char ueIP[50];
    int seId;
} UEPduRatioPair;
typedef enum {
    OK = 200,
    CREATED = 201,
    BAD_REQUEST = 400,
    NOT_FOUND = 404,
    INTERNAL_SERVER_ERROR = 500,
    NOT_IMPLEMENTED = 501
} HTTP_status;
typedef struct {
    char *body;
    HTTP_status status;
} HTTP_response;
void get_relase_time(char *buffer, size_t buffer_size);
uint32_t convert_ipv4_string_to_uint32(const char *key_src);
void parse_supi(const char *supi, char **ue_Ip, int *seid);
typedef struct MHD_Response* RSPX;
RSPX HTTP_build_response_JSON(const char *message);
RSPX HTTP_build_created_response_JSON(const char *message, const char* newSubId, char * url_str);
char *simple_message(const char *message_str);
void log_api(const char *url, const char *method);
HTTP_response evaluate_notification(const char *body);
enum MHD_Result default_handler(void *cls, struct MHD_Connection *connection, const char *url,
                                const char *method, const char *version, const char *upload_data,
                                size_t *upload_data_size, void **con_cls);
void *subscriber_server(void*);
#endif //OPEN5GS_SUBSCRIBER_SERVER_H
