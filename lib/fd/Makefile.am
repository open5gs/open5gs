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

pkglib_LTLIBRARIES = libfd.la

libfd_la_SOURCES = \
	fd-message.h fd-logger.h fd-lib.h \
	libapp_sip.c fd-message.c fd-logger.c fd-config.c fd-init.c \
	\
	s6a/s6a-dict.h s6a/s6a-message.h \
	s6a/dict-init.c \
	\
	gx/gx-dict.h gx/gx-message.h \
	gx/dict-init.c gx/gx-message.c \
	\
	rx/rx-dict.h rx/rx-message.h \
	rx/dict-init.c rx/rx-message.c \
	$(NULL)

libfd_la_DEPENDENCIES = \
	@OGSCORE_LIBS@ \
    $(top_srcdir)/lib/@FREEDIAMETER_DIR@/libfdcore/libfdcore.la \
    $(top_srcdir)/lib/@FREEDIAMETER_DIR@/libfdproto/libfdproto.la \
    $(top_srcdir)/lib/base/libbase.la \
	$(NULL)

libfd_la_LIBADD = \
	@OGSCORE_LIBS@ \
    $(top_srcdir)/lib/@FREEDIAMETER_DIR@/libfdcore/libfdcore.la \
    $(top_srcdir)/lib/@FREEDIAMETER_DIR@/libfdproto/libfdproto.la \
    $(top_srcdir)/lib/base/libbase.la \
	$(NULL)

AM_CPPFLAGS = \
	@OGSCORE_CFLAGS@ \
    -I$(top_srcdir)/lib \
	-I$(top_srcdir)/lib/@FREEDIAMETER_DIR@/include \
	$(NULL)

AM_CFLAGS = \
	-Wall -Werror \
	$(NULL)
