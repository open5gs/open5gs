/*
 * analytics_subset.h
 *
 * Possible values are: - NUM_OF_UE_REG: The number of UE registered. This value is only applicable to NSI_LOAD_LEVEL event. - NUM_OF_PDU_SESS_ESTBL: The number of PDU sessions established. This value is only applicable to NSI_LOAD_LEVEL event. - RES_USAGE: The current usage of the virtual resources assigned to the NF instances belonging to a particular network slice instance. This value is only applicable to NSI_LOAD_LEVEL event. - NUM_OF_EXCEED_RES_USAGE_LOAD_LEVEL_THR: The number of times the resource usage threshold of the network slice instance is reached or exceeded if a threshold value is provided by the consumer. This value is only applicable to NSI_LOAD_LEVEL event. - PERIOD_OF_EXCEED_RES_USAGE_LOAD_LEVEL_THR: The time interval between each time the threshold being met or exceeded on the network slice (instance). This value is only applicable to NSI_LOAD_LEVEL event. - EXCEED_LOAD_LEVEL_THR_IND: Whether the Load Level Threshold is met or exceeded by the statistics value. This value is only applicable to NSI_LOAD_LEVEL event. - LIST_OF_TOP_APP_UL: The list of applications that contribute the most to the traffic in the UL direction. This value is only applicable to USER_DATA_CONGESTION event. - LIST_OF_TOP_APP_DL: The list of applications that contribute the most to the traffic in the DL direction. This value is only applicable to USER_DATA_CONGESTION event. - NF_STATUS: The availability status of the NF on the Analytics target period, expressed as a percentage of time per status value (registered, suspended, undiscoverable). This value is only applicable to NF_LOAD event. - NF_RESOURCE_USAGE: The average usage of assigned resources (CPU, memory, storage). This value is only applicable to NF_LOAD event. - NF_LOAD: The average load of the NF instance over the Analytics target period. This value is only applicable to NF_LOAD event. - NF_PEAK_LOAD: The maximum load of the NF instance over the Analytics target period. This value is only applicable to NF_LOAD event. - NF_LOAD_AVG_IN_AOI: The average load of the NF instances over the area of interest. This value is only applicable to NF_LOAD event. - DISPER_AMOUNT: Indicates the dispersion amount of the reported data volume or transaction dispersion type. This value is only applicable to DISPERSION event. - DISPER_CLASS: Indicates the dispersion mobility class: fixed, camper, traveller upon set its usage threshold, and/or the top-heavy class upon set its percentile rating threshold. This value is only applicable to DISPERSION event. - RANKING: Data/transaction usage ranking high (i.e.value 1), medium (2) or low (3). This value is only applicable to DISPERSION event. - PERCENTILE_RANKING: Percentile ranking of the target UE in the Cumulative Distribution Function of data usage for the population of all UEs. This value is only applicable to DISPERSION event. - RSSI: Indicated the RSSI in the unit of dBm. This value is only applicable to WLAN_PERFORMANCE event. - RTT: Indicates the RTT in the unit of millisecond. This value is only applicable to WLAN_PERFORMANCE event. - TRAFFIC_INFO: Traffic information including UL/DL data rate and/or Traffic volume. This value is only applicable to WLAN_PERFORMANCE event. - NUMBER_OF_UES: Number of UEs observed for the SSID. This value is only applicable to WLAN_PERFORMANCE event. - APP_LIST_FOR_UE_COMM: The analytics of the application list used by UE. This value is only applicable to UE_COMM event. - N4_SESS_INACT_TIMER_FOR_UE_COMM: The N4 Session inactivity timer. This value is only applicable to UE_COMM event. - AVG_TRAFFIC_RATE: Indicates average traffic rate. This value is only applicable to DN_PERFORMANCE event. - MAX_TRAFFIC_RATE: Indicates maximum traffic rate. This value is only applicable to DN_PERFORMANCE event. - AVG_PACKET_DELAY: Indicates average Packet Delay. This value is only applicable to DN_PERFORMANCE event. - MAX_PACKET_DELAY: Indicates maximum Packet Delay. This value is only applicable to DN_PERFORMANCE event. - AVG_PACKET_LOSS_RATE: Indicates average Loss Rate. This value is only applicable to DN_PERFORMANCE event. - UE_LOCATION: Indicates UE location information. This value is only applicable to SERVICE_EXPERIENCE event. - LIST_OF_HIGH_EXP_UE: Indicates list of high experienced UE. This value is only applicable to SM_CONGESTION event. - LIST_OF_MEDIUM_EXP_UE: Indicates list of medium experienced UE. This value is only applicable to SM_CONGESTION event. - LIST_OF_LOW_EXP_UE: Indicates list of low experienced UE. This value is only applicable to SM_CONGESTION event. - AVG_UL_PKT_DROP_RATE: Indicates average uplink packet drop rate on GTP-U path on N3. This value is only applicable to RED_TRANS_EXP event. - VAR_UL_PKT_DROP_RATE: Indicates variance of uplink packet drop rate on GTP-U path on N3. This value is only applicable to RED_TRANS_EXP event. - AVG_DL_PKT_DROP_RATE: Indicates average downlink packet drop rate on GTP-U path on N3. This value is only applicable to RED_TRANS_EXP event. - VAR_DL_PKT_DROP_RATE: Indicates variance of downlink packet drop rate on GTP-U path on N3. This value is only applicable to RED_TRANS_EXP event. - AVG_UL_PKT_DELAY: Indicates average uplink packet delay round trip on GTP-U path on N3. This value is only applicable to RED_TRANS_EXP event. - VAR_UL_PKT_DELAY: Indicates variance uplink packet delay round trip on GTP-U path on N3. This value is only applicable to RED_TRANS_EXP event. - AVG_DL_PKT_DELAY: Indicates average downlink packet delay round trip on GTP-U path on N3. This value is only applicable to RED_TRANS_EXP event. - VAR_DL_PKT_DELAY: Indicates variance downlink packet delay round trip on GTP-U path on N3. This value is only applicable to RED_TRANS_EXP event. 
 */

#ifndef _OpenAPI_analytics_subset_H_
#define _OpenAPI_analytics_subset_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "analytics_subset_any_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_analytics_subset_s OpenAPI_analytics_subset_t;
typedef struct OpenAPI_analytics_subset_s {
} OpenAPI_analytics_subset_t;

OpenAPI_analytics_subset_t *OpenAPI_analytics_subset_create(
);
void OpenAPI_analytics_subset_free(OpenAPI_analytics_subset_t *analytics_subset);
OpenAPI_analytics_subset_t *OpenAPI_analytics_subset_parseFromJSON(cJSON *analytics_subsetJSON);
cJSON *OpenAPI_analytics_subset_convertToJSON(OpenAPI_analytics_subset_t *analytics_subset);
OpenAPI_analytics_subset_t *OpenAPI_analytics_subset_copy(OpenAPI_analytics_subset_t *dst, OpenAPI_analytics_subset_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_analytics_subset_H_ */

