/*
 * Copyright (C) 2019-2026 by Arthur SC Chan <arthur.chan@adalogics.com>
 *
 * This file is part of Open5GS.
 *
 * Threat model: GTPv1-C/GTPv2-C from a peer MME, SGW or PGW. gtp_message_fuzz
 * decodes messages only. xact.c (1026) and context.c (624) are 0-covered in
 * the 20261114 report, as are v1/v2 path.c and v2/build.c. This target matches
 * sequence numbers into the transaction table. It does not commit (that
 * sends). Sequence-number and message-type bytes are the attacker lever.
 */

#include <stdint.h>
#include <string.h>

#include "fuzzing.h"
#include "ogs-app.h"
#include "ogs-gtp.h"

#define kMinInputLength 8
#define kMaxInputLength 1024
#define GTP_VERSION(byte) (((byte) >> 5) & 0x07)

static ogs_gtp_node_t *peer;

static void init_once(void)
{
    ogs_sockaddr_t addr;

    if (initialized)
        return;
    initialize();
    ogs_log_install_domain(&__ogs_gtp_domain, "gtp", OGS_LOG_NONE);
    ogs_log_install_domain(&__ogs_tlv_domain, "tlv", OGS_LOG_NONE);
    ogs_app_context_init();
    ogs_app()->timer_mgr = ogs_timer_mgr_create(64);
    ogs_app()->pool.gtp_node = 8;
    ogs_app()->pool.xact = 32;
    ogs_local_conf()->time.message.duration = ogs_time_from_sec(4);
    ogs_local_conf()->time.message.gtp.n3_response_rcount = 3;
    ogs_local_conf()->time.message.gtp.t3_response_duration = ogs_time_from_sec(1);
    ogs_local_conf()->time.message.gtp.n3_holding_rcount = 1;
    ogs_local_conf()->time.message.gtp.t3_holding_duration = ogs_time_from_sec(3);
    ogs_gtp_context_init(4);
    ogs_gtp_xact_init();

    memset(&addr, 0, sizeof(addr));
    addr.sin.sin_family = AF_INET;
    addr.sin.sin_addr.s_addr = htonl(0x7f000001);
    addr.sin.sin_port = htons(2123);
    ogs_list_init(&ogs_gtp_self()->gtpu_peer_list);
    peer = ogs_gtp_node_add_by_addr(&ogs_gtp_self()->gtpu_peer_list, &addr);
}

extern int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size)
{
    ogs_pkbuf_t *pkbuf;
    ogs_gtp_xact_t *xact = NULL;

    if (Size < kMinInputLength || Size > kMaxInputLength)
        return 0;

    init_once();
    if (!peer)
        return 0;

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    if (!pkbuf)
        return 0;
    ogs_pkbuf_put_data(pkbuf, Data, Size);

    if (GTP_VERSION(Data[0]) == 1) {
        ogs_gtp1_message_t message;
        memset(&message, 0, sizeof(message));
        if (ogs_gtp1_parse_msg(&message, pkbuf) == OGS_OK) {
            xact = NULL;
            if (ogs_gtp1_xact_receive(peer, &message.h, &xact) == OGS_OK)
                ogs_gtp_xact_delete_all(peer);
        }
    } else {
        ogs_gtp2_message_t message;
        memset(&message, 0, sizeof(message));
        if (ogs_gtp2_parse_msg(&message, pkbuf) == OGS_OK) {
            xact = NULL;
            if (ogs_gtp_xact_receive(peer, &message.h, &xact) == OGS_OK)
                ogs_gtp_xact_delete_all(peer);
        }
    }

    ogs_pkbuf_free(pkbuf);
    return 0;
}
