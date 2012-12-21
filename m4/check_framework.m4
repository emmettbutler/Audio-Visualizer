dnl m4 macro that checks for the presence of osx frameworks

m4_define([MY_CHECK_FRAMEWORK],
  [AC_CACHE_CHECK([if -framework $1 works],[my_cv_framework_$1],
     [save_LIBS="$LIBS"
     LIBS="$LIBS -framework $1"
     AC_LINK_IFELSE([AC_LANG_PROGRAM([],[])],
             [my_cv_framework_$1=yes],
            [my_cv_framework_$1=no])
     LIBS="$save_LIBS"
    ])
dnl the double-brackets here are resolved to singles
   if [[ "$my_cv_framework_$1" == 'yes' ]] ;
   then
       AC_DEFINE(AS_TR_CPP([HAVE_FRAMEWORK_$1]),1,
            [Define if you have the  $1 framework])
       AS_TR_CPP([FRAMEWORK_$1])="-framework $1"
       AC_SUBST(AS_TR_CPP([FRAMEWORK_$1]), "-framework $1")
   fi])
