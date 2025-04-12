//
// Created by Fatemeh Shafiei Ardestani on 2024-08-18.
//
#include "types.h"
UeIpAddressVersion getUeIpAddressVersion(const char *s) {
  if (strcmp(s, "V4") == 0) return V4;
  if (strcmp(s, "V6") == 0) return V6;
  if (strcmp(s, "V6Prefix") == 0) return V6Prefix;
  return V4;
}

UpfEventTrigger getUpfEventTrigger(const char *s) {
  if (strcmp(s, "ONE_TIME") == 0) return ONE_TIME;
  if (strcmp(s, "PERIODIC") == 0) return PERIODIC;
  return ONE_TIME;
}

PartitioningCriteria getPartitioningCriteria(const char *s) {
  if (strcmp(s, "TAC") == 0) return TAC;
  if (strcmp(s, "SUBPLMN") == 0) return SUBPLMN;
  if (strcmp(s, "GEOAREA") == 0) return GEOAREA;
  if (strcmp(s, "SNSSAI") == 0) return SNSSAI;
  if (strcmp(s, "DNN") == 0) return DNN;
  return TAC;
}

NotificationFlag getNotificationFlag(const char *s) {
  if (strcmp(s, "ACTIVATE") == 0) return ACTIVATE;
  if (strcmp(s, "DEACTIVATE") == 0) return DEACTIVATE;
  if (strcmp(s, "RETRIEVAL") == 0) return RETRIEVAL;
  return ACTIVATE;
}

BufferedNotificationsAction getBufferedNotificationsAction(const char *s) {
  if (strcmp(s, "SEND_ALL") == 0) return SEND_ALL;
  if (strcmp(s, "DISCARD_ALL") == 0) return DISCARD_ALL;
  if (strcmp(s, "DROP_OLD") == 0) return DROP_OLD;
  return SEND_ALL;
}

SubscriptionAction getSubscriptionAction(const char *s) {
  if (strcmp(s, "CLOSE") == 0) return CLOSE;
  if (strcmp(s, "CONTINUE_WITH_MUTING") == 0) return CONTINUE_WITH_MUTING;
  if (strcmp(s, "CONTINUE_WITHOUT_MUTING") == 0) return CONTINUE_WITHOUT_MUTING;
  return CLOSE;
}

EventType getEventType(const char *s) {
  if (strcmp(s, "QOS_MONITORING") == 0) return QOS_MONITORING;
  if (strcmp(s, "USER_DATA_USAGE_MEASURES") == 0) return USER_DATA_USAGE_MEASURES;
  if (strcmp(s, "USER_DATA_USAGE_TRENDS") == 0) return USER_DATA_USAGE_TRENDS;
  if (strcmp(s, "TSC_MNGT_INFO") == 0) return TSC_MNGT_INFO;
  return QOS_MONITORING;
}

MeasurementType getMeasurementType(const char *s) {
  if (strcmp(s, "VOLUME_MEASUREMENT") == 0) return VOLUME_MEASUREMENT;
  if (strcmp(s, "THROUGHPUT_MEASUREMENT") == 0) return THROUGHPUT_MEASUREMENT;
  if (strcmp(s, "APPLICATION_RELATED_INFO") == 0) return APPLICATION_RELATED_INFO;
  return VOLUME_MEASUREMENT;
}

FlowDirection getFlowDirection(const char *s) {
  if (strcmp(s, "DOWNLINK") == 0) return DOWNLINK;
  if (strcmp(s, "UPLINK") == 0) return UPLINK;
  if (strcmp(s, "BIDIRECTIONAL") == 0) return BIDIRECTIONAL;
  if (strcmp(s, "UNSPECIFIED") == 0) return UNSPECIFIED;
  return UNSPECIFIED;
}

GranularityOfMeasurement getGranularityOfMeasurement(const char *s) {
  if (strcmp(s, "PER_APPLICATION") == 0) return PER_APPLICATION;
  if (strcmp(s, "PER_SESSION") == 0) return PER_SESSION;
  if (strcmp(s, "PER_FLOW") == 0) return PER_FLOW;
  return PER_APPLICATION;
}

ReportingUrgency getReportingUrgency(const char *s) {
  if (strcmp(s, "DELAY_TOLERANT") == 0) return DELAY_TOLERANT;
  if (strcmp(s, "NON_DELAY_TOLERANT") == 0) return NON_DELAY_TOLERANT;
  return DELAY_TOLERANT;
}

