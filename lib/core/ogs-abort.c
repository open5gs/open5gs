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

#include "core-config-private.h"

#if HAVE_EXECINFO_H
#include <execinfo.h>
#endif

#include "ogs-core.h"

OGS_GNUC_NORETURN void ogs_abort(void)
{
#if HAVE_BACKTRACE
    int i;
    int nptrs;
    void *buffer[100];
    char **strings;

    nptrs = backtrace(buffer, OGS_ARRAY_SIZE(buffer));
    ogs_fatal("backtrace() returned %d addresses", nptrs);

    strings = backtrace_symbols(buffer, nptrs);
    if (strings) {
        for (i = 1; i < nptrs; i++)
            ogs_log_print(OGS_LOG_FATAL, "%s\n", strings[i]);

        free(strings);
    }

    abort();
#elif defined(_WIN32)
    DebugBreak();
    abort();
    ExitProcess(127);
#else
    abort();
#endif
}
