/*
 * Copyright (C) 2019,2020 by Sukchan Lee <acetcom@gmail.com>
 *
 * This file is part of Open5GS.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "ogs-nas-common.h"

int __ogs_nas_domain;

static uint8_t nas_ambr_from_kbps(
    uint8_t *br, uint8_t *extended, uint8_t *extended2,
    uint64_t input)
{
    uint8_t length = 0;

    /*
     * Octet 3
     * 11111111 0kbps
     */
    if (input < 1) {
        *br = 0xff;
        length = ogs_max(length, 1);

        return length;
    }

    /*
     * Octet 3
     *
     * In network to UE direction:
     * 00000000 Reserved
     *
     * Octet 3 : 00000001 - 00111111
     * The APN-AMBR is binary coded in 8 bits, using a granularity of 1 kbps
     * giving a range of values from 1 kbps to 63 kbps in 1 kbps increments.
     *
     * Octet 3 : 01000000 - 01111111
     * The APN-AMBR is
     *   64 kbps + ((the binary coded value in 8 bits – 01000000) * 8 kbps)
     * giving a range of values from 64 kbps to 568 kbps in 8 kbps increments.
     *
     * Octet 3 : 10000000 - 11111110
     * The APN-AMBR is
     *   576 kbps + ((the binary coded value in 8 bits – 10000000) * 64 kbps)
     * giving a range of values from 576 kbps to 8640 kbps in 64 kbps increments
     */

    /* giving a range of 1 kbps to 63 kbps in 1 kbps increments. */
    if (input >= 1 && input <= 63) {
        *br = input;
        length = ogs_max(length, 1);
    }
    /* giving a range of 64 kbps to 568 kbps in 8 kbps increments. */
    else if (input >= 64 && input <= 568) {
        *br = ((input - 64) / 8) + 0b01000000;
        length = ogs_max(length, 1);
    }
    /* Set to 568 Kbps */
    else if (input > 568 && input < 576) {
        *br = 0b01111111;
        length = ogs_max(length, 1);
    }
    /* giving a range of 576 kbps to 8640 kbps in 64 kbps increments. */
    else if (input >= 576 && input <= 8640) {
        *br = ((input - 576) / 64) + 0b10000000;
        length = ogs_max(length, 1);
    }
    /* Set to 8640 Kbps */
    else if (input > 8640 && input < 8700) {
        *br = 0b11111110;
        length = ogs_max(length, 1);
    }

    /*
     * Octet 5 : 00000000
     * Use the value indicated by the APN-AMBR for downlink in octet 3.
     *
     * Octet 5 : 00000001 - 01001010
     * The APN-AMBR is
     *   8600 kbps + ((the binary coded value in 8 bits) * 100 kbps),
     * giving a range of values from 8700 kbps to 16000 kbps
     *   in 100 kbps increments.
     *
     * Octet 5 : 01001011 - 10111010
     * The APN-AMBR is
     *   16 Mbps + ((the binary coded value in 8 bits - 01001010) * 1 Mbps),
     * giving a range of values from 17 Mbps to 128 Mbps in 1 Mbps increments.
     *
     * Octet 5 : 10111011 - 11111010
     * 128 Mbps + (the binary coded value in 8 bits - 10111010) * 2 Mbps
     * giving a range of 130 Mbps to 256 Mbps in 2 Mbps increments.
     *
     * All other values shall be interpreted as '11111010'.
     */

    /* giving a range of 8700 kbps to 16000 kbps in 100 kbps increments. */
    else if (input >= 8700 && input <= 16000) {
        *br = 0b11111110;
        *extended = ((input - 8600) / 100);
        length = ogs_max(length, 2);
    }
    /* Set to 16000 Kbps */
    else if (input > 16000 && input < (17*1000)) {
        *br = 0b11111110;
        *extended = 0b01001010;
        length = ogs_max(length, 2);
    }
    /* giving a range of 17 Mbps to 128 Mbps in 1 Mbps increments. */
    else if (input >= (17*1000) && input <= (128*1000)) {
        *br = 0b11111110;
        *extended = ((input - (16*1000)) / (1*1000)) + 0b01001010;
        length = ogs_max(length, 2);
    }
    /* Set to 128 Mbps */
    else if (input > (128*1000) && input < (130*1000)) {
        *br = 0b11111110;
        *extended = 0b10111010;
        length = ogs_max(length, 2);
    }
    /* giving a range of 130 Mbps to 256 Mbps in 2 Mbps increments. */
    else if (input >= (130*1000) && input <= (256*1000)) {
        *br = 0b11111110;
        *extended = ((input - (128*1000)) / (2*1000)) + 0b10111010;
        length = ogs_max(length, 2);
    }
    /* Set to 256 Mbps */
    else if (input > (1*256*1000) && input < (2*256*1000)) {
        *br = 0b11111110;
        *extended = 0b11111010;
        length = ogs_max(length, 2);
    }

    /*
     * Octet 7 : 00000000
     * Use the value indicated by the APN-AMBR for downlink and
     *   APN-AMBR for downlink (extended) in octets 3 and 5.
     *
     * Octet 7 : 00000001 - 11111110
     * The APN-AMBR is
     *   (the binary coded value in 8 bits) * 256 Mbps +
     *      "the value indicated by the APN-AMBR for downlink
     *          and APN-AMBR for downlink (extended) in octets 3 and 5",
     */

    /* giving a range of values from 260M to 500M in 4M increments */
    else if (input >= (2*256*1000) && input <= (255*256*1000)) {
        *br = 0b11111110;
        *extended = 0b11111010;
        *extended2 = (input - (1*256*1000)) / (256*1000);
        length = ogs_max(length, 3);
    }
    /* if the sending entity want to indicate BR higher than 65280M */
    else if (input > (255*256*1000)) {
        *br = 0b11111110;
        *extended = 0b11111010;
        *extended2 = 0b11111110;
        length = ogs_max(length, 3);
    }

    return length;
}

