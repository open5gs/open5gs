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

    context.h
    path.h
    util.h
    xact.h
    v1/build.h
    v1/conv.h
    v1/message.h
    v1/path.h
    v1/types.h
    v2/build.h
    v2/conv.h
    v2/message.h
    v2/path.h
    v2/types.h

    context.c
    path.c
    util.c
    xact.c
    v1/build.c
    v1/conv.c
    v1/message.c
    v1/path.c
    v1/types.c
    v2/build.c
    v2/conv.c
    v2/message.c
    v2/path.c
    v2/types.c
'''.split())

libgtp_inc = include_directories('.')

libgtp = library('ogsgtp',
    sources : libgtp_sources,
    version : libogslib_version,
    c_args : '-DOGS_GTP_COMPILATION',
    include_directories : [libgtp_inc, libinc],
    dependencies : [libcore_dep, libapp_dep, libnas_common_dep],
    install : true)

libgtp_dep = declare_dependency(
    link_with : libgtp,
    include_directories : [libgtp_inc, libinc],
    dependencies : [libcore_dep, libapp_dep, libnas_common_dep])
