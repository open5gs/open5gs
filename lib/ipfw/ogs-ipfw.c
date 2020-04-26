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

#ifndef IF_NAMESIZE
#define	IF_NAMESIZE	16
#ifndef IFNAMSIZ
#define	IFNAMSIZ	IF_NAMESIZE
#endif
#endif

#ifndef OGS_ERROR
#define OGS_ERROR -1
#endif

#ifndef OGS_OK
#define OGS_OK 0
#endif

#ifndef OGS_IPV6_LEN
#define OGS_IPV6_LEN 16
#endif

#include "ipfw2.h"
#include "objs/include_e/netinet/ip_fw.h"

#include "ogs-ipfw.h"

#define MAX_NUM_OF_TOKEN 32
#define MAX_NUM_OF_RULE_BUFFER 1024

void compile_rule(char *av[], uint32_t *rbuf, int *rbufsize, void *tstate);

int ogs_ipfw_compile_rule(ogs_ipfw_rule_t *ipfw_rule, char *description)
{
    ogs_ipfw_rule_t zero_rule;
    char *token, *dir;
    char *saveptr;
    int i = 2;

    char *av[MAX_NUM_OF_TOKEN];
	uint32_t rulebuf[MAX_NUM_OF_RULE_BUFFER];
	int rbufsize;
	struct ip_fw_rule *rule = (struct ip_fw_rule *)rulebuf;

	int l;
	ipfw_insn *cmd;

    if (!ipfw_rule) {
        fprintf(stderr, "ipfw_rule is NULL\n");
        return OGS_ERROR;
    }

	rbufsize = sizeof(rulebuf);
	memset(rulebuf, 0, rbufsize);

    av[0] = NULL;

    /* ACTION */
    if (!description) { /* FIXME : OLD gcc generates uninitialized warning */
        fprintf(stderr, "description is NULL\n");
        return OGS_ERROR;
    }
    token = strtok_r(description, " ", &saveptr);
    if (strcmp(token, "permit") != 0) {
        fprintf(stderr, "Not begins with reserved keyword : 'permit'");
        return OGS_ERROR;
    }
    av[1] = token;

    /* Save DIRECTION */
    dir = token = strtok_r(NULL, " ", &saveptr);
    if (strcmp(token, "out") != 0) {
        fprintf(stderr, "Not begins with reserved keyword : 'permit out'");
        return OGS_ERROR;
    }

    /* ADDR */
    token = strtok_r(NULL, " ", &saveptr);
    while (token != NULL) {
        av[i++] = token;
        token = strtok_r(NULL, " ", &saveptr);
    }

    /* Add DIRECTION */
    av[i++] = dir;

    av[i] = NULL;

	compile_rule(av, (uint32_t *)rule, &rbufsize, NULL);

    memset(ipfw_rule, 0, sizeof(ogs_ipfw_rule_t));
	for (l = rule->act_ofs, cmd = rule->cmd;
			l > 0 ; l -= F_LEN(cmd) , cmd += F_LEN(cmd)) {
        uint32_t *a = NULL;
        uint16_t *p = NULL;
		switch (cmd->opcode) {
        case O_PROTO:
            ipfw_rule->proto = cmd->arg1;
            break;
        case O_IP_SRC:
        case O_IP_SRC_MASK:
            a = ((ipfw_insn_u32 *)cmd)->d;
            ipfw_rule->ipv4_local = 1;
            ipfw_rule->ip.local.addr[0] = a[0];
            if (cmd->opcode == O_IP_SRC_MASK)
                ipfw_rule->ip.local.mask[0] = a[1];
            else
                ipfw_rule->ip.local.mask[0] = 0xffffffff;
            break;
        case O_IP_DST:
        case O_IP_DST_MASK:
            a = ((ipfw_insn_u32 *)cmd)->d;
            ipfw_rule->ipv4_remote = 1;
            ipfw_rule->ip.remote.addr[0] = a[0];
            if (cmd->opcode == O_IP_DST_MASK)
                ipfw_rule->ip.remote.mask[0] = a[1];
            else
                ipfw_rule->ip.remote.mask[0] = 0xffffffff;
            break;
        case O_IP6_SRC:
        case O_IP6_SRC_MASK:
            a = ((ipfw_insn_u32 *)cmd)->d;
            ipfw_rule->ipv6_local = 1;
            memcpy(ipfw_rule->ip.local.addr, a, OGS_IPV6_LEN);
            if (cmd->opcode == O_IP6_SRC_MASK)
                memcpy(ipfw_rule->ip.local.mask, a+4, OGS_IPV6_LEN);
            else
                n2mask((struct in6_addr *)ipfw_rule->ip.local.mask, 128);
            break;
        case O_IP6_DST:
        case O_IP6_DST_MASK:
            a = ((ipfw_insn_u32 *)cmd)->d;
            ipfw_rule->ipv6_remote = 1;
            memcpy(ipfw_rule->ip.remote.addr, a, OGS_IPV6_LEN);
            if (cmd->opcode == O_IP6_DST_MASK)
                memcpy(ipfw_rule->ip.remote.mask, a+4, OGS_IPV6_LEN);
            else
                n2mask((struct in6_addr *)ipfw_rule->ip.remote.mask, 128);
            break;
        case O_IP_SRCPORT:
            p = ((ipfw_insn_u16 *)cmd)->ports;
            ipfw_rule->port.local.low = p[0];
            ipfw_rule->port.local.high = p[1];
            break;
        case O_IP_DSTPORT:
            p = ((ipfw_insn_u16 *)cmd)->ports;
            ipfw_rule->port.remote.low = p[0];
            ipfw_rule->port.remote.high = p[1];
            break;
        }
	}

    memset(&zero_rule, 0, sizeof(ogs_ipfw_rule_t));
    if (memcmp(ipfw_rule, &zero_rule, sizeof(ogs_ipfw_rule_t)) == 0) {
        fprintf(stderr, "Cannot find Flow-Description");
        return OGS_ERROR;
    }

    return OGS_OK;
}
