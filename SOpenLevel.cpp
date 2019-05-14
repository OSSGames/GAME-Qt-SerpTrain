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
 * $LastChangedDate: 2009-03-10 12:23:02 +0100 (Di, 10. Mär 2009) $
 * $LastChangedRevision: 238 $
*/

#include "SOpenLevel.h"
#include "SConfig.h"
#include "SHttpGet.h"
#include "VersionInformation.h"
#include "SXMLParse.h"
#include <QMessageBox>
#include <QTextStream>
#include <QSettings>

#define COL_DIFF 0
#define COL_NAME 1
#define COL_TRIED 2
#define COL_SOLVED 3

const int FilenameRole = Qt::UserRole;
SOpenLevel::SOpenLevel(QWidget *parent)
    : QDialog(parent), m_http ( NULL )
{
	QDir dir;
	dir.mkpath(SConfig::LEVEL_DIR() );

	ui.setupUi(this);
	
	UpdateLevels();
}

SOpenLevel::~SOpenLevel()
{

}

void SOpenLevel::on_playButton_clicked() {
	int curRow = ui.listLevels->currentRow();
	QTableWidgetItem * curItem = ui.listLevels->item(curRow, COL_NAME); 
	if ( curItem != NULL ) {
		m_result = curItem->data(FilenameRole).toString();
		if ( ! m_result.isEmpty() )
			QDialog::accept();
	}
}

void SOpenLevel::on_backButton_clicked() {
	QDialog::reject();
}

void SOpenLevel::on_updateButton_clicked() {
	if ( m_http != NULL ) 
		QMessageBox::information(this, "Already updating...", "The update process is already started.");
	else {
		m_http = new SHttpGet(SConfig::UPDATE_LEVEL_URL());
		
		throwIfFalse(QObject::connect(m_http, SIGNAL(TransferFinished()), this, SLOT(HttpMainTransferFinished())));
		throwIfFalse(QObject::connect(m_http, SIGNAL(Error(QString)), this, SLOT(HttpError(QString))));
				
		m_http->Start();
	}
}

void SOpenLevel::HttpMainTransferFinished() {
	// Gerade wurde die SConfig::UPDATE_LEVEL_URL() heruntergeladen
	try {
		// toDo: toAssci ist unsauber
		SXMLParse xmlRoot(m_http->GetResult().toAscii());
		
		m_http->deleteLater();
		m_http = NULL;
		
		int formatNo = xmlRoot.GetIntAttribute("formatNo", 0, 0);
		if ( formatNo > VersionInformation::CurUpdateFormatNumber )
			QMessageBox::warning(NULL, "Update recommended", "The update file was created for a newer version of this program and therefore <b>could</b> contain elements that this version might not understand. The current version will try to update as well as possible, but you are strongly recommended to update the programm if you want to use the update function.");

		SXMLParse xmlLevel = xmlRoot.GetSubElement();
		while ( xmlLevel.HasNextElement("level") ) {
			xmlLevel.ToNextElement("level");
			
			SUpdateLevelData levelData;
			levelData.m_url = xmlLevel.GetAttribute("filename");
			levelData.m_uuid = xmlLevel.GetAttribute("uuid");
			levelData.m_versionNo = xmlLevel.GetIntAttribute("versionNo", 0);
			
			m_updateLevelData << levelData;
		}
	
		LoadNextLevel();
	}
	catch ( SXMLParseException & e) {
		HttpError( e.what() );
	}
}

void SOpenLevel::HttpLevelTransferFinished() {
	// Es wurde gerade der Level m_updateLevelData.head().m_uuid heruntergeladen
	throwIfFalse( m_updateLevelData.count() >= 1 );
	
	SXMLParse xmlParse(m_http->GetResult().toAscii());
	throwIfFalse ( xmlParse.GetAttribute("uuid") == m_updateLevelData.head().m_uuid );
	
	QString localFilename = m_updateLevelData.head().GetLocalFilename();
	QFile file(localFilename);
 	if ( file.open(QIODevice::WriteOnly | QIODevice::Text) ) { 
		QTextStream out(&file);
		out << m_http->GetResult();

	  	file.close();
	 	
		m_http->deleteLater();
		m_http = NULL;

	 	m_updateLevelData.dequeue();
		LoadNextLevel();
 	} else 
 		HttpError("The file '" + localFilename + "' could not be opened.");
 }

void SOpenLevel::HttpError(QString f_error) {
	throwIfFalse( m_http != NULL );
	QMessageBox::critical(this, "Updated failed", "The update process failed because of the following error:\n" + f_error);
	
	HttpRemoveTrash();
}

