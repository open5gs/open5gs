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

#include "ogs-pfcp.h"

int __ogs_pfcp_domain;

const char *ogs_pfcp_cause_get_name(uint8_t cause)
{
    switch(cause) {
    case OGS_PFCP_CAUSE_REQUEST_ACCEPTED:
        return "OGS_PFCP_CAUSE_REQUEST_ACCEPTED";
        break;
    case OGS_PFCP_CAUSE_REQUEST_REJECTED:
        return "OGS_PFCP_CAUSE_REQUEST_REJECTED";
        break;
    case OGS_PFCP_CAUSE_SESSION_CONTEXT_NOT_FOUND:
        return "OGS_PFCP_CAUSE_SESSION_CONTEXT_NOT_FOUND";
        break;
    case OGS_PFCP_CAUSE_MANDATORY_IE_MISSING:
        return "OGS_PFCP_CAUSE_MANDATORY_IE_MISSING";
        break;
    case OGS_PFCP_CAUSE_CONDITIONAL_IE_MISSING:
        return "OGS_PFCP_CAUSE_CONDITIONAL_IE_MISSING";
        break;
    case OGS_PFCP_CAUSE_INVALID_LENGTH:
        return "OGS_PFCP_CAUSE_INVALID_LENGTH";
        break;
    case OGS_PFCP_CAUSE_MANDATORY_IE_INCORRECT:
        return "OGS_PFCP_CAUSE_MANDATORY_IE_INCORRECT";
        break;
    case OGS_PFCP_CAUSE_INVALID_FORWARDING_POLICY:
        return "OGS_PFCP_CAUSE_INVALID_FORWARDING_POLICY";
        break;
    case OGS_PFCP_CAUSE_INVALID_F_TEID_ALLOCATION_OPTION:
        return "OGS_PFCP_CAUSE_INVALID_F_TEID_ALLOCATION_OPTION";
        break;
    case OGS_PFCP_CAUSE_NO_ESTABLISHED_PFCP_ASSOCIATION:
        return "OGS_PFCP_CAUSE_NO_ESTABLISHED_PFCP_ASSOCIATION";
        break;
    case OGS_PFCP_CAUSE_RULE_CREATION_MODIFICATION_FAILURE:
        return "OGS_PFCP_CAUSE_RULE_CREATION_MODIFICATION_FAILURE";
        break;
    case OGS_PFCP_CAUSE_PFCP_ENTITY_IN_CONGESTION:
        return "OGS_PFCP_CAUSE_PFCP_ENTITY_IN_CONGESTION";
        break;
    case OGS_PFCP_CAUSE_NO_RESOURCES_AVAILABLE:
        return "OGS_PFCP_CAUSE_NO_RESOURCES_AVAILABLE";
        break;
    case OGS_PFCP_CAUSE_SERVICE_NOT_SUPPORTED:
        return "OGS_PFCP_CAUSE_SERVICE_NOT_SUPPORTED";
        break;
    case OGS_PFCP_CAUSE_SYSTEM_FAILURE:
        return "OGS_PFCP_CAUSE_SYSTEM_FAILURE";
        break;
    default:
        break;
    }
    return "OGS_PFCP_CAUSE_UNKNOWN";
}

int16_t ogs_pfcp_build_user_plane_ip_resource_info(
        ogs_tlv_octet_t *octet,
        ogs_pfcp_user_plane_ip_resource_info_t *info,
        void *data, int data_len)
{
    ogs_pfcp_user_plane_ip_resource_info_t target;
    int16_t size = 0;

    ogs_assert(info);
    ogs_assert(octet);
    ogs_assert(data);
    ogs_assert(data_len);

    octet->data = data;
    memcpy(&target, info, sizeof(ogs_pfcp_user_plane_ip_resource_info_t));

    ogs_assert(size + sizeof(target.flags) <= data_len);
    memcpy((unsigned char *)octet->data + size,
            &target.flags, sizeof(target.flags));
    size += sizeof(target.flags);

    if (target.teidri) {
        ogs_assert(size + sizeof(target.teid_range) <= data_len);
        memcpy((unsigned char *)octet->data + size,
                &target.teid_range, sizeof(target.teid_range));
        size += sizeof(target.teid_range);
    }

    if (target.v4) {
        ogs_assert(size + sizeof(target.addr) <= data_len);
        memcpy((unsigned char *)octet->data + size,
                &target.addr, sizeof(target.addr));
        size += sizeof(target.addr);
    }

    if (target.v6) {
        ogs_assert(size + OGS_IPV6_LEN <= data_len);
        memcpy((unsigned char *)octet->data + size,
                &target.addr6, OGS_IPV6_LEN);
        size += OGS_IPV6_LEN;
    }

    if (target.assoni) {
        int len = ogs_fqdn_build((char *)octet->data + size,
                target.network_instance, strlen(target.network_instance));
        size += len;
    }

    if (target.assosi) {
        ogs_assert(size + sizeof(target.source_interface) <= data_len);
        memcpy((unsigned char *)octet->data + size,
                &target.source_interface, sizeof(target.source_interface));
        size += sizeof(target.source_interface);
    }

    octet->len = size;

    return octet->len;
}

