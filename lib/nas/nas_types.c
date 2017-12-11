#define TRACE_MODULE _nas_types

#include "core_debug.h"
#include "core_lib.h"

#include "nas_types.h"

static c_uint8_t br_calculate(
    c_uint8_t *br, c_uint8_t *extended, c_uint8_t *extended2,
    c_uint64_t input)
{
    c_uint8_t length = 0;

    /* Octet 3 : 00000000 Reserved, 11111111 0kbps */
    if (input < 1)
    {
        *br = 0xff;
        length = c_max(length, 1);

        return length;
    }

    /*
     * Octet 7 : 00000000 
     * Use the value indicated by the APN-AMBR for downlink and
     *   APN-AMBR for downlink (extended) in octets 3 and 5.
     *
     * Octet 7 : 00000001 - 11111110
     * The APN-AMBR is
     *   (the binary coded value in 8 bits) * 256 Mbps +
     *   (the value indicated by the APN-AMBR for downlink
     *      and APN-AMBR for downlink (extended) in octets 3 and 5),
     * giving a range of 256 Mbps to 65280 Mbps.  */
    if (input > (65200*1024))
    {
        *extended2 = 0b11111110;
        length = c_max(length, 3);

        input %= (256*1024);
    }
    else if (input >= (256*1024) && input <= (65200*1024))
    {
        *extended2 = input / (256*1024);
        length = c_max(length, 3);

        input %= (256*1024);
    }

    /* Octet 3 : 00000001 -  00111111 
     * The APN-AMBR is binary coded in 8 bits, using a granularity of 1 kbps
     * giving a range of values from 1 kbps to 63 kbps in 1 kbps increments. */
    if (input >= 1 && input <= 63)
    {
        *br = input;
        length = c_max(length, 1);
    }
    /* Octet 3 : 01000000 -  01111111 
     * The APN-AMBR is 
     *   64 kbps + ((the binary coded value in 8 bits –01000000) * 8 kbps) 
     * giving a range of values from 64 kbps to 568 kbps 
     *   in 8 kbps increments. */
    else if (input >= 64 && input <= 568)
    {
        *br = ((input - 64) / 8) + 0b01000000;
        length = c_max(length, 1);
    }
    /* Set to 568 Kbps */
    else if (input > 568 && input < 576)
    {
        *br = 0b01111111;
        length = c_max(length, 1);
    }
    /* Octet 3 : 10000000 -  111111110
     * The APN-AMBR is
     *   576 kbps + ((the binary coded value in 8 bits –10000000) * 64 kbps)
     * giving a range of values from 576 kbps to 8640 kbps
     *   in 64 kbps increments. */
    else if (input >= 576 && input <= 8640)
    {
        *br = ((input - 576) / 64) + 0b10000000;
        length = c_max(length, 1);
    }
    /* Set to 8640 Kbps */
    else if (input > 8640 && input < 8700)
    {
        *br = 0b11111110;
        length = c_max(length, 1);
    }

    /* If the network wants to indicate an APN-AMBR 
     * for downlink higher than 8640 kbps, 
     * it shall set octet 3 to "11111110", i.e. 8640 kbps, 
     * and shall encode the value for the APN-AMBR in octet 5. 
     *
     * Octet 5 : 00000000 
     * Use the value indicated by the APN-AMBR for downlink in octet 3.
     *
     * Octet 5 : All other values shall be interpreted as '11111010'.
     *
     * Octet 5 : 00000001 - 01001010 
     * The APN-AMBR is
     *   8600 kbps + ((the binary coded value in 8 bits) * 100 kbps),
     * giving a range of values from 8700 kbps to 16000 kbps
     *   in 100 kbps increments.
     */
    else if (input >= 8700 && input <= 16000)
    {
        *br = 0b11111110;
        *extended = ((input - 8600) / 100);
        length = c_max(length, 2);
    }
    /* Set to 16000 Kbps */
    else if (input > 16000 && input < (17*1024))
    {
        *br = 0b11111110;
        *extended = 0b01001010;
        length = c_max(length, 2);
    }
    /* Octet 5: 01001011 - 10111010 
     * The APN-AMBR is
     *   16 Mbps + ((the binary coded value in 8 bits - 01001010) * 1 Mbps),
     * giving a range of values from 17 Mbps to 128 Mbps
     *   in 1 Mbps increments. */
    else if (input >= (17*1024) && input <= (128*1024))
    {
        *br = 0b11111110;
        *extended = ((input - (16*1024)) / (1*1024)) + 0b01001010;
        length = c_max(length, 2);
    }
    /* Set to 128 Mbps */
    else if (input > (128*1024) && input < (130*1024))
    {
        *br = 0b11111110;
        *extended = 0b10111010;
        length = c_max(length, 2);
    }
    /* Octet 5: 10111011 - 11111010
     * The APN-AMBR is
     *   128 Mbps + ((the binary coded value in 8 bits - 10111010) * 2 Mbps),
     * giving a range of values from 130 Mbps to 256 Mbps
     *   in 2 Mbps increments. */
    else if (input >= (130*1024) && input <= (256*1024))
    {
        *br = 0b11111110;
        *extended = ((input - (128*1024)) / (2*1024)) + 0b10111010;
        length = c_max(length, 2);
    }

    return length;
}

