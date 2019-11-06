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

fdcore_sources = files('''
    fdcore-internal.h 
    apps.c
    cnxctx.h
    config.c
    core.c
    cnxctx.c
    endpoints.c
    events.c 
    extensions.c
    fifo_stats.c
    hooks.c
    dict_base_proto.c
    messages.c 
    queues.c
    peers.c
    p_ce.c
    p_cnx.c
    p_dw.c
    p_dp.c
    p_expiry.c 
    p_out.c
    p_psm.c
    p_sr.c
    routing_dispatch.c
    server.c
    tcp.c 
    version.c 
'''.split())

if libsctp.found()
fdcore_sources += files('sctp.c', 'sctp3436.c')
endif

lgen = generator(flex,
    output : '@PLAINNAME@.yy.c',
    arguments : ['-p', '-Cem', '-Pfdd', '-o', '@OUTPUT@', '@INPUT@'])

lfiles = lgen.process('fdd.l')

pgen = generator(bison,
    output : ['@BASENAME@.tab.c', '@BASENAME@.tab.h'],
    arguments : ['-p', 'fdd', '-d',
                 '@INPUT@', '--defines=@OUTPUT1@', '--output=@OUTPUT0@'])

pfiles = pgen.process('fdd.y')

libfdcore = library('fdcore',
    sources : [lfiles, pfiles, fdcore_sources],
    version : freeDiameter_version,
    soversion : freeDiameter_soversion,
    include_directories : freeDiameter_includes,
    dependencies : [libfdproto_dep,
                    libdl, libsctp, libgcrypt, libgnutls],
    install : true)

libfdcore_dep = declare_dependency(
    link_with : libfdcore,
    include_directories : freeDiameter_includes,
    dependencies : [libfdproto_dep,
                    libdl, libsctp, libgcrypt, libgnutls])
