#include "core.h"
#include "core_errno.h"
#include "core_general.h"
#include "core_debug.h"
#include "core_pkbuf.h"

#include "testutil.h"

#include "nas_message.h"

static void nas_message_test1(abts_case *tc, void *data)
{
    /* Attach Request */
    char *payload[] = {
        "\x17\xdf\x67\x5a\xa8\x05\x07\x41\x02\x0b\xf6\x00\xf1\x10\x00\x02\x01\x03\x00\x03\xe6\x05\xf0\x70\x00\x00\x10\x00\x05\x02\x15\xd0\x11\xd1\x52\x00\xf1\x10\x30\x39\x5c\x0a\x00\x31\x03\xe5\xe0\x34\x90\x11\x03\x57\x58\xa6\x5d\x01\x00\xe0\xc1"
    };

    int i = 0; 

    nas_message_t message;
    pkbuf_t *pkbuf;
    status_t rv;

    pkbuf = pkbuf_alloc(0, NAS_SDU_SIZE);
    ABTS_PTR_NOTNULL(tc, pkbuf);
    pkbuf->len = 59;
    memcpy(pkbuf->payload, payload[0], pkbuf->len);

    rv = nas_decode_pdu(&message, pkbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    pkbuf_free(pkbuf);
}

static void nas_message_test2(abts_case *tc, void *data)
{
    /* Attach Accept */
    char *payload[] = {
        "\x07\x42\x02\x23\x06\x00\x14\xf7\x99\x30\x39\x00\x32\x52\x01\xc1\x01\x09\x09\x08\x69\x6e\x74\x65\x72\x6e\x65\x74\x05\x01\x0a\xe1\x00\x0a\x27\x1b\x80\x80\x21\x10\x02\x02\x00\x10\x81\x06\xc0\xa8\xa8\x01\x83\x06\xc0\xa8\xa8\x01\x00\x0d\x04\xc0\xa8\xa8\x01\x50\x0b\xf6\x14\xf7\x99\x23\x45\xe1\x00\x00\x04\x56\x13\x00\xf1\x20\xff\xfd\x23\x05\xf4\x00\xe1\x02\xd4\x64\x01\x23"
    };

    char *esm_payload[] = {
        "\x52\x01\xc1\x01\x09\x09\x08\x69\x6e\x74\x65\x72\x6e\x65\x74\x05\x01\x0a\xe1\x00\x0a\x27\x1b\x80\x80\x21\x10\x02\x02\x00\x10\x81\x06\xc0\xa8\xa8\x01\x83\x06\xc0\xa8\xa8\x01\x00\x0d\x04\xc0\xa8\xa8\x01"
    };

    nas_message_t message;
    nas_attach_accept_t *attach_accept = &message.emm.attach_accept;

    pkbuf_t *pkbuf = NULL;
    status_t rv;

    memset(&message, 0, sizeof(message));
    message.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.h.message_type = NAS_ATTACH_ACCEPT;
    attach_accept->attach_result.result = 
        NAS_ATTACH_RESULT_COMBINED_EPS_IMSI_ATTACH;
    attach_accept->t3412_value.unit = 
        NAS_GRPS_TIMER_UNIT_MULTIPLES_OF_1_MM;
    attach_accept->t3412_value.timer_value = 3;
    attach_accept->tai_list.length = 6;
    attach_accept->tai_list.u.type0.mcc_digit1 = 4;
    attach_accept->tai_list.u.type0.mcc_digit2 = 1;
    attach_accept->tai_list.u.type0.mcc_digit3 = 7;
    attach_accept->tai_list.u.type0.mnc_digit1 = 9;
    attach_accept->tai_list.u.type0.mnc_digit2 = 9;
    attach_accept->tai_list.u.type0.mnc_digit3 = 0xf;
    attach_accept->tai_list.u.type0.tac[0] = 12345;
    attach_accept->esm_message_container.length = 50;
    attach_accept->esm_message_container.buffer = (c_uint8_t*)esm_payload[0];

    attach_accept->presencemask |= NAS_ATTACH_ACCEPT_GUTI_PRESENT;
    attach_accept->guti.length = 11;
    attach_accept->guti.type_of_identity = NAS_EPS_MOBILE_IDENTITY_GUTI;
    attach_accept->guti.u.guti.mcc_digit1 = 4;
    attach_accept->guti.u.guti.mcc_digit2 = 1;
    attach_accept->guti.u.guti.mcc_digit3 = 7;
    attach_accept->guti.u.guti.mnc_digit1 = 9;
    attach_accept->guti.u.guti.mnc_digit2 = 9;
    attach_accept->guti.u.guti.mnc_digit3 = 0xf;
    attach_accept->guti.u.guti.mme_group_id = 9029;
    attach_accept->guti.u.guti.mme_code = 225;
    attach_accept->guti.u.guti.m_tmsi = 0x00000456;

    rv = nas_encode_pdu(&pkbuf, &message);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
#if 0
    {
        int i = 0;
        unsigned char *p = pkbuf->payload;
        for (i = 0; i < pkbuf->len; i++)
            printf("0x%x, 0x%x\n", payload[0][i], p[i]);

    }
#endif
    ABTS_TRUE(tc, memcmp(pkbuf->payload, payload[0], pkbuf->len) == 0);

    pkbuf_free(pkbuf);
}

abts_suite *test_nas_message(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, nas_message_test1, NULL);
    abts_run_test(suite, nas_message_test2, NULL);

    return suite;
}
