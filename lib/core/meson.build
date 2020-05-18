# Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>

# This file is part of Open5GS.

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

libcore_conf = configuration_data()

libcore_headers = ('''
    arpa/inet.h
    ctype.h
    errno.h
    execinfo.h
    fcntl.h
    ifaddrs.h
    netdb.h
    pthread.h
    signal.h
    stdarg.h
    stddef.h
    stdio.h
    stdint.h
    stdbool.h
    stdlib.h
    string.h
    strings.h
    time.h
    sys/time.h
    unistd.h
    net/if.h
    netinet/in.h
    netinet/in_systm.h
    netinet/udp.h
    netinet/tcp.h
    sys/ioctl.h
    sys/param.h
    sys/random.h
    sys/socket.h
    sys/stat.h
    limits.h
    sys/syslimits.h
    sys/types.h
    sys/wait.h
    sys/uio.h
'''.split())

foreach h : libcore_headers
    if cc.has_header(h)
        define = 'HAVE_' + h.underscorify().to_upper()
        libcore_conf.set(define, 1)
    endif
endforeach

libcore_functions = ('''
    arc4random
    arc4random_buf
    getrandom
    localtime_r
    getifaddrs
    getenv
    putenv
    setenv
    unsetenv
    strerror_r
    sigaction
    sigwait
    sigsuspend
    eventfd
    kqueue
    epoll_ctl
'''.split())

foreach f : libcore_functions
    if cc.has_function(f)
        define = 'HAVE_' + f.underscorify().to_upper()
        libcore_conf.set(define, 1)
        set_variable('have_func_' + f, true)
    else
        set_variable('have_func_' + f, false)
    endif
endforeach

# Determination of thread implementation
threads = []
if host_system == 'windows' and not get_option('force_posix_threads')
else
    threads = dependency('threads')
    if cc.has_header_symbol('pthread.h', 'pthread_barrier_wait')
        libcore_conf.set('HAVE_PTHREAD_BAR', 1)
    endif
endif

# Check for sys_syslist
if cc.has_header_symbol('signal.h', 'sys_siglist')
    libcore_conf.set('HAVE_DECL_SYS_SIGLIST', 1,
            description: 'Define to 1 if you have the declaration of \`sys_siglist\', and to 0 if you don\'t.')
endif

# Check whether strerror_r returns char *
if have_func_strerror_r
    if cc.compiles('''#define _GNU_SOURCE
                      #include <string.h>
                      int func (void) {
                          char error_string[256];
                          char *ptr = strerror_r (-2, error_string, 256);
                          char c = *strerror_r (-2, error_string, 256);
                          return c != 0 && ptr != (void*) 0L;
                      }''', name : 'strerror_r() returns char *')
        libcore_conf.set('STRERROR_R_CHAR_P', 1,
                description: 'Defined if strerror_r returns char *')
    endif
endif

# Check for backtrace()
libexecinfo = cc.find_library('execinfo', required : false)
if libcore_conf.has('HAVE_EXECINFO_H')
    if cc.has_function('backtrace', prefix : '#include <execinfo.h>')
        libcore_conf.set('HAVE_BACKTRACE', 1)
    elif libexecinfo.found() and cc.has_function('backtrace',
                                            prefix : '#include <execinfo.h>',
                                            dependencies : libexecinfo)
        libcore_conf.set('HAVE_BACKTRACE', 1)
    endif
endif    

# Check for clock_gettime()
clock_gettime_test_code = '''
    #include <time.h>
    struct timespec t;
    int main (int argc, char ** argv) {
        return clock_gettime(CLOCK_REALTIME, &t);
    }'''
librt = []
if cc.links(clock_gettime_test_code, name : 'clock_gettime()')
    libcore_conf.set('HAVE_CLOCK_GETTIME', 1)
elif cc.links(clock_gettime_test_code, args : '-lrt', name : 'clock_gettime() in librt')
    libcore_conf.set('HAVE_CLOCK_GETTIME', 1)
    librt = cc.find_library('rt')
elif cc.links(clock_gettime_test_code, args : '-lposix4', name : 'clock_gettime() in libposix4')
    libcore_conf.set('HAVE_CLOCK_GETTIME', 1)
    librt = cc.find_library('posix4')
else
    error('Could not find clock_gettime()')
endif

# Check for /dev/urandom
if run_command('[', '-c', '/dev/urandom', ']').returncode() == 0
    libcore_conf.set_quoted('OGS_DEV_RANDOM', '/dev/urandom',
                    description: 'a suitable file to read random data from')
endif

# Check for eventfd(2)
if cc.links('''#include <sys/eventfd.h>
               #include <unistd.h>
               int main (int argc, char ** argv) {
                   eventfd (0, EFD_CLOEXEC);
                   return 0;
               }''', name : 'eventfd(2) system call')
    libcore_conf.set('HAVE_EVENTFD', 1)
endif

# Check for epoll
if have_func_epoll_ctl
    libcore_conf.set('HAVE_EPOLL', 1, description: 'Defined if your system supports the epoll system calls')
endif

# Check for socket
libsocket = cc.find_library('socket', required : false)
if host_system != 'windows'
    # socket()
    socket_test = '''#include <sys/types.h>
                     #include <sys/socket.h>
                     int main (int argc, char ** argv) {
                         return socket(1, 2, 3);
                     }'''
    if cc.links(socket_test, name : 'socket()')
    elif libsocket.found() and cc.links(socket_test,
                                        dependencies : libsocket,
                                        name : 'socket() in -lsocket')
    else
        error('Could not find socket()')
    endif
endif

configure_file(output : 'core-config-private.h', configuration : libcore_conf)

ogs_libcore_conf = configuration_data()

ogs_libcore_conf.set('_GNU_SOURCE', true)
ogs_libcore_conf.set('OGS_BIG_ENDIAN', 4321)
ogs_libcore_conf.set('OGS_LITTLE_ENDIAN', 1234)

if host_machine.endian() == 'big'
    ogs_libcore_conf.set('OGS_BYTE_ORDER', 'OGS_BIG_ENDIAN')
else
    ogs_libcore_conf.set('OGS_BYTE_ORDER', 'OGS_LITTLE_ENDIAN')
endif

if host_system == 'windows'
ogs_libcore_conf.set('OGS_DIR_SEPARATOR', '\'\\\\\'')
ogs_libcore_conf.set_quoted('OGS_DIR_SEPARATOR_S', '\\\\')
else
ogs_libcore_conf.set('OGS_DIR_SEPARATOR', '\'/\'')
ogs_libcore_conf.set_quoted('OGS_DIR_SEPARATOR_S', '/')
endif

configure_file(output : 'core-config.h', configuration : ogs_libcore_conf)

libcore_sources = files('''
    ogs-core.h

    ogs-compat.h
    ogs-macros.h
    ogs-pool.h
    ogs-list.h
    ogs-abort.h
    ogs-errno.h
    ogs-strings.h
    ogs-time.h
    ogs-conv.h
    ogs-log.h
    ogs-pkbuf.h
    ogs-memory.h
    ogs-rbtree.h
    ogs-timer.h
    ogs-rand.h
    ogs-uuid.h
    ogs-thread.h
    ogs-signal.h
    ogs-process.h
    ogs-sockaddr.h
    ogs-socket.h
    ogs-sockpair.h
    ogs-socknode.h
    ogs-udp.h
    ogs-tcp.h
    ogs-tun.h
    ogs-queue.h
    ogs-poll.h
    ogs-notify.h
    ogs-tlv.h
    ogs-tlv-msg.h
    ogs-env.h
    ogs-fsm.h
    ogs-hash.h
    ogs-misc.h
    ogs-getopt.h
    ogs-3gpp-types.h
    abts.h

    ogs-abort.c
    ogs-errno.c
    ogs-strings.c
    ogs-time.c
    ogs-conv.c
    ogs-log.c
    ogs-pkbuf.c
    ogs-memory.c
    ogs-rbtree.c
    ogs-timer.c
    ogs-rand.c
    ogs-uuid.c
    ogs-thread.c
    ogs-signal.c
    ogs-process.c
    ogs-sockaddr.c
    ogs-socket.c
    ogs-sockpair.c
    ogs-socknode.c
    ogs-udp.c
    ogs-tcp.c
    ogs-tun.c
    ogs-queue.c
    ogs-select.c
    ogs-poll.c
    ogs-notify.c
    ogs-tlv.c
    ogs-tlv-msg.c
    ogs-env.c
    ogs-fsm.c
    ogs-hash.c
    ogs-misc.c
    ogs-getopt.c
    ogs-3gpp-types.c
    ogs-core.c
    abts.c
'''.split())

if have_func_epoll_ctl
    libcore_sources += files('ogs-epoll.c')
endif
if have_func_kqueue
    libcore_sources += files('ogs-kqueue.c')
endif

libcore_inc = include_directories('.')

libcore = library('ogscore',
    sources : libcore_sources,
    version : libogslib_version,
    c_args : '-DOGS_CORE_COMPILATION',
    include_directories : [libcore_inc, libinc],
    dependencies : [threads, librt, libexecinfo, libsocket],
    install : true)

libcore_dep = declare_dependency(
    link_with : libcore,
    include_directories : [libcore_inc, libinc],
    dependencies : [threads, librt, libexecinfo, libsocket])
