AC_INIT([langdetect], [1.0.0], [https://github.com/kirasystems/langdetect])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

AC_PROG_CXX
AC_LANG([C++])
CXXFLAGS=${CXXFLAGS:-""}
AC_PREFIX_DEFAULT([/usr/local])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
 Makefile
 src/Makefile
])

# ----------------------------------------
# Add host checks
# ----------------------------------------
AM_PROG_AR
LT_INIT

# ----------------------------------------
# C++ related options
# ----------------------------------------
AC_LANG_CPLUSPLUS

CXXFLAGS="$CXXFLAGS -std=c++11"

# ----------------------------------------
# Configuration file for pkg-config
# ----------------------------------------
AC_CONFIG_FILES([langdetect.pc])

AC_OUTPUT

