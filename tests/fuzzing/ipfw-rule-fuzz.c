/*
 * Copyright (C) 2019-2026 by Sukchan Lee <acetcom@gmail.com>
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

/*
 * IPFilterRule flow-description parser fuzz target.
 *
 * A PCC rule carries its packet filter as an IPFilterRule text string, which
 * reaches the SMF from the PCF over N7 and from the PGW over Gx.
 * ogs_ipfw_compile_rule() tokenises that string and hands it to compile_rule()
 * in the bundled FreeBSD ipfw parser, so the whole of lib/ipfw parses remote
 * input. No existing fuzz target links lib/ipfw, so none of it is instrumented.
 *
 * The bundled parser redefines errx() as a log call (lib/ipfw/ipfw2.c), so the
 * paths FreeBSD ends with a fatal error instead continue with whatever state
 * they had reached. That makes the error paths worth exercising, not just the
 * accepting ones.
 */

#include <stdio.h>
#include <stdint.h>

#include "fuzzing.h"
#include "ipfw/ogs-ipfw.h"

/* "permit out" is the shortest prefix the parser accepts. */
#define kMinInputLength 4
#define kMaxInputLength 512

extern int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size)
{
    char *flow_description;
    ogs_ipfw_rule_t ipfw_rule;

    if (Size < kMinInputLength || Size > kMaxInputLength) {
        return 1;
    }

    if (!initialized) {
        initialize();
    }

    /* The parser uses ogs_strtok_r() and strcmp(), so the input has to be a
     * NUL-terminated string. An exact-size allocation keeps a read past the
     * terminator detectable. */
    flow_description = ogs_malloc(Size + 1);
    if (flow_description == NULL) {
        return 1;
    }
    memcpy(flow_description, Data, Size);
    flow_description[Size] = '\0';

    ogs_ipfw_compile_rule(&ipfw_rule, flow_description);

    ogs_free(flow_description);

    return 0;
}
