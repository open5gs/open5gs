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

libipfw_sources = files('''
    objs/include_e/timeconv.h
    objs/include_e/net/if_var.h
    objs/include_e/netinet/ip_fw.h
    objs/include_e/netinet/ip_dummynet.h
    objs/include_e/sys/sockio.h
    objs/include_e/libutil.h
    objs/include_e/timeconv.h
    objs/include_e/net/if_var.h
    objs/include_e/netinet/ip_fw.h
    objs/include_e/netinet/ip_dummynet.h
    objs/include_e/sys/sockio.h
    objs/include_e/libutil.h

    ipfw2.h
    glue.h
    missing.h

    ipfw2.c
    dummynet.c
    ipv6.c
    tables.c
    expand_number.c
    humanize_number.c
    glue.c

    ogs-ipfw.h
    ogs-ipfw.c
'''.split())

libipfw_inc = include_directories('objs/include_e')

ipfw_cc_flags = ['-D_DEFAULT_SOURCE', '-D_BSD_SOURCE', '-DUSERSPACE',
                 '-D__BSD_VISIBLE', '-DNEED_STRTONUM', '-DNEED_ROUNDUP2' ]

if host_system == 'linux'
    ipfw_cc_flags += ['-DNEED_SYSCTLBYNAME', '-DNEED_SIN_LEN']
elif host_system == 'freebsd'
    ipfw_cc_flags += ['-DNEED_SYSCTLBYNAME']
endif

if cc.get_id() == 'gcc' or cc.get_id() == 'clang'
    ipfw_cc_flags += cc.get_supported_arguments([
                    '-Wno-shift-negative-value',
                    '-Wno-unused-but-set-variable',
                    '-Wno-unknown-warning-option',

                    '-Wno-missing-prototypes',
                    '-Wno-missing-declarations',
                    '-Wno-discarded-qualifiers',
                    '-Wno-redundant-decls',
                    '-Wno-shift-overflow',
                    '-Wno-float-equal',
                    '-Wno-implicit-fallthrough',
                    '-Wno-incompatible-pointer-types-discards-qualifiers',
                    '-Wno-format-nonliteral',
                    '-Wno-cpp',
    ])
endif

libipfw = library('ogsipfw',
    sources : libipfw_sources,
    version : libogslib_version,
    c_args : ['-include', 'glue.h', ipfw_cc_flags],
    include_directories : libipfw_inc,
    dependencies : libcore_dep,
    install : true)

libipfw_dep = declare_dependency(
    link_with : libipfw,
    include_directories : libinc,
    dependencies : libcore_dep)
