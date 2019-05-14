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
 * $LastChangedDate: 2009-03-25 14:11:18 +0100 (Mi, 25. Mär 2009) $
 * $LastChangedRevision: 250 $
*/

#include "SGameBoard.h"
#include "SConfig.h"
#include "SBall.h"
#include "SXMLParse.h"
#include "SBoardTunnel.h"
#include "SLayer.h"
#include "SXMLDoc.h"
#include "VersionInformation.h"
#include "SEdtLevelDlg.h"

#include <iostream>
#include <QMessageBox>
#include <QUuid>
#include <QTextStream>
#include <QFile>
#include <QSettings>
#include <QFileDialog>
#include <QTemporaryFile>
#include <QEventLoop>
#include <QPainter>

/*static */ int SGameBoard::m_nextID = 0;

SGameBoard::SGameBoard(QFile * f_file, enum BOARD_MODE f_boardMode) : m_crashedBall1 ( NULL ),
	m_crashedBall2 ( NULL ), m_layers ( NULL ), m_timestep ( 0 ), m_layersToFinish ( 0 ),
	m_amountLayers ( 0 ), m_file ( f_file ), m_boardMode ( f_boardMode ),
	m_versionNo ( 0 ), m_difficulty ( 0 ), m_restartDialog ( NULL ), m_shouldRestart ( false ),
	m_timerID ( 0 ), m_ID ( m_nextID++ )
{
	throwIfFalse(m_file != NULL);
	
	Init();
}

/*static*/ void SGameBoard::StartLevel(QFile * f_file, enum BOARD_MODE f_boardmode) {
	throwIfFalse(f_file != NULL);
	bool restart;
	do { 
		restart = false;
		try {
			// QFile-Object wird jedesmal neu erstellt, da es Probleme mit internen Dateienamen gab
			// z.B.: ":/levels/tut1.xml"; ToDo: Dies ist unschön!!!
			QFile file(f_file->fileName());
			SGameBoard gameboard(&file, f_boardmode);
		
			QEventLoop eventLoop;
			throwIfFalse(QObject::connect(&gameboard, SIGNAL(EveryWindowClosed()), &eventLoop, SLOT(quit()))); 
			eventLoop.exec();
			
			restart = gameboard.ShouldRestart();
		} 
		catch ( std::exception & e) {
			SException::Report(e);
			restart = false;
		}
	} while ( restart ); 
}

SGameBoard::~SGameBoard()
{
	if ( m_boardMode == MODE_EDITOR ) {
		if ( ! SConfig::LAST_EDITED_LEVEL().isEmpty() ) {
			QFile file(SConfig::LAST_EDITED_LEVEL() );
			SaveLevelToFile(&file);
		}
	}
	
	// Timer stoppen
	if ( m_timerID != 0 )
		killTimer(m_timerID);
	
	// Aufräumen
	for ( int i = 0; i < m_amountLayers; i++ )
		delete m_layers[i];
	
	delete[] m_layers;
	
	delete m_restartDialog;
}

