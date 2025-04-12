//
// Created by Fatemeh Shafiei Ardestani on 2024-12-10.
//

#include "subscriber_client.h"
#include "ogs-gtp.h"
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
  size_t realsize = size * nmemb;
  printf("%.*s", (int)realsize, (char *)contents); // Print the response data
  return realsize;
}
char * make_subscription_request(int period){
  json_t *payload = json_object();
  json_object_set_new(payload, "notificationURI", json_string("http://smf2-nsmf.open5gs.svc.cluster.local:8081/notification"));
  json_t *eventSubscriptions = json_array();
  json_t *eventSubscription = json_object();
  json_object_set_new(eventSubscription, "event", json_string("ABNORMAL_BEHAVIOUR"));
  json_t *excepRequs = json_array();
  json_t *excepRequsEntry = json_object();
  json_object_set_new(excepRequsEntry, "excepId", json_string("SUSPICION_OF_DDOS_ATTACK"));
  json_array_append_new(excepRequs, excepRequsEntry);
  json_object_set_new(eventSubscription, "excepRequs", excepRequs);
  json_object_set_new(eventSubscription, "notificationMethod", json_string("PERIODIC"));
  json_object_set_new(eventSubscription, "repetitionPeriod", json_integer(period));
  json_array_append_new(eventSubscriptions, eventSubscription);
  json_object_set_new(payload, "eventSubscriptions", eventSubscriptions);
  char *jsonString = json_dumps(payload, JSON_INDENT(4));
  fatemeh_log("the jsonString is %s", jsonString);
  return jsonString;

}
void send_subscription_request(void){
  json_t *payload = json_object();
  json_object_set_new(payload, "notificationURI", json_string("http://smf2-nsmf.open5gs.svc.cluster.local:8081/notification"));
  char * json_payload = make_subscription_request(1);
  CURL *curl;
  CURLcode res;
  curl_global_init(CURL_GLOBAL_DEFAULT);
  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, "http://oai-nwdaf-nbi-gateway:80/nnwdaf-eventssubscription/v1/subscriptions");
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_payload);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, -1L);
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Expect:"); // To Disable 100 continue
    headers = curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
    res = curl_easy_perform(curl);
    if (res != CURLE_OK)
      fatemeh_log("curl_easy_perform() failed: \n");

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    fatemeh_log("[smf_subscriber_client] after sending the subscription request");
  }
}
