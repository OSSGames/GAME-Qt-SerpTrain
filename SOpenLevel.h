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

#ifndef SOPENLEVEL_H
#define SOPENLEVEL_H

#include <QtGui/QDialog>
#include "ui_SOpenLevel.h"

#include <QQueue>

class SHttpGet;
class SXMLParse;

class SUpdateLevelData {
	public:
		SUpdateLevelData() : m_versionNo ( -1 ) {};
		
		QString m_url;
		int m_versionNo;
		QString m_uuid;
		
		QString GetLocalFilename();
};

class SOpenLevel : public QDialog
{
    Q_OBJECT

public:
    SOpenLevel(QWidget *parent = 0);
    QString GetLevel() { return m_result; }
    ~SOpenLevel();
  
private slots:
	void on_playButton_clicked();
	void on_backButton_clicked();
	void on_updateButton_clicked();
	void HttpMainTransferFinished();
	void HttpLevelTransferFinished();
	void HttpError(QString f_error);
	
private:
	void HttpRemoveTrash();
	void LoadNextLevel();
	void UpdateLevels();

private:
    Ui::SOpenLevelClass ui;
    QString m_result;
    
    SHttpGet * m_http;
    QQueue<SUpdateLevelData> m_updateLevelData;
};

#endif // SOPENLEVEL_H
