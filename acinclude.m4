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
