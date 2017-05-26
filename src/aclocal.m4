## --------------------------------------------- ##
## Search several directories for header file.   ##
## Built around a non-caching and silent version ##
## of AC_CHECK_HEADER.                           ##
## From Lars Hecking                             ##
## --------------------------------------------- ##

# serial 2

dnl GP_PATH_HEADER(HEADER-FILE, SEARCH-DIRS [,ACTION-IF-FOUND [,ACTION-IF-NOT-FOUND]])
AC_DEFUN(GP_PATH_HEADER,
[ac_safe=`echo "$1" | sed 'y%./+-%__p_%'`
changequote(, )dnl
  ac_tr_hdr=HAVE_`echo $1 | sed 'y%abcdefghijklmnopqrstuvwxyz./-%ABCDEFGHIJKLMNOPQRSTUVWXYZ___%'`
changequote([, ])dnl
AC_MSG_CHECKING([for $1])
gp_save_CPPFLAGS="$CPPFLAGS"
if test "$2" != yes && test "$2" != no; then
  gp_h_path=`echo "$2" | sed -e 's%/lib$1\.a$%%'`
  gp_h_prfx=`echo "$gp_h_path" | sed -e 's%/lib$%%' -e 's%/include$%%'`
  gp_h_list="$gp_h_prfx $gp_h_prfx/include $gp_h_path"
else
  gp_h_list=''
fi
for ac_dir in $gp_h_list '' /usr/local/include ; do
  test x${ac_dir} != x && CPPFLAGS="$gp_save_CPPFLAGS -I${ac_dir}"
  AC_TRY_CPP([#include <$1>], eval "ac_cv_header_$ac_safe=yes",
    eval "ac_cv_header_$ac_safe=no")
  if eval "test \"`echo '$ac_cv_header_'$ac_safe`\" = yes"; then
    break
  fi
done

if eval "test \"`echo '$ac_cv_header_'$ac_safe`\" = yes"; then
  AC_DEFINE_UNQUOTED($ac_tr_hdr)
  AC_MSG_RESULT(yes)
  ifelse([$3], , :, [$3])
else
  AC_MSG_RESULT(no)
  CPPFLAGS="$gp_save_CPPFLAGS"
  ifelse([$4], , , [$4
])dnl
fi
])


## ------------------------------ ##
## Like AC_CHECK_LIB, but quiet,  ##
## and no caching.                ##
## From Lars Hecking              ##
## ------------------------------ ##

# serial 2

dnl AC_CHECK_LIB(LIBRARY, FUNCTION [, OTHER-LIBRARIES])
AC_DEFUN(GP_CHECK_LIB_QUIET,
[ac_lib_var=`echo $1['_']$2 | sed 'y%./+-%__p_%'`
ac_save_LIBS="$LIBS"
LIBS="$TERMLIBS $TERMXLIBS -l$1 $3 $LIBS"
AC_TRY_LINK(dnl
ifelse([$2], [main], , dnl Avoid conflicting decl of main.
[/* Override any gcc2 internal prototype to avoid an error.  */
]ifelse(AC_LANG, CPLUSPLUS, [#ifdef __cplusplus
extern "C"
#endif
])dnl
[/* We use char because int might match the return type of a gcc2
    builtin and then its argument prototype would still apply.  */
char $2();
]),
            [$2()],
            eval "ac_cv_lib_$ac_lib_var=yes",
            eval "ac_cv_lib_$ac_lib_var=no")
LIBS="$ac_save_LIBS"
if eval "test \"`echo '$ac_cv_lib_'$ac_lib_var`\" = yes"; then
changequote(, )dnl
  ac_tr_lib=HAVE_LIB`echo $1 | sed -e 's/[^a-zA-Z0-9_]/_/g' \
    -e 'y/abcdefghijklmnopqrstuvwxyz/ABCDEFGHIJKLMNOPQRSTUVWXYZ/'`
changequote([, ])dnl
dnl  LIBS="$LIBS -l$1"
fi
])

## ------------------------------------------- ##
## Search several directories for library.     ##
## NOTE: OTHER_LIBRARIES are NOT automatically ##
## added to TERMLIBS. This must be done in     ##
## configure.in!                               ##
## From Lars Hecking                           ##
## ------------------------------------------- ##

# serial 2

dnl GP_PATH_LIB(LIBRARY, FUNCTION, SEARCH-DIRS [, OTHER-LIBRARIES])
AC_DEFUN(GP_PATH_LIB,
[ac_lib_var=`echo $1['_']$2 | sed 'y%./+-%__p_%'`
changequote(, )dnl
  gp_tr_lib=HAVE_LIB`echo $1 | sed -e 's/[^a-zA-Z0-9_]/_/g' \
    -e 'y/abcdefghijklmnopqrstuvwxyz/ABCDEFGHIJKLMNOPQRSTUVWXYZ/'`
changequote([, ])dnl
AC_MSG_CHECKING([for $2 in -l$1])
gp_save_TERMLIBS="$TERMLIBS"
if test "$3" != yes && test "$3" != no; then
  gp_l_path=`echo "$3" | sed -e 's%/lib$1\.a$%%'`
  gp_l_prfx=`echo $gp_l_path | sed -e 's%/lib$%%' -e 's%/include$%%'`
  gp_l_list="$gp_l_prfx $gp_l_prfx/lib $gp_l_path"
else
  gp_l_list=''
fi
for ac_dir in $gp_l_list '' /usr/local/lib ; do
  test x${ac_dir} != x && TERMLIBS="-L${ac_dir} $gp_save_TERMLIBS"
  GP_CHECK_LIB_QUIET($1,$2,$4)
  if eval "test \"`echo '$ac_cv_lib_'$ac_lib_var`\" = yes"; then
    break
  fi
done

if eval "test \"`echo '$ac_cv_lib_'$ac_lib_var`\" = yes"; then
  TERMLIBS="$TERMLIBS -l$1"
  AC_MSG_RESULT(yes)
else
  AC_MSG_RESULT(no)
  TERMLIBS="$gp_save_TERMLIBS"
fi
])


dnl 
dnl AQ_CHECK_MOZILLA()
dnl	Sets up LIBS and CFLAGS for you
dnl	Dominic Tracey 1/23/02
AC_DEFUN(AQ_CHECK_MOZILLA, [

AC_MSG_CHECKING("Mozilla lib dir")
mozilla_lib_dir="/usr/local/mozilla"
AC_ARG_WITH(mozilla-dir, [--with-mozilla-dir=DIR        specify where mozilla is installed],
        mozilla_lib_dir="$withval")
AC_MSG_RESULT($mozilla_lib_dir)

dnl check for mozilla lib (strip trailing backslash)
mozilla_lib_dir=`echo $mozilla_lib_dir | sed 's/\/$//'`
moz_lib_file="/libgtkembedmoz.so"
moz_lib=$mozilla_lib_dir$moz_lib_file

if test ! -e $moz_lib; then
        AC_MSG_ERROR([Couldn't find the Mozilla installation. If
you don't have mozilla installed, please download it from
http://mozilla.org before continuing with this installation.
If you have Mozilla installed, try telling configure
the directory where it can be found with:

        ./configure --with-mozilla-dir=DIR

replacing DIR with the Mozilla installation directory.
See the file 'config.log' for further diagnostics.])
fi

LIBS="$LIBS -L$mozilla_lib_dir"

dnl now the include file -- assume its distributed with the program
dnl being installed (though allow an over-ride if someone wants to
dnl use the version in a source tree). Not sure how platform independent
dnl gtkmozembed.h is.

AC_MSG_CHECKING("Mozilla include dir")
mozilla_incl_dir="."
AC_ARG_WITH(mozilla-incl-dir, [--with-mozilla-incl-dir=DIR        specify where mozilla header file gtkembed.h is],
        mozilla_incl_dir="$withval")
AC_MSG_RESULT($mozilla_incl_dir)

dnl check for gtkmozembed.h 
dnl strip trailing backslash 
mozilla_incl_dir=`echo $mozilla_incl_dir | sed 's/\/$//'`
moz_incl_file="/gtkmozembed.h"
moz_incl=$mozilla_incl_dir$moz_incl_file

if test ! -e $moz_incl; then
        AC_MSG_ERROR([Couldn't find the Mozilla header file gtkmozembed.h. 
If you don't have this file on your system, please download a source package
from http://mozilla.org before continuing with this installation.
If you have this file, try telling configure where you have it installed with:

        ./configure --with-mozilla-incl-dir=DIR

replacing DIR with the directory name where gtkmozembed.h lives.
See the file 'config.log' for further diagnostics.])
fi

dnl Note the C++ hard code here. If you use AC_PROG_CC in your configure.in,
dnl you'll want to change the CXXFLAGS below to CFLAGS.
if test $mozilla_incl_dir != "."; then
	CXXFLAGS="$CXXFLAGS  -I$mozilla_incl_dir `gtk-config --cflags`"
else
	CXXFLAGS="$CXXFLAGS `gtk-config --cflags`"
fi

dnl try it out
AC_CHECK_LIB(gtkembedmoz,gtk_moz_embed_new,,BAD_LINK=t)

if test BAD_LINK = t; then
        AC_MSG_ERROR([

The specified library, $MOZ_LIB, did not work properly.
Perhaps you need a newer version of Mozilla.

If you don't have mozilla installed, please download it from
http://mozilla.org before continuing with this installation.
If you have Mozilla installed, try telling configure
where you have it installed with:

        ./configure --with-mozilla-dir=DIR

replacing DIR with the Mozilla installation directory.
See the file 'config.log' for further diagnostics.])

fi
])
