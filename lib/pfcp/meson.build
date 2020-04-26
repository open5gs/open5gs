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

libpfcp_sources = files('''
    ogs-pfcp.h

    message.h
    types.h
    conv.h
    n4-build.h
    path.h
    xact.h
    context.h

    message.c
    types.c
    conv.c
    n4-build.c
    path.c
    xact.c
    context.c
'''.split())

libpfcp_inc = include_directories('.')

libpfcp = library('ogspfcp',
    sources : libpfcp_sources,
    version : libogslib_version,
    c_args : '-DOGS_PFCP_COMPILATION',
    include_directories : [libpfcp_inc, libinc],
    dependencies : [libcore_dep, libapp_dep],
    install : true)

libpfcp_dep = declare_dependency(
    link_with : libpfcp,
    include_directories : [libpfcp_inc, libinc],
    dependencies : [libcore_dep, libapp_dep])
