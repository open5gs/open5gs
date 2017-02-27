#include "core.h"
#include "core_signal.h"

CORE_DECLARE(status_t) core_kill(pid_t pid, int signum)
{
#ifdef OS2
    /* SIGTERM's don't work too well in OS/2 (only affects other EMX
     * programs). CGIs may not be, esp. REXX scripts, so use a native
     * call instead
     */
    if (signum == SIGTERM) {
        return APR_FROM_OS_ERROR(DosSendSignalException(pid,
                                                     XCPT_SIGNAL_BREAK));
    }
#endif /* OS2 */

    if (kill(pid, signum) == -1) {
        return errno;
    }

    return CORE_OK;
}

#if HAVE_SIGACTION

#if defined(__NetBSD__) || defined(DARWIN)
static void avoid_zombies(int signo)
{
    int exit_status;

    while (waitpid(-1, &exit_status, WNOHANG) > 0) {
        /* do nothing */
    }
}
#endif /* DARWIN */

/*
 * Replace standard signal() with the more reliable sigaction equivalent
 * from W. Richard Stevens' "Advanced Programming in the UNIX Environment"
 * (the version that does not automatically restart system calls).
 */
core_sigfunc_t *core_signal(int signo, core_sigfunc_t *func)
{
    struct sigaction act, oact;

    act.sa_handler = func;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
#ifdef SA_INTERRUPT             /* SunOS */
    act.sa_flags |= SA_INTERRUPT;
#endif
#if defined(__osf__) && defined(__alpha)
    /* XXX jeff thinks this should be enabled whenever SA_NOCLDWAIT is defined */

    /* this is required on Tru64 to cause child processes to
     * disappear gracefully - XPG4 compatible
     */
    if ((signo == SIGCHLD) && (func == SIG_IGN))
    {
        act.sa_flags |= SA_NOCLDWAIT;
    }
#endif
#if defined(__NetBSD__) || defined(DARWIN)
    /* ignoring SIGCHLD or leaving the default disposition doesn't avoid zombies,
     * and there is no SA_NOCLDWAIT flag, so catch the signal and reap status in
     * the handler to avoid zombies
     */
    if ((signo == SIGCHLD) && (func == SIG_IGN))
    {
        act.sa_handler = avoid_zombies;
    }
#endif
    if (sigaction(signo, &act, &oact) < 0)
        return SIG_ERR;
    return oact.sa_handler;
}

#endif

static void remove_sync_sigs(sigset_t *sig_mask)
{
#ifdef SIGABRT
    sigdelset(sig_mask, SIGABRT);
#endif
#ifdef SIGBUS
    sigdelset(sig_mask, SIGBUS);
#endif
#ifdef SIGEMT
    sigdelset(sig_mask, SIGEMT);
#endif
#ifdef SIGFPE
    sigdelset(sig_mask, SIGFPE);
#endif
#ifdef SIGILL
    sigdelset(sig_mask, SIGILL);
#endif
#ifdef SIGIOT
    sigdelset(sig_mask, SIGIOT);
#endif
#ifdef SIGPIPE
    sigdelset(sig_mask, SIGPIPE);
#endif
#ifdef SIGSEGV
    sigdelset(sig_mask, SIGSEGV);
#endif
#ifdef SIGSYS
    sigdelset(sig_mask, SIGSYS);
#endif
#ifdef SIGTRAP
    sigdelset(sig_mask, SIGTRAP);
#endif
#ifdef SIGCHLD
    sigdelset(sig_mask, SIGCHLD);
#endif
#ifdef SIGWINCH
    sigdelset(sig_mask, SIGWINCH);
#endif

/* the rest of the signals removed from the mask in this function
 * absolutely must be removed; you cannot block synchronous signals
 * (requirement of pthreads API)
 *
 * SIGUSR2 is being removed from the mask for the convenience of
 * Purify users (Solaris, HP-UX, SGI) since Purify uses SIGUSR2
 */
#ifdef SIGUSR2
    sigdelset(sig_mask, SIGUSR2);
#endif
}

