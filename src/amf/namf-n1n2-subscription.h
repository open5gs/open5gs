#ifndef AMF_SUBS_H
#define AMF_SUBS_H

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct N1_N2_Subscription_s {
    ogs_lnode_t     node; 
    uint64_t id;
    char *supi;
    OpenAPI_n1_message_class_e msg_type;
    char *notify_url;
    ogs_sbi_client_t *client;
} N1_N2_Subscription_t;

typedef struct N1_N2_Notify_Data_s {
    ogs_sbi_message_t *msg;
    ogs_sbi_request_t *req;
} N1_N2_Notify_Data_t;

uint64_t amf_ue_n1_n2_subscription_get_next_id(void);
void amf_ue_n1_n2_subscription_add(amf_ue_t *ue,N1_N2_Subscription_t *newSub);
int amf_ue_n1_n2_subscription_get_all_by_type(amf_ue_t *ue, OpenAPI_n1_message_class_e type, ogs_list_t *retListPtr);
N1_N2_Subscription_t *amf_ue_n1_n2_subscription_get_by_id(amf_ue_t *ue, uint64_t id);
int amf_ue_n1_n2_subscription_forward(amf_ue_t *ue, int payloadContainerType, ogs_nas_payload_container_t *payloadContainer);
bool amf_ue_n1_n2_send_request(N1_N2_Subscription_t *sub, ogs_nas_payload_container_t *data, OpenAPI_n1_message_class_e messageClass);
int amf_ue_n1_n2_request_callback(int status, ogs_sbi_response_t *response, void *data);
ogs_sbi_client_t *amf_ue_n1_n2_allocate_client(char *uri);
OpenAPI_n1_message_class_e amf_get_sbi_message_class_by_payload_container_type(int payloadContainerType);

int amf_ue_n1_n2_subscription_handle_delete(amf_ue_t *ue, uint64_t id);
void amf_ue_n1_n2_subscriptions_delete(amf_ue_t *amf_ue);
void amf_ue_n1_n2_subscription_free(N1_N2_Subscription_t *sub);

#ifdef __cplusplus
}
#endif

#endif 