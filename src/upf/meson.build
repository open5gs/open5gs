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
#// Modified by Fatemeh Shafiei Ardestani on 2025-04-06

mhd_dep = dependency('libmicrohttpd', required: true)  # Added line
jn_dep = dependency('jansson', required: true)         # Added line
curl_dep = dependency('libcurl', required: true)       # Added line
zmq_lib = dependency('libzmq', required: true)        # Added line
#if not zmq_lib.found()
#    zmq_lib = cc.find_library('zmq', required: true)
#endif
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
    jansson.h
    microhttpd.h
    cvector.h
    cvector_utils.h
    stb_ds.h
    ee_event.h
    zmq.h

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
    btree.h
    shared_variables.h
    types.h
    decoder.h
    upf_prepare_data.h
    encoder.h
    EE-init.h
    ee_client.h
    ee_server.h
    handler.h
    response_builder.h
    send_data.h
    manage_collection.h
    utils.h


    rule-match.c
    ee_client.c
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
    utils.c
    btree.c
    types.c
    upf_prepare_data.c
    decoder.c
    encoder.c
    EE-init.c
    ee_server.c
    handler.c
    response_builder.c
    send_data.c

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
        mhd_dep,  # Added line
        jn_dep,   # Added line
        curl_dep,
        zmq_lib,# Added line
    ],
    install : false)

libupf_dep = declare_dependency(
    link_with : libupf,
    dependencies : [
        libmetrics_dep,
        libpfcp_dep,
        libtun_dep,
        libarp_nd_dep,
        mhd_dep,  # Added line
        jn_dep,   # Added line
        curl_dep, # Added line
        zmq_lib,
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
