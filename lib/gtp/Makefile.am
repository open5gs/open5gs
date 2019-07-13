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

pkglib_LTLIBRARIES = libgtp.la

libgtp_la_SOURCES = \
	gtp-tlv.h gtp-message.h gtp-types.h gtp-conv.h \
	gtp-node.h gtp-path.h gtp-xact.h \
	gtp-tlv.c gtp-message.c gtp-types.c gtp-conv.c \
	gtp-node.c gtp-path.c gtp-xact.c \
	$(NULL)

libgtp_la_DEPENDENCIES = \
    $(top_srcdir)/lib/base/libbase.la \
	$(NULL)

libgtp_la_LIBADD = \
	@OGSCORE_LIBS@ \
    $(top_srcdir)/lib/base/libbase.la \
	$(NULL)

AM_CPPFLAGS = \
	@OGSCORE_CFLAGS@ \
    -I$(top_srcdir)/lib \
	$(NULL)

AM_CFLAGS = \
	-Wall -Werror \
	$(NULL)
