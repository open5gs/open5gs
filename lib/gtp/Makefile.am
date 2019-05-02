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
	gtp_tlv.h gtp_message.h gtp_types.h gtp_conv.h \
	gtp_node.h gtp_path.h gtp_xact.h \
	gtp_tlv.c gtp_message.c gtp_types.c gtp_conv.c \
	gtp_node.c gtp_path.c gtp_xact.c \
	$(NULL)

libgtp_la_DEPENDENCIES = \
	@OGSCORE_LIBS@ \
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
