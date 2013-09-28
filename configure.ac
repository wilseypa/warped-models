AC_INIT([warped-models],[1.0])

AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([foreign])
AC_PREREQ(2.61)

AC_LANG([C++])

dnl Checks for programs.
AC_PROG_LIBTOOL
AC_PROG_CXX
AC_PROG_INSTALL

dnl LT_INIT

m4_include([m4/ax_cxx_compile_stdcxx_11.m4])
AX_CXX_COMPILE_STDCXX_11([noext], [mandatory])

AH_TEMPLATE([USE_CPPUNIT], [Compile cppunit tests])

dnl Look for cppunit
dnl Check to see if cppunit was enabled.
AC_ARG_ENABLE(cppunit,
AS_HELP_STRING([--enable-cppunit],[build support for cppunit]),
AC_DEFINE(USE_CPPUNIT)
[case "${enableval}" in
  yes) cppunit=true ;;
  no)  cppunit=false ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-cppunit) ;;
esac],[cppunit=false])
AM_CONDITIONAL(USE_CPPUNIT, test x$cppunit = xtrue)

if test x$cppunit = xtrue; then
m4_include([m4/check_lib_cppunit.m4])
CHECK_LIB_CPPUNIT
fi

AC_SUBST(CPPFLAGS)
AC_SUBST(CFLAGS)
AC_SUBST(CXXFLAGS)
AC_SUBST(LDFLAGS)
AC_SUBST(LIBS)
AC_SUBST(CXX)
AC_SUBST(MAKEDEPEND)
AC_CONFIG_FILES([Makefile src/Makefile thirdparty/Makefile thirdparty/tclap/Makefile])
AC_OUTPUT