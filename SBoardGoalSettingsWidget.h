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

#ifndef SBOARDGOALSETTINGSWIDGET_H
#define SBOARDGOALSETTINGSWIDGET_H

#include <QtGui/QWidget>
#include <QRadioButton>
#include "ui_SBoardGoalSettingsWidget.h"
#include "SBoardGoal.h"

class SBoardGoalSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    SBoardGoalSettingsWidget(bool f_continueFlag, bool f_showAllFlag);
    ~SBoardGoalSettingsWidget();

	void SetGoalFields(SArray<SColor, SBoardGoal::AmountFields> f_goalColors);
		
signals:
	void GoalColorsChanged(SArray<SColor, SBoardGoal::AmountFields>);
	void ContinueFlagChanged(bool f_continueFlag);
	void ShowAllFlagChanged(bool f_showAllFlag);
    
private slots:
	void radioButtons_clicked();
	void on_checkBoxConnected_toggled(bool f_newValue);
	void on_checkBoxShowAll_toggled(bool f_newValue);

private:
    enum COLOR GetCheckedColor(int f_fieldNr);
    void UpdateColors();
	void SetFieldColor(int f_colorNr, enum COLOR); 
    
    Ui::SBoardGoalSettingsWidgetClass ui;
    
    QHash<enum COLOR, QRadioButton *> m_radioButtons[SBoardGoal::AmountFields];
};

#endif // SBOARDGOALSETTINGSWIDGET_H
