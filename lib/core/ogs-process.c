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

#include "core-config-private.h"

#if HAVE_SYS_WAIT_H
#include <sys/wait.h>
#endif

#if HAVE_FCNTL_H
#include <fcntl.h>
#endif

#if HAVE_SIGNAL_H
#include <signal.h>
#endif

#if HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "ogs-core.h"

int ogs_proc_create(const char *const commandLine[], int options,
                    ogs_proc_t *const out_process)
{
#if defined(_WIN32)
    struct process_process_information_s {
        void *hProcess;
        void *hThread;
        unsigned long dwProcessId;
        unsigned long dwThreadId;
    };

    ogs_proc_tecurity_attributes_s {
        unsigned long nLength;
        void *lpSecurityDescriptor;
        int bInheritHandle;
    };

    ogs_proc_ttartup_info_s {
        unsigned long cb;
        char *lpReserved;
        char *lpDesktop;
        char *lpTitle;
        unsigned long dwX;
        unsigned long dwY;
        unsigned long dwXSize;
        unsigned long dwYSize;
        unsigned long dwXCountChars;
        unsigned long dwYCountChars;
        unsigned long dwFillAttribute;
        unsigned long dwFlags;
        unsigned short wShowWindow;
        unsigned short cbReserved2;
        unsigned char *lpReserved2;
        void *hStdInput;
        void *hStdOutput;
        void *hStdError;
    };

    int fd;
    void *rd, *wr;
    char *commandLineCombined;
    size_t len;
    int i, j;
    const unsigned long startFUseStdHandles = 0x00000100;
    const unsigned long handleFlagInherit = 0x00000001;
    struct process_process_information_s processInfo;
    ogs_proc_tecurity_attributes_s saAttr = {sizeof(saAttr), 0, 1};
    char *environment = 0;
    ogs_proc_ttartup_info_s startInfo = { 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                0, 0, 0, 0, 0, 0, 0, 0, 0 };

    ogs_assert(out_process);

    startInfo.cb = sizeof(startInfo);
    startInfo.dwFlags = startFUseStdHandles;

    if (ogs_proc_option_inherit_environment !=
        (options & ogs_proc_option_inherit_environment)) {
        environment = "\0\0";
    }

    if (!CreatePipe(&rd, &wr, (LPSECURITY_ATTRIBUTES)&saAttr, 0)) {
        return OGS_ERROR;
    }

    if (!SetHandleInformation(wr, handleFlagInherit, 0)) {
        return OGS_ERROR;
    }

    fd = _open_osfhandle((intptr_t)wr, 0);

    if (-1 != fd) {
        out_process->stdin_file = _fdopen(fd, "wb");

        if (0 == out_process->stdin_file) {
            return OGS_ERROR;
        }
    }

    startInfo.hStdInput = rd;

    if (!CreatePipe(&rd, &wr, (LPSECURITY_ATTRIBUTES)&saAttr, 0)) {
        return OGS_ERROR;
    }

    if (!SetHandleInformation(rd, handleFlagInherit, 0)) {
        return OGS_ERROR;
    }

    fd = _open_osfhandle((intptr_t)rd, 0);

    if (-1 != fd) {
        out_process->stdout_file = _fdopen(fd, "rb");

        if (0 == out_process->stdout_file) {
            return OGS_ERROR;
        }
    }

    startInfo.hStdOutput = wr;

    if (ogs_proc_option_combined_stdout_stderr ==
        (options & ogs_proc_option_combined_stdout_stderr)) {
        out_process->stderr_file = out_process->stdout_file;
        startInfo.hStdError = startInfo.hStdOutput;
    } else {
        if (!CreatePipe(&rd, &wr, (LPSECURITY_ATTRIBUTES)&saAttr, 0)) {
            return OGS_ERROR;
        }

        if (!SetHandleInformation(rd, handleFlagInherit, 0)) {
            return OGS_ERROR;
        }

        fd = _open_osfhandle((intptr_t)rd, 0);

        if (-1 != fd) {
            out_process->stderr_file = _fdopen(fd, "rb");

            if (0 == out_process->stderr_file) {
                return OGS_ERROR;
            }
        }

        startInfo.hStdError = wr;
    }

    // Combine commandLine together into a single string
    len = 0;
    for (i = 0; commandLine[i]; i++) {
        // For the ' ' between items and trailing '\0'
        len++;

        for (j = 0; '\0' != commandLine[i][j]; j++) {
            len++;
        }
    }

    commandLineCombined = (char *)_alloca(len);

    if (!commandLineCombined) {
        return OGS_ERROR;
    }

    // Gonna re-use len to store the write index into commandLineCombined
    len = 0;

    for (i = 0; commandLine[i]; i++) {
        if (0 != i) {
        commandLineCombined[len++] = ' ';
    }

    for (j = 0; '\0' != commandLine[i][j]; j++) {
        commandLineCombined[len++] = commandLine[i][j];
        }
    }

    commandLineCombined[len] = '\0';

    if (!CreateProcessA(NULL,
        commandLineCombined, // command line
        NULL,                // process security attributes
        NULL,                // primary thread security attributes
        1,                   // handles are inherited
        0,                   // creation flags
        environment,         // use parent's environment
        NULL,                // use parent's current directory
        (LPSTARTUPINFOA)&startInfo, // STARTUPINFO pointer
        (LPPROCESS_INFORMATION)&processInfo)) {
        return OGS_ERROR;
    }

    out_process->hProcess = processInfo.hProcess;
    out_process->dwProcessId = processInfo.dwProcessId;

    // We don't need the handle of the primary thread in the called process.
    CloseHandle(processInfo.hThread);

    return OGS_OK;
#else
    int stdinfd[2];
    int stdoutfd[2];
    int stderrfd[2];
    pid_t child;

    ogs_assert(out_process);

    if (0 != pipe(stdinfd)) {
        return OGS_ERROR;
    }

    if (0 != pipe(stdoutfd)) {
        return OGS_ERROR;
    }

    if (ogs_proc_option_combined_stdout_stderr !=
        (options & ogs_proc_option_combined_stdout_stderr)) {
        if (0 != pipe(stderrfd)) {
            return OGS_ERROR;
        }
    }

    child = fork();

    if (-1 == child) {
        return OGS_ERROR;
    }

    if (0 == child) {
        // Close the stdin write end
        close(stdinfd[1]);
        // Map the read end to stdin
        dup2(stdinfd[0], STDIN_FILENO);

        // Close the stdout read end
        close(stdoutfd[0]);
        // Map the write end to stdout
        dup2(stdoutfd[1], STDOUT_FILENO);

        if (ogs_proc_option_combined_stdout_stderr ==
            (options & ogs_proc_option_combined_stdout_stderr)) {
            dup2(STDOUT_FILENO, STDERR_FILENO);
        } else {
            // Close the stderr read end
            close(stderrfd[0]);
            // Map the write end to stdout
            dup2(stderrfd[1], STDERR_FILENO);
        }

        if (ogs_proc_option_inherit_environment !=
            (options & ogs_proc_option_inherit_environment)) {
            char *const environment[1] = {0};
            exit(execve(commandLine[0], (char *const *)commandLine, environment));
        } else {
            exit(execvp(commandLine[0], (char *const *)commandLine));
        }
    } else {
        // Close the stdin read end
        close(stdinfd[0]);
        // Store the stdin write end
        out_process->stdin_file = fdopen(stdinfd[1], "wb");

        // Close the stdout write end
        close(stdoutfd[1]);
        // Store the stdout read end
        out_process->stdout_file = fdopen(stdoutfd[0], "rb");

        if (ogs_proc_option_combined_stdout_stderr ==
            (options & ogs_proc_option_combined_stdout_stderr)) {
            out_process->stderr_file = out_process->stdout_file;
        } else {
            // Close the stderr write end
            close(stderrfd[1]);
            // Store the stderr read end
            out_process->stderr_file = fdopen(stderrfd[0], "rb");
        }

        // Store the child's pid
        out_process->child = child;

        return OGS_OK;
    }
#endif
}