void SGameBoard::Init() {
	SXMLParse root(m_file);
	m_levelName = root.GetAttribute("name", "Untitled level");
	m_levelComment = root.GetAttribute("comment");
	m_versionNo = root.GetIntAttribute("versionNo", 0, 0);
	m_difficulty = root.GetIntAttribute("difficulty", 0, 0, 3);
	m_author = root.GetAttribute("author");
	m_contact = root.GetAttribute("contact");
	
	m_uuid = root.GetAttribute("uuid");
	if ( m_uuid.isEmpty() )
		m_uuid = QUuid::createUuid().toString().remove('{').remove('}');
	
	int formatNo = root.GetIntAttribute("formatNo", 0, 0);
	if ( formatNo > VersionInformation::CurFormatNumber )
		QMessageBox::warning(NULL, "Update recommended", "The level was created with a newer version of this program and therefore <b>could</b> contain elements that this version might not understand. The current version will now load the level as well as possible, but you are strongly recommended to update the programm if you want to use this level.");
	
	m_layersToFinish = m_amountLayers = root.GetIntAttribute("layers", 0, 1);
	
 	if ( m_amountLayers <= 0 )
 		throwOwnEx(SXMLParseException, "The board should not be empty!" ); 
	
	m_layers = new SLayer * [m_amountLayers];
		
	SXMLParse layer = root.GetSubElement();
	for ( int layerNr = 0; layerNr < m_amountLayers; layerNr++ ) {
		layer.ToNextElement("layer");
		m_layers[layerNr] = new SLayer(m_boardMode, m_ID);
		
		InitLayer(m_layers[layerNr], layer.GetLocked());
	}
	
	for ( int layerNr = 0; layerNr < m_amountLayers; layerNr++ ) {
		QString error = m_layers[layerNr]->GetAdditionalInitErrors();
		if ( ! error.isEmpty() )
			root.AddError("LayerNr " + QString::number(layerNr) + " : " + error );
	}
	
	if ( root.GetErrors() > 0 ) {
		QMessageBox::warning ( NULL, "In the File are some errors", "The level contains some errors, but it was loaded as good as possible.\nDetails will be written in File 'error.log'." );
		
		QFile file("error.log");
		if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
			QTextStream out(&file);
			out << root.GetLog();
		}
	}
	
	if ( m_boardMode == MODE_PLAY || m_boardMode == MODE_TEST ) {
		// Statistik schreiben
		if ( m_boardMode == MODE_PLAY ) {
			QSettings settings;
			settings.beginGroup("LevelStatistics");
			int tries = settings.value(m_uuid + "/tries", 0).toInt();
			settings.setValue(m_uuid + "/tries", tries + 1);
			settings.setValue(m_uuid + "/lastTriedVersion", m_versionNo);
		}		
		
 		// Solange Startfenster offen ist, dürfen die Fenster nicht geschlossen werden
 		for ( int i = 0; i < m_amountLayers; i++ )
 			m_layers[i]->SetIsClosable(false);

	    QMessageBox msgBox(QMessageBox::NoIcon, m_levelName, m_levelComment + "<br><br>Press OK to start...");
 		msgBox.setStandardButtons(QMessageBox::Ok);
 		msgBox.setTextFormat(Qt::RichText);
 		msgBox.setWindowModality(Qt::NonModal); // Changing this property while the window is visible has no effect; you must hide() the widget first, then show() it again.
 		msgBox.hide(); msgBox.show();
 		msgBox.exec();
 		
 		// Fenster dürfen wieder geschlossen werden
 		for ( int i = 0; i < m_amountLayers; i++ )
 			m_layers[i]->SetIsClosable(true);
 		 		    
    	m_timerID = startTimer(1000 / SConfig::TIMESTEPS_PER_SECONDS());
    	
	}

   	m_shouldRestart = false;
}
	
void SGameBoard::timerEvent(QTimerEvent *) {
	if ( m_crashedBall1 != NULL ) {
		if ( m_timestep++ % 10 == 0 ){
			m_crashedBall1->ChangeVisibility();
			
			if ( m_crashedBall2 != NULL )
				m_crashedBall2->ChangeVisibility();
		}
	}
	else
	{	
		for ( int layer = 0; layer < m_amountLayers; layer++ )
			m_layers[layer]->Timestep(m_timestep);
			
		m_timestep++;
	}
}

void SGameBoard::BallsCrashed(SBall * f_firstBall, SBall * f_secondBall) {
	throwIfFalse(f_firstBall != NULL );
	
	m_crashedBall1 = f_firstBall;
	m_crashedBall2 = f_secondBall;
	
	if ( f_secondBall != NULL && f_firstBall != NULL ) {
		// Wenn die Bälle zu stark überlappen, dann lasse sie abwechselnd blinken!
		if ( f_secondBall->DistanceToBall(f_firstBall) <= 0.2 * SConfig::RAIL_WIDTH() )
			m_crashedBall2->ChangeVisibility();
	}
	
	delete m_restartDialog;
	m_restartDialog = new QMessageBox(QMessageBox::Warning, "Crash detected...", "Crash detected...\nYou lost!");

	QPushButton * closeButton = m_restartDialog->addButton("&Close Level", QMessageBox::ActionRole);
	QPushButton * restartButton = m_restartDialog->addButton("&Restart Level", QMessageBox::ActionRole);

	m_restartDialog->setDefaultButton(restartButton);
	
	// Icon setzen!
	QPixmap pixmap;
	if ( f_secondBall != NULL )
		pixmap = GetCrashIcon(f_firstBall->GetColor().GetQColor(), f_secondBall->GetColor().GetQColor());
	else
		pixmap = GetCrashIcon(f_firstBall->GetColor().GetQColor());
	
	m_restartDialog->setIconPixmap(pixmap);
	
	throwIfFalse(QObject::connect(restartButton, SIGNAL(clicked ()), this, SLOT(RestartLevel()))); 
	throwIfFalse(QObject::connect(closeButton, SIGNAL(clicked ()), this, SLOT(CloseEveryWindow()))); 

	m_restartDialog->show();
}

