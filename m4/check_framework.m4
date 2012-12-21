m4_define([MY_CHECK_FRAMEWORK],
  [AC_CACHE_CHECK([if -framework $1 works],[my_cv_framework_$1],
     [save_LIBS="$LIBS"
     LIBS="$LIBS -framework $1"
     AC_LINK_IFELSE([AC_LANG_PROGRAM([],[])],
             [my_cv_framework_$1=yes],
            [my_cv_framework_$1=no])
     LIBS="$save_LIBS"
    ])
   if test "$my_cv_framework_$1"="yes"; then
       AC_DEFINE(AS_TR_CPP([HAVE_FRAMEWORK_$1]),1,
            [Define if you have the  $1 framework])
   AS_TR_CPP([FRAMEWORK_$1])="-framework $1"
   AC_SUBST(AS_TR_CPP([FRAMEWORK_$1]), "-framework $1")
   fi])
