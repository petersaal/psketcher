## Process this file with automake to produce Makefile.in

LIB_OCC= -L$(CASROOT)/$(STATION)/lib -lTKernel -lTKMath -lTKG2d -lTKG3d -lTKGeomBase -lTKBRep -lTKGeomAlgo -lTKTopAlgo \
	 -lTKPrim -lTKBool -lTKFeat -lTKFillet -lTKOffset -lTKHLR \
	 -lTKService -lTKV2d -lTKV3d -lTKMesh -lTKPCAF -lTKLCAF -lTKPLCAF -lTKCDF -lTKCAF \
         -lPTKernel -lTKIGES -lTKSTEP -lTKSTEPBase -lTKSTEPAttr -lTKSTEP209 -lTKSTL -lTKVRML -lTKShHealing \
         -lTKXSBase -lTKPShape -lTKShapeSchema -lTKOpenGl

AM_CXXFLAGS=-DCSFDB -DNO_CXX_EXCEPTION -DNo_Exception -DHAVE_CONFIG_H -DHAVE_WOK_CONFIG_H -DLIN -DLININTEL

AM_CPPFLAGS= \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	@GTK_CFLAGS@ @GTKGLEXT_CFLAGS@ @LIBGLADE_CFLAGS@ @GMODULE_CFLAGS@ -I$(CASROOT)/inc -I$(CASROOT)/src/WOKTclLib

bin_PROGRAMS = ark3d

ark3d_SOURCES = \
	main.cpp \
	Support.cpp Support.h \
	Interface.cpp Interface.h \
	Callbacks.cpp Callbacks.h \
	OccGtkGLView.cpp OccGtkGLView.h \
	Translate.cpp Translate.h \
	MakeBottle.cpp

ark3d_LDADD = @GTK_LIBS@ @GTKGLEXT_LIBS@ @LIBGLADE_LIBS@ @GMODULE_LIBS@ $(INTLLIBS) $(LIB_OCC)

