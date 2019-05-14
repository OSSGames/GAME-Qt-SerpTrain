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
 * $LastChangedDate: 2009-03-23 16:06:13 +0100 (Mo, 23. Mär 2009) $
 * $LastChangedRevision: 246 $
*/

#ifndef SEDTLEVELDLG_H
#define SEDTLEVELDLG_H

#include <QtGui/QDialog>
#include "ui_SEdtLevelDlg.h"

class SEdtLevelDlg : public QDialog
{
    Q_OBJECT

public:
    SEdtLevelDlg(QWidget *f_parent, QString f_name, QString f_desc, QString f_author,
    				QString f_contact, int f_amountLayers, QString f_uuid, int f_version, int f_difficulty,
    				bool f_readOnly = false);
    ~SEdtLevelDlg();
    
    QString GetName();
    QString GetDesc();
    QString GetAuthor();
    QString GetContact();
    QString GetUuid();
	int GetDifficulty();
    
private slots:
	void on_saveButton_clicked();
	void on_cancelButton_clicked();
	void on_genUuidButton_clicked();
	
private:
    Ui::SEdtLevelDlgClass ui;
    bool m_readOnly;
};

#endif // SEDTLEVELDLG_H
