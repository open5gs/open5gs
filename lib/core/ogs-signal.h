/* Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
 *
 * This file is part of Open5GS.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
