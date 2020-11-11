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

libsctp_conf = configuration_data()

libsctp_headers = ('''
    netinet/sctp.h
'''.split())

foreach h : libsctp_headers
    if cc.has_header(h)
        define = 'HAVE_' + h.underscorify().to_upper()
        libsctp_conf.set(define, 1)
    endif
endforeach

libsctp_sources = files('''
    ogs-sctp.h

    ogs-sctp.c
'''.split())

sctp_dep = cc.find_library('sctp', required : false)
if sctp_dep.found()
    libsctp_sources += files('ogs-lksctp.c')
else
    sctp_dep = dependency('usrsctp',
        version: ['>=1.0.0', '<2'],
        fallback: ['usrsctp', 'usrsctp_dep'],
        default_options: [
            'sctp_build_programs=false',
#            'sctp_debug=@0@'.format(true),
        ])

    libsctp_sources += files('ogs-usrsctp.c')
    libsctp_conf.set('HAVE_USRSCTP', 1)
endif

configure_file(output : 'sctp-config.h', configuration : libsctp_conf)

libsctp_inc = include_directories('.')

libsctp = library('ogssctp',
    sources : libsctp_sources,
    version : libogslib_version,
    c_args : '-DOGS_SCTP_COMPILATION',
    include_directories : [libsctp_inc, libinc],
    dependencies : [libcore_dep, libapp_dep, sctp_dep],
    install : true)

libsctp_dep = declare_dependency(
    link_with : libsctp,
    include_directories : [libsctp_inc, libinc],
    dependencies : [libcore_dep, libapp_dep, sctp_dep])
