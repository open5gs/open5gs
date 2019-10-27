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

#ifndef OGS_SIGNAL_H
#define OGS_SIGNAL_H

#ifdef __cplusplus
extern "C" {
#endif

#if HAVE_SIGACTION

#if defined(__APPLE__) && !defined(__cplusplus) && !defined(_ANSI_SOURCE)
/* work around Darwin header file bugs
 *   http://www.opensource.apple.com/bugs/X/BSD%20Kernel/2657228.html
 */
#undef SIG_DFL
#undef SIG_IGN
#undef SIG_ERR
#define SIG_DFL (void (*)(int))0
#define SIG_IGN (void (*)(int))1
#define SIG_ERR (void (*)(int))-1
#endif

typedef void ogs_sigfunc_t(int);

ogs_sigfunc_t *ogs_signal(int signo, ogs_sigfunc_t *func);

#if defined(SIG_IGN) && !defined(SIG_ERR)
#define SIG_ERR ((ogs_sigfunc_t *) -1)
#endif

#else /* !HAVE_SIGACTION */
#define ogs_signal(a, b) signal(a, b)
#endif


void ogs_signal_init(void);
const char *ogs_signal_description_get(int signum);

int ogs_setup_signal_thread(void);
int ogs_signal_thread(int(*signal_handler)(int signum));

int ogs_signal_block(int signum);
int ogs_signal_unblock(int signum);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OGS_SIGNAL_H */
