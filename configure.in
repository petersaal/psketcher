dnl Process this file with autoconf to produce a configure script.

AC_INIT([ark3d],[0 .1],[grem0009@users.sourceforge.net])
AM_INIT_AUTOMAKE([-Wall -Werror])
AM_CONFIG_HEADER(config.h)
AM_MAINTAINER_MODE

CFLAGS="$CFLAGS "
CXXFLAGS="$CXXFLAGS "

AC_ISC_POSIX
AC_PROG_CC
AM_PROG_CC_STDC
AC_HEADER_STDC
AC_PROG_CXX
AC_PROG_CC_C_O
AM_C_PROTOTYPES

dnl Checks for X window system directories.
AC_PATH_X
if test "x$ac_x_includes" = "x"; then
  X_INCLUDE=""
else
  X_INCLUDE="-I$ac_x_includes"
fi
if test "x$ac_x_libraries" = "x"; then
  X_LIBS="-lXt -lX11"
else
  X_LIBS="-L$ac_x_libraries -lXt -lX11"
fi


AC_HEADER_STDC
AC_LANG([C])
AC_MSG_CHECKING([ for C header files ])
AC_CHECK_HEADERS(dlfcn.h dl.h ieeefp.h time.h sys/time.h pwd.h)
AC_CHECK_HEADERS(sys/statvfs.h sys/vfs.h sys/param.h osfcn.h netdb.h)
AC_CHECK_HEADERS(sys/ioctl.h net/if.h sys/systeminfo.h sys/utsname.h)
AC_CHECK_HEADERS(sysent.h unistd.h sys/unistd.h sys/socket.h)
AC_CHECK_HEADERS(ndir.h sys/ndir.h sys/dir.h signal.h sys/signal.h)
AC_CHECK_HEADERS(sigfpe.h floatingpoint.h sys/machsig.h sys/siginfo.h)
AC_CHECK_HEADERS(malloc.h strings.h sys/stat.h sys/sem.h sys/ipc.h)
AC_CHECK_HEADERS(sys/times.h dirent.h getopt.h sys/vnode.h)


AC_LANG([C++])
AC_MSG_CHECKING([ for C++ header files ])
AC_CHECK_HEADERS(istream ostream istream fstream ios iomanip iostream )
AC_CHECK_HEADERS(stream.h strstream.h istream.h ostream.h fstream.h stdlib.h ios.h iostream.h)
AC_CHECK_HEADERS(iomanip.h limits.h values.h float.h)
AC_CHECK_HEADERS(siginfo.h bits/sigset.h bstring.h sys/types.h sys/select.h)
AC_CHECK_HEADERS(X11/extensions/transovl.h X11/extensions/readdisplay.h)
AC_CHECK_HEADERS(X11/extensions/multibuf.h)
AC_CHECK_HEADERS(sys/filio.h sys/mman.h libc.h)

dnl Make sure CSFDB is defined in CXXFLAGS and CFLAGS
CXXFLAGS="$CXXFLAGS -DCSFDB -DNO_CXX_EXCEPTION"
CFLAGS="$CFLAGS -DCSFDB -DNO_CXX_EXCEPTION"

case $platform in
	Linux*) CXXFLAGS="$CXXFLAGS -DLIN -DLININTEL -D_GNU_SOURCE=1"
		CFLAGS="$CFLAGS -DLIN -DLININTEL";;
	SunOS*) CXXFLAGS="$CXXFLAGS -Usun -DSOLARIS"
		CFLAGS="$CFLAGS -Usun -DSOLARIS";;
        OSF1*)  CXXFLAGS="$CXXFLAGS -DDECOSF1 -D__USE_STD_IOSTREAM -D_RWSTD_NO_EXCEPTIONS" 
                CFLAGS="$CFLAGS -std1 -DDECOSF1"
		LD=$CXX;;
	*) ;;
esac
AC_SUBST(platform)

dnl Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T
AC_STRUCT_TM

dnl Checks for library functions.
AC_FUNC_ALLOCA
AC_FUNC_MEMCMP
AC_TYPE_SIGNAL
AC_CHECK_FUNCS(gethostname putenv re_comp regcomp strcspn strdup strtol)
AC_CHECK_FUNCS(statfs statvfs)


