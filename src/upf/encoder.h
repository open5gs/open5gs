//
// Created by Fatemeh Shafiei Ardestani on 2024-07-08.
//

#ifndef REST_API_C_ENCODER_H
#define REST_API_C_ENCODER_H
#include <jansson.h>
#include "./types.h"
#include <stdio.h>
#include "cvector.h"
#include "ogs-gtp.h"

json_t * json_string_or_null(const char* str);
json_t *time_to_json(time_t timestamp);
json_t *serialize_eth_flow_description(const EthFlowDescription *eth);
json_t *serialize_flow_information(const FlowInformation *flow);
json_t *serialize_suggestion_information(ReportingSuggestionInformation Sinfo);
json_t *serialize_upf_event(const UpfEvent *event);
json_t *serialize_event_reporting_mode(const EventReportingMode *mode);
json_t *serialize_snssai(const Snssai snssai);
json_t *serialize_UeIpAddress(const UeIpAddressVersion ueIpAddressVersion, const char* ueIpAddress );
json_t *serialize_upf_event_subscription(const UpfEventSubscription *subscription);
json_t *serialize_created_response(const UpfEventSubscription *subscription,char * subscriptionId );
json_t *serialize_DomainInformation(DomainInformation *domainInfo);
json_t *serialize_VolumeMeasurement(VolumeMeasurement *volumeMeasurement);
json_t *serialize_ThroughputMeasurement(ThroughputMeasurement *throughputMeasurement);
json_t *serialize_ApplicationRelatedInformation(ApplicationRelatedInformation *applicationRelatedInformation);
json_t *serialize_ThroughputStatisticsMeasurement(ThroughputStatisticsMeasurement *throughputStatisticsMeasurement);
json_t *serialize_UserDataUsageMeasurements(UserDataUsageMeasurements *userDataUsageMeasurements);

json_t *serialize_Notification_Item(NotificationItem *notificationItem);
json_t *serialize_callBack(NotificationItem *notificationItem, const char *correlationId , int achievedSampRatio);



#endif //REST_API_C_ENCODER_H
