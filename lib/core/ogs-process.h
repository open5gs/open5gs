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

#ifndef OGS_PROCESS_H
#define OGS_PROCESS_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * The following code is stolen from process.h
 * https://github.com/sheredom/process.h
 */

#if defined(_WIN32)
typedef struct _PROCESS_INFORMATION *LPPROCESS_INFORMATION;
typedef struct _SECURITY_ATTRIBUTES *LPSECURITY_ATTRIBUTES;
typedef struct _STARTUPINFOA *LPSTARTUPINFOA;

__declspec(dllimport) int __stdcall SetHandleInformation(void *, unsigned long,
                                                         unsigned long);
__declspec(dllimport) int __stdcall CreatePipe(void **, void **,
                                               LPSECURITY_ATTRIBUTES,
                                               unsigned long);
__declspec(dllimport) int __stdcall CreateProcessA(
    const char *, char *, LPSECURITY_ATTRIBUTES, LPSECURITY_ATTRIBUTES, int,
    unsigned long, void *, const char *, LPSTARTUPINFOA, LPPROCESS_INFORMATION);
__declspec(dllimport) int __stdcall CloseHandle(void *);
__declspec(dllimport) unsigned long __stdcall WaitForSingleObject(
    void *, unsigned long);
__declspec(dllimport) int __stdcall GetExitCodeProcess(
    void *, unsigned long *lpExitCode);
__declspec(dllimport) int __cdecl _open_osfhandle(intptr_t, int);
void *__cdecl _alloca(size_t);
#endif

typedef struct ogs_proc_s {
    FILE *stdin_file;
    FILE *stdout_file;
    FILE *stderr_file;

#if defined(_WIN32)
    void *hProcess;
    unsigned long dwProcessId;
#else
    pid_t child;
#endif
} ogs_proc_t;

enum ogs_proc_option_e {
    // stdout and stderr are the same FILE.
    ogs_proc_option_combined_stdout_stderr = 0x1,

    // The child process should inherit the environment variables of the parent.
    ogs_proc_option_inherit_environment = 0x2
};

int ogs_proc_create(const char *const command_line[], int options,
                    ogs_proc_t *const out_process);

FILE *ogs_proc_stdin(const ogs_proc_t *const process);
FILE *ogs_proc_stdout(const ogs_proc_t *const process);
FILE *ogs_proc_stderr(const ogs_proc_t *const process);

int ogs_proc_join(ogs_proc_t *const process, int *const out_return_code);
int ogs_proc_destroy(ogs_proc_t *const process);

int ogs_proc_terminate(ogs_proc_t *const process);
int ogs_proc_kill(ogs_proc_t *const process);

#ifdef __cplusplus
}
#endif

#endif /* OGS_PROCESS_H */