static uint8_t nas_qos_from_kbps(
    uint8_t *br, uint8_t *extended, uint8_t *extended2,
    uint64_t input)
{
    uint8_t length = 0;

    /*
     * Octet 4
     * 11111111 0kbps
     */
    if (input < 1) {
        *br = 0xff;
        length = ogs_max(length, 1);

        return length;
    }

    /*
     * Octet 4
     *
     * In UE to network direction:
     * 00000000 Subscribed maximum bit rate
     *
     * In network to UE direction:
     * 00000000 Reserved
     *
     * Octet 4 : 00000001 - 00111111
     * giving a range of 1 kbps to 63 kbps in 1 kbps increments.
     *
     * Octet 4 : 01000000 - 01111111
     * 64 kbps + (the binary coded value in 8 bits - 01000000) * 8 kbps
     * giving a range of 64 kbps to 568 kbps in 8 kbps increments.
     *
     * Octet 4 : 10000000 - 11111110
     * 576 kbps + (the binary coded value in 8 bits – 10000000) * 64 kbps
     * giving a range of 576 kbps to 8640 kbps in 64 kbps increments.
     */

    /* giving a range of 1 kbps to 63 kbps in 1 kbps increments. */
    if (input >= 1 && input <= 63) {
        *br = input;
        length = ogs_max(length, 1);
    }
    /* giving a range of 64 kbps to 568 kbps in 8 kbps increments. */
    else if (input >= 64 && input <= 568) {
        *br = ((input - 64) / 8) + 0b01000000;
        length = ogs_max(length, 1);
    }
    /* Set to 568 Kbps */
    else if (input > 568 && input < 576) {
        *br = 0b01111111;
        length = ogs_max(length, 1);
    }
    /* giving a range of 576 kbps to 8640 kbps in 64 kbps increments. */
    else if (input >= 576 && input <= 8640) {
        *br = ((input - 576) / 64) + 0b10000000;
        length = ogs_max(length, 1);
    }
    /* Set to 8640 Kbps */
    else if (input > 8640 && input < 8700) {
        *br = 0b11111110;
        length = ogs_max(length, 1);
    }

    /*
     * Octet 8
     * 00000000 Use the value indicated by the bit rate in octet 4
     *
     * Octet 8 : 00000001 - 01001010
     * 8600 kbps + (the binary coded value in 8 bits) * 100 kbps
     * giving a range of 8700 kbps to 16000 kbps in 100 kbps increments.
     *
     * Octet 8 : 01001011 - 10111010
     * 16 Mbps + (the binary coded value in 8 bits - 01001010) * 1 Mbps
     * giving a range of 17 Mbps to 128 Mbps in 1 Mbps increments.
     *
     * Octet 8 : 10111011 - 11111010
     * 128 Mbps + (the binary coded value in 8 bits - 10111010) * 2 Mbps
     * giving a range of 130 Mbps to 256 Mbps in 2 Mbps increments.
     */

    /* giving a range of 8700 kbps to 16000 kbps in 100 kbps increments. */
    else if (input >= 8700 && input <= 16000) {
        *br = 0b11111110;
        *extended = ((input - 8600) / 100);
        length = ogs_max(length, 2);
    }
    /* Set to 16000 Kbps */
    else if (input > 16000 && input < (17*1000)) {
        *br = 0b11111110;
        *extended = 0b01001010;
        length = ogs_max(length, 2);
    }
    /* giving a range of 17 Mbps to 128 Mbps in 1 Mbps increments. */
    else if (input >= (17*1000) && input <= (128*1000)) {
        *br = 0b11111110;
        *extended = ((input - (16*1000)) / (1*1000)) + 0b01001010;
        length = ogs_max(length, 2);
    }
    /* Set to 128 Mbps */
    else if (input > (128*1000) && input < (130*1000)) {
        *br = 0b11111110;
        *extended = 0b10111010;
        length = ogs_max(length, 2);
    }
    /* giving a range of 130 Mbps to 256 Mbps in 2 Mbps increments. */
    else if (input >= (130*1000) && input <= (256*1000)) {
        *br = 0b11111110;
        *extended = ((input - (128*1000)) / (2*1000)) + 0b10111010;
        length = ogs_max(length, 2);
    }
    /* Set to 256 Mbps */
    else if (input > (256*1000) && input < (260*1000)) {
        *br = 0b11111110;
        *extended = 0b11111010;
        length = ogs_max(length, 2);
    }

    /*
     * Octet 12 : 00000000
     * 00000000 Use the value indicated by the bit rate in octet 4 and 8
     *
     * Octet 12 : 00000001 - 00111101
     * 256Mbps + the binary coded value in 8 bits * 4Mbps
     * giving a range of 260 Mbps to 500 Mbps in 4 Mbps increments.
     *
     * Octet 12 : 00111110 - 10100001
     * 500Mbps + (the binary coded value in 8 bits - 00111101) * 10Mbps
     * giving a range of 510 Mbps to 1500 Mbps in 10 Mbps increments.
     *
     * Octet 12 : 10100010 - 11110110
     * 1500Mbps + (the binary coded value in 8 bits - 10100001) * 100Mbps
     * giving a range of 1600 Mbps to 10 Gbps Mbps in 100 Mbps increaments.
     */

    /* giving a range of values from 260M to 500M in 4M increments */
    else if (input >= (260*1000) && input <= (500*1000)) {
        *br = 0b11111110;
        *extended = 0b11111010;
        *extended2 = (input - (256*1000)) / (4*1000);
        length = ogs_max(length, 3);
    }
    /* if a range of values from 500M to 510M */
    else if (input > (500*1000) && input < (510*1000)) {
        *br = 0b11111110;
        *extended = 0b11111010;
        *extended2 = 0b00111101;
        length = ogs_max(length, 3);
    }
    /* giving a range of values from 510M to 1500M in 10M increments */
    else if (input >= (510*1000) && input <= (1500*1000)) {
        *br = 0b11111110;
        *extended = 0b11111010;
        *extended2 = (input - (500*1000)) / (10*1000) + 0b00111101;
        length = ogs_max(length, 3);
    }
    /* if a range of values from 1500M to 1600M */
    else if (input > (1500*1000) && input < (1600*1000)) {
        *br = 0b11111110;
        *extended = 0b11111010;
        *extended2 = 0b10100001;
        length = ogs_max(length, 3);
    }
    /* giving a range of values from 1600M to 10000M in 100M increments */
    else if (input >= (1600*1000) && input <= (10*1000*1000)) {
        *br = 0b11111110;
        *extended = 0b11111010;
        *extended2 = (input - (1500*1000)) / (100*1000) + 0b10100001;
        length = ogs_max(length, 3);
    }
    /* if the sending entity want to indicate BR higher than 10000Mbps */
    else if (input > (10*1000*1000)) {
        *br = 0b11111110;
        *extended = 0b11111010;
        *extended2 = 0b11110110;
        length = ogs_max(length, 3);
    }

    return length;
}

