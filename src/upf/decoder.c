/*
 * Copyright (C) 2019-2023 by Sukchan Lee <acetcom@gmail.com>
 *
 * This file is part of Open5GS.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
//
// Created by Fatemeh Shafiei Ardestani on 2024-08-18.
//
#include "decoder.h"
  UpfEventSubscription *parse_subscription_request(const char *body){
  json_error_t error;
  json_t *root = json_loads(body, 0, &error);
  UpfEventSubscription * eventSubscription = NULL;
  //TODO handle this in actuall UPF if(!root);
  json_t *subscription_request = json_object_get(root, "subscription");
  if(json_is_object(subscription_request)){
    eventSubscription = malloc(sizeof(UpfEventSubscription)) ;
    eventSubscription->EventList = NULL;
    json_t *eventList = json_object_get(subscription_request, "eventList");
    if(json_is_array(eventList)){
      size_t index_event_list;
      json_t *upf_event;
      json_array_foreach(eventList, index_event_list, upf_event){
        UpfEvent upfEvent;
        upfEvent.type = getEventType(json_string_value(json_object_get(upf_event, "type"))); // **
        upfEvent.immediateFlag = json_boolean(json_object_get(upf_event, "immediateFlag")); // **
        json_t * measurementTypes_json = json_object_get(upf_event, "measurementTypes");
        size_t num_Mtypes = json_array_size(measurementTypes_json);

        upfEvent.measurementTypes = NULL;
        for (size_t i = 0; i < num_Mtypes; i++) {
          const char * s = json_string_value(json_array_get(measurementTypes_json, i));
          cvector_push_back(upfEvent.measurementTypes,getMeasurementType(s));
        }

        //upfEvent.measurementTypes

        json_t *json_appIds = json_object_get(upf_event, "appIds");
        int num_ids = json_array_size(json_appIds);
        //const char ** appIds = malloc(num_ids * sizeof(char *));
        upfEvent.appIds = NULL;
        for (size_t i = 0; i < num_ids; i++) {
          const char *id_str = json_string_value(json_array_get(json_appIds, i));
          cvector_push_back(upfEvent.appIds, id_str);
        }

        // Making the trafficFilters
        upfEvent.TrafficFilters = NULL;
        json_t* trafficFilters = json_object_get(upf_event, "trafficFilters");
        if(json_is_array(trafficFilters)){

          size_t index_filters;
          json_t *flow_info;
          flow_info = (json_t *)malloc(sizeof(json_t));

          json_array_foreach(trafficFilters, index_filters, flow_info){
            FlowInformation flowInformation;
            flowInformation.flowDescription = json_string_value(json_object_get(flow_info,"flowDescription"));
            flowInformation.packFiltId = json_string_value(json_object_get(flow_info,"packFiltId"));
            flowInformation.packetFilterUsage = json_boolean(json_object_get(upf_event, "packetFilterUsage"));
            flowInformation.tosTrafficClass = json_string_value(json_object_get(flow_info,"tosTrafficClass"));
            flowInformation.spi = json_string_value(json_object_get(flow_info,"spi"));
            flowInformation.flowLabel = json_string_value(json_object_get(flow_info,"flowLabel"));
            flowInformation.fDir = getFlowDirection(json_string_value(json_object_get(flow_info,"flowDirection")));
            //  making EthFlowDescriptio
            EthFlowDescription *ethFlowDescription = malloc(sizeof(EthFlowDescription));
            {
              json_t* ethflow_json = json_object_get(flow_info, "ethFlowDescription");
              ethFlowDescription->destMacAddr = json_string_value(json_object_get(ethflow_json, "destMacAddr"));
              const char* ethType = json_string_value(json_object_get(ethflow_json, "ethType"));
              ethFlowDescription->ethType = ethType;
              const char* fDesc = json_string_value(json_object_get(ethflow_json, "fDesc"));
              ethFlowDescription->fDesc = fDesc;
              FlowDirection fDir = getFlowDirection(json_string_value(json_object_get(ethflow_json, "fDir")));
              ethFlowDescription->fDir = fDir;
              ethFlowDescription->sourceMacAddr = json_string_value(json_object_get(ethflow_json, "sourceMacAddr"));
              json_t* vlan_json = json_object_get(ethflow_json, "vlanTags");
              size_t num_vlan = json_array_size(vlan_json);
              ethFlowDescription->vlanTags = NULL;
              for (size_t i = 0; i < num_vlan; i++) {
                cvector_push_back(ethFlowDescription->vlanTags, json_string_value(json_array_get(vlan_json, i)));
              }
              ethFlowDescription->srcMacAddrEnd = json_string_value(json_object_get(ethflow_json, "srcMacAddrEnd"));
              ethFlowDescription->destMacAddrEnd= json_string_value(json_object_get(ethflow_json, "destMacAddrEnd"));

            }
            flowInformation.ethFlowDescription = ethFlowDescription;
            upfEvent.TrafficFilters = NULL;
            cvector_push_back(upfEvent.TrafficFilters,flowInformation);
          }
        }

        upfEvent.granularityOfMeasurement = getGranularityOfMeasurement(json_string_value(json_object_get(upf_event, "granularityOfMeasurement")));
        json_t *report_info_json = json_object_get(upf_event, "reportingSuggestionInfo");
        upfEvent.reportingSuggestionInfo.reportingUrgency = getReportingUrgency(json_string_value(json_object_get(report_info_json, "reportingUrgency")));
        upfEvent.reportingSuggestionInfo.reportingTimeInfo = json_integer_value(json_object_get(report_info_json, "reportingTimeInfo"));

        cvector_push_back(eventSubscription->EventList, upfEvent);
      }
    }
    eventSubscription->eventNotifyUri = json_string_value(json_object_get(subscription_request, "eventNotifyUri"));
    eventSubscription->notifyCorrelationId = json_string_value(json_object_get(subscription_request, "notifyCorrelationId"));
    eventSubscription->nfId = json_string_value(json_object_get(subscription_request, "nfId"));
    json_t* ueIpAddress_json = json_object_get(subscription_request, "ueIpAddress");
    {
      if(json_is_string(json_object_get(ueIpAddress_json,"ipv4Addr"))){
        eventSubscription->ueIpAddressVersion = getUeIpAddressVersion("V4");
        eventSubscription->ueIpAddress = json_string_value(json_object_get(ueIpAddress_json,"ipv4Addr"));
      }
      else if(json_is_string(json_object_get(ueIpAddress_json,"ipv6Addr"))){
        eventSubscription->ueIpAddressVersion = getUeIpAddressVersion("V6");
        eventSubscription->ueIpAddress = json_string_value(json_object_get(ueIpAddress_json,"ipv6Addr"));
      }
      else if(json_is_string(json_object_get(ueIpAddress_json,"ipv6Prefix"))){
        eventSubscription->ueIpAddressVersion = getUeIpAddressVersion("V6Prefix");
        eventSubscription->ueIpAddress = json_string_value(json_object_get(ueIpAddress_json,"ipv6Prefix"));
      }

    }
    eventSubscription->supi = json_string_value(json_object_get(subscription_request, "supi"));
    eventSubscription->gpsi = json_string_value(json_object_get(subscription_request, "gpsi"));
    eventSubscription->pei = json_string_value(json_object_get(subscription_request, "pei"));
    eventSubscription->anyUe = json_boolean(json_object_get(subscription_request, "anyUe"));
    eventSubscription->dnn = json_string_value(json_object_get(subscription_request, "dnn"));
    {
      json_t *reportingMode_json = json_object_get(subscription_request, "eventReportingMode");
      EventReportingMode eventReportingMode;
      eventReportingMode.trigger = getUpfEventTrigger(json_string_value(json_object_get(reportingMode_json, "trigger")));
      eventReportingMode.maxReports = json_integer_value(json_object_get(reportingMode_json, "maxReports"));
      eventReportingMode.sent_reports = 0;
      eventReportingMode.expiry = json_string_value(json_object_get(reportingMode_json, "expiry"));
      eventReportingMode.repPeriod = json_integer_value(json_object_get(reportingMode_json, "repPeriod"));
      eventReportingMode.sampRatio = json_integer_value(json_object_get(reportingMode_json, "sampRatio"));
      eventReportingMode.TimeOfLastReport = 0;
      time(&eventReportingMode.TimeOfSubscription );
      json_t * partitioningCriteria_json = json_object_get(reportingMode_json, "partitioningCriteria");
      eventReportingMode.partitioningCriteria = NULL;

      size_t num_Criteria = json_array_size(partitioningCriteria_json);
      for (size_t i = 0; i < num_Criteria; i++ ){
        cvector_push_back(eventReportingMode.partitioningCriteria, getPartitioningCriteria(json_string_value(json_array_get(partitioningCriteria_json, i))));
      }
      eventReportingMode.notifFlag = getNotificationFlag(json_string_value(json_object_get(reportingMode_json, "notifFlag")));
      json_t* ExcInstructions_json = json_object_get(reportingMode_json, "mutingExcInstructions");
      eventReportingMode.mutingExcInstructions.Subscriptioninstructions = getSubscriptionAction(json_string_value(json_object_get(ExcInstructions_json, "subscription")));
      eventReportingMode.mutingExcInstructions.BufferedNotificationinstructions = getBufferedNotificationsAction(json_string_value(json_object_get(ExcInstructions_json, "bufferedNotifs")));
      eventSubscription->eventReportingMode = eventReportingMode;

    }
    return eventSubscription;
  }
  return eventSubscription;
}