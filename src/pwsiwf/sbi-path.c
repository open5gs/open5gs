#include "sbi-path.h"
#include "ogs-sbi.h"
#include "context.h"
#include "namf-build.h"
#include "namf-handler.h"

int pwsiws_sbi_open(void) 
{
    // Start all SBI servers using the standard SBI context
    int count = 0;
    ogs_sbi_server_t *server = NULL;
    ogs_list_for_each(&ogs_sbi_self()->server_list, server) {
        count++;
    }
    ogs_info("pwsiws_sbi_open: Starting %d SBI servers", count);
    if (ogs_sbi_server_start_all(pwsiws_sbi_server_callback) != OGS_OK) {
        ogs_error("Failed to start PWS-IWS SBI servers");
        return OGS_ERROR;
    }
    
    // Start SBI client for AMF communication
    if (pwsiws_sbi_client_open() != OGS_OK) {
        ogs_error("Failed to start PWS-IWS SBI client");
        return OGS_ERROR;
    }
    
    return OGS_OK;
}

void pwsiws_sbi_close(void) 
{
    pwsiws_sbi_client_close();
    ogs_sbi_server_stop_all();
    ogs_sbi_server_remove_all();
}

int pwsiws_sbi_server_callback(ogs_sbi_request_t *request, void *data) 
{ 
    ogs_sbi_message_t message;
    ogs_sbi_stream_t *stream = NULL;
    int rv;
    
    ogs_assert(request);
    stream = (ogs_sbi_stream_t *)data;
    ogs_assert(stream);
    
    rv = ogs_sbi_parse_request(&message, request);
    if (rv != OGS_OK) {
        ogs_error("Cannot parse HTTP request");
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                NULL, "Cannot parse HTTP request", NULL, NULL));
        return OGS_ERROR;
    }
    
    ogs_info("PWS-IWS SBI server callback received: %s %s", 
            message.h.method, message.h.service.name);
    
    SWITCH(message.h.service.name)
    CASE(OGS_SBI_SERVICE_NAME_NPWS_CALLBACK)
        SWITCH(message.h.resource.component[0])
        CASE(OGS_SBI_RESOURCE_NAME_N1_N2_FAILURE_NOTIFY)
            SWITCH(message.h.method)
            CASE(OGS_SBI_HTTP_METHOD_POST)
                rv = pwsiws_nonuen2_comm_handle_nonuen2_message_transfer_failure_notify(
                        stream, &message);
                break;
            DEFAULT
                ogs_error("Invalid HTTP method [%s]", message.h.method);
                ogs_assert(true ==
                    ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                        &message, "Invalid HTTP method", NULL, NULL));
                break;
            END
            break;
        DEFAULT
            ogs_error("Invalid resource name [%s]", message.h.resource.component[0]);
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    &message, "Invalid resource name", NULL, NULL));
            break;
        END
        break;
    DEFAULT
        ogs_error("Not supported service [%s]", message.h.service.name);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                &message, "Not supported service", NULL, NULL));
        break;
    END
    
    ogs_sbi_message_free(&message);
    return OGS_OK;
}

int pwsiws_sbi_discover_and_send(ogs_sbi_service_type_e service_type, 
        ogs_sbi_request_t *(*build)(pwsiws_warning_t *warning, void *data),
        pwsiws_warning_t *warning, ogs_sbi_xact_t *xact, void *data) 
{ 
    return OGS_OK; 
}

int pwsiws_sbi_client_open(void) 
{ 
    ogs_info("PWS-IWS SBI client opened");
    return OGS_OK; 
}

void pwsiws_sbi_client_close(void) 
{
    ogs_info("PWS-IWS SBI client closed");
}

int pwsiws_sbi_client_callback(int status, ogs_sbi_response_t *response, void *data) 
{ 
    ogs_sbi_message_t message;
    int rv;
    
    if (status != OGS_OK) {
        ogs_log_message(
                status == OGS_DONE ? OGS_LOG_DEBUG : OGS_LOG_WARN, 0,
                "pwsiws_sbi_client_callback() failed [%d]", status);
        return OGS_ERROR;
    }
    
    if (!response) {
        ogs_error("No response received");
        return OGS_ERROR;
    }
    
    rv = ogs_sbi_parse_response(&message, response);
    if (rv != OGS_OK) {
        ogs_error("Cannot parse HTTP response");
        ogs_sbi_response_free(response);
        return OGS_ERROR;
    }
    
    ogs_info("PWS-IWS SBI client callback: status=%d, service=%s", 
            message.res_status, message.h.service.name);
    
    // Handle the response based on the service and state
    if (data) {
        pwsiws_warning_t *warning = (pwsiws_warning_t *)data;
        // For now, use a default state since we can't get it from response
        int state = PWSIWS_WARNING_MESSAGE_BROADCAST;
        
        if (strcmp(message.h.service.name, OGS_SBI_SERVICE_NAME_NAMF_COMM) == 0) {
            pwsiws_nonuen2_comm_handle_nonuen2_message_transfer(
                    warning, state, &message);
        }
    }
    
    ogs_sbi_message_free(&message);
    ogs_sbi_response_free(response);
    return OGS_OK;
}

int pwsiws_nonuen2_comm_send_nonuen2_message_transfer(
        pwsiws_warning_t *warning, pwsiws_nonuen2_message_transfer_param_t *param)
{
    ogs_sbi_request_t *request = NULL;
    bool rc;
    
    ogs_assert(warning);
    ogs_assert(param);
    ogs_assert(pwsiws_self()->amf_sbi);
    
    // Build the Non-UE N2 message transfer request
    request = pwsiws_nonuen2_comm_build_nonuen2_message_transfer(warning, param);
    if (!request) {
        ogs_error("Failed to build Non-UE N2 message transfer request");
        return OGS_ERROR;
    }
    
    // Send the request to AMF
    rc = ogs_sbi_send_request_to_client(
            pwsiws_self()->amf_sbi, 
            pwsiws_sbi_client_callback, 
            request, 
            warning);
    
    if (!rc) {
        ogs_error("Failed to send Non-UE N2 message transfer request to AMF");
        ogs_sbi_request_free(request);
        return OGS_ERROR;
    }
    
    ogs_info("[Warning ID:%d] Non-UE N2 message transfer request sent to AMF", 
            warning->warning_id);
    
    return OGS_OK;
} 