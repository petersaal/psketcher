LINK_DIRECTORIES ("../mmcMatrix")
LINK_DIRECTORIES ("../NumOptimization")


# create the ark3d library
ADD_LIBRARY (Ark3d STATIC ConstraintSolver.cpp Sketch.cpp DOF.cpp IndependentDOF.cpp DependentDOF.cpp PrimitiveBase.cpp Ark3DModel.cpp Point.cpp Vector.cpp SketchPlane.cpp Primitive2DBase.cpp Point2D.cpp Edge2DBase.cpp Line.cpp Line2D.cpp ConstraintEquationBase.cpp DistancePoint2D.cpp DistancePoint.cpp ParallelLine.cpp ParallelLine2D.cpp TangentEdge2D.cpp AngleLine2D.cpp Arc2D.cpp EdgeLoop2D.cpp DistancePointLine2D.cpp)

# The following module is included so that the pkg_check_modules macro can be used below
find_package(PkgConfig)

#Boost
find_package( Boost 1.34 COMPONENTS filesystem )
link_directories ( ${Boost_LIBRARY_DIRS} )
include_directories ( ${Boost_INCLUDE_DIRS} )

#gsl
pkg_check_modules(GSL gsl)
LINK_DIRECTORIES (${GSL_LIBRARY_DIRS})
INCLUDE_DIRECTORIES (${GSL_INCLUDE_DIRS})

#ginac
pkg_check_modules(GINAC ginac)
LINK_DIRECTORIES (${GINAC_LIBRARY_DIRS})
INCLUDE_DIRECTORIES (${GINAC_INCLUDE_DIRS})

#SQLite3
pkg_check_modules(SQLITE3 sqlite3)
LINK_DIRECTORIES (${SQLITE3_LIBRARY_DIRS})
INCLUDE_DIRECTORIES (${SQLITE3_INCLUDE_DIRS})


