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

libsgw_sources = files('''
    sgw-event.h
    sgw-context.h 
    sgw-gtp-path.h
    sgw-sm.h
    sgw-s11-handler.h
    sgw-s5c-handler.h 

    sgw-init.c
    sgw-event.c
    sgw-context.c 
    sgw-gtp-path.c
    sgw-sm.c
    sgw-s11-handler.c
    sgw-s5c-handler.c 
'''.split())

libsgw = static_library('sgw',
    sources : libsgw_sources,
    dependencies : [libapp_dep, libgtp_dep],
    install : false)

libsgw_dep = declare_dependency(
    link_with : libsgw,
    dependencies : [libapp_dep, libgtp_dep])

sgw_sources = files('''
    app-init.c
    ../main.c
'''.split())

executable('open5gs-sgwd',
    sources : sgw_sources,
    c_args : '-DDEFAULT_CONFIG_FILENAME="@0@/sgw.yaml"'.format(open5gs_sysconfdir),
    include_directories : srcinc,
    dependencies : [versiondep, libsgw_dep],
    install_rpath : libdir,
    install : true)