void apn_ambr_build(
    ogs_nas_apn_aggregate_maximum_bit_rate_t *apn_aggregate_maximum_bit_rate,
    uint32_t dl_apn_ambr, uint32_t ul_apn_ambr)
{
    uint8_t length = 0;

    dl_apn_ambr = dl_apn_ambr / 1000; /* Kbps */
    ul_apn_ambr = ul_apn_ambr / 1000; /* Kbps */

    memset(apn_aggregate_maximum_bit_rate, 0,
        sizeof(ogs_nas_apn_aggregate_maximum_bit_rate_t));

    length = ogs_max(length, nas_ambr_from_kbps(
                &apn_aggregate_maximum_bit_rate->dl_apn_ambr,
                &apn_aggregate_maximum_bit_rate->dl_apn_ambr_extended,
                &apn_aggregate_maximum_bit_rate->dl_apn_ambr_extended2,
                dl_apn_ambr));

    length = ogs_max(length, nas_ambr_from_kbps(
                &apn_aggregate_maximum_bit_rate->ul_apn_ambr,
                &apn_aggregate_maximum_bit_rate->ul_apn_ambr_extended,
                &apn_aggregate_maximum_bit_rate->ul_apn_ambr_extended2,
                ul_apn_ambr));

    apn_aggregate_maximum_bit_rate->length = length*2;
}

