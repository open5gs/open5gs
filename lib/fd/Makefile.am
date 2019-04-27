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
	fd_message.h fd_logger.h fd_lib.h \
	libapp_sip.c fd_message.c fd_logger.c fd_config.c fd_init.c \
	\
	s6a/s6a_dict.h s6a/s6a_message.h \
	s6a/dict_init.c \
	\
	gx/gx_dict.h gx/gx_message.h \
	gx/dict_init.c gx/gx_message.c \
	\
	rx/rx_dict.h rx/rx_message.h \
	rx/dict_init.c rx/rx_message.c \
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
