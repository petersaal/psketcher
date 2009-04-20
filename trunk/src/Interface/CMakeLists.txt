# The fllowing forces cmake to show the compiler command line arguements, usefull for debugging
SET (CMAKE_VERBOSE_MAKEFILE TRUE)

# Add local link paths
LINK_DIRECTORIES ("../mmcMatrix")
LINK_DIRECTORIES ("../NumOptimization")
LINK_DIRECTORIES ("../ConstraintSolver")
LINK_DIRECTORIES ("../QtBinding")
# LINK_DIRECTORIES ("../PythonBinding")
LINK_DIRECTORIES ("../InteractiveConstructors")

find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})
set(ark3d_SRCS
    main.cpp
    Ark3DWindow.cpp
    Ark3DWidget.cpp
    Point2DEditDialog.cpp
)
set(ark3d_MOC_SRCS
    ./Ark3DWindow.h
    ./Ark3DWidget.h
	./Point2DEditDialog.h
)
set(ark3d_RCCS
    Ark3D.qrc
)
qt4_wrap_cpp(ark3d_MOCS ${ark3d_MOC_SRCS})
qt4_add_resources(ark3d_RCC_SRCS ${ark3d_RCCS})
add_definitions(-DQT_NO_DEBUG)

# LINK_LIBRARIES(boost_python)

#OpenGL
FIND_PACKAGE (OpenGL REQUIRED)
INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})

#Glut
FIND_PACKAGE (GLUT REQUIRED)

# The following module is included so that the pkg_check_modules macro can be used below
find_package(PkgConfig)

#Boost
find_package( Boost 1.34 COMPONENTS filesystem )
link_directories ( ${Boost_LIBRARY_DIRS} )
include_directories ( ${Boost_INCLUDE_DIRS} )

#SQLite3
pkg_check_modules(sqlite3 sqlite3)
LINK_DIRECTORIES (${sqlite3_LIBRARY_DIRS})

#gsl
pkg_check_modules(GSL gsl)
LINK_DIRECTORIES (${GSL_LIBRARY_DIRS})
INCLUDE_DIRECTORIES (${GSL_INCLUDE_DIRS})

#ginac
pkg_check_modules(GINAC ginac)
LINK_DIRECTORIES (${GINAC_LIBRARY_DIRS})
INCLUDE_DIRECTORIES (${GINAC_INCLUDE_DIRS})

#Python
# FIND_PACKAGE(PythonLibs)
# INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})

add_executable(ark3d
    ${ark3d_SRCS}
    ${ark3d_MOCS}
    ${ark3d_RCC_SRCS})

TARGET_LINK_LIBRARIES (ark3d ${Boost_LIBRARIES})
TARGET_LINK_LIBRARIES (ark3d ${sqlite3_LIBRARIES})
TARGET_LINK_LIBRARIES(ark3d ${OPENGL_LIBRARIES})
TARGET_LINK_LIBRARIES(ark3d ${GLUT_Xmu_LIBRARY})
target_link_libraries(ark3d ${QT_LIBRARIES})
TARGET_LINK_LIBRARIES(ark3d dime)
#TARGET_LINK_LIBRARIES (ark3d ark3d_module)
TARGET_LINK_LIBRARIES (ark3d Ark3dQtBinding)
TARGET_LINK_LIBRARIES (ark3d Ark3d)
TARGET_LINK_LIBRARIES (ark3d mmcMatrix)
TARGET_LINK_LIBRARIES (ark3d InteractiveConstructors)
TARGET_LINK_LIBRARIES (ark3d ${GSL_LIBRARIES})
TARGET_LINK_LIBRARIES (ark3d ${GINAC_LIBRARIES})
TARGET_LINK_LIBRARIES (ark3d bfgs)
#TARGET_LINK_LIBRARIES (ark3d ${PYTHON_LIBRARIES})



