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

project('freediameter', 'c',
    version : '1.2.1',
    license : 'BSD-3-clause',
    meson_version : '>= 0.43.0',
    default_options : [
        'warning_level=1',
        'c_std=gnu89',
        'prefix=/usr',
    ],
)

freeDiameter_version = meson.project_version()
freeDiameter_soversion = '6'

prefix = get_option('prefix')
bindir = join_paths(prefix, get_option('bindir'))
libdir = join_paths(prefix, get_option('libdir'))
sysconfdir = join_paths(prefix, get_option('sysconfdir'))
localstatedir = join_paths(prefix, get_option('localstatedir'))

cc = meson.get_compiler('c')
host_system = host_machine.system()

flex = find_program('flex')
bison = find_program('bison')

# Compiler flags
if cc.get_id() == 'gcc' or cc.get_id() == 'clang'
    possible_cc_flags = [
        '-Werror',

        '-Wno-unused-variable',
        '-Wno-unused-but-set-variable',
        '-Wno-maybe-uninitialized',
        '-Wno-format-truncation',
        '-Wno-stringop-truncation',
    ]
else
    possible_cc_flags = []
endif

add_project_arguments(
    cc.get_supported_arguments(possible_cc_flags),
    language : 'c')

freeDiameter_includes = include_directories('''
    include
    .
'''.split())

subdir('include/freeDiameter')
subdir('libfdproto')
subdir('libfdcore')
subdir('extensions')
