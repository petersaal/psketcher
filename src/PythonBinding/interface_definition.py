import os
from pyplusplus import module_builder

#Creating an instance of class that will help you to expose your declarations
mb = module_builder.module_builder_t( [r"/home/grem/ark3d/src/ConstraintSolver/Ark3DModel.h"]
                                      , gccxml_path=r"" 
                                      , working_directory=r"/home/grem/ark3d/src/ConstraintSolver"
                                      , include_paths=['/home/grem/ark3d/src/ConstraintSolver', '/home/grem/ark3d/src/ConstraintSolver']
                                      , define_symbols=[] )


#Well, don't you want to see what is going on?
mb.print_declarations()

#Creating code creator. After this step you should not modify/customize declarations.
mb.build_code_creator( module_name='ark3d_modules' )

#Writing code to file.
mb.write_module( './python_bindings.cpp' )
