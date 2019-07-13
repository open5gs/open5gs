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

ACLOCAL_AMFLAGS = -I m4
EXTRA_DIST = build/git-version-gen .version README.md LICENSE

SUBDIRS = lib src support tests

bin_PROGRAMS = nextepc-mmed nextepc-hssd nextepc-sgwd nextepc-pgwd nextepc-pcrfd nextepc-epcd

dist_nextepc_mmed_SOURCES = main.c
nextepc_mmed_LDADD = $(top_srcdir)/src/libmme.la

dist_nextepc_hssd_SOURCES = main.c
nextepc_hssd_LDADD = $(top_srcdir)/src/libhss.la

dist_nextepc_sgwd_SOURCES = main.c
nextepc_sgwd_LDADD = $(top_srcdir)/src/libsgw.la

dist_nextepc_pgwd_SOURCES = main.c
nextepc_pgwd_LDADD = $(top_srcdir)/src/libpgw.la

dist_nextepc_pcrfd_SOURCES = main.c
nextepc_pcrfd_LDADD = $(top_srcdir)/src/libpcrf.la

dist_nextepc_epcd_SOURCES = main.c
nextepc_epcd_LDADD = $(top_srcdir)/src/libepc.la

AM_CPPFLAGS = \
	@OGSCORE_CFLAGS@ \
    -I$(top_srcdir)/lib \
    -I$(top_srcdir)/src \
	$(NULL)

install-data-hook:
	$(MKDIR_P) $(DESTDIR)/$(localstatedir)/log/nextepc
	$(MKDIR_P) $(DESTDIR)/$(localstatedir)/run/nextepc-mmed
	$(MKDIR_P) $(DESTDIR)/$(localstatedir)/run/nextepc-sgwd
	$(MKDIR_P) $(DESTDIR)/$(localstatedir)/run/nextepc-pgwd
	$(MKDIR_P) $(DESTDIR)/$(localstatedir)/run/nextepc-pcrfd
	$(MKDIR_P) $(DESTDIR)/$(localstatedir)/run/nextepc-hssd
	$(MKDIR_P) $(DESTDIR)/$(localstatedir)/run/nextepc-epcd

BUILT_SOURCES = $(top_srcdir)/.version
$(top_srcdir)/.version:
	echo $(VERSION) > $@-t && mv $@-t $@
dist-hook:
	echo $(VERSION) > $(distdir)/.tarball-version
