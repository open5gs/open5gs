#include "ee_client.h"

__attribute__((__noreturn__)) void* periodic_sending(void*){
  while (1) {
    sleep(1);  // Wait for 1 second before continuing the loop
    EventReport_UDUM(NULL);  // Send the event report
  }
//  return NULL;
}
