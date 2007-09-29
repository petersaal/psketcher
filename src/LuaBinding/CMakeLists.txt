# The following module is included so that the pkg_check_modules macro can be used below
find_package(PkgConfig)

#lualib50
pkg_check_modules(LUALIB lualib50)
INCLUDE_DIRECTORIES (${LUALIB_INCLUDE_DIRS})

ADD_LIBRARY (Ark3dLuaModule STATIC ark3d_module_wrap.cxx)