status_t signal_thread(int (*signal_handler)(int signum))
{
    sigset_t sig_mask;
    int (*sig_func)(int signum) = (int (*)(int))signal_handler;

    /* This thread will be the one responsible for handling signals */
    sigfillset(&sig_mask);

    /* On certain platforms, sigwait() returns EINVAL if any of various
     * unblockable signals are included in the mask.  This was first
     * observed on AIX and Tru64.
     */
#ifdef SIGKILL
    sigdelset(&sig_mask, SIGKILL);
#endif
#ifdef SIGSTOP
    sigdelset(&sig_mask, SIGSTOP);
#endif
#ifdef SIGCONT
    sigdelset(&sig_mask, SIGCONT);
#endif
#ifdef SIGWAITING
    sigdelset(&sig_mask, SIGWAITING);
#endif

    /* no synchronous signals should be in the mask passed to sigwait() */
    remove_sync_sigs(&sig_mask);

    /* On AIX (4.3.3, at least), sigwait() won't wake up if the high-
     * order bit of the second word of flags is turned on.  sigdelset()
     * returns an error when trying to turn this off, so we'll turn it
     * off manually.
     *
     * Note that the private fields differ between 32-bit and 64-bit
     * and even between _ALL_SOURCE and !_ALL_SOURCE.  Except that on
     * AIX 4.3 32-bit builds and 64-bit builds use the same definition.
     *
     * Applicable AIX fixes such that this is no longer needed:
     *
     * APAR IY23096 for AIX 51B, fix included in AIX 51C, and
     * APAR IY24162 for 43X.
     */
#if defined(_AIX)
#if defined(__64BIT__) && defined(_AIXVERSION_510)
#ifdef _ALL_SOURCE
        sig_mask.ss_set[3] &= 0x7FFFFFFF;
#else /* not _ALL_SOURCE */
        sig_mask.__ss_set[3] &= 0x7FFFFFFF;
#endif
#else /* not 64-bit build, or 64-bit build on 4.3 */
#ifdef _ALL_SOURCE
        sig_mask.hisigs &= 0x7FFFFFFF;
#else /* not _ALL_SOURCE */
        sig_mask.__hisigs &= 0x7FFFFFFF;
#endif
#endif
#endif /* _AIX */

    while (1) {
#if HAVE_SIGWAIT
        int signal_received;

        if (sigwait(&sig_mask, &signal_received) != 0)
        {
            /* handle sigwait() error here */
        }

        if (sig_func(signal_received) == 1)
        {
            return CORE_OK;
        }
#elif HAVE_SIGSUSPEND
        sigsuspend(&sig_mask);
#else
#error No sigwait() and no sigsuspend()
#endif
    }
}

status_t signal_init(void)
{
    sigset_t sig_mask;
    int rv;

    /* All threads should mask out signals to be handled by
     * the thread doing sigwait().
     *
     * No thread should ever block synchronous signals.
     * See the Solaris man page for pthread_sigmask() for
     * some information.  Solaris chooses to knock out such
     * processes when a blocked synchronous signal is 
     * delivered, skipping any registered signal handler.
     * AIX doesn't call a signal handler either.  At least
     * one level of linux+glibc does call the handler even
     * when the synchronous signal is blocked.
     */
    sigfillset(&sig_mask);
    remove_sync_sigs(&sig_mask);

#if defined(SIGPROCMASK_SETS_THREAD_MASK) || ! APR_HAS_THREADS
    if ((rv = sigprocmask(SIG_SETMASK, &sig_mask, NULL)) != 0) {
        rv = errno;
    }
#else
    if ((rv = pthread_sigmask(SIG_SETMASK, &sig_mask, NULL)) != 0) {
#ifdef HAVE_ZOS_PTHREADS
        rv = errno;
#endif
    }
#endif
    return rv;
}

status_t signal_block(int signum)
{
#if HAVE_SIGACTION
    sigset_t sig_mask;
    int rv;

    sigemptyset(&sig_mask);

    sigaddset(&sig_mask, signum);

#if defined(SIGPROCMASK_SETS_THREAD_MASK)
    if ((rv = sigprocmask(SIG_BLOCK, &sig_mask, NULL)) != 0) {
        rv = errno;
    }
#else
    if ((rv = pthread_sigmask(SIG_BLOCK, &sig_mask, NULL)) != 0) {
#ifdef HAVE_ZOS_PTHREADS
        rv = errno;
#endif
    }
#endif
    return rv;
#else
    return CORE_ENOTIMPL;
#endif
}

status_t signal_unblock(int signum)
{
#if HAVE_SIGACTION
    sigset_t sig_mask;
    int rv;

    sigemptyset(&sig_mask);

    sigaddset(&sig_mask, signum);

#if defined(SIGPROCMASK_SETS_THREAD_MASK)
    if ((rv = sigprocmask(SIG_UNBLOCK, &sig_mask, NULL)) != 0) {
        rv = errno;
    }
#else
    if ((rv = pthread_sigmask(SIG_UNBLOCK, &sig_mask, NULL)) != 0) {
#ifdef HAVE_ZOS_PTHREADS
        rv = errno;
#endif
    }
#endif
    return rv;
#else
    return CORE_ENOTIMPL;
#endif
}