int16_t ogs_pfcp_parse_user_plane_ip_resource_info(
        ogs_pfcp_user_plane_ip_resource_info_t *info,
        ogs_tlv_octet_t *octet)
{
    int16_t size = 0;

    ogs_assert(info);
    ogs_assert(octet);

    memset(info, 0, sizeof(ogs_pfcp_user_plane_ip_resource_info_t));

    memcpy(&info->flags,
            (unsigned char *)octet->data + size, sizeof(info->flags));
    size++;

    if (info->teidri) {
        ogs_assert(size + sizeof(info->teid_range) <= octet->len);
        memcpy(&info->teid_range, (unsigned char *)octet->data + size,
                sizeof(info->teid_range));
        size += sizeof(info->teid_range);
    }

    if (info->v4) {
        ogs_assert(size + sizeof(info->addr) <= octet->len);
        memcpy(&info->addr,
                (unsigned char *)octet->data + size,
                sizeof(info->addr));
        size += sizeof(info->addr);
    }

    if (info->v6) {
        ogs_assert(size + OGS_IPV6_LEN <= octet->len);
        memcpy(&info->addr6, (unsigned char *)octet->data + size, OGS_IPV6_LEN);
        size += OGS_IPV6_LEN;
    }

    if (info->assoni) {
        int len = octet->len - size;
        if (info->assosi) len--;

        ogs_fqdn_parse(info->network_instance, (char *)octet->data + size, len);
        size += len;
    }

    if (info->assosi) {
        ogs_assert(size + sizeof(info->source_interface) <=
                octet->len);
        memcpy(&info->source_interface, (unsigned char *)octet->data + size,
                sizeof(info->source_interface));
        size += sizeof(info->source_interface);
    }

    ogs_assert(size == octet->len);

    return size;
}

int16_t ogs_pfcp_build_sdf_filter(
        ogs_tlv_octet_t *octet, ogs_pfcp_sdf_filter_t *filter,
        void *data, int data_len)
{
    ogs_pfcp_sdf_filter_t target;
    int16_t size = 0;

    ogs_assert(filter);
    ogs_assert(octet);
    ogs_assert(data);
    ogs_assert(data_len);

    octet->data = data;
    memcpy(&target, filter, sizeof(ogs_pfcp_sdf_filter_t));

    ogs_assert(size + sizeof(target.flags) <= data_len);
    memcpy((unsigned char *)octet->data + size,
            &target.flags, sizeof(target.flags));
    size += sizeof(target.flags);

    ogs_assert(size + sizeof(target.spare2) <= data_len);
    memcpy((unsigned char *)octet->data + size,
            &target.spare2, sizeof(target.spare2));
    size += sizeof(target.spare2);

    if (target.fd) {
        ogs_assert(size + sizeof(target.flow_description_len) <= data_len);
        target.flow_description_len = htobe16(target.flow_description_len);
        memcpy((unsigned char *)octet->data + size,
                &target.flow_description_len,
                sizeof(target.flow_description_len));
        size += sizeof(target.flow_description_len);

        ogs_assert(size + filter->flow_description_len <= data_len);
        memcpy((char *)octet->data + size,
                filter->flow_description, filter->flow_description_len);
        size += filter->flow_description_len;
    }

    if (target.ttc) {
        ogs_assert(size + sizeof(target.tos_traffic_class) <= data_len);
        target.tos_traffic_class = htobe16(target.tos_traffic_class);
        memcpy((unsigned char *)octet->data + size,
                &target.tos_traffic_class, sizeof(target.tos_traffic_class));
        size += sizeof(target.tos_traffic_class);
    }

    if (target.spi) {
        ogs_assert(size + sizeof(target.security_parameter_index) <= data_len);
        target.security_parameter_index =
            htobe32(target.security_parameter_index);
        memcpy((unsigned char *)octet->data + size,
                &target.security_parameter_index,
                sizeof(target.security_parameter_index));
        size += sizeof(target.security_parameter_index);
    }

    if (target.fl) {
        int bit24_len = 3;
        ogs_assert(size + bit24_len <= data_len);
        target.flow_label = htobe32(target.flow_label);
        memcpy((unsigned char *)octet->data + size,
                &target.flow_label, bit24_len);
        size += bit24_len;
    }

    if (target.bid) {
        ogs_assert(size + sizeof(target.sdf_filter_id) <= data_len);
        target.sdf_filter_id =
            htobe32(target.sdf_filter_id);
        memcpy((unsigned char *)octet->data + size,
                &target.sdf_filter_id, sizeof(target.sdf_filter_id));
        size += sizeof(target.sdf_filter_id);
    }

    octet->len = size;

    return octet->len;
}