FILE *ogs_proc_stdin(const ogs_proc_t *const process)
{
    ogs_assert(process);
    return process->stdin_file;
}

FILE *ogs_proc_stdout(const ogs_proc_t *const process)
{
    ogs_assert(process);
    return process->stdout_file;
}

FILE *ogs_proc_stderr(const ogs_proc_t *const process)
{
    ogs_assert(process);
    if (process->stdout_file != process->stderr_file) {
        return process->stderr_file;
    } else {
        return OGS_OK;
    }
}

int ogs_proc_join(ogs_proc_t *const process, int *const out_return_code)
{
#if defined(_WIN32)
    const unsigned long infinite = 0xFFFFFFFF;

    ogs_assert(process);
    ogs_assert(out_return_code);
    if (0 != process->stdin_file) {
        fclose(process->stdin_file);
        process->stdin_file = 0;
    }

    WaitForSingleObject(process->hProcess, infinite);

    if (out_return_code) {
        if (!GetExitCodeProcess(process->hProcess,
            (unsigned long *)out_return_code)) {
            return OGS_ERROR;
        }
    }

    return OGS_OK;
#else
    int status;

    ogs_assert(process);
    ogs_assert(out_return_code);
    if (0 != process->stdin_file) {
        fclose(process->stdin_file);
        process->stdin_file = 0;
    }

    if (process->child != waitpid(process->child, &status, 0)) {
        return OGS_ERROR;
    }

    if (out_return_code) {
        if (WIFEXITED(status)) {
            *out_return_code = WEXITSTATUS(status);
        } else {
            *out_return_code = 0;
        }
    }

    return OGS_OK;
#endif
}

int ogs_proc_destroy(ogs_proc_t *const process)
{
    ogs_assert(process);
    if (0 != process->stdin_file) {
        fclose(process->stdin_file);
    }

    fclose(process->stdout_file);

    if (process->stdout_file != process->stderr_file) {
        fclose(process->stderr_file);
    }

#if defined(_WIN32)
    CloseHandle(process->hProcess);
#endif

    return OGS_OK;
}

int ogs_proc_terminate(ogs_proc_t *const process)
{
    ogs_assert(process);
#if defined(_WIN32)
    // `GenerateConsoleCtrlEvent` can only be called on a process group. To call
    // `GenerateConsoleCtrlEvent` on a single child process it has to be put in
    // its own process group (which we did when starting the child process).
    if (!GenerateConsoleCtrlEvent(CTRL_BREAK_EVENT, process->dwProcessId)) {
        return OGS_ERROR;
    }
#else
    if (kill(process->child, SIGTERM) == -1) {
        return OGS_ERROR;
    }
#endif

    return OGS_OK;
}
    
int ogs_proc_kill(ogs_proc_t *const process)
{
    ogs_assert(process);
#if defined(_WIN32)
    // We use 137 as the exit status because it is the same exit status as a
    // process that is stopped with the `SIGKILL` signal on POSIX systems.
    if (!TerminateProcess(process->hProcess, 137)) {
        return OGS_ERROR;
    }
#else
    if (kill(process->child, SIGKILL) == -1) {
        return OGS_ERROR;
    }
#endif

    return OGS_OK;
}
