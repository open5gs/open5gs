/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
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

#include <cstdint>

#include <tins/arp.h>
#include <tins/ethernetII.h>
#include <tins/hw_address.h>
#include <tins/icmpv6.h>
#include <tins/exceptions.h>

#include "arp-nd.h"

using namespace::Tins;


uint8_t _serialize_reply(uint8_t *reply_data, EthernetII &reply)
{
    PDU::serialization_type serialized = reply.serialize();
    memcpy(reply_data, serialized.data(), serialized.size());
    return serialized.size();
}

bool _parse_arp(EthernetII &pdu)
{
    if (pdu.payload_type() == ETHERTYPE_ARP) {
        const ARP& arp = pdu.rfind_pdu<ARP>();
        return arp.opcode() == ARP::REQUEST && pdu.dst_addr().is_broadcast();
    }
    return false;
}

bool is_arp_req(uint8_t *data, uint len)
{
    EthernetII pdu(data, len);
    return _parse_arp(pdu);
}

uint32_t arp_parse_target_addr(uint8_t *data, uint len)
{
    EthernetII pdu(data, len);
    if (pdu.payload_type() == ETHERTYPE_ARP) {
        const ARP& arp = pdu.rfind_pdu<ARP>();
        return arp.target_ip_addr();
    }
    return 0x0;
}

uint8_t arp_reply(uint8_t *reply_data, uint8_t *request_data, uint len,
        const uint8_t *mac)
{
    EthernetII pdu(request_data, len);
    if (_parse_arp(pdu)) {
        HWAddress<ETHER_ADDR_LEN> source_mac(mac);
        const ARP& arp = pdu.rfind_pdu<ARP>();
        EthernetII reply = ARP::make_arp_reply(
            arp.sender_ip_addr(),
            arp.target_ip_addr(),
            arp.sender_hw_addr(),
            source_mac);
        return _serialize_reply(reply_data, reply);
    }
    return 0;
}

bool _parse_nd(EthernetII &pdu)
{
    if (pdu.payload_type() == ETHERTYPE_IPV6) {
        try {
            const ICMPv6& icmp6 = pdu.rfind_pdu<ICMPv6>();
            return icmp6.type() == ICMPv6::NEIGHBOUR_SOLICIT;
        }
        catch (Tins::pdu_not_found& e) {
            /* If it is not an ICMPv6 message, it can not be a NEIGHBOUR_SOLICIT */
            return false;
        }
    }
    return false;
}

bool is_nd_req(uint8_t *data, uint len)
{
    if (len < MAX_ND_SIZE) {
        EthernetII pdu(data, len);
        return _parse_nd(pdu);
    }
    return false;
}

uint8_t nd_reply(uint8_t *reply_data, uint8_t *request_data, uint len,
        const uint8_t *mac)
{
    EthernetII pdu(request_data, len);
    if (_parse_nd(pdu)) {
        HWAddress<ETHER_ADDR_LEN> source_mac(mac);
        const ICMPv6& icmp6 = pdu.rfind_pdu<ICMPv6>();
        EthernetII reply(pdu.src_addr(), pdu.dst_addr());
        ICMPv6 nd_reply(ICMPv6::NEIGHBOUR_ADVERT);
        nd_reply.target_link_layer_addr(source_mac);
        nd_reply.target_addr(icmp6.target_addr());
        reply /= nd_reply;
        return _serialize_reply(reply_data, reply);
    }
    return 0;
}
