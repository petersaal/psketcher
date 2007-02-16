#ifndef TRANSLATE_H
#define TRANSLATE_H

#include <gtk/gtk.h>

#include <AIS_InteractiveContext.hxx>
#include <TopTools_HSequenceOfShape.hxx>

class TranslateDlg;

class Translate
{
    public:
	enum
	{
	    FormatBREP, 
	    FormatIGES, 
	    FormatSTEP, 
	    FormatCSFDB, 
	    FormatVRML, 
	    FormatSTL
	};

	Translate(GtkWidget* widget);
	~Translate();

	bool                              importModel( const int, const Handle(AIS_InteractiveContext)& );
	bool                              exportModel( const int, const Handle(AIS_InteractiveContext)& );

	std::string                           info() const;

    private:
	Handle(TopTools_HSequenceOfShape) importModel( const int, const std::string& );
	bool                              exportModel( const int, const std::string&,
		const Handle(TopTools_HSequenceOfShape)& );
	std::string                       selectFileName( const int, const bool );
	TranslateDlg*                     getDialog( const int, const bool );
	Handle(TopTools_HSequenceOfShape) getShapes( const Handle(AIS_InteractiveContext)& );

	Handle(TopTools_HSequenceOfShape) importBREP( const std::string& );
	Handle(TopTools_HSequenceOfShape) importIGES( const std::string& );
	Handle(TopTools_HSequenceOfShape) importSTEP( const std::string& );
	Handle(TopTools_HSequenceOfShape) importCSFDB( const std::string& );

	bool exportBREP( const std::string&, const Handle(TopTools_HSequenceOfShape)& );
	bool exportIGES( const std::string&, const Handle(TopTools_HSequenceOfShape)& );
	bool exportSTEP( const std::string&, const Handle(TopTools_HSequenceOfShape)& );
	bool exportCSFDB( const std::string&, const Handle(TopTools_HSequenceOfShape)& );
	bool exportSTL( const std::string&, const Handle(TopTools_HSequenceOfShape)& );
	bool exportVRML( const std::string&, const Handle(TopTools_HSequenceOfShape)& );

	bool checkFacetedBrep( const Handle(TopTools_HSequenceOfShape)& );

    private:
	GtkWidget*			  myParent;
	TranslateDlg*                     myDlg;
	std::string                       myInfo;
};

#endif
