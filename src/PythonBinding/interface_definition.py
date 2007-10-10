import os
from pyplusplus import module_builder

#Creating an instance of class that will help you to expose your declarations
mb = module_builder.module_builder_t( [r"/home/grem/ark3d/src/ConstraintSolver/Ark3DModel.h"]
                                      , gccxml_path=r"" 
                                      , working_directory=r"/home/grem/ark3d/src/ConstraintSolver"
                                      , include_paths=['/home/grem/ark3d/src/ConstraintSolver', '/home/grem/ark3d/src/ConstraintSolver']
                                      , define_symbols=[] )

#exclude ConstraintSolver class
mb.class_('ConstraintSolver').exclude()

#exclude IndependentDOF and DependentDOF classes
mb.class_('IndependentDOF').exclude()
mb.class_('DependentDOF').exclude()

#exclude classes that contain the word Base in the name since they aren't useful to the interface
mb.classes( lambda decl: 'Base' in decl.name ).exclude()

#exlude methods that contain 'GetTangent' used by the Edge2D derived classes since these aren't needed in the interface (these methods are used internally to construct tangency constraints)
mb.member_functions( lambda decl: 'GetTangent' in decl.name ).exclude()

#exlude methods that return GiNaC::ex objects
mb.member_functions( return_type='::GiNaC::ex' ).exclude()
	 

#Well, don't you want to see what is going on?
mb.print_declarations()

#Creating code creator. After this step you should not modify/customize declarations.
mb.build_code_creator( module_name='ark3d_module' )

#Writing code to file.
mb.write_module( './python_bindings.cpp' )
