/*
 * Copyright (C) 2019-2020 by Sukchan Lee <acetcom@gmail.com>
 *
 * The code is stolen from process.h
 * https://github.com/sheredom/process.h
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
 */

#if !defined(OGS_CORE_INSIDE) && !defined(OGS_CORE_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_PROCESS_H
#define OGS_PROCESS_H

#ifdef __cplusplus
extern "C" {
#endif

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

int ogs_proc_create(const char *const commandLine[], int options,
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