AC_CACHE_CHECK(if it is OK to define stream input and output,
def_ios_ok,
[AC_LANG([C++])
 AC_TRY_COMPILE([#include <stream.h>
static const int input = (ios::in) | (ios::nocreate);
static const int output= (ios::out);],
[return 0;],
 def_ios_ok=yes, def_ios_ok=no)
])
if test "$def_ios_ok" = yes; then
  AC_DEFINE(DEF_IOS_OK,,[define if the compiler allows redefinition of stream input and output])
fi


AC_CACHE_CHECK([if class ostream has member function form],
ostream_form_ok,
[AC_LANG([C++])
 AC_TRY_COMPILE([#include <iostream.h>],
[cout << cout.form("%9.3e", 3.14159627e-4) << endl;
return 0;],
 ostream_form_ok=yes, ostream_form_ok=no)
])
if test "$ostream_form_ok" = yes; then
  AC_DEFINE(OSTREAM_FORM_OK,,[define if the class ostream has member function form])
fi


dnl Checks for typedefs, structures, and compiler characteristics.

# if the following code compiles then the union semun is
# defined in the header <sys/sem.h>

AC_CACHE_CHECK(if union semun is defined in <sys/sem.h>,
semun_defined,
[AC_LANG([C])
AC_TRY_COMPILE([#include <sys/time.h>
#include <sys/sem.h>],
[union semun c;],
 semun_defined=yes, semun_defined=no)
])
if test "$semun_defined" = yes; then
  AC_DEFINE(SEMUN_DEFINED,,[define if the union semun is in sys/sem.h])
fi

# if the following code compiles then the union semun has
# the member __buf

if test "$semun_defined" = yes; then

 AC_CACHE_CHECK(if union semun has member __buf,
 semun_buf_defined,
 [AC_LANG([C])
 AC_TRY_COMPILE([#include <sys/sem.h>],
 [ union semun c; c.__buf = 0;],
 semun_buf_defined=yes, semun_buf_defined=no)
 ])
 if test "$sumun_buf_defined" = yes; then
  AC_DEFINE(SEMUN_BUF_DEFINED,,[define if semun has member __buf])
 fi
fi


AC_CACHE_CHECK(if function semop takes a value,
semop_no_reference,
[AC_LANG([C])
AC_TRY_COMPILE([#include <sys/sem.h>
static struct sembuf event_flag ;],
[int status;
 int *semid;
 status = semop(*semid,event_flag,1);
],
 semop_no_reference=yes, semop_no_reference=no)
])
if test "$semop_no_reference" = yes; then
  AC_DEFINE(SEMOP_NO_REFERENCE,,[define if the function semop takes a value])
fi


AC_CACHE_CHECK(if function semctl takes a value,
semctl_no_reference,
[AC_LANG([C])
AC_TRY_COMPILE([#include <sys/sem.h>
int get_semaphore(semid,event,value)
 int *semid, *event, *value;
{
 int status;
 status = semctl((*semid),*event,GETVAL,value);
 return 0;
}
int *a,*b,*c;],
[if(get_semaphore(a,b,c))
 exit 0;],
 semctl_no_reference=yes, semctl_no_reference=no)
])
if test "$semctl_no_reference" = yes; then
  AC_DEFINE(SEMCTL_NO_REFERENCE,,[define if the function semctl takes a value])
fi

AC_CHECK_LIB(malloc,mallinfo,[
  AC_DEFINE(HAVE_MALLINFO,,[Define if we have a function called "mallinfo" in -lmalloc.])
  LIBS="$LIBS -lmalloc"
])

AC_CHECK_LIB(sunmath,ieee_handler,[
  AC_DEFINE(HAVE_SUNMATH,,[Define if we have a function called "ieee_handler" in -lsunmath.])
  LIBS="$LIBS -lsunmath"
])

AC_CHECK_LIB(m,finite,[
  AC_DEFINE(HAVE_FINITE,,[Define if we have a function called "finite" in -lm.])
])

#---------------------------------------------------------------------
#
# Define the Following vars : ( for OpenGL Libraries )
#  
#  
#---------------------------------------------------------------------

AC_MSG_CHECKING([for OpenGL libraries])
AC_ARG_WITH([gl-include],
[  --with-gl-include=DIR   The directory where the OpenGL headers are found],
[echo "with gl-include is " $with_gl_include;
if test x"${with_gl_include}" != x"" ; then
   if test -d "${with_gl_include}" ; then 
     if test -f "${with_gl_include}/GL/gl.h"  ;then
	if test -f "${with_gl_include}/GL/glu.h"  ;then
	   GL_INCLUDE=`(cd ${with_gl_include}; pwd)`
	   GL_INCLUDE="-I${GL_INCLUDE}"
	else
	   AC_MSG_ERROR([the ${with_gl_include} directory given by --with-gl-include doesn't contains GL/glu.h])
	fi
     else
	AC_MSG_ERROR([the ${with_gl_include} directory  given by --with-gl-include doesn't contains GL/gl.h])
     fi
  else
     AC_MSG_ERROR([the ${with_gl_include} directory  given by --with-gl-include does not exist ])
  fi     	
else
   echo "Assuming gl headers are in the compiler's search path..."
fi
],[echo "No --with-gl-include=DIR was specified"])

# check for gl library
AC_MSG_CHECKING(for Gl library)
AC_ARG_WITH([gl-library],
[  --with-gl-library=DIR   The directory where the OpenGL libraries are found],
[echo "with gl-library is " $with_gl_library;
if test -d "${with_gl_library}"  ; then
   CSF_OpenGlLibs_LIB=`(cd  ${with_gl_library}; pwd)`
   if test -f "$CSF_OpenGlLibs_LIB/libGL.so"  ; then
      CSF_OpenGlLibs_LIB="-L${CSF_OpenGlLibs_LIB} -lGLU -lGL"
   else
      AC_MSG_ERROR([the ${with_gl_library} directory  given by --with-gl-library does not contains libGL.so])
   fi
else
   AC_MSG_ERROR([the ${with_gl_library} directory  given by --with-gl-library does not exist ])
fi
],[echo "No --with-gl-library=DIR was specified"
CSF_OpenGlLibs_LIB="-lGLU -lGL"])

AC_LANG([C])
CFLAGS_save=$CFLAGS
CFLAGS="$CXXFLAGS $GL_INCLUDE"
AC_CHECK_TYPE(_GLUfuncptr,[],[CFLAGS_save="-DNOGLUfuncptr=1 $CFLAGS_save"],[#include "GL/glu.h"])
CFLAGS="$CFLAGS_save"

#---------------------------------------------------------------------
#
# Define the Following vars : ( for Java Header Files )
#  
#  
#---------------------------------------------------------------------

AC_MSG_CHECKING(for Java header files)
AC_ARG_WITH([java-include],
[  --with-java-include=DIR   The directory where the Java headers are found],
[echo " java-include is $with_java_include";
JAVA_INCLUDE=`(cd  ${with_java_include}; pwd)`
JAVA_INCLUDE_PLAT=`(find ${with_java_include} -name jni_md.h | sed 's/\.\///' | sed 's/\/jni_md.h//')`
if test x"$JAVA_INCLUDE_PLAT" != x"" ; then
   JAVA_INCLUDE="-I${JAVA_INCLUDE} -I${JAVA_INCLUDE_PLAT}"
else
   echo "Assuming Java headers are in the compiler's search path..."
fi
],[echo "No --with-java-include=DIR was specified"])

AC_LANG([C])
CXXFLAGS="$CXXFLAGS $JAVA_INCLUDE"
CPPFLAGS="$CPPFLAGS $JAVA_INCLUDE"
AC_CHECK_HEADER([jni_md.h],[build_TKjcas="TKjcas"],[build_TKjcas=""])

if test "x$build_TKjcas" != "x"; then
  AC_MSG_CHECKING("JAVA_INCLUDE = $JAVA_INCLUDE")
  AC_MSG_CHECKING(for Java Native Interface version 1.2)
  AC_TRY_COMPILE([#include <jni.h>],
  [#ifndef JNI_VERSION_1_2
    choke me
  #endif],
  [AC_MSG_RESULT([version 1.2 or greater])],
  [AC_MSG_WARN([JNI version is below 1.2, and so will not work with OpenCASCADE.])
  build_TKjcas=""])
fi

AC_MSG_CHECKING( for DPS header files )
AC_ARG_WITH([dps-include],
[  --with-dps-include=DIR   The directory where the DPS headers are found],
[echo "with dps-include is " $with_dps_include;
if test x"${with_dps_include}" != x"" ; then
   if test -d "${with_dps_include}" ; then
	dps_inc="${with_dps_include} ${with_dps_include}/X11"
	CSF_dpsLibs_INCLUDES=""
	for dps_dir in $dps_inc; do
	if test -f "${dps_dir}/DPS/dpsXclient.h" ; then
          CSF_dpsLibs_INCLUDES="-I${dps_dir}"
	break
	fi
	done
       if test -z $CSF_dpsLibs_INCLUDES ; then
	  AC_MSG_ERROR([the ${with_dps_include} directory  given by --with-dps-include does not contain DPS/dpsXclient.h])
       fi
    else
       AC_MSG_ERROR([the ${with_dps_include} directory given by --with-dps-include does not exist])
    fi
else
   echo "Assuming DPS headers are in the compiler's search path..."
fi
],[echo "No --with-dps-include=DIR was specified"])
if test x"$dps_includes" != x""; then
   CSF_dpsLibs_INCLUDES="-I$dps_include"
fi

AC_ARG_WITH([dps-library],
[  --with-dps-library=DIR   The directory where the DPS library is found],
[echo "with dps-library is " $with_dps_library;
INCLUDE_save=$INCLUDE
INCLUDE="$X_INCLUDE $CSF_dpsLibs_INCLUDES $INCLUDE"
CFLAGS_save=$CFLAGS
CFLAGS="$CFLAGS $INCLUDE"
CPPFLAGS_save=$CPPFLAGS
CPPFLAGS="$CPPFLAGS $INCLUDE"
LIBS_save=$LIBS
if test -d "${with_dps_library}"  ; then
  LIBS="$LIBS $X_LIBS -L`(cd  ${with_dps_library}; pwd)` -ldps"
  if test x"$with_dps_library" != x"" ; then
	CSF_dpsLibs_LIB="-L`(cd  ${with_dps_library}; pwd)` -ldps"
	AC_CHECK_HEADERS(DPS/dpsXclient.h, 
	AC_CHECK_LIB(dps, DPSInitialize, 
	AC_MSG_RESULT([dps library found.])
	, AC_MSG_ERROR([the ${with_dps_library} directory given by --with-dps-library does not contains DPS library]), -lXt -lX11)
	)
   else
     CSF_dpsLibs_LIB="-ldps"
     echo "Assuming DPS headers are in the compiler's search path..."
   fi
else
   AC_MSG_ERROR([the ${with_dps_library} directory  given by --with-dps-library does not exist ])
fi

LIBS=$LIBS_save
CFLAGS=$CFLAGS_save
CPPFLAGS=$CPPFLAGS_save
INCLUDE=$INCLUDE_save
],[AC_MSG_WARN(No --with-dps-library=DIR was specified)])

#---------------------------------------------------------------------
#
# Define the Following vars :
#  CSF_XmuLibs_INCLUDES
#  CSF_XmuLibs_LIB
#---------------------------------------------------------------------

AC_MSG_CHECKING(for Xmu header files)
AC_ARG_WITH([xmu-include],
[  --with-xmu-include=DIR   The directory where the Xmu headers are found],
[echo "with xmu-include is " $with_xmu_include;
if test x"${with_xmu_include}" != x"" ; then
   if test -d "${with_xmu_include}" ; then
	xmu_inc="${with_xmu_include} ${with_xmu_include}/X11"
	CSF_XmuLibs_INCLUDES=""
	for xmu_dir in $xmu_inc; do
	if test -f "${xmu_dir}/Xmu/Xmu.h" ; then
          CSF_XmuLibs_INCLUDES="-I${xmu_dir}"
	break
	fi
	done
       if test -z $CSF_XmuLibs_INCLUDES ; then
	  AC_MSG_ERROR([the ${with_xmu_include} directory  given by --with-xmu-include does not contain Xmu/StdCmap.h])
       fi
    else
       AC_MSG_ERROR([the ${with_xmu_include} directory given by --with-xmu-include does not exist])
    fi
else
   echo "Assuming Xmu headers are in the compiler's search path..."
fi
],[echo "No --with-xmu-include=DIR was specified"])
if test x"$xmu_includes" != x""; then
   CSF_XmuLibs_INCLUDES="-I$xmu_include"
fi

AC_ARG_WITH([xmu-library],
[  --with-xmu-library=DIR   The directory where the Xmu library is found],
[echo "with xmu-library is " $with_xmu_library;
INCLUDE_save=$INCLUDE
INCLUDE="$X_INCLUDE $CSF_XmuLibs_INCLUDES $INCLUDE"
CFLAGS_save=$CFLAGS
CFLAGS="$CFLAGS $INCLUDE"
CPPFLAGS_save=$CPPFLAGS
CPPFLAGS="$CPPFLAGS $INCLUDE"
LIBS_save=$LIBS
if test -d "${with_xmu_library}"  ; then
  LIBS="$LIBS $X_LIBS -L`(cd  ${with_xmu_library}; pwd)` -lXmu"
  if test x"$with_xmu_library" != x"" ; then
	CSF_XmuLibs_LIB="-L`(cd  ${with_xmu_library}; pwd)` -lXmu"
	AC_CHECK_HEADERS(Xmu/Editres.h, 
	AC_CHECK_LIB(Xmu, _XEditResCheckMessages, 
	AC_MSG_RESULT([Xmu library found.]),
	AC_MSG_ERROR([the ${with_xmu_library} directory given by --with-xmu-library does not contains xmu library]), -lXt -lX11),,[#include <X11/Intrinsic.h>]
	)
   else
     CSF_XmuLibs_LIB="-lXmu"
     echo "Assuming Xmu headers are in the compiler's search path..."
   fi
else
   AC_MSG_ERROR([the ${with_xmu_library} directory  given by --with-xmu-library does not exist ])
fi

LIBS=$LIBS_save
CFLAGS=$CFLAGS_save
CPPFLAGS=$CPPFLAGS_save
INCLUDE=$INCLUDE_save
],[AC_MSG_WARN(No --with-xmu-library=DIR was specified)])

#---------------------------------------------------------------------
#
# Define the Following vars :
#  STLPortInclude
#  STLPortLib
#---------------------------------------------------------------------

AC_LANG([C++])
AC_MSG_CHECKING(for STLPort header files)
AC_ARG_WITH([stlport-include],
[  --with-stlport-include=DIR   The directory where the STLPort headers are found],
[echo "with stlport-include is " $with_stlport_include;
if test x"${with_stlport_include}" != x"" ; then
   if test -d "${with_stlport_include}" ; then
	STLPort_INCLUDES="${with_stlport_include}"
	if test -f "${STLPort_INCLUDES}/algorithm" ; then
          STLPort_INCLUDES="-I${STLPort_INCLUDES}"
	fi
	if test -z $STLPort_INCLUDES ; then
	  AC_MSG_ERROR([the ${STLPort_INCLUDES} directory  given by --with-stlport-include does not contain algorithm])
	fi
   else
	AC_MSG_ERROR([the ${STLPort_INCLUDES} directory given by --with-stlport-include does not exist])
   fi
else
   echo "Assuming STLPort headers are in the compiler's search path..."
fi
],[echo "No --with-stlport-include=DIR was specified"])
if test x"$stlport_includes" != x""; then
   STLPort_INCLUDES="-I$stlport_include"
fi

AC_ARG_WITH([stlport-libname],
[  --with-stlport-libname=NAME   The name of STLPort library is found],
[echo "with stlport-libname is " $with_stlport_libname;
  if test x"$with_stlport_libname" != x"" ; then	
	STLPort_LIB_CC=$with_stlport_libname
  else
	STLPort_LIB_CC="stlport"
	echo "Assuming STLPort library name is stlport"
  fi
],[STLPort_LIB_CC="stlport"
   AC_MSG_WARN(No --with-stlport-libname=NAME was specified)])

AC_ARG_WITH([stlport-library],
[  --with-stlport-library=DIR   The directory where the STLPort library is found],
[echo "with stlport-library is " $with_stlport_library;
INCLUDE_save=$INCLUDE
INCLUDE="$STLPort_INCLUDES $INCLUDE"
CPPFLAGS_save=$CPPFLAGS
CPPFLAGS="$INCLUDE $CPPFLAGS"
CXXFLAGS_save=$CPPFLAGS
CXXFLAGS="$INCLUDE $CPPFLAGS"
LDFLAGS_save=$LDFLAGS
if test -d "${with_stlport_library}"  ; then
  STLPort_LIB="-L`(cd  ${with_stlport_library}; pwd)` -l$STLPort_LIB_CC"
  LDFLAGS="$LDFLAGS $STLPort_LIB"
#  if test x"$with_stlport_library" != x"" ; then	
#	AC_CHECK_HEADERS(algorithm, 
#	AC_CHECK_LIB($STLPort_LIB_CC, main,
#	AC_MSG_RESULT([STLPort library found.]),
#	AC_MSG_ERROR([the $STLPort_LIB directory given by --with-stlport-library does not contains $STLPort_LIB_CC library]), -l$STLPort_LIB_CC))
#   else
#     STLPort_LIB="-l$STLPort_LIB_CC"
#     echo "Assuming STLPort libraries are in the compiler's search path..."
#   fi
else
   AC_MSG_ERROR([the ${with_stlport_library} directory  given by --with-stlport-library does not exist ])
fi

LDFLAGS="$LDFLAGS_save"
CPPFLAGS="$CPPFLAGS_save"
INCLUDE=$INCLUDE_save
CXXFLAGS="$CXXFLAGS_save"
],[AC_MSG_WARN(No --with-stlport-library=DIR was specified)])

AC_SUBST(STLPort_INCLUDES)
AC_SUBST(STLPort_LIB)
#AC_SUBST(JAVA_INCLUDE)
AC_SUBST(build_TKjcas)
AC_SUBST(LIBS)
AC_SUBST(LDFLAGS)


CSF_SOCKETLibs_LIB=""
CSF_XwLibs_LIB="$X_LIBS"
CSF_OpenGlLibs_INCLUDES="$GL_INCLUDE"

AC_SUBST(CSF_SOCKETLibs_LIB)
AC_SUBST(CSF_FPELibs_LIB)
AC_SUBST(CSF_OpenGlLibs_INCLUDES)
AC_SUBST(CSF_OpenGlLibs_LIB)
AC_SUBST(CSF_advapi32_LIB)
AC_SUBST(CSF_gdi32_LIB)
AC_SUBST(CSF_user32_LIB)
AC_SUBST(CSF_wsock32_LIB)
AC_SUBST(CSF_XwLibs_LIB)
AC_SUBST(CSF_dpsLibs_LIB)
AC_SUBST(CSF_dpsLibs_INCLUDES)
AC_SUBST(CSF_XmuLibs_LIB)
AC_SUBST(CSF_XmuLibs_INCLUDES)

PKG_CHECK_MODULES(GTK, [gtk+-2.0 >= 2.0.0])
AC_SUBST(GTK_CFLAGS)
AC_SUBST(GTK_LIBS)

PKG_CHECK_MODULES(GTKGLEXT, [gtkglext-1.0 >= 1.0.6])
AC_SUBST(GTKGLEXT_CFLAGS)
AC_SUBST(GTKGLEXT_LIBS)

PKG_CHECK_MODULES(LIBGLADE, [libglade-2.0 >= 2.0.0])
AC_SUBST(LIBGLADE_CFLAGS)
AC_SUBST(LIBGLADE_LIBS)

PKG_CHECK_MODULES(GMODULE, [gmodule-2.0])
AC_SUBST(GMODULE_CFLAGS)
AC_SUBST(GMODULE_LIBS)

#needed for libglade app
#TEMP ="--export-dynamic" 
#LD_FLAGS="$LD_FLAGS $TEMP"

GETTEXT_PACKAGE=ark3d
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE", [Gettext package.])

dnl Add the languages which your application supports here.
ALL_LINGUAS=""
AM_GLIB_GNU_GETTEXT

AC_OUTPUT([
Makefile
src/Makefile
po/Makefile.in
])