void apn_ambr_build(
    nas_apn_aggregate_maximum_bit_rate_t *apn_aggregate_maximum_bit_rate,
    c_uint32_t dl_apn_ambr, c_uint32_t ul_apn_ambr)
{
    c_uint8_t length = 0;

    dl_apn_ambr = dl_apn_ambr / 1024; /* Kbps */
    ul_apn_ambr = ul_apn_ambr / 1024; /* Kbps */

    memset(apn_aggregate_maximum_bit_rate, 0,
        sizeof(nas_apn_aggregate_maximum_bit_rate_t));

    length = c_max(length, br_calculate(
                &apn_aggregate_maximum_bit_rate->dl_apn_ambr,
                &apn_aggregate_maximum_bit_rate->dl_apn_ambr_extended,
                &apn_aggregate_maximum_bit_rate->dl_apn_ambr_extended2,
                dl_apn_ambr));

    length = c_max(length, br_calculate(
                &apn_aggregate_maximum_bit_rate->ul_apn_ambr,
                &apn_aggregate_maximum_bit_rate->ul_apn_ambr_extended,
                &apn_aggregate_maximum_bit_rate->ul_apn_ambr_extended2,
                ul_apn_ambr));

    apn_aggregate_maximum_bit_rate->length = length*2;
}

void eps_qos_build(nas_eps_quality_of_service_t *eps_qos, c_uint8_t qci,
    c_uint64_t dl_mbr, c_uint64_t ul_mbr, c_uint64_t dl_gbr, c_uint64_t ul_gbr)
{
    c_uint8_t length = 0;

    dl_mbr = dl_mbr / 1024; /* Kbps */
    ul_mbr = ul_mbr / 1024; /* Kbps */
    dl_gbr = dl_gbr / 1024; /* Kbps */
    ul_gbr = ul_gbr / 1024; /* Kbps */

    memset(eps_qos, 0, sizeof(nas_eps_quality_of_service_t));

    eps_qos->qci = qci;

    if (dl_mbr)
        length = c_max(length, br_calculate(
                    &eps_qos->dl_mbr,
                    &eps_qos->dl_mbr_extended,
                    &eps_qos->dl_mbr_extended2,
                    dl_mbr));

    if (ul_mbr)
        length = c_max(length, br_calculate(
                    &eps_qos->ul_mbr,
                    &eps_qos->ul_mbr_extended,
                    &eps_qos->ul_mbr_extended2,
                    ul_mbr));

    if (dl_gbr)
        length = c_max(length, br_calculate(
                    &eps_qos->dl_gbr,
                    &eps_qos->dl_gbr_extended,
                    &eps_qos->dl_gbr_extended2,
                    dl_gbr));

    if (ul_gbr)
        length = c_max(length, br_calculate(
                    &eps_qos->ul_gbr,
                    &eps_qos->ul_gbr_extended,
                    &eps_qos->ul_gbr_extended2,
                    ul_gbr));

    eps_qos->length = length*4+1;
}

void nas_tai_list_build(
        nas_tracking_area_identity_list_t *target,
        tai0_list_t *source0, tai2_list_t *source2)
{
    int i = 0, j = 0, size = 0;

    tai0_list_t target0;
    tai2_list_t target2;

    d_assert(target, return,);
    d_assert(source0, return,);
    d_assert(source2, return,);

    memset(target, 0, sizeof(nas_tracking_area_identity_list_t));
    memset(&target0, 0, sizeof(tai0_list_t));
    memset(&target2, 0, sizeof(tai2_list_t));

    for (i = 0; source0->tai[i].num; i++)
    {
        d_assert(source0->tai[i].type == TAI0_TYPE,
            return, "type = %d", source0->tai[i].type);
        target0.tai[i].type = source0->tai[i].type;

        /* <Spec> target->num = source->num - 1 */
        d_assert(source0->tai[i].num < MAX_NUM_OF_TAI,
                return, "num = %d", source0->tai[i].num);
        target0.tai[i].num = source0->tai[i].num - 1;
        memcpy(&target0.tai[i].plmn_id,
               &source0->tai[i].plmn_id, PLMN_ID_LEN);

        for (j = 0; j < source0->tai[i].num; j++) 
        {
            target0.tai[i].tac[j] = htons(source0->tai[i].tac[j]);
        }

        size = (1 + 3 + 2 * source0->tai[i].num);
        if ((target->length + size) > NAS_MAX_TAI_LIST_LEN)
        {
            d_warn("Overflow: Ignore remained TAI LIST(length:%d, size:%d)",
                    target->length, size);
            return;
        }
        memcpy(target->buffer + target->length, &target0.tai[i], size);
        target->length += size;
    }

    if (source2->num)
    {
        memset(&target2, 0, sizeof(target2));

        d_assert(source2->type == TAI1_TYPE || source2->type == TAI2_TYPE,
            return, "type = %d", source2->type);
        target2.type = source2->type;

        /* <Spec> target->num = source->num - 1 */
        d_assert(source2->num < MAX_NUM_OF_TAI,
                return, "num = %d", source2->num);
        target2.num = source2->num - 1;

        size = (1 + (3 + 2) * source2->num);
        if ((target->length + size) > NAS_MAX_TAI_LIST_LEN)
        {
            d_warn("Overflow: Ignore remained TAI LIST(length:%d, size:%d)",
                    target->length, size);
            return;
        }
        for (i = 0; i < source2->num; i++) 
        {
            memcpy(&target2.tai[i].plmn_id,
                   &source2->tai[i].plmn_id, PLMN_ID_LEN);
            target2.tai[i].tac = htons(source2->tai[i].tac);
        }
        memcpy(target->buffer + target->length, &target2, size);
        target->length += size;
    }
}
