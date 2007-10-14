import os

try:
  from pyplusplus import module_builder
except ImportError:
  print ("Warning: py++ not present so the Python bindings for Ark3D cannot be rebuilt if the header files change. Will use the python_bindings.cpp from the SVN repository")
  exit(0)


#Creating an instance of class that will help you to expose your declarations
mb = module_builder.module_builder_t( [os.path.abspath('../ConstraintSolver/Ark3DModel.h')]
                                      , gccxml_path=r"" 
                                      , working_directory=os.path.abspath('./')
                                      , include_paths=[os.path.abspath('../ConstraintSolver')]
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
#mb.print_declarations()

#Creating code creator. After this step you should not modify/customize declarations.
mb.build_code_creator( module_name='ark3d_module' )

#Writing code to file.
mb.write_module( './python_bindings.cpp' )
