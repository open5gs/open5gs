/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_FDD_SUBPROJECTS_FREEDIAMETER_LIBFDCORE_2059059_FDCORE_SHA_FDD_TAB_H_INCLUDED
# define YY_FDD_SUBPROJECTS_FREEDIAMETER_LIBFDCORE_2059059_FDCORE_SHA_FDD_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int fdddebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    LEX_ERROR = 258,
    QSTRING = 259,
    INTEGER = 260,
    IDENTITY = 261,
    REALM = 262,
    PORT = 263,
    SECPORT = 264,
    SEC3436 = 265,
    NOIP = 266,
    NOIP6 = 267,
    NOTCP = 268,
    NOSCTP = 269,
    PREFERTCP = 270,
    OLDTLS = 271,
    NOTLS = 272,
    SCTPSTREAMS = 273,
    APPSERVTHREADS = 274,
    ROUTINGINTHREADS = 275,
    ROUTINGOUTTHREADS = 276,
    QINLIMIT = 277,
    QOUTLIMIT = 278,
    QLOCALLIMIT = 279,
    LISTENON = 280,
    THRPERSRV = 281,
    PROCESSINGPEERSPATTERN = 282,
    PROCESSINGPEERSMINIMUM = 283,
    TCTIMER = 284,
    TWTIMER = 285,
    NORELAY = 286,
    LOADEXT = 287,
    CONNPEER = 288,
    CONNTO = 289,
    TLS_CRED = 290,
    TLS_CA = 291,
    TLS_CRL = 292,
    TLS_PRIO = 293,
    TLS_DH_BITS = 294,
    TLS_DH_FILE = 295,
    RR_IN_ANSWERS = 296,
    ALWAYS = 297,
    NEVER = 298
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 83 "../subprojects/freeDiameter/libfdcore/fdd.y"

	char 		 *string;	/* The string is allocated by strdup in lex.*/
	int		  integer;	/* Store integer values */

#line 106 "subprojects/freeDiameter/libfdcore/2059059@@fdcore@sha/fdd.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif



int fddparse (struct fd_config * conf);

#endif /* !YY_FDD_SUBPROJECTS_FREEDIAMETER_LIBFDCORE_2059059_FDCORE_SHA_FDD_TAB_H_INCLUDED  */