void SOpenLevel::HttpRemoveTrash() {
	if ( m_http ) {
		m_http->deleteLater();
		m_http = NULL;
	}
	
	m_updateLevelData.clear();
}

void SOpenLevel::LoadNextLevel() {
	if ( m_updateLevelData.count() >= 1 ) {
		bool noUpdateNeeded = false;
		QString localFilename = m_updateLevelData.head().GetLocalFilename();
		if ( QFile::exists( localFilename ) ) {
			try {
				QFile file(localFilename);
				SXMLParse oldLevel(&file);
				if ( oldLevel.GetIntAttribute("versionNo", 0) >= m_updateLevelData.head().m_versionNo )
					noUpdateNeeded = true;
			}
			catch ( SXMLParseException ) 
			{ } // Bei einem Parsefehler soll die alte Datei einfach überschrieben werden
		}
			
		if ( noUpdateNeeded ) {
			m_updateLevelData.dequeue();
			LoadNextLevel();
		}
		else {
			throwIfFalse(m_http == NULL);
			
			m_http = new SHttpGet(m_updateLevelData.head().m_url);
			throwIfFalse(QObject::connect(m_http, SIGNAL(TransferFinished()), this, SLOT(HttpLevelTransferFinished())));
			throwIfFalse(QObject::connect(m_http, SIGNAL(Error(QString)), this, SLOT(HttpError(QString))));
			m_http->Start();
		}
	} else {
		HttpRemoveTrash();
		UpdateLevels();
		QMessageBox::information(this, "Update finished", "The update process is finished." );
	}
}

QString SUpdateLevelData::GetLocalFilename() {
	return SConfig::LEVEL_DIR() + "/" + m_uuid + ".xml";
}

void SOpenLevel::UpdateLevels() {
	ui.listLevels->setSortingEnabled( false );

	ui.listLevels->setRowCount(0);
	
	QDir dir(SConfig::LEVEL_DIR());
	dir.setFilter(QDir::Files | QDir::Hidden );

	QFileInfoList list = dir.entryInfoList();
	for (int i = 0; i < list.size(); ++i) {
		QFileInfo fileInfo = list.at(i);
		
		QSettings settings;
		settings.beginGroup("LevelStatistics");
		
		try {
			QFile file(fileInfo.filePath());
			SXMLParse parse(&file);
			if ( parse.GetTagName() == "level" ) {
				QString name = parse.GetAttribute("name");
				QString uuid = parse.GetAttribute("uuid");
				int difficulty = parse.GetIntAttribute("difficulty", 0, 0, 3);
				int version = parse.GetIntAttribute("versionNo", 0, 0);
				if ( ! name.isEmpty() && ! uuid.isEmpty() ) {
					// Zeile hinzufügen
					int newRow = ui.listLevels->rowCount();
					ui.listLevels->setRowCount(newRow+1);
					
					int tries = settings.value(uuid + "/tries", 0).toInt();
					int wins = settings.value(uuid + "/wins", 0).toInt();
					int lastTriedVersion = settings.value(uuid + "/lastTriedVersion", 0).toInt();

					QTableWidgetItem * newDiff = new QTableWidgetItem(QIcon(":/icons/" + QString::number(difficulty) + "Stars.png"), QString(difficulty, ' '));
					QTableWidgetItem * newName = new QTableWidgetItem(name);
					QTableWidgetItem * newTried = new QTableWidgetItem(tries > 0 ? QIcon(":/icons/check.png"): QIcon(), "");
					QTableWidgetItem * newSolved = new QTableWidgetItem(wins > 0 ? QIcon(":/icons/check.png"): QIcon(), "");
					
					newName->setData(FilenameRole, fileInfo.filePath());
					if ( version > lastTriedVersion ) {
						QFont font = newName->font();
						font.setBold(true);
						newName->setFont(font);
					}
					
					ui.listLevels->setItem(newRow, COL_DIFF, newDiff);
					ui.listLevels->setItem(newRow, COL_NAME, newName);
					ui.listLevels->setItem(newRow, COL_TRIED, newTried);
					ui.listLevels->setItem(newRow, COL_SOLVED, newSolved);
				}
			}
		}
		catch ( SXMLParseException ) {
		}
	}


	ui.listLevels->resizeColumnsToContents();
	ui.listLevels->resizeRowsToContents();

	ui.listLevels->sortItems(COL_NAME);
	ui.listLevels->sortItems(COL_DIFF);
}
