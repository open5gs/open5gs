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

upf_conf = configuration_data()

upf_headers = ('''
    ifaddrs.h
    net/ethernet.h
    net/if.h
    net/if_dl.h
    netinet/in.h
    netinet/ip.h
    netinet/ip6.h
    netinet/ip_icmp.h
    netinet/icmp6.h
    sys/ioctl.h
    sys/socket.h
'''.split())

foreach h : upf_headers
    if cc.has_header(h)
        define = 'HAVE_' + h.underscorify().to_upper()
        upf_conf.set(define, 1)
    endif
endforeach

if have_func_kqueue
    upf_conf.set('HAVE_KQUEUE', 1)
endif

configure_file(output : 'upf-config.h', configuration : upf_conf)

libupf_sources = files('''
    rule-match.h
    event.h
    timer.h
    metrics.h
    context.h
    upf-sm.h
    gtp-path.h
    pfcp-path.h
    n4-build.h
    n4-handler.h

    rule-match.c
    init.c
    metrics.c
    event.c
    timer.c
    context.c
    upf-sm.c
    pfcp-sm.c
    gtp-path.c
    pfcp-path.c
    n4-build.c
    n4-handler.c
'''.split())

libtins_dep = dependency('libtins',
    version: ['>=3.4'],
    fallback: ['libtins', 'libtins_dep'])

libarp_nd = static_library('arp_nd',
    sources : files('''
      arp-nd.cpp
      arp-nd.h
    '''.split()),
    dependencies : libtins_dep,
    install : false)
libarp_nd_dep = declare_dependency(
    link_with : libarp_nd,
    dependencies : libtins_dep)

libupf = static_library('upf',
    sources : libupf_sources,
    dependencies : [
        libmetrics_dep,
        libpfcp_dep,
        libtun_dep,
        libarp_nd_dep,
    ],
    install : false)

libupf_dep = declare_dependency(
    link_with : libupf,
    dependencies : [
        libmetrics_dep,
        libpfcp_dep,
        libtun_dep,
        libarp_nd_dep,
    ])

upf_sources = files('''
    app.c
    ../main.c
'''.split())

executable('open5gs-upfd',
    sources : upf_sources,
    c_args : '-DDEFAULT_CONFIG_FILENAME="@0@/upf.yaml"'.format(open5gs_sysconfdir),
    include_directories : srcinc,
    dependencies : libupf_dep,
    install_rpath : libdir,
    install : true)
