/****************************************************************************
**
** This file is part of the pSketcher project.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** Copyright (C) 2006-2008 Michael Greminger. All rights reserved.
**
****************************************************************************/

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
	connect(sLineEdit_, SIGNAL(textChanged(const QString &)), this, SLOT(textChanged()));
	sLabel_->setBuddy(sLineEdit_);
	sFixedCheckBox_ = new QCheckBox(tr("Fixed Parameter"));

	tLabel_ = new QLabel(tr("&t value:"));
	tLineEdit_ = new QLineEdit;
	tLineEdit_->setValidator(new QDoubleValidator(this));
	connect(tLineEdit_, SIGNAL(textChanged(const QString &)), this, SLOT(textChanged()));
	tLabel_->setBuddy(tLineEdit_);
	tFixedCheckBox_ = new QCheckBox(tr("Fixed Parameter"));

	buttonBox_ = new QDialogButtonBox(QDialogButtonBox::Ok
                                    | QDialogButtonBox::Cancel
                                    | QDialogButtonBox::Apply
                                    | QDialogButtonBox::Reset);

	okButton_ = buttonBox_->button(QDialogButtonBox::Ok);
	applyButton_ = buttonBox_->button(QDialogButtonBox::Apply);
	resetButton_ = buttonBox_->button(QDialogButtonBox::Reset);

	connect(buttonBox_, SIGNAL(accepted()), this, SLOT(accept()));
	connect(buttonBox_, SIGNAL(rejected()), this, SLOT(reject()));
	connect(buttonBox_, SIGNAL(clicked(QAbstractButton *)), this, SLOT(buttonClicked(QAbstractButton*)));

	connect(applyButton_, SIGNAL(clicked()), this, SLOT(applyChanges()));
	connect(resetButton_, SIGNAL(clicked()), this, SLOT(resetDialog()));


	// package widgets
	QGridLayout *grid = new QGridLayout;
	grid->addWidget(sLabel_,0,0);
	grid->addWidget(sLineEdit_,0,1);
	grid->addWidget(sFixedCheckBox_,0,2);
	grid->addWidget(tLabel_,1,0);
	grid->addWidget(tLineEdit_,1,1);
	grid->addWidget(tFixedCheckBox_,1,2);

	QVBoxLayout *hbox = new QVBoxLayout();
	hbox->addLayout(grid);
	hbox->addWidget(buttonBox_);

	setLayout(hbox);

	setWindowTitle(tr("Edit Point"));

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
	sLineEdit_->setText(QString("%1").arg(starting_s_));
	tLineEdit_->setText(QString("%1").arg(starting_t_));
	sFixedCheckBox_->setChecked( ! starting_s_free_);
	tFixedCheckBox_->setChecked( ! starting_t_free_);
}

// apply the changes if valid values have been entered
void Point2DEditDialog::applyChanges()
{
	point_->SetSValue(sLineEdit_->text().toDouble());
	point_->SetTValue(tLineEdit_->text().toDouble());
	point_->SetSFree( ! sFixedCheckBox_->isChecked());
	point_->SetTFree( ! tFixedCheckBox_->isChecked());

	emit modelChanged(tr("Point Edit"));
}


void Point2DEditDialog::textChanged()
{
	bool acceptable_input;

	acceptable_input = sLineEdit_->hasAcceptableInput() && tLineEdit_->hasAcceptableInput();

	okButton_->setEnabled(acceptable_input);
	applyButton_->setEnabled(acceptable_input);
}