QPixmap SGameBoard::GetCrashIcon(QColor f_firstColor, QColor f_secondColor /*= QColor()*/ ) {
	const int pixSize = SConfig::RAIL_WIDTH()*3/2;
	QPixmap pixmap(pixSize*3/2, pixSize);
	
	pixmap.fill(QColor(0,0,0,0)); // Transparent
	QPainter painter(&pixmap);
	painter.setRenderHint(QPainter::Antialiasing);
	
	// Ersten Ball zeichnen
	painter.setBrush( f_firstColor );
	QRect rect = QRect(	( pixSize - SConfig::RAIL_WIDTH() ) / 2,
						( pixSize - SConfig::RAIL_WIDTH() ) / 2,
						SConfig::RAIL_WIDTH(), SConfig::RAIL_WIDTH() );
	painter.drawEllipse( rect );
	if ( f_secondColor.isValid() ) {
		// Zweiten Ball zeichnen
		painter.setBrush( f_secondColor );
		rect = QRect( ( pixSize - SConfig::RAIL_WIDTH() ) / 2 + pixSize / 2,
						( pixSize - SConfig::RAIL_WIDTH() ) / 2,
						SConfig::RAIL_WIDTH(), SConfig::RAIL_WIDTH() );
		painter.drawEllipse( rect );
	} else {
		// Wand zeichnen
		painter.setBrush( Qt::black );
		rect = QRect( ( pixSize - SConfig::RAIL_WIDTH() ) / 2 + SConfig::RAIL_WIDTH(),
						0, SConfig::RAIL_WIDTH() / 2, pixSize );
		painter.drawRect ( rect );
	}
	
	return pixmap;
}


void SGameBoard::LayerFinished() {
	m_layersToFinish--;
	throwIfFalse(m_layersToFinish >= 0);
	if ( m_layersToFinish == 0 && m_boardMode != MODE_EDITOR ) {
		// Statistik schreiben
		if ( m_boardMode == MODE_PLAY ) {
			QSettings settings;
			settings.beginGroup("LevelStatistics");
			int wins = settings.value(m_uuid + "/wins", 0).toInt();
			settings.setValue(m_uuid + "/wins", wins + 1);
		}		
		
		QMessageBox::information ( NULL, "Congratulation!", "Congratulation!\nYou solved the puzzle!" );
	}
}

void SGameBoard::TestLevel() {
	if ( SConfig::SAVE_BEFORE_TEST() )
		SaveLevel();
	
	QTemporaryFile file;
	
	for ( int layerNr = 0; layerNr < m_amountLayers; layerNr++ ) {
		m_layers[layerNr]->SetEnabled(false);
	}
	
	SaveLevelToFile(& file);
	StartLevel(& file, MODE_TEST);
	
	for ( int layerNr = 0; layerNr < m_amountLayers; layerNr++ ) {
		m_layers[layerNr]->SetEnabled(true);
	}
}

void SGameBoard::SaveLevelToFile(QFile * f_file) {
	SXMLDoc doc;
	SXMLElement level = doc.GenerateRootElement("level");
	level.setAttribute("layers", QString::number(m_amountLayers));
	level.setAttribute("name", m_levelName );
	level.setAttribute("comment", m_levelComment );
	level.setAttribute("buildNumber", QString::number(VersionInformation::BuildNumber));
	
	level.setAttribute("author", m_author );
	level.setAttribute("contact", m_contact );
	level.setAttribute("uuid", m_uuid );
	level.setAttribute("versionNo", QString::number(++m_versionNo) );
	level.setAttribute("formatNo", QString::number(VersionInformation::CurFormatNumber) );
	level.setAttribute("difficulty", QString::number(m_difficulty) );

	for ( int layerNr = 0; layerNr < m_amountLayers; layerNr++ ){
		level.appendChild(m_layers[layerNr]->ExportXmlInfo(doc));
	}
	
	doc.WriteToFile(f_file);
}

void SGameBoard::CloseEveryWindow() {
	for ( int layerNr = 0; layerNr < m_amountLayers; layerNr++ ) {
		m_layers[layerNr]->CloseWindow();
	}
	
	emit EveryWindowClosed();
}

void SGameBoard::SaveLevel() {
	QFileInfo fileInfo(*m_file);

	// Prüft, ob die Datei schreibbar ist, und ob sie in SConfig::OWN_LEVEL_DIR() oder einem Unterordner liegt!
	if ( fileInfo.isWritable() && fileInfo.dir().absolutePath().startsWith(QDir(SConfig::OWN_LEVEL_DIR()).absolutePath()))
		SaveLevel(m_file);
	else
		SaveLevelAs();
}

void SGameBoard::SaveLevelAs() {
	QString filename = QFileDialog::getSaveFileName(NULL, QObject::tr("Save Level as ..."),
                             SConfig::OWN_LEVEL_DIR(),
                             QObject::tr("Levels (*.xml)"));
                             
    if ( ! filename.isEmpty() ) {
    	QFileInfo fileInfo(filename);
    	
    	if ( fileInfo.suffix() != "xml" )
    		filename += ".xml";
    		
    	QFile * file = new QFile(filename, this);
    	SaveLevel(file);
    }    	
}