void eps_qos_build(ogs_nas_eps_quality_of_service_t *eps_qos, uint8_t qci,
    uint64_t dl_mbr, uint64_t ul_mbr, uint64_t dl_gbr, uint64_t ul_gbr)
{
    uint8_t length = 0;

    dl_mbr = dl_mbr / 1000; /* Kbps */
    ul_mbr = ul_mbr / 1000; /* Kbps */
    dl_gbr = dl_gbr / 1000; /* Kbps */
    ul_gbr = ul_gbr / 1000; /* Kbps */

    memset(eps_qos, 0, sizeof(ogs_nas_eps_quality_of_service_t));

    eps_qos->qci = qci;

    if (dl_mbr)
        length = ogs_max(length, nas_qos_from_kbps(
                    &eps_qos->dl_mbr,
                    &eps_qos->dl_mbr_extended,
                    &eps_qos->dl_mbr_extended2,
                    dl_mbr));

    if (ul_mbr)
        length = ogs_max(length, nas_qos_from_kbps(
                    &eps_qos->ul_mbr,
                    &eps_qos->ul_mbr_extended,
                    &eps_qos->ul_mbr_extended2,
                    ul_mbr));

    if (dl_gbr)
        length = ogs_max(length, nas_qos_from_kbps(
                    &eps_qos->dl_gbr,
                    &eps_qos->dl_gbr_extended,
                    &eps_qos->dl_gbr_extended2,
                    dl_gbr));

    if (ul_gbr)
        length = ogs_max(length, nas_qos_from_kbps(
                    &eps_qos->ul_gbr,
                    &eps_qos->ul_gbr_extended,
                    &eps_qos->ul_gbr_extended2,
                    ul_gbr));

    eps_qos->length = length*4+1;
}

