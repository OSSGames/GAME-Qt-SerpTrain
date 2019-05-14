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
 * $LastChangedDate: 2009-03-04 12:14:03 +0100 (Mi, 04. Mär 2009) $
 * $LastChangedRevision: 222 $
*/

#include "SChgLayerSizeDlg.h"
#include "SException.h"

SChgLayerSizeDlg::SChgLayerSizeDlg(QWidget *f_parent, int f_rows, int f_cols)
    : QDialog(f_parent)
{
	ui.setupUi(this);
	SetAddColState(true);
	SetAddRowState(true);
	
	ui.whereToAddCol->addItem("before 1. column");
	for ( int i = 1; i <= f_cols; i++ ) {
		ui.deleteCol->addItem(QString::number(i) + ".");
		ui.whereToAddCol->addItem("after " + QString::number(i) + ". col");
	}

	ui.whereToAddRow->addItem("before 1. row");
	for ( int i = 1; i <= f_rows; i++ ) {
		ui.deleteRow->addItem(QString::number(i) + ".");
		ui.whereToAddRow->addItem("after " + QString::number(i) + ". row");
	}
}

SChgLayerSizeDlg::~SChgLayerSizeDlg()
{
}

#define IMPLEMENT_FUNCTIONS_FOR(COL_OR_ROW)											\
void SChgLayerSizeDlg::SetAdd ## COL_OR_ROW ## State( bool f_state ) {				\
	ui.radioAdd ## COL_OR_ROW->setChecked(f_state);									\
	ui.radioDel ## COL_OR_ROW->setChecked(!f_state);								\
																					\
	for (int i = 0; i < ui.add ## COL_OR_ROW ## Layout->count(); ++i) {				\
		QWidget * subWidget = ui.add ## COL_OR_ROW ## Layout->itemAt(i)->widget();	\
    	if ( subWidget != NULL )													\
    		subWidget->setEnabled(f_state);											\
	}																				\
																					\
	for (int i = 0; i < ui.del ## COL_OR_ROW ## Layout->count(); ++i) {				\
		QWidget * subWidget = ui.del ## COL_OR_ROW ## Layout->itemAt(i)->widget();	\
    	if ( subWidget != NULL )													\
    		subWidget->setEnabled(!f_state);										\
	}																				\
}																					\
																					\
bool SChgLayerSizeDlg::Add ## COL_OR_ROW() {										\
	throwIfFalse(ui.radioAdd ## COL_OR_ROW->isChecked() != ui.radioDel## COL_OR_ROW->isChecked());	\
	return ui.radioAdd ## COL_OR_ROW->isChecked();									\
}																					\
																					\
int SChgLayerSizeDlg::Amount ## COL_OR_ROW ## ToAdd() { 							\
	throwIfFalse(Add ## COL_OR_ROW() == true);										\
	return ui.amountToAdd ## COL_OR_ROW->value();									\
}																					\
																					\
int SChgLayerSizeDlg::Add ## COL_OR_ROW ## Before() { 								\
	throwIfFalse(Add ## COL_OR_ROW() == true && true);								\
	return ui.whereToAdd ## COL_OR_ROW->currentIndex();								\
}																					\
																					\
int SChgLayerSizeDlg::Delete ## COL_OR_ROW ## At() { 								\
	throwIfFalse(Add ## COL_OR_ROW() == false );									\
	return ui.delete ## COL_OR_ROW->currentIndex();									\
}																					\

IMPLEMENT_FUNCTIONS_FOR(Col)
IMPLEMENT_FUNCTIONS_FOR(Row)
