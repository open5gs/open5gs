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

libsgwc_sources = files('''
    event.h
    timer.h
    context.h 
    gtp-path.h
    s11-build.h
    s11-handler.h
    s5c-handler.h 
    sxa-build.h
    sxa-handler.h
    pfcp-path.h
    sgwc-sm.h

    init.c
    event.c
    timer.c
    context.c 
    gtp-path.c
    s11-build.c
    s11-handler.c
    s5c-handler.c 
    sxa-build.c
    sxa-handler.c
    pfcp-path.c
    pfcp-sm.c
    sgwc-sm.c
'''.split())

libsgwc = static_library('sgwc',
    sources : libsgwc_sources,
    dependencies : [libapp_dep,
                    libgtp_dep,
                    libpfcp_dep],
    install : false)

libsgwc_dep = declare_dependency(
    link_with : libsgwc,
    dependencies : [libapp_dep,
                    libgtp_dep,
                    libpfcp_dep])

sgwc_sources = files('''
    app.c
    ../main.c
'''.split())

executable('open5gs-sgwcd',
    sources : sgwc_sources,
    c_args : '-DDEFAULT_CONFIG_FILENAME="@0@/sgwc.yaml"'.format(open5gs_sysconfdir),
    include_directories : srcinc,
    dependencies : libsgwc_dep,
    install_rpath : libdir,
    install : true)
