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

smf_conf = configuration_data()

smf_headers = ('''
    net/if.h
    netinet/ip.h
    netinet/ip6.h
    netinet/ip_icmp.h
    netinet/icmp6.h
'''.split())

foreach h : smf_headers
    if cc.has_header(h)
        define = 'HAVE_' + h.underscorify().to_upper()
        smf_conf.set(define, 1)
    endif
endforeach

configure_file(output : 'smf-config.h', configuration : smf_conf)

libsmf_sources = files('''
    event.h
    timer.h
    context.h
    smf-sm.h
    gtp-path.h
    gn-build.h
    gn-handler.h
    s5c-build.h
    s5c-handler.h
    fd-path.h
    gx-handler.h
    gy-handler.h
    pfcp-path.h
    n4-build.h
    n4-handler.h
    binding.h
    nnrf-build.h
    nnrf-handler.h
    nudm-build.h
    nudm-handler.h
    nsmf-handler.h
    namf-build.h
    namf-handler.h
    npcf-build.h
    npcf-handler.h
    sbi-path.h
    gsm-build.h
    gsm-handler.h
    nas-path.h
    ngap-build.h
    ngap-handler.h
    ngap-path.h

    init.c
    event.c
    timer.c
    context.c
    smf-sm.c
    nf-sm.c
    gsm-sm.c
    pfcp-sm.c
    gtp-path.c
    gn-build.c
    gn-handler.c
    s5c-build.c
    s5c-handler.c
    fd-path.c
    gx-path.c
    gy-path.c
    s6b-path.c
    gx-handler.c
    gy-handler.c
    pfcp-path.c
    n4-build.c
    n4-handler.c
    binding.c
    nnrf-build.c
    nnrf-handler.c
    nudm-build.c
    nudm-handler.c
    nsmf-handler.c
    namf-build.c
    namf-handler.c
    npcf-build.c
    npcf-handler.c
    sbi-path.c
    gsm-build.c
    gsm-handler.c
    nas-path.c
    ngap-build.c
    ngap-handler.c
    ngap-path.c
'''.split())

libsmf = static_library('smf',
    sources : libsmf_sources,
    dependencies : [libapp_dep,
                    libsbi_dep,
                    libngap_dep,
                    libnas_5gs_dep,
                    libdiameter_gx_dep,
                    libdiameter_gy_dep,
                    libdiameter_s6b_dep,
                    libgtp_dep,
                    libpfcp_dep],
    install : false)

libsmf_dep = declare_dependency(
    link_with : libsmf,
    dependencies : [libapp_dep,
                    libsbi_dep,
                    libngap_dep,
                    libnas_5gs_dep,
                    libdiameter_gx_dep,
                    libdiameter_gy_dep,
                    libdiameter_s6b_dep,
                    libgtp_dep,
                    libpfcp_dep])

smf_sources = files('''
    app.c
    ../main.c
'''.split())

executable('open5gs-smfd',
    sources : smf_sources,
    c_args : '-DDEFAULT_CONFIG_FILENAME="@0@/smf.yaml"'.format(open5gs_sysconfdir),
    include_directories : srcinc,
    dependencies : libsmf_dep,
    install_rpath : libdir,
    install : true)
