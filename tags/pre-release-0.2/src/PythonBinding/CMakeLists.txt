

# ADD_CUSTOM_TARGET (PythonBindings ALL echo)
ADD_CUSTOM_COMMAND(
    SOURCE    interface_definition.py
    COMMAND   python
    ARGS      interface_definition.py
    TARGET    PythonBindings
    DEPENDS   ../ConstraintSolver/Sketch.h ../ConstraintSolver/Ark3DModel.h ../ConstraintSolver/ConstraintSolver.h ../ConstraintSolver/Primitives.h
    OUTPUTS   python_bindings.cpp
)


FIND_PACKAGE(PythonLibs)
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})

ADD_LIBRARY (ark3d_module STATIC python_bindings.cpp)
