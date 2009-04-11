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

#ifndef POINT2DEDITDIALOG_H
#define POINT2DEDITDIALOG_H

#include <QDialog>
#include "../ConstraintSolver/Primitives.h"

class QDialogButtonBox;
class QLabel;
class QCheckBox;
class QLineEdit;
class QAbstractButton;

class Point2DEditDialog : public QDialog
{
	Q_OBJECT

	public:
		Point2DEditDialog(Point2DPointer point, QWidget *parent = 0);

	signals:
		void modelChanged(QString description);  // emitted when changes are applied

	private slots:
		void accept();
		void textChanged();
		void applyChanges();
		void resetDialog();

	private:
		// methods

		// Parameters
		Point2DPointer point_; // point that is being editted by this dialog box
		double starting_s_; // starting values, used if user clicks reset button
		double starting_t_;
		bool starting_s_free_;
		bool starting_t_free_;

		QDialogButtonBox *buttonBox_;
		
		QLabel *sLabel_;
		QCheckBox *sFixedCheckBox_;
		QLineEdit *sLineEdit_;

		QLabel *tLabel_;
		QCheckBox *tFixedCheckBox_;
		QLineEdit *tLineEdit_;

		// pointers to standard buttons created by QDialogButtonBox, stored for convenience
		QPushButton *okButton_;
		QPushButton *applyButton_;
		QPushButton *resetButton_;
};



#endif // POINT2DEDITDIALOG_H