void ogs_nas_bitrate_from_uint64(ogs_nas_bitrate_t *nas, uint64_t bitrate)
{
    ogs_assert(nas);
    ogs_assert(bitrate);

    bitrate /= 1000; /* bps to Kbps */

    for (nas->unit = OGS_NAS_BR_UNIT_1K;
            nas->unit < OGS_NAS_BR_UNIT_256P; nas->unit++) {
        if (bitrate <= 0xFFFFUL) {
            break;
        }
        bitrate >>= 2;
    }
    nas->value = bitrate;
}

bool ogs_nas_bitrate_unit_is_valid(ogs_nas_bitrate_t *nas_bitrate)
{
    ogs_assert(nas_bitrate);

    return nas_bitrate->unit >= OGS_NAS_BR_UNIT_1K &&
        nas_bitrate->unit <= OGS_NAS_BR_UNIT_256P;
}

uint64_t ogs_nas_bitrate_to_uint64(ogs_nas_bitrate_t *nas_bitrate)
{
    ogs_assert(nas_bitrate);

    if (!ogs_nas_bitrate_unit_is_valid(nas_bitrate)) {
        ogs_error("Invalid bitrate unit [%d]", nas_bitrate->unit);
        return 0;
    }

    if (nas_bitrate->unit == OGS_NAS_BR_UNIT_1K)
        return nas_bitrate->value * 1000;
    if (nas_bitrate->unit == OGS_NAS_BR_UNIT_4K)
        return nas_bitrate->value * 4 * 1000;
    if (nas_bitrate->unit == OGS_NAS_BR_UNIT_16K)
        return nas_bitrate->value * 16 * 1000;
    if (nas_bitrate->unit == OGS_NAS_BR_UNIT_64K)
        return nas_bitrate->value * 64 * 1000;
    if (nas_bitrate->unit == OGS_NAS_BR_UNIT_256K)
        return nas_bitrate->value * 256 * 1000;
    if (nas_bitrate->unit == OGS_NAS_BR_UNIT_1M)
        return nas_bitrate->value * 1000 * 1000;
    if (nas_bitrate->unit == OGS_NAS_BR_UNIT_4M)
        return nas_bitrate->value * 4 * 1000 * 1000;
    if (nas_bitrate->unit == OGS_NAS_BR_UNIT_16M)
        return nas_bitrate->value * 16 * 1000 * 1000;
    if (nas_bitrate->unit == OGS_NAS_BR_UNIT_64M)
        return nas_bitrate->value * 64 * 1000 * 1000;
    if (nas_bitrate->unit == OGS_NAS_BR_UNIT_256M)
        return nas_bitrate->value * 256 * 1000 * 1000;
    if (nas_bitrate->unit == OGS_NAS_BR_UNIT_1G)
        return nas_bitrate->value * 1000 * 1000 * 1000;
    if (nas_bitrate->unit == OGS_NAS_BR_UNIT_4G)
        return nas_bitrate->value * 4 * 1000 * 1000 * 1000;
    if (nas_bitrate->unit == OGS_NAS_BR_UNIT_16G)
        return nas_bitrate->value * 16 * 1000 * 1000 * 1000;
    if (nas_bitrate->unit == OGS_NAS_BR_UNIT_64G)
        return nas_bitrate->value * 64 * 1000 * 1000 * 1000;
    if (nas_bitrate->unit == OGS_NAS_BR_UNIT_256G)
        return nas_bitrate->value * 256 * 1000 * 1000 * 1000;
    if (nas_bitrate->unit == OGS_NAS_BR_UNIT_1T)
        return nas_bitrate->value * 1000 * 1000 * 1000 * 1000;
    if (nas_bitrate->unit == OGS_NAS_BR_UNIT_4T)
        return nas_bitrate->value * 4 * 1000 * 1000 * 1000 * 1000;
    if (nas_bitrate->unit == OGS_NAS_BR_UNIT_16T)
        return nas_bitrate->value * 16 * 1000 * 1000 * 1000 * 1000;
    if (nas_bitrate->unit == OGS_NAS_BR_UNIT_64T)
        return nas_bitrate->value * 64 * 1000 * 1000 * 1000 * 1000;
    if (nas_bitrate->unit == OGS_NAS_BR_UNIT_256T)
        return nas_bitrate->value * 256 * 1000 * 1000 * 1000 * 1000;
    if (nas_bitrate->unit == OGS_NAS_BR_UNIT_1P)
        return nas_bitrate->value * 1000 * 1000 * 1000 * 1000 * 1000;
    if (nas_bitrate->unit == OGS_NAS_BR_UNIT_4P)
        return nas_bitrate->value * 4 * 1000 * 1000 * 1000 * 1000 * 1000;
    if (nas_bitrate->unit == OGS_NAS_BR_UNIT_16P)
        return nas_bitrate->value * 16 * 1000 * 1000 * 1000 * 1000 * 1000;
    if (nas_bitrate->unit == OGS_NAS_BR_UNIT_64P)
        return nas_bitrate->value * 64 * 1000 * 1000 * 1000 * 1000 * 1000;
    if (nas_bitrate->unit == OGS_NAS_BR_UNIT_256P)
        return nas_bitrate->value * 256 * 1000 * 1000 * 1000 * 1000 * 1000;

    return 0;
}

