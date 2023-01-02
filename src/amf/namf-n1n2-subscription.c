#include "namf-n1n2-subscription.h"

#include "context.h"

uint64_t actualSubscriptionId = 1;

uint64_t amf_ue_n1_n2_subscription_get_next_id(){
    if(actualSubscriptionId == 0){
        //TODO: What to do here?
        ogs_error("EXCEEDED MAXIMUM SUBSCRIBERS!");
        return ++actualSubscriptionId;
    }
    return actualSubscriptionId++;
}

void amf_ue_n1_n2_subscription_add(amf_ue_t *ue, N1_N2_Subscription_t *newSub){
    newSub->client = amf_ue_n1_n2_allocate_client(newSub->notify_url);
    ogs_list_add(&ue->n1_n2_subscriptions,&newSub->node);
}

int amf_ue_n1_n2_subscription_get_all_by_type(amf_ue_t *ue, OpenAPI_n1_message_class_e type, ogs_list_t *retListPtr){
    N1_N2_Subscription_t *spec = NULL;
    N1_N2_Subscription_t *next = NULL;
    
    int count = 0;

    ogs_list_for_each_entry_safe(&(ue->n1_n2_subscriptions), next, spec, node) {
        if(spec->msg_type == type){
            count++;
            ogs_list_add(retListPtr,&spec->node);
        }
    }

    return count;
}

N1_N2_Subscription_t *amf_ue_n1_n2_subscription_get_by_id(amf_ue_t *ue, uint64_t id){
    N1_N2_Subscription_t *sub = NULL;

    N1_N2_Subscription_t *spec = NULL;
    N1_N2_Subscription_t *next = NULL;

    ogs_list_for_each_entry_safe(&(ue->n1_n2_subscriptions), next, spec, node) {
        if(spec->id == id){
            sub = spec;
            break;
        }
    }

    return sub;
}

int amf_ue_n1_n2_subscription_handle_delete(amf_ue_t *ue, uint64_t id){
    N1_N2_Subscription_t *sub = amf_ue_n1_n2_subscription_get_by_id(ue,id);
    if(!sub){
        return OGS_ERROR;
    }
    ogs_list_remove(&ue->n1_n2_subscriptions,sub);
    amf_ue_n1_n2_subscription_free(sub);
    return OGS_OK;
}

OpenAPI_n1_message_class_e amf_get_sbi_message_class_by_payload_container_type(int payloadContainerType){
    switch(payloadContainerType){
        case OGS_NAS_PAYLOAD_CONTAINER_LPP:
            return OpenAPI_n1_message_class_LPP;
        case OGS_NAS_PAYLOAD_CONTAINER_SMS:
            return OpenAPI_n1_message_class_SMS;
        default:
            ogs_error("INVALID PAYLOAD CONTAINER TYPE %d",payloadContainerType);
            ogs_assert_if_reached();
    }
    return OpenAPI_n1_message_class_NULL;
}


int amf_ue_n1_n2_subscription_forward(amf_ue_t *ue, int payloadContainerType, ogs_nas_payload_container_t *payloadContainer){
    ogs_list_t found;
    ogs_list_init(&found);

    OpenAPI_n1_message_class_e msgType = amf_get_sbi_message_class_by_payload_container_type(payloadContainerType);

    int count = amf_ue_n1_n2_subscription_get_all_by_type(ue,msgType,&found);

    N1_N2_Subscription_t *spec = NULL;
    N1_N2_Subscription_t *next = NULL;

    if(count > 0){
        ogs_list_for_each_entry_safe(&(found), next, spec, node) {
            amf_ue_n1_n2_send_request(spec, payloadContainer, msgType);       
        }
    }
    ogs_list_empty(&found);
    return OGS_OK;
}

ogs_sbi_client_t *amf_ue_n1_n2_allocate_client(char *uri){
    //TODO: is this the correct way to init a http client?
    //TODO: is this a good practice to initiate the client when the 
    //      callback is registered?
    bool rc=false;
    OpenAPI_uri_scheme_e scheme = OpenAPI_uri_scheme_NULL;
    ogs_sockaddr_t *addr = NULL;
    ogs_sbi_client_t *client = NULL;

    rc = ogs_sbi_getaddr_from_uri(&scheme, &addr, uri);
    if (rc == false || scheme == OpenAPI_uri_scheme_NULL) {
        ogs_error("Invalid URL [%s]",
                uri);
        return NULL;
    }

    client = ogs_sbi_client_add(scheme, addr);
    ogs_free(addr);

    return client;
}

