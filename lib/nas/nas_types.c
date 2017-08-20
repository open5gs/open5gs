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

