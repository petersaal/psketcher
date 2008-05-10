PROJECT (ark3d)

set (ARK3D_VERSION 0.1)
ADD_DEFINITIONS (-DVERSION="\\"${ARK3D_VERSION}\\"")

# FindPkcConfig is in the following directory since it is not part of the
# official cmake release yet (it's in CVS)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake_scripts)


add_subdirectory (src/Interface)
add_subdirectory (src/NumOptimization)
add_subdirectory (src/mmcMatrix)
add_subdirectory (src/ConstraintSolver)
add_subdirectory (src/QtBinding)
#add_subdirectory (src/PythonBinding)
add_subdirectory (src/InteractiveConstructors)

