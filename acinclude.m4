dnl
dnl CORE_SUBST(varname)
dnl
dnl Adds variable with it's value into Makefile, e.g.:
dnl CC = gcc
dnl
AC_DEFUN([CORE_SUBST],[
  CORE_VAR_SUBST="$CORE_VAR_SUBST $1"
])

dnl
dnl CORE_SUBST_OLD(varname)
dnl
dnl Same as CORE_SUBST() but also substitutes all @VARNAME@
dnl instances in every file passed to AC_OUTPUT()
dnl
AC_DEFUN([CORE_SUBST_OLD],[
  CORE_SUBST($1)
  AC_SUBST($1)
])

dnl
dnl CORE_CONFIG_NICE(filename)
dnl
dnl Generates the config.nice file
dnl
AC_DEFUN([CORE_CONFIG_NICE],[
  AC_REQUIRE([AC_PROG_EGREP])
  AC_REQUIRE([LT_AC_PROG_SED])
  CORE_SUBST_OLD(EGREP)
  CORE_SUBST_OLD(SED)
  test -f $1 && mv $1 $1.old
  rm -f $1.old
  cat >$1<<EOF
#! /bin/sh
#
# Created by configure

EOF

  for var in CFLAGS CXXFLAGS CPPFLAGS LDFLAGS EXTRA_LDFLAGS_PROGRAM LIBS CC CXX; do
    eval val=\$$var
    if test -n "$val"; then
      echo "$var='$val' \\" >> $1
    fi
  done

  echo "'[$]0' \\" >> $1
  if test `expr -- [$]0 : "'.*"` = 0; then
    CONFIGURE_COMMAND="$CONFIGURE_COMMAND '[$]0'"
  else 
    CONFIGURE_COMMAND="$CONFIGURE_COMMAND [$]0"
  fi
  for arg in $ac_configure_args; do
     if test `expr -- $arg : "'.*"` = 0; then
        if test `expr -- $arg : "--.*"` = 0; then
       	  break;
        fi
        echo "'[$]arg' \\" >> $1
        CONFIGURE_OPTIONS="$CONFIGURE_OPTIONS '[$]arg'"
     else
        if test `expr -- $arg : "'--.*"` = 0; then
       	  break;
        fi
        echo "[$]arg \\" >> $1
        CONFIGURE_OPTIONS="$CONFIGURE_OPTIONS [$]arg"
     fi
  done
  echo '"[$]@"' >> $1
  chmod +x $1
  CONFIGURE_COMMAND="$CONFIGURE_COMMAND $CONFIGURE_OPTIONS"
  CORE_SUBST_OLD(CONFIGURE_COMMAND)
  CORE_SUBST_OLD(CONFIGURE_OPTIONS)
])

AC_DEFUN([AX_CHECK_COMPILE_FLAG],
[AC_PREREQ(2.59)dnl for _AC_LANG_PREFIX
AS_VAR_PUSHDEF([CACHEVAR],[ax_cv_check_[]_AC_LANG_ABBREV[]flags_$4_$1])dnl
AC_CACHE_CHECK([whether _AC_LANG compiler accepts $1], CACHEVAR, [
  ax_check_save_flags=$[]_AC_LANG_PREFIX[]FLAGS
  _AC_LANG_PREFIX[]FLAGS="$[]_AC_LANG_PREFIX[]FLAGS $4 $1"
  AC_COMPILE_IFELSE([m4_default([$5],[AC_LANG_PROGRAM()])],
    [AS_VAR_SET(CACHEVAR,[yes])],
    [AS_VAR_SET(CACHEVAR,[no])])
  _AC_LANG_PREFIX[]FLAGS=$ax_check_save_flags])
AS_IF([test x"AS_VAR_GET(CACHEVAR)" = xyes],
  [m4_default([$2], :)],
  [m4_default([$3], :)])
AS_VAR_POPDEF([CACHEVAR])dnl
])dnl AX_CHECK_COMPILE_FLAGS

# adl_RECURSIVE_EVAL(VALUE, RESULT)
# =================================
# Interpolate the VALUE in loop until it doesn't change,
# and set the result to $RESULT.
# WARNING: It's easy to get an infinite loop with some unsane input.
# For example ${datadir} becomes ${datarootdir}, and then ${prefix}/share, and
# finally ${prefix} is replaced by the prefix.
AC_DEFUN([adl_RECURSIVE_EVAL],
[_lcl_receval="$1"
$2=`(test "x$prefix" = xNONE && prefix="$ac_default_prefix"
     test "x$exec_prefix" = xNONE && exec_prefix="${prefix}"
     _lcl_receval_old=''
     while test "[$]_lcl_receval_old" != "[$]_lcl_receval"; do
       _lcl_receval_old="[$]_lcl_receval"
       eval _lcl_receval="\"[$]_lcl_receval\""
     done
     echo "[$]_lcl_receval")`])