const char* getUpfEventTriggerString(UpfEventTrigger trigger) {
  switch (trigger) {
    case ONE_TIME: return "ONE_TIME";
    case PERIODIC: return "PERIODIC";
    default: return "ONE_TIME";
  }
}

const char* getPartitioningCriteriaString(PartitioningCriteria criteria) {
  switch (criteria) {
    case TAC: return "TAC";
    case SUBPLMN: return "SUBPLMN";
    case GEOAREA: return "GEOAREA";
    case SNSSAI: return "SNSSAI";
    case DNN: return "DNN";
    default: return "TAC";
  }
}

const char* getNotificationFlagString(NotificationFlag flag) {
  switch (flag) {
    case ACTIVATE: return "ACTIVATE";
    case DEACTIVATE: return "DEACTIVATE";
    case RETRIEVAL: return "RETRIEVAL";
    default: return "ACTIVATE";
  }
}

const char* getBufferedNotificationsActionString(BufferedNotificationsAction action) {
  switch (action) {
    case SEND_ALL: return "SEND_ALL";
    case DISCARD_ALL: return "DISCARD_ALL";
    case DROP_OLD: return "DROP_OLD";
    default: return "SEND_ALL";
  }
}

const char* getSubscriptionActionString(SubscriptionAction action) {
  switch (action) {
    case CLOSE: return "CLOSE";
    case CONTINUE_WITH_MUTING: return "CONTINUE_WITH_MUTING";
    case CONTINUE_WITHOUT_MUTING: return "CONTINUE_WITHOUT_MUTING";
    default: return "CLOSE";
  }
}

const char* getEventTypeString(EventType type) {
  switch (type) {
    case QOS_MONITORING: return "QOS_MONITORING";
    case USER_DATA_USAGE_MEASURES: return "USER_DATA_USAGE_MEASURES";
    case USER_DATA_USAGE_TRENDS: return "USER_DATA_USAGE_TRENDS";
    case TSC_MNGT_INFO: return "TSC_MNGT_INFO";
    default: return "QOS_MONITORING";
  }
}

const char* getMeasurementTypeString(MeasurementType type) {
  switch (type) {
    case VOLUME_MEASUREMENT: return "VOLUME_MEASUREMENT";
    case THROUGHPUT_MEASUREMENT: return "THROUGHPUT_MEASUREMENT";
    case APPLICATION_RELATED_INFO: return "APPLICATION_RELATED_INFO";
    default: return "VOLUME_MEASUREMENT";
  }
}

const char* getFlowDirectionString(FlowDirection direction) {
  switch (direction) {
    case DOWNLINK: return "DOWNLINK";
    case UPLINK: return "UPLINK";
    case BIDIRECTIONAL: return "BIDIRECTIONAL";
    case UNSPECIFIED: return "UNSPECIFIED";
    default: return "UNSPECIFIED";
  }
}

const char* getGranularityOfMeasurementString(GranularityOfMeasurement granularity) {
  switch (granularity) {
    case PER_APPLICATION: return "PER_APPLICATION";
    case PER_SESSION: return "PER_SESSION";
    case PER_FLOW: return "PER_FLOW";
    default: return "PER_APPLICATION";
  }
}

const char* getReportingUrgencyString(ReportingUrgency urgency) {
  switch (urgency) {
    case DELAY_TOLERANT: return "DELAY_TOLERANT";
    case NON_DELAY_TOLERANT: return "NON_DELAY_TOLERANT";
    default: return "DELAY_TOLERANT";
  }
}


DnProtocol getDnProtocol(const char *s) {
  if (strcmp(s, "DNS_QNAME") == 0) return DNS_QNAME;
  if (strcmp(s, "TLS_SNI") == 0) return TLS_SNI;
  if (strcmp(s, "TLS_SAN") == 0) return TLS_SAN;
  if (strcmp(s, "TLS_SCN") == 0) return TLS_SCN;
  return DNS_QNAME; // Default value if no match is found
}

const char* getDnProtocolString(DnProtocol protocol) {
  switch (protocol) {
    case DNS_QNAME:
      return "DNS_QNAME";
    case TLS_SNI:
      return "TLS_SNI";
    case TLS_SAN:
      return "TLS_SAN";
    case TLS_SCN:
      return "TLS_SCN";
    default:
      return "DNS_QNAME"; // Default string if no match is found
  }
}