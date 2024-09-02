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

libdiameter_common_sources = files('''
    ogs-diameter-common.h

    message.h
    logger.h
    stats.h
    base.h

    libapp_sip.c
    dict.c
    message.c
    logger.c
    stats.c
    config.c
    util.c
    init.c
'''.split())

libdiameter_common_inc = include_directories('.')

# At this point, Open5GS always use the subproject of freeDiameter
freeDiameter_proj = subproject('freeDiameter')
libfdcore_dep = freeDiameter_proj.get_variable('libfdcore_dep')

libdiameter_common_cc_flags = ['-DOGS_DIAM_COMPILATION']
libdiameter_common_cc_flags += cc.get_supported_arguments([
    '-Wno-missing-prototypes',
    '-Wno-missing-declarations',
    '-Wno-discarded-qualifiers',
    '-Wno-incompatible-pointer-types',
    '-Wno-strict-prototypes',
])

libdiameter_common = library('ogsdiameter-common',
    sources : libdiameter_common_sources,
    version : libogslib_version,
    c_args : libdiameter_common_cc_flags,
    include_directories : [libdiameter_common_inc, libinc],
    dependencies : [libcore_dep, libfdcore_dep, libapp_dep],
    install : true)

libdiameter_common_dep = declare_dependency(
    link_with : libdiameter_common,
    include_directories : [libdiameter_common_inc, libinc],
    dependencies : [libcore_dep, libfdcore_dep, libapp_dep])
