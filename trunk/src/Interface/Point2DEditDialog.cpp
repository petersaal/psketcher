#include <QtGui>
#include "Point2DEditDialog.h"

Point2DEditDialog::Point2DEditDialog(Point2DPointer point, QWidget *parent) :
point_(point), QDialog(parent)
{
	// store initial values
	starting_s_ = point_->GetSValue(); // starting values, used if user clicks reset button
	starting_t_ = point_->GetTValue();
	starting_s_free_ = point_->SIsFree();
	starting_t_free_ = point_->TIsFree();

	// create widgets
	sLabel_ = new QLabel(tr("&s value:"));
	sLineEdit_ = new QLineEdit;
	sLineEdit_->setValidator(new QDoubleValidator(this));
	connect(sLineEdit_, SIGNAL(textChanged(const QString &)), this, SLOT(sTextChanged()));
	sLabel_->setBuddy(sLineEdit_);
	sFixedCheckBox_ = new QCheckBox(tr("Fixed Parameter"));

	tLabel_ = new QLabel(tr("&t value:"));
	tLineEdit_ = new QLineEdit;
	tLineEdit_->setValidator(new QDoubleValidator(this));
	connect(sLineEdit_, SIGNAL(textChanged(const QString &)), this, SLOT(sTextChanged()));
	tLabel_->setBuddy(tLineEdit_);
	tFixedCheckBox_ = new QCheckBox(tr("Fixed Parameter"));

	buttonBox_ = new QDialogButtonBox(QDialogButtonBox::Ok
                                    | QDialogButtonBox::Cancel
                                    | QDialogButtonBox::Apply
                                    | QDialogButtonBox::Reset);

	connect(buttonBox_, SIGNAL(accepted()), this, SLOT(accept()));
	connect(buttonBox_, SIGNAL(rejected()), this, SLOT(reject()));
	connect(buttonBox_, SIGNAL(clicked(QAbstractButton *)), this, SLOT(buttonClicked(QAbstractButton*)));

	// package widgets
	
	// initialize dialog
	resetDialog();	// initialize the displayed dialog values
}

// user accepted changes, so apply the changes and let QDialog take care of the rest
void Point2DEditDialog::accept()
{
	applyChanges();	
	QDialog::accept();
}

// initialize dialog to the initial values
void Point2DEditDialog::resetDialog()
{

}

// apply the changes if valid values have been entered
void Point2DEditDialog::applyChanges()
{

}

// handle cases where user selects apply or reset buttons (Ok and Cancel are handled seperately)
void Point2DEditDialog::buttonClicked(QAbstractButton *button)
{

}

void Point2DEditDialog::sTextChanged()
{

}

void Point2DEditDialog::tTextChanged()
{

}
