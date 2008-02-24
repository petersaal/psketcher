# The following module is included so that the pkg_check_modules macro can be used below
find_package(PkgConfig)

#gtk+-2.0
pkg_check_modules(GTK gtk+-2.0)
INCLUDE_DIRECTORIES (${GTK_INCLUDE_DIRS})

#OpenCascade
INCLUDE_DIRECTORIES (${CASROOT}/inc)
INCLUDE_DIRECTORIES (${CASROOT}/src/WOKTclLib)

# create the ark3d library
ADD_LIBRARY (InteractiveConstructors STATIC GtkEventProperties.cpp  InteractiveConstructorBase.cpp  Line2DConstructor.cpp  Point2DConstructor.cpp DistancePoint2DConstructor.cpp AngleLine2DConstructor.cpp)


