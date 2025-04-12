#include <string.h>
#include <stdio.h>
#include "ee_event.h"
#include "decoder.h"
#include "encoder.h"
#include <jansson.h>
#include "cvector_utils.h"
#include "utils.h"
#include <microhttpd.h>
#include "types.h"

typedef struct MHD_Response* RSPX;

HTTP_response create_subscription(const char *body, bool *created, char **newSubId);
HTTP_response subscription_router(const char *url, const char *method, const char *body, char *subscription_id, bool *created,
                                  char **newSubId);
RSPX HTTP_build_response_JSON(const char *message);
RSPX HTTP_build_created_response_JSON(const char *message, const char* newSubId, char * url_str);