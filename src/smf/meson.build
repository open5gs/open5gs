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
    s5c-build.h
    s5c-handler.h
    fd-path.h
    gx-handler.h
    pfcp-path.h
    n4-build.h
    n4-handler.h
    bearer-binding.h

    init.c
    event.c
    timer.c
    context.c
    smf-sm.c
    pfcp-sm.c
    gtp-path.c
    s5c-build.c
    s5c-handler.c 
    fd-path.c
    gx-handler.c 
    pfcp-path.c
    n4-build.c
    n4-handler.c
    bearer-binding.c
'''.split())

libsmf = static_library('smf',
    sources : libsmf_sources,
    link_with : libipfw,
    dependencies : [
        libapp_dep, libdiameter_gx_dep, libgtp_dep, libpfcp_dep, libipfw_dep
    ],
    install : false)

libsmf_dep = declare_dependency(
    link_with : libsmf,
    dependencies : [
        libapp_dep, libdiameter_gx_dep, libgtp_dep, libpfcp_dep, libipfw_dep
    ])

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
