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

#include "SBoardGoalSettingsWidget.h"
#include "SBoardTunnelSettingsWidget.h"

SBoardGoalSettingsWidget::SBoardGoalSettingsWidget(bool f_continueFlag, bool f_showAllFlag)
    : QWidget()
{
	ui.setupUi(this);
	
	for ( int fieldNr = 0; fieldNr < SBoardGoal::AmountFields; fieldNr++ ) {
		QHBoxLayout * hLayout = new QHBoxLayout();
		ui.radioLayout->addLayout(hLayout);
		
		QButtonGroup * buttonGroup = new QButtonGroup(this);
		
		QLabel * label = new QLabel(QString::number(fieldNr + 1) + ".", this);
		hLayout->addWidget(label);
				
		QRadioButton * button = new QRadioButton();
		buttonGroup->addButton(button);
		button->setText("none");
		hLayout->addWidget(button);
		throwIfFalse(QObject::connect(button, SIGNAL(clicked()), this, SLOT(radioButtons_clicked())));
		m_radioButtons[fieldNr].insert(RAIL_COLOR, button);
		button->setChecked(true);

		for ( SColor ballColor = FIRST_BALL_COLOR; ballColor <= LAST_BALL_COLOR; ++ballColor) {
			QRadioButton * button = new QRadioButton(this);
			buttonGroup->addButton(button);
			button->setIcon(SBoardTunnelSettingsWidget::GetBallIcon(ballColor));
			hLayout->addWidget(button);
			throwIfFalse(QObject::connect(button, SIGNAL(clicked()), this, SLOT(radioButtons_clicked())));
			m_radioButtons[fieldNr].insert(ballColor, button);
		}
	}
	
	ui.checkBoxConnected->setChecked(f_continueFlag);
	ui.checkBoxShowAll->setChecked(f_showAllFlag);
}

SBoardGoalSettingsWidget::~SBoardGoalSettingsWidget()
{
}

void SBoardGoalSettingsWidget::radioButtons_clicked() {
	UpdateColors();
}

void SBoardGoalSettingsWidget::on_checkBoxConnected_toggled(bool f_newValue) {
	emit ContinueFlagChanged(f_newValue);	
}

void SBoardGoalSettingsWidget::on_checkBoxShowAll_toggled(bool f_newValue) {
	emit ShowAllFlagChanged(f_newValue);	
}

void SBoardGoalSettingsWidget::SetFieldColor(int f_fieldNr, enum COLOR f_color) {
	throwIfFalse(f_fieldNr >= 0 && f_fieldNr <= SBoardGoal::AmountFields);
	throwIfFalse(m_radioButtons[f_fieldNr].contains(f_color));
	m_radioButtons[f_fieldNr][f_color]->setChecked(true);
}

enum COLOR SBoardGoalSettingsWidget::GetCheckedColor(int f_fieldNr) {
	throwIfFalse(f_fieldNr >= 0 && f_fieldNr <= SBoardGoal::AmountFields);

	QHashIterator<enum COLOR, QRadioButton *> i(m_radioButtons[f_fieldNr]);
	while (i.hasNext()) {
		i.next();
		if ( i.value()->isChecked() )
			return i.key();
	}
	
	throwEx("No color is checked!");
	return BLACK;
}

void SBoardGoalSettingsWidget::SetGoalFields(SArray<SColor, SBoardGoal::AmountFields> f_goalColors) {
	int colorIndex = 0;
	// Zuerst werden alle Farbigen Bälle gesetzt (None-Bälle werden nicht gesetzt, weil die None's nach unten sollen)
	for ( int goalField = 0; goalField < SBoardGoal::AmountFields; goalField++ ) {
		if ( f_goalColors[goalField] != RAIL_COLOR )
			SetFieldColor(colorIndex++, f_goalColors[goalField]);
	}
	
	// Restlichen Plätze auf none setzen
	for ( int i = colorIndex; i < SBoardGoal::AmountFields; i++ ) 
		SetFieldColor(i, RAIL_COLOR);
}

void SBoardGoalSettingsWidget::UpdateColors() {
	SArray<SColor, SBoardGoal::AmountFields> goalColors;
	
	for ( int fieldNr = 0; fieldNr < SBoardGoal::AmountFields; fieldNr++ ) {
		goalColors[fieldNr] = GetCheckedColor(fieldNr);
		
		// Wenn das none war, dann alle weiteren Farben löschen
		if ( goalColors[fieldNr] == RAIL_COLOR ) {
			for ( fieldNr++; fieldNr < SBoardGoal::AmountFields; fieldNr++ ) {
				goalColors[fieldNr] = RAIL_COLOR;
				SetFieldColor(fieldNr, RAIL_COLOR);
			}
		}
	}
	
	SArray<SColor, SBoardGoal::AmountFields> result;
	// Farben so umsortieren, dass am Anfang alle none's stehen
	int amountNone = 0;
	for ( int fieldNr = 0; fieldNr < SBoardGoal::AmountFields; fieldNr++ ) {
		if ( goalColors[fieldNr] == RAIL_COLOR )
			amountNone++;
	}
	
	for ( int fieldNr = 0; fieldNr < amountNone; fieldNr++ )
		result[fieldNr] = RAIL_COLOR;
		
	int colorIndex = amountNone;
	for ( int goalField = 0; goalField < SBoardGoal::AmountFields; goalField++ ) {
		if ( goalColors[goalField] != RAIL_COLOR )
			result[colorIndex++] = goalColors[goalField];
	}
	
	throwIfFalse(colorIndex == SBoardGoal::AmountFields);

	emit GoalColorsChanged(result);
}
