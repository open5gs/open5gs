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

libgtp_sources = files('''
    ogs-gtp.h

    message.h
    types.h
    conv.h
    node.h
    build.h
    path.h
    xact.h
    util.h

    message.c
    types.c
    conv.c
    node.c
    build.c
    path.c
    xact.c
    util.c
'''.split())

libgtp_inc = include_directories('.')

libgtp = library('ogsgtp',
    sources : libgtp_sources,
    version : libogslib_version,
    c_args : '-DOGS_GTP_COMPILATION',
    include_directories : [libgtp_inc, libinc],
    dependencies : [libcore_dep, libapp_dep],
    install : true)

libgtp_dep = declare_dependency(
    link_with : libgtp,
    include_directories : [libgtp_inc, libinc],
    dependencies : [libcore_dep, libapp_dep])
