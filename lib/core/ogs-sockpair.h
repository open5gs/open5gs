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

#if !defined(OGS_CORE_INSIDE) && !defined(OGS_CORE_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_SOCKPAIR_H
#define OGS_SOCKPAIR_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
#define AF_SOCKPAIR     AF_INET
#else
#define AF_SOCKPAIR     AF_UNIX
#endif

int ogs_socketpair(int family, int type, int protocol, ogs_socket_t fd[2]);

#ifdef __cplusplus
}
#endif

#endif /* OGS_SOCKPAIR_H */
