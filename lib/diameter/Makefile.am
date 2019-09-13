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

pkglib_LTLIBRARIES = libogsdiameter.la

libogsdiameter_la_SOURCES = \
	message.h logger.h base.h \
	libapp_sip.c message.c logger.c \
	config.c init.c \
	\
	s6a/dict.h s6a/message.h s6a/dict.c \
	gx/dict.h gx/message.h gx/dict.c gx/message.c \
	rx/dict.h rx/message.h rx/dict.c rx/message.c \
	$(NULL)

libogsdiameter_la_DEPENDENCIES = \
    $(top_srcdir)/lib/freeDiameter/libfdcore/libfdcore.la \
    $(top_srcdir)/lib/freeDiameter/libfdproto/libfdproto.la \
	$(NULL)

libogsdiameter_la_LIBADD = \
	@OGSCORE_LIBS@ \
    $(top_srcdir)/lib/freeDiameter/libfdcore/libfdcore.la \
    $(top_srcdir)/lib/freeDiameter/libfdproto/libfdproto.la \
	$(NULL)

AM_CPPFLAGS = \
	@OGSCORE_CFLAGS@ \
    -I$(top_srcdir)/lib \
	-I$(top_srcdir)/lib/freeDiameter/include \
	$(NULL)

AM_CFLAGS = \
	-DOGS_DIAMETER_COMPILATION \
	-Wall -Werror \
	$(NULL)
