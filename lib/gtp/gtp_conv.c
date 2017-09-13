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
    (*bearers)[11] = &req->bearer_context_11;
    (*bearers)[12] = &req->bearer_context_12;
    (*bearers)[13] = &req->bearer_context_13;
    (*bearers)[14] = &req->bearer_context_14;
    (*bearers)[15] = &req->bearer_context_15;
    (*bearers)[16] = &req->bearer_context_16;
    (*bearers)[17] = &req->bearer_context_17;
    (*bearers)[18] = &req->bearer_context_18;
    (*bearers)[19] = &req->bearer_context_19;
    (*bearers)[20] = &req->bearer_context_20;
    (*bearers)[21] = &req->bearer_context_21;
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
    (*bearers)[11] = &rsp->bearer_context_11;
    (*bearers)[12] = &rsp->bearer_context_12;
    (*bearers)[13] = &rsp->bearer_context_13;
    (*bearers)[14] = &rsp->bearer_context_14;
    (*bearers)[15] = &rsp->bearer_context_15;
    (*bearers)[16] = &rsp->bearer_context_16;
    (*bearers)[17] = &rsp->bearer_context_17;
    (*bearers)[18] = &rsp->bearer_context_18;
    (*bearers)[19] = &rsp->bearer_context_19;
    (*bearers)[20] = &rsp->bearer_context_20;
    (*bearers)[21] = &rsp->bearer_context_21;
}