int16_t ogs_pfcp_parse_sdf_filter(
        ogs_pfcp_sdf_filter_t *filter, ogs_tlv_octet_t *octet)
{
    int16_t size = 0;

    ogs_assert(filter);
    ogs_assert(octet);

    memset(filter, 0, sizeof(ogs_pfcp_sdf_filter_t));

    ogs_assert(size + sizeof(filter->flags) <= octet->len);
    memcpy(&filter->flags,
            (unsigned char *)octet->data + size, sizeof(filter->flags));
    size++;

    ogs_assert(size + sizeof(filter->spare2) <= octet->len);
    memcpy(&filter->spare2,
            (unsigned char *)octet->data + size, sizeof(filter->flags));
    size++;

    if (filter->fd) {
        ogs_assert(size + sizeof(filter->flow_description_len) <= octet->len);
        memcpy(&filter->flow_description_len,
                (unsigned char *)octet->data + size,
                sizeof(filter->flow_description_len));
        filter->flow_description_len = be16toh(filter->flow_description_len);
        size += sizeof(filter->flow_description_len);

        filter->flow_description = (char *)octet->data + size;
        size += filter->flow_description_len;
    }

    if (filter->ttc) {
        ogs_assert(size + sizeof(filter->tos_traffic_class) <= octet->len);
        memcpy(&filter->tos_traffic_class,
                (unsigned char *)octet->data + size,
                sizeof(filter->tos_traffic_class));
        filter->tos_traffic_class = be32toh(filter->tos_traffic_class);
        size += sizeof(filter->tos_traffic_class);
    }

    if (filter->spi) {
        ogs_assert(size + sizeof(filter->security_parameter_index) <=
                octet->len);
        memcpy(&filter->security_parameter_index,
                (unsigned char *)octet->data + size,
                sizeof(filter->security_parameter_index));
        filter->security_parameter_index =
            be32toh(filter->security_parameter_index);
        size += sizeof(filter->security_parameter_index);
    }

    if (filter->fl) {
        int bit24_len = 3;
        ogs_assert(size + bit24_len <= octet->len);
        memcpy(&filter->flow_label,
                (unsigned char *)octet->data + size, bit24_len);
        filter->flow_label = be32toh(filter->flow_label);
        size += bit24_len;
    }

    if (filter->bid) {
        ogs_assert(size + sizeof(filter->sdf_filter_id) <= octet->len);
        memcpy(&filter->sdf_filter_id, (unsigned char *)octet->data + size,
                sizeof(filter->sdf_filter_id));
        filter->sdf_filter_id = be32toh(filter->sdf_filter_id);
        size += sizeof(filter->sdf_filter_id);
    }

    ogs_assert(size == octet->len);

    return size;
}

int16_t ogs_pfcp_build_bitrate(ogs_tlv_octet_t *octet,
        ogs_pfcp_bitrate_t *bitrate, void *data, int data_len)
{
    ogs_pfcp_bitrate_t target;
    int16_t size = 0;

    ogs_assert(bitrate);
    ogs_assert(octet);
    ogs_assert(data);
    ogs_assert(data_len >= OGS_PFCP_BITRATE_LEN);

    octet->data = data;
    memcpy(&target, bitrate, sizeof(ogs_pfcp_bitrate_t));

    /*
     * Ch 8.15 Bearer QoS in TS 29.274 v15.9.0
     *
     * The UL/DL MBR and GBR fields are encoded as kilobits
     * per second (1 kbps = 1000 bps) in binary value.
     */
    ogs_uint64_to_buffer(target.uplink / 1000, 5,
            (unsigned char *)octet->data + size);
    size += 5;
    ogs_uint64_to_buffer(target.downlink / 1000, 5,
            (unsigned char *)octet->data + size);
    size += 5;

    octet->len = size;

    return octet->len;
}
int16_t ogs_pfcp_parse_bitrate(
        ogs_pfcp_bitrate_t *bitrate, ogs_tlv_octet_t *octet)
{
    int16_t size = 0;

    ogs_assert(bitrate);
    ogs_assert(octet);
    ogs_assert(octet->len == OGS_PFCP_BITRATE_LEN);

    memset(bitrate, 0, sizeof(ogs_pfcp_bitrate_t));

    /*
     * Ch 8.15 Bearer QoS in TS 29.274 v15.9.0
     *
     * The UL/DL MBR and GBR fields are encoded as kilobits
     * per second (1 kbps = 1000 bps) in binary value.
     */
    bitrate->uplink = ogs_buffer_to_uint64(
            (unsigned char *)octet->data + size, 5) * 1000;
    size += 5;
    bitrate->downlink = ogs_buffer_to_uint64(
            (unsigned char *)octet->data + size, 5) * 1000;
    size += 5;

    ogs_assert(size == octet->len);

    return size;
}