/*
 * TS 24.008 10.5.3.5a Network Name
 *
 * The AMF and MME each built this IE as UCS-2, coding scheme 001, from an
 * open-coded loop the comment itself called a workaround.  That is legal, but
 * it is not what live networks send, and at least one widely deployed modem
 * family (Quectel RG5xx, as found in Teltonika RUTX50 CPE) mis-handles the
 * UCS-2 form: it completes registration and then never establishes a PDU
 * session, leaving the subscriber with signalling but no data.
 *
 * So prefer the GSM 7 bit default alphabet (TS 23.038 6.2.1), coding scheme
 * 000, and fall back to UCS-2 only for a name that 7 bit cannot carry.  Almost
 * every operator name is plain ASCII and takes the 7 bit path; a name with
 * accented or non-Latin characters still gets encoded correctly rather than
 * mangled, at the cost of landing back on the coding scheme such modems
 * dislike.  That is a better trade than silently replacing the characters.
 *
 * An absent or empty name leaves length at 0, which is how the callers
 * suppress the IE altogether.  Note that a zero length is NOT the same as a
 * one octet IE carrying only a coding scheme, which is what the previous code
 * produced for an empty name.
 */

/* Not a valid septet value, so it can mark the code points the 7 bit default
 * alphabet cannot reach directly. */
#define OGS_NAS_GSM7_NO_SEPTET 0xff

/* Septet for each printable ASCII code point 0x20..0x7e.  Most of ASCII maps
 * straight through, and the handful of entries that do not are the reason this
 * table is spelled out rather than computed.  Code points reachable only
 * through the alphabet's escape table are marked NO_SEPTET and send the whole
 * name down the UCS-2 path instead: encoding them here would need a two septet
 * escape sequence, which complicates the length and spare bit arithmetic for
 * characters that do not occur in operator names in practice.
 */
static const uint8_t gsm7_septet_from_ascii[0x7e - 0x20 + 1] = {
    /* 0x20 */ 0x20, 0x21, 0x22, 0x23, 0x02, 0x25, 0x26, 0x27,
    /* 0x28 */ 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
    /* 0x30 */ 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
    /* 0x38 */ 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f,
    /* 0x40 */ 0x00, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
    /* 0x48 */ 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
    /* 0x50 */ 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
    /* 0x58 */ 0x58, 0x59, 0x5a, 0xff, 0xff, 0xff, 0xff, 0x11,
    /* 0x60 */ 0xff, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
    /* 0x68 */ 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f,
    /* 0x70 */ 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77,
    /* 0x78 */ 0x78, 0x79, 0x7a, 0xff, 0xff, 0xff, 0xff,
};

/* The IE length field is a single octet and counts the coding scheme octet as
 * well as the packed text, so the text can never fill the buffer. */
#define OGS_NAS_NETWORK_NAME_MAX_TEXT (OGS_NAS_MAX_NETWORK_NAME_LEN - 1)

static bool network_name_is_gsm7(const char *str)
{
    int i;

    for (i = 0; str[i]; i++) {
        uint8_t c = (uint8_t)str[i];

        if ((c < 0x20) || (c > 0x7e))
            return false;
        if (gsm7_septet_from_ascii[c - 0x20] == OGS_NAS_GSM7_NO_SEPTET)
            return false;
    }

    return true;
}