void SGameBoard::SaveLevel(QFile * f_file) {
	throwIfFalse(f_file != NULL);
	
	SaveLevelToFile(f_file);
	if ( f_file != m_file ) {
		m_file = f_file;
	}
}

void SGameBoard::EditLevel() {
	SEdtLevelDlg dialog(NULL, m_levelName, m_levelComment, m_author, m_contact, m_amountLayers, m_uuid, m_versionNo, m_difficulty);

	if ( QDialog::Accepted == dialog.exec() ) {
		m_levelName = dialog.GetName();
		m_levelComment = dialog.GetDesc();
		m_author = dialog.GetAuthor();
		m_contact = dialog.GetContact();
		m_uuid = dialog.GetUuid();
		m_difficulty = dialog.GetDifficulty();
	}
}

void SGameBoard::RestartLevel() {
	m_shouldRestart = true;
	CloseEveryWindow();
}

void SGameBoard::ShowLevel() {
	// Dialog nur read-only starten!
	SEdtLevelDlg dialog(NULL, m_levelName, m_levelComment, m_author, m_contact, m_amountLayers, m_uuid, m_versionNo, m_difficulty, true);

	dialog.exec();
}

       
void SGameBoard::AddLayer() {
	SLayer * * m_layers_old = m_layers;
	
	// Array vergrößern
	m_layersToFinish++; m_amountLayers++;
	m_layers = new SLayer * [m_amountLayers];
	for ( int i = 0; i < m_amountLayers - 1; i++ )
		m_layers[i] = m_layers_old[i];
		
	delete[] m_layers_old; m_layers_old = NULL;
		
	// Neues Layer anlegen
	m_layers[m_amountLayers - 1] = new SLayer(m_boardMode, m_ID); 
	
	QFile file(":/levels/emptyLayer.xml");
	SXMLParse layerXML(&file);
	
	InitLayer(m_layers[m_amountLayers - 1], layerXML);
}

void SGameBoard::InitLayer(SLayer * f_layer, SXMLParse f_layerXML) {
	throwIfFalse(QObject::connect(f_layer, SIGNAL(BallsCrashed(SBall *, SBall *)), this, SLOT(BallsCrashed(SBall *, SBall *)))); 
	throwIfFalse(QObject::connect(f_layer, SIGNAL(LayerFinished()), this, SLOT(LayerFinished ())));
	throwIfFalse(QObject::connect(f_layer, SIGNAL(TestLevel()), this, SLOT(TestLevel ())));
	throwIfFalse(QObject::connect(f_layer, SIGNAL(SaveLevel()), this, SLOT(SaveLevel ())));
	throwIfFalse(QObject::connect(f_layer, SIGNAL(SaveLevelAs()), this, SLOT(SaveLevelAs ())));
	throwIfFalse(QObject::connect(f_layer, SIGNAL(WindowClosed()), this, SLOT(CloseEveryWindow())));
	throwIfFalse(QObject::connect(f_layer, SIGNAL(EditLevel()), this, SLOT(EditLevel())));
	throwIfFalse(QObject::connect(f_layer, SIGNAL(AddLayer()), this, SLOT(AddLayer())));
	throwIfFalse(QObject::connect(f_layer, SIGNAL(DeleteThisLayer(SLayer *)), this, SLOT(DeleteThisLayer(SLayer *))));
	throwIfFalse(QObject::connect(f_layer, SIGNAL(RestartLevel()), this, SLOT(RestartLevel())));
	throwIfFalse(QObject::connect(f_layer, SIGNAL(ShowLevel()), this, SLOT(ShowLevel())));
	
	f_layer->InitByXML(f_layerXML);
}        

void SGameBoard::DeleteThisLayer(SLayer * f_layer) {
	if ( m_amountLayers <= 1 )
		QMessageBox::information(f_layer->Widget(), "Layer cannot be removed", "The last layer cannot be removed. If you really need to remove it, you can simply delete the level file.");
	else {
		if ( QMessageBox::Yes == QMessageBox::question(f_layer->Widget(), "Remove Layer?", "Do you really want to remove the layer? This action is unrevokable.", QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) ) {
			// Layer entfernen
			m_amountLayers--; // ToDo: Eigentlich müsste hier auch noch m_layersToFinish angepasst werden
			SLayer * * oldLayers = m_layers;
			m_layers = new SLayer * [m_amountLayers];
			
			int n, o;  // n-new, o-old
			for ( o = 0, n = 0; o <= m_amountLayers; o++ ) {
				if ( oldLayers[o] == f_layer ) {
					f_layer->deleteLater();
					f_layer = NULL;
				}
				else {
					throwIfFalse(n < m_amountLayers); 
					m_layers[n++] = oldLayers[o];
				}
			}
			
			throwIfFalse(n == m_amountLayers);
			
			delete[] oldLayers;
		}
	}
}

