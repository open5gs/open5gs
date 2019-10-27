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

subdir('ipfw')

pgw_conf = configuration_data()

pgw_headers = ('''
    net/if.h
    netinet/ip.h
    netinet/ip6.h
    netinet/ip_icmp.h
    netinet/icmp6.h
'''.split())

foreach h : pgw_headers
    if cc.has_header(h)
        define = 'HAVE_' + h.underscorify().to_upper()
        pgw_conf.set(define, 1)
    endif
endforeach

if have_func_kqueue
    pgw_conf.set('HAVE_KQUEUE', 1)
endif

configure_file(output : 'pgw-config.h', configuration : pgw_conf)

libpgw_sources = files('''
    ogs-tun.h
    ogs-tun.c 

    pgw-ipfw.h
    pgw-event.h
    pgw-context.h
    pgw-sm.h
    pgw-gtp-path.h
    pgw-s5c-build.h
    pgw-s5c-handler.h
    pgw-fd-path.h
    pgw-gx-handler.h

    pgw-ipfw.c
    pgw-init.c
    pgw-event.c
    pgw-context.c
    pgw-sm.c
    pgw-gtp-path.c
    pgw-s5c-build.c
    pgw-s5c-handler.c 
    pgw-fd-path.c
    pgw-gx-handler.c 
'''.split())

libpgw = static_library('pgw',
    sources : libpgw_sources,
    link_with : libipfw,
    dependencies : [libapp_dep, libdiameter_gx_dep, libgtp_dep],
    install : false)

libpgw_dep = declare_dependency(
    link_with : libpgw,
    dependencies : [libapp_dep, libdiameter_gx_dep, libgtp_dep])

pgw_sources = files('''
    app-init.c
    ../main.c
'''.split())

executable('open5gs-pgwd',
    sources : pgw_sources,
    c_args : '-DDEFAULT_CONFIG_FILENAME="@0@/pgw.yaml"'.format(open5gs_sysconfdir),
    include_directories : srcinc,
    dependencies : libpgw_dep,
    install_rpath : libdir,
    install : true)