static void network_name_build_gsm7(
        ogs_nas_network_name_t *network_name, const char *str)
{
    int nbit = 0;
    int i;

    for (i = 0; str[i]; i++) {
        uint8_t septet;
        int octet;
        int shift;

        if ((nbit + 7) > (OGS_NAS_NETWORK_NAME_MAX_TEXT * 8))
            break;

        septet = gsm7_septet_from_ascii[(uint8_t)str[i] - 0x20];
        ogs_assert(septet != OGS_NAS_GSM7_NO_SEPTET);

        /* Septets are packed low bits first: septet n starts at bit 7*n of
         * the text, so it straddles two octets whenever it starts at bit 2
         * or later within one. */
        octet = nbit / 8;
        shift = nbit % 8;

        network_name->name[octet] |= (septet << shift) & 0xff;
        if (shift > 1)
            network_name->name[octet + 1] |= septet >> (8 - shift);

        nbit += 7;
    }

    if (nbit == 0)
        return;

    network_name->ext = 1;
    network_name->coding_scheme = 0; /* GSM 7 bit default alphabet */
    network_name->length = 1 + ((nbit + 7) / 8);
    network_name->number_of_spare_bits_in_last_octet = (8 - (nbit % 8)) % 8;
}

/* Decode one UTF-8 sequence and advance *pos past it.  Malformed input yields
 * '?' and consumes a single byte, so the caller's loop always progresses on
 * whatever the yaml parser handed over.  Never reads beyond the terminator:
 * a truncated sequence fails the continuation-byte test on the NUL. */
static uint32_t utf8_next(const char *str, size_t *pos)
{
    const uint8_t *s = (const uint8_t *)str + *pos;
    uint32_t cp;
    int extra;
    int i;

    if (s[0] < 0x80) {
        *pos += 1;
        return s[0];
    } else if ((s[0] & 0xe0) == 0xc0) {
        cp = s[0] & 0x1f;
        extra = 1;
    } else if ((s[0] & 0xf0) == 0xe0) {
        cp = s[0] & 0x0f;
        extra = 2;
    } else if ((s[0] & 0xf8) == 0xf0) {
        cp = s[0] & 0x07;
        extra = 3;
    } else {
        *pos += 1;
        return 0x003f;
    }

    for (i = 1; i <= extra; i++) {
        if ((s[i] & 0xc0) != 0x80) {
            *pos += 1;
            return 0x003f;
        }
        cp = (cp << 6) | (s[i] & 0x3f);
    }

    *pos += extra + 1;

    return cp;
}

static void network_name_build_ucs2(
        ogs_nas_network_name_t *network_name, const char *str)
{
    size_t pos = 0;
    int len = 0;

    while (str[pos]) {
        uint32_t cp = utf8_next(str, &pos);

        if ((len + 2) > OGS_NAS_NETWORK_NAME_MAX_TEXT)
            break;

        /* UCS-2 is the BMP only, so anything above it has no representation
         * here.  Note the previous code copied raw bytes, which turned any
         * multi-byte UTF-8 character into two mojibake code points. */
        if (cp > 0xffff)
            cp = 0x003f;

        network_name->name[len++] = (cp >> 8) & 0xff;
        network_name->name[len++] = cp & 0xff;
    }

    if (len == 0)
        return;

    network_name->ext = 1;
    network_name->coding_scheme = 1; /* UCS-2 */
    network_name->length = 1 + len;
    /* number_of_spare_bits_in_last_octet only means anything for 7 bit
     * packing, and stays 0 here. */
}

void ogs_nas_build_network_name(
        ogs_nas_network_name_t *network_name, const char *str)
{
    ogs_assert(network_name);

    memset(network_name, 0, sizeof(*network_name));

    if (!str)
        return;

    if (network_name_is_gsm7(str))
        network_name_build_gsm7(network_name, str);
    else
        network_name_build_ucs2(network_name, str);
}
