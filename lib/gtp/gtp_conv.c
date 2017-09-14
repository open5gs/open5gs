#define TRACE_MODULE _gtp_conv

#include "core_debug.h"

#include "gtp_conv.h"

void gtp_bearers_in_create_indirect_tunnel_request(
        tlv_bearer_context_t *bearers[][GTP_MAX_NUM_OF_INDIRECT_TUNNEL],
        gtp_create_indirect_data_forwarding_tunnel_request_t *req)
{

    (*bearers)[0] = &req->bearer_context_0;
    (*bearers)[1] = &req->bearer_context_1;
    (*bearers)[2] = &req->bearer_context_2;
    (*bearers)[3] = &req->bearer_context_3;
    (*bearers)[4] = &req->bearer_context_4;
    (*bearers)[5] = &req->bearer_context_5;
    (*bearers)[6] = &req->bearer_context_6;
    (*bearers)[7] = &req->bearer_context_7;
    (*bearers)[8] = &req->bearer_context_8;
    (*bearers)[9] = &req->bearer_context_9;
    (*bearers)[10] = &req->bearer_context_10;
}

void gtp_bearers_in_create_indirect_tunnel_response(
        tlv_bearer_context_t *bearers[][GTP_MAX_NUM_OF_INDIRECT_TUNNEL],
        gtp_create_indirect_data_forwarding_tunnel_response_t *rsp)
{
    (*bearers)[0] = &rsp->bearer_context_0;
    (*bearers)[1] = &rsp->bearer_context_1;
    (*bearers)[2] = &rsp->bearer_context_2;
    (*bearers)[3] = &rsp->bearer_context_3;
    (*bearers)[4] = &rsp->bearer_context_4;
    (*bearers)[5] = &rsp->bearer_context_5;
    (*bearers)[6] = &rsp->bearer_context_6;
    (*bearers)[7] = &rsp->bearer_context_7;
    (*bearers)[8] = &rsp->bearer_context_8;
    (*bearers)[9] = &rsp->bearer_context_9;
    (*bearers)[10] = &rsp->bearer_context_10;
}
