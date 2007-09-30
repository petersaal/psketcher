# The following module is included so that the pkg_check_modules macro can be used below
find_package(PkgConfig)

# need to foolowing module to run swig
FIND_PACKAGE(SWIG)
INCLUDE(${SWIG_USE_FILE})

SET(CMAKE_SWIG_FLAGS "")

SET_SOURCE_FILES_PROPERTIES(ark3d_module.i PROPERTIES CPLUSPLUS ON)
SET_SOURCE_FILES_PROPERTIES(ark3d_module.i PROPERTIES SWIG_FLAGS "")
SWIG_ADD_MODULE(ark3d lua ark3d_module.i)
#SWIG_LINK_LIBRARIES(example ${PYTHON_LIBRARIES})

#lualib
pkg_check_modules(LUALIB lua5.1)
INCLUDE_DIRECTORIES (${LUALIB_INCLUDE_DIRS})

ADD_LIBRARY (Ark3dLuaModule STATIC ark3d_moduleLUA_wrap.cxx)
