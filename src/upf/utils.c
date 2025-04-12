//
// Created by Fatemeh Shafiei Ardestani on 2024-08-18.
//
#include "utils.h"
char *simple_message(const char *message_str) {
  char *formatted_message = NULL;

  size_t formatted_message_size = strlen(message_str) + 20;
  formatted_message = (char *)malloc(formatted_message_size);

  if (formatted_message) {
    snprintf(formatted_message, formatted_message_size, "{\"message\": \"%s\"}", message_str);
  }

  return formatted_message;
}

bool validate_route(const char *url, const char *route) {
  return strstr(url, route) != NULL;
}

bool validate_method(const char *method, const char *valid_method) {
  return strcmp(method, valid_method) == 0;
}

HTTP_response validate_result(char *result) {
  if (result == NULL) {
    return (HTTP_response){
            .body = simple_message("Internal server error"),
            .status = INTERNAL_SERVER_ERROR
    };
  }

  return (HTTP_response){
          .body = result,
          .status = OK
  };
}