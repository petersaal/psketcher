LINK_DIRECTORIES ("../mmcMatrix")
LINK_DIRECTORIES ("../NumOptimization")

# create the ark3d library
ADD_LIBRARY (Ark3d STATIC ConstraintSolver.cpp Ark3DModel.cpp Primitives.cpp)

# The following module is included so that the pkg_check_modules macro can be used below
find_package(PkgConfig)

#gsl
pkg_check_modules(GSL gsl)
LINK_DIRECTORIES (${GSL_LIBRARY_DIRS})
INCLUDE_DIRECTORIES (${GSL_INCLUDE_DIRS})

#ginac
pkg_check_modules(GINAC ginac)
LINK_DIRECTORIES (${GINAC_LIBRARY_DIRS})
INCLUDE_DIRECTORIES (${GINAC_INCLUDE_DIRS})

ADD_EXECUTABLE (test test.cpp ConstraintSolver.cpp)
TARGET_LINK_LIBRARIES(test mmcMatrix)
TARGET_LINK_LIBRARIES (test ${GSL_LIBRARIES})
TARGET_LINK_LIBRARIES (test ${GINAC_LIBRARIES})
TARGET_LINK_LIBRARIES(test bfgs)

ADD_EXECUTABLE (test2 test2.cpp)
TARGET_LINK_LIBRARIES(test2 Ark3d)
TARGET_LINK_LIBRARIES(test2 mmcMatrix)
TARGET_LINK_LIBRARIES (test2 ${GSL_LIBRARIES})
TARGET_LINK_LIBRARIES (test2 ${GINAC_LIBRARIES})
TARGET_LINK_LIBRARIES(test2 bfgs)