bool amf_ue_n1_n2_send_request(N1_N2_Subscription_t *sub, ogs_nas_payload_container_t *data, OpenAPI_n1_message_class_e messageClass){

    bool rc = false;

    const char *binRefStack = "amf-n1data"; //STATIC ID
    char *binRefHeap = ogs_strdup(binRefStack);

    char *subscriptionId = ogs_uint64_to_string(sub->id);

    OpenAPI_ref_to_binary_data_t *ref2bin = OpenAPI_ref_to_binary_data_create(binRefHeap);

    OpenAPI_n1_message_container_t *n1container = OpenAPI_n1_message_container_create(
        messageClass,
        ref2bin,
        NULL,
        NULL
    );
    //TODO: are more fields required?
    OpenAPI_n1_message_notification_t *n1_mess_not = OpenAPI_n1_message_notification_create(
        subscriptionId,
        n1container,
        NULL,
        NULL,
        NULL,
        NULL,
        false,
        0,
        NULL,
        NULL
    );
    
    ogs_sbi_message_t *msg;
    //TODO: I wasn't able to use a new message from the stack
    msg = ogs_calloc(1,sizeof(ogs_sbi_message_t));
    msg->N1MessageNotification = n1_mess_not;

    ogs_sbi_part_t part0;
    part0.content_id = binRefHeap;
    part0.content_type = ogs_strdup(OGS_SBI_CONTENT_5GNAS_TYPE); //Is this correct?

    ogs_pkbuf_t *pkbuf;
    pkbuf = ogs_pkbuf_alloc(NULL,  data->length);

    ogs_pkbuf_put_data(pkbuf,
                data->buffer, data->length);

    part0.pkbuf = pkbuf;


    msg->num_of_part = 1;
    msg->part[0] = part0;

    msg->h.uri = ogs_strdup(sub->notify_url);
    msg->h.method = ogs_strdup(OGS_SBI_HTTP_METHOD_POST);

    ogs_sbi_request_t *request = ogs_sbi_build_request(msg);

    N1_N2_Notify_Data_t *datap;
    datap = ogs_calloc(1,sizeof(N1_N2_Notify_Data_t));
    datap->msg = msg;
    datap->req = request;

    rc = ogs_sbi_client_send_request(
            sub->client, amf_ue_n1_n2_request_callback, request, datap);

    if(rc == false){
        ogs_error("Invalid Response for request");
        //TODO: I'm cleaning up in callback, is there a possibility
        //      that the callback might not get called?
    }

    return rc;
}

int amf_ue_n1_n2_request_callback(
        int status, ogs_sbi_response_t *response, void *data)
{   

    N1_N2_Notify_Data_t *datap = data;
    
    //TODO: I tested it and I needed to do this extra cleanup stuff
    //      what am I doing wrong / what do I need to change?
    int i=0;
    for(i=0;i<datap->msg->num_of_part;i++){
        //Dont need to clean content_id since OpenAPI_ref_to_binary_data_free should do it
        if(datap->msg->part[i].content_type)
            ogs_free(datap->msg->part[i].content_type);
    }
    //They are not deleted by ogs_sbi_message_free (?)
    if(datap->msg->h.method)
        ogs_free(datap->msg->h.method);
    if(datap->msg->h.uri)
        ogs_free(datap->msg->h.uri);
    

    ogs_sbi_message_free(datap->msg);
    ogs_sbi_request_free(datap->req);

    //Since I create message on heap, I need to free it
    ogs_free(datap->msg);
    ogs_free(datap);

    if (status != OGS_OK) {
        ogs_log_message(
                status == OGS_DONE ? OGS_LOG_DEBUG : OGS_LOG_WARN, 0,
                "ogs_sbi_client_handler() failed [%d]", status);
        return OGS_ERROR;
    }

    ogs_assert(response);

    //Ignoring response for now..
    //TODO: do something with it?
    //Expect 204 (NO_CONTENT) but maybe handle error / log something
    ogs_sbi_response_free(response);

    return OGS_OK;
}

void amf_ue_n1_n2_subscriptions_delete(amf_ue_t *amf_ue){
    N1_N2_Subscription_t *spec = NULL;
    N1_N2_Subscription_t *next = NULL;

    ogs_list_for_each_entry_safe(&(amf_ue->n1_n2_subscriptions), next, spec, node) {
        amf_ue_n1_n2_subscription_free(spec);
    }
}

void amf_ue_n1_n2_subscription_free(N1_N2_Subscription_t *sub){
    
    ogs_sbi_client_remove(sub->client);
    
    if(sub->notify_url)
        ogs_free(sub->notify_url);
    if(sub->supi)
        ogs_free(sub->supi);

    ogs_free(sub);
}