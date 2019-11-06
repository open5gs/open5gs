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
#

freeDiameter_version = '1.2.1'
freeDiameter_host_conf = configuration_data()
freeDiameter_version_conf = configuration_data()

freeDiameter_sysconfdir = join_paths(sysconfdir, 'freeDiameter')
freeDiameter_extensionsdir = join_paths(libdir, 'freeDiameter')

# Informations to display in daemon's help
freeDiameter_host_conf.set_quoted('FD_PROJECT_BINARY', meson.project_name() + 'd')
freeDiameter_host_conf.set_quoted('FD_PROJECT_NAME', meson.project_name())
freeDiameter_host_conf.set_quoted('FD_PROJECT_VERSION', freeDiameter_version)
freeDiameter_host_conf.set_quoted('FD_PROJECT_COPYRIGHT', 'Copyright (c) 2008-2015, WIDE Project (www.wide.ad.jp) and NICT (www.nict.go.jp)')

# Version of the source code
version_arr = freeDiameter_version.split('.')
freeDiameter_host_conf.set('FD_PROJECT_VERSION_MAJOR', version_arr[0].to_int())
freeDiameter_host_conf.set('FD_PROJECT_VERSION_MINOR', version_arr[1].to_int())
freeDiameter_host_conf.set('FD_PROJECT_VERSION_REV',  version_arr[2].to_int())

# Version of the API with the library
freeDiameter_host_conf.set('FD_PROJECT_VERSION_API', 6)
freeDiameter_host_conf.set_quoted('DEFAULT_CONF_PATH',
        freeDiameter_sysconfdir)
freeDiameter_host_conf.set_quoted('DEFAULT_EXTENSIONS_PATH',
        freeDiameter_extensionsdir)
freeDiameter_host_conf.set_quoted('FD_DEFAULT_CONF_FILENAME',
        'freeDiameter.conf')

if host_machine.endian() == 'big'
  freeDiameter_host_conf.set('HOST_BIG_ENDIAN', 1)
endif

freeDiameter_host_conf.set('FD_IS_CONFIG', true)
freeDiameter_host_conf.set('_GNU_SOURCE', true)
freeDiameter_host_conf.set('FD_HOOK_HANDLE_LIMIT', 5)

freeDiameter_headers = ('''
    malloc.h
    signalent.h
'''.split())

foreach h : freeDiameter_headers
    if cc.has_header(h)
        define = 'HAVE_' + h.underscorify().to_upper()
        freeDiameter_host_conf.set(define, 1)
    endif
endforeach

freeDiameter_functions = ('''
    strndup
    getopt_long
    getifaddrs
'''.split())

foreach f : freeDiameter_functions
    if cc.has_function(f)
        define = 'HAVE_' + f.underscorify().to_upper()
        freeDiameter_host_conf.set(define, 1)
        set_variable('have_func_' + f, true)
    else
        set_variable('have_func_' + f, false)
    endif
endforeach

# Check for ntohll
if cc.has_header_symbol('netinet/in.h', 'ntohll')
    freeDiameter_host_conf.set('HAVE_NTOHLL', 1)
endif

# Check for AI_ADDRCONFIG
if cc.has_header_symbol('netdb.h', 'AI_ADDRCONFIG')
    freeDiameter_host_conf.set('HAVE_AI_ADDRCONFIG', 1)
endif

# Determination of thread implementation
threads = []
if host_system == 'windows' and not get_option('force_posix_threads')
else
    threads = dependency('threads')
    if cc.has_header_symbol('pthread.h', 'pthread_barrier_wait')
        freeDiameter_host_conf.set('HAVE_PTHREAD_BAR', 1)
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
    freeDiameter_host_conf.set('HAVE_CLOCK_GETTIME', 1)
elif cc.links(clock_gettime_test_code, args : '-lrt', name : 'clock_gettime() in librt')
    freeDiameter_host_conf.set('HAVE_CLOCK_GETTIME', 1)
    librt = cc.find_library('rt')
elif cc.links(clock_gettime_test_code, args : '-lposix4', name : 'clock_gettime() in libposix4')
    freeDiameter_host_conf.set('HAVE_CLOCK_GETTIME', 1)
    librt = cc.find_library('posix4')
else
    error('Could not find clock_gettime()')
endif

# Check for dlopen()
dlopen_dlsym_test_code = '''
    #include <dlfcn.h>
    int underscore_test (void) { return 42; }
    int main (int argc, char ** argv) {
        void *f1 = (void*)0, *f2 = (void*)0, *handle;
        handle = dlopen ((void*)0, 0);
        if (handle) {
            f1 = dlsym (handle, "underscore_test");
            f2 = dlsym (handle, "_underscore_test");
        }
        return (!f2 || f1);
    }'''
libdl = []
if cc.links(dlopen_dlsym_test_code, name : 'dlopen() and dlsym() in system libraries')
    freeDiameter_host_conf.set('HAVE_DLOPEN', 1)
elif cc.links(dlopen_dlsym_test_code, args : '-ldl', name : 'dlopen() and dlsym() in libdl')
    freeDiameter_host_conf.set('HAVE_DLOPEN', 1)
    libdl = cc.find_library('dl')
else
    error('Could not find dlopen()')
endif

# Check for gcrypt/idn library
libgcrypt = cc.find_library('gcrypt')
libidn = cc.find_library('idn')

# Check for gnutls library
libgnutls = cc.find_library('gnutls')
if cc.has_function('gnutls_hash', args : '-lgnutls')
    freeDiameter_host_conf.set('GNUTLS_VERSION_210', 1)
endif
if cc.has_function('gnutls_transport_set_vec_push_function', args : '-lgnutls')
    freeDiameter_host_conf.set('GNUTLS_VERSION_212', 1)
endif
if cc.has_function('gnutls_x509_trust_list_verify_crt', args : '-lgnutls')
    freeDiameter_host_conf.set('GNUTLS_VERSION_300', 1)
endif
if cc.has_function('gnutls_handshake_set_timeout', args : '-lgnutls')
    freeDiameter_host_conf.set('GNUTLS_VERSION_310', 1)
endif

# Check for SCTP
libsctp = cc.find_library('sctp', required : false)
if libsctp.found()
    if cc.compiles('''#include <unistd.h>
                      #include <netinet/sctp.h>
                      int main (int argc, char ** argv) {
                          sctp_connectx(0, NULL, 0, NULL);
                      }''', name : 'sctp_connectx() accepts 4 arguments')
        freeDiameter_host_conf.set('SCTP_CONNECTX_4_ARGS', 1)
    endif
    if host_system == 'freebsd'
        freeDiameter_host_conf.set('SCTP_USE_MAPPED_ADDRESSES', 1)
    endif
else
    freeDiameter_host_conf.set('DISABLE_SCTP', 1)
endif

configure_file(output : 'freeDiameter-host.h',
               configuration : freeDiameter_host_conf)

configure_file(output : 'version.h',
               configuration : freeDiameter_version_conf)
