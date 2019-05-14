/* SerpTrain (a game with balls on rails)
 * Copyright (C) 2008, 2009  Michael \"Serpedon\" Walz (http://serpedon.de)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  US
 * 
 * $LastChangedDate: 2009-03-27 00:22:56 +0100 (Fr, 27. Mär 2009) $
 * $LastChangedRevision: 251 $
*/

#include "SConfigDlg.h"
#include "SException.h"
#include <QPushButton>
#include <QGridLayout>
#include <QColorDialog>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>
#include <QSpinBox>

SConfigDlg::SConfigDlg(QWidget *parent)
    : QDialog(parent)
{
	ui.setupUi(this);
	
	ui.tabs->clear();
	
	QWidget * curTab = NULL;
	QGridLayout * curLayout = NULL;
	int curRow;
		
	#define NEW_SECTION(NAME) {															\
		if ( curTab != NULL || curLayout != NULL )										\
			FinishTab( curTab, curLayout );												\
		curTab = new QWidget();															\
		curLayout = new QGridLayout(curTab);											\
		ui.tabs->addTab(curTab, NAME);													\
		curRow = 0;																		\
		curLayout->addWidget(new QLabel("Default\nvalue?", curTab), curRow, 2);			\
	}				

	#define NEW_CONFIG(TYPE, NAME, STD_VALUE, DESC) {									\
		throwIfFalse(curLayout != NULL );												\
		SConfigItem_##TYPE * configItem =												\
			new SConfigItem_##TYPE( curTab, m_##NAME, m_##NAME##default,				\
									STD_VALUE, Set##NAME );								\
		QLabel * label = new QLabel(tr(DESC ":"), curTab);								\
		QCheckBox * checkBox = new QCheckBox(curTab);									\
																						\
		checkBox->setChecked( m_##NAME##default );										\
		configItem->SetDefault( m_##NAME##default );									\
	 	label->setBuddy(configItem);													\
	 	label->setWordWrap( true );														\
	 	throwIfFalse(QObject::connect(	configItem, SIGNAL(ValueChanged(bool)),			\
	 									checkBox, SLOT(setChecked(bool))));		 		\
	 	throwIfFalse(QObject::connect(	checkBox, SIGNAL(toggled(bool)),				\
	 									configItem, SLOT(SetDefault(bool))));	 		\
		throwIfFalse(QObject::connect(	this, SIGNAL(accepted()), 						\
										configItem, SLOT(Save())));						\
	 																					\
		curRow++;																		\
		curLayout->addWidget(label, curRow, 0);											\
		curLayout->addWidget(configItem, curRow, 1);									\
		curLayout->addWidget(checkBox, curRow, 2);										\
	}
	
	#define NEW_CONST(TYPE, NAME, VALUE, DESC) {};
	
	#include "SConfig.res"

	FinishTab( curTab, curLayout );
}

SConfigDlg::~SConfigDlg()
{
}

void SConfigDlg::FinishTab( QWidget * curTab, QGridLayout * curLayout ) {
	throwIfFalse(curTab != NULL);throwIfFalse(curLayout != NULL);
	
	QSpacerItem * verticalSpacer = new QSpacerItem(1, 1, QSizePolicy::Minimum, QSizePolicy::Expanding);
    curLayout->addItem(verticalSpacer, curLayout->rowCount(), 0);
}

/* Hilfsklassen für SConfigDlg */

SConfigItem_QString::SConfigItem_QString(QWidget * f_parent, const QString & f_curValue, bool f_default,
	const QString & f_stdValue, void (*f_saveFunc)(QString, bool) )
   	: QLineEdit(f_curValue, f_parent), m_stdValue ( f_stdValue ), m_default ( f_default ), m_saveFunc ( f_saveFunc )
{
	throwIfFalse(QObject::connect(this, SIGNAL(textChanged(QString)), this, SLOT(QLineEditValueChanged())));
}
 
SConfigItem_int::SConfigItem_int(QWidget * f_parent, const int & f_curValue, bool f_default,
	const int & f_stdValue, void (*f_saveFunc)(int, bool) )
   	: QSpinBox(f_parent), m_stdValue ( f_stdValue ), m_default ( f_default ), m_saveFunc ( f_saveFunc )
{
	this->setRange(	qMin(SConfig::CONFIG_INT_MINIMUM(), m_stdValue),
					qMax(SConfig::CONFIG_INT_MAXIMUM(), m_stdValue));
	
	SetValue(f_curValue);
	
	throwIfFalse(QObject::connect(this, SIGNAL(valueChanged(int)), this, SLOT(QSpinBoxValueChanged())));
}

SConfigItem_bool::SConfigItem_bool(QWidget * f_parent, const bool & f_curValue, bool f_default,
	const bool & f_stdValue, void (*f_saveFunc)(bool, bool) )
   	: QCheckBox(f_parent), m_stdValue ( f_stdValue ), m_default ( f_default ), m_saveFunc ( f_saveFunc )
{
	SetValue(f_curValue);
	
	throwIfFalse(QObject::connect(this, SIGNAL(toggled(bool)), this, SLOT(QCheckBoxValueChanged())));
}
 
SConfigItem_QColor::SConfigItem_QColor(QWidget * f_parent, const QColor & f_curValue, bool f_default,
	const QColor & f_stdValue, void (*f_saveFunc)(QColor, bool) )
   	: QPushButton("Change Color", f_parent), m_stdValue ( f_stdValue ), m_default ( f_default ), m_saveFunc ( f_saveFunc )
{
	this->setIconSize(QSize(48,16));
	SetValue(f_curValue);
	
	throwIfFalse(QObject::connect(this, SIGNAL(clicked(bool)), this, SLOT(QPushButtonClicked())));
}
 

void SConfigItem_QColor::SetValue( QColor f_value ) {
	m_curValue = f_value;
	QPixmap pixmap(48, 16);
	pixmap.fill(m_curValue);
	this->setIcon(pixmap);
}

void SConfigItem_QColor::QPushButtonClicked() {
	QRgb rgb = QColorDialog::getRgba ( GetCurValue().rgba(), NULL, this );
	QColor newColor; newColor.setRgba( rgb );
	SetValue( newColor );
	
	if ( GetCurValue() != m_stdValue ) emit ValueChanged( false );
}
 