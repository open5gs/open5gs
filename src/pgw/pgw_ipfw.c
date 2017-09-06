#define TRACE_MODULE _pgw_ipfw

#include "pgw_ipfw.h"

#include "ipfw2.h"
#include "objs/include_e/netinet/ip_fw.h"

#define MAX_NUM_OF_TOKEN 32
#define MAX_NUM_OF_RULE_BUFFER 1024

void compile_rule(char *av[], uint32_t *rbuf, int *rbufsize, void *tstate);

status_t pgw_compile_packet_filter(pgw_rule_t *pgw_rule, c_int8_t *description)
{
    pgw_rule_t zero_rule;
    char *token, *dir;
    char *saveptr;
    int i = 2;

    char *av[MAX_NUM_OF_TOKEN];
	uint32_t rulebuf[MAX_NUM_OF_RULE_BUFFER];
	int rbufsize;
	struct ip_fw_rule *rule = (struct ip_fw_rule *)rulebuf;

	int l;
	ipfw_insn *cmd;

    d_assert(pgw_rule, return CORE_ERROR, "Null param");
    d_assert(description, return CORE_ERROR, "Null param");

	rbufsize = sizeof(rulebuf);
	memset(rulebuf, 0, rbufsize);

    av[0] = NULL;

    /* ACTION */
    token = strtok_r(description, " ", &saveptr);
    if (strcmp(token, "permit") != 0)
    {
        d_error("Not begins with reserved keyword : 'permit'");
        return CORE_ERROR;
    }
    av[1] = token;

    /* Save DIRECTION */
    dir = token = strtok_r(NULL, " ", &saveptr);
    if (strcmp(token, "out") != 0)
    {
        d_error("Not begins with reserved keyword : 'permit out'");
        return CORE_ERROR;
    }

    /* ADDR */
    token = strtok_r(NULL, " ", &saveptr);
    while(token != NULL)
    {
        av[i++] = token;
        token = strtok_r(NULL, " ", &saveptr);
    }

    /* Add DIRECTION */
    av[i++] = dir;

    av[i] = NULL;

	compile_rule(av, (uint32_t *)rule, &rbufsize, NULL);

    memset(pgw_rule, 0, sizeof(pgw_rule_t));
	for (l = rule->act_ofs, cmd = rule->cmd;
			l > 0 ; l -= F_LEN(cmd) , cmd += F_LEN(cmd))
    {
		switch(cmd->opcode) 
        {
            case O_PROTO:
                pgw_rule->proto = cmd->arg1;
                break;
            case O_IP_SRC:
            case O_IP_SRC_MASK:
            {
                uint32_t *a = ((ipfw_insn_u32 *)cmd)->d;
                pgw_rule->ipv4.local.addr = a[0];
                if (cmd->opcode == O_IP_SRC_MASK)
                    pgw_rule->ipv4.local.mask = a[1];
                else
                    pgw_rule->ipv4.local.mask = 0xffffffff;

                break;
            }
            case O_IP_DST:
            case O_IP_DST_MASK:
            {
                uint32_t *a = ((ipfw_insn_u32 *)cmd)->d;
                pgw_rule->ipv4.remote.addr = a[0];
                if (cmd->opcode == O_IP_SRC_MASK)
                    pgw_rule->ipv4.remote.mask = a[1];
                else
                    pgw_rule->ipv4.remote.mask = 0xffffffff;
                break;
            }
            case O_IP_SRCPORT:
            {
                uint16_t *p = ((ipfw_insn_u16 *)cmd)->ports;
                pgw_rule->port.local.low = p[0];
                pgw_rule->port.local.high = p[1];
                break;
            }
            case O_IP_DSTPORT:
            {
                uint16_t *p = ((ipfw_insn_u16 *)cmd)->ports;
                pgw_rule->port.remote.low = p[0];
                pgw_rule->port.remote.high = p[1];
                break;
            }
        }
	}

    memset(&zero_rule, 0, sizeof(pgw_rule_t));
    if (memcmp(pgw_rule, &zero_rule, sizeof(pgw_rule_t)) == 0)
    {
        d_error("Cannot find Flow-Description");
        return CORE_ERROR;
    }

    return CORE_OK;
}
