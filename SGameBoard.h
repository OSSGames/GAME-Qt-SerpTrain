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
 * $LastChangedDate: 2009-03-23 18:56:45 +0100 (Mo, 23. Mär 2009) $
 * $LastChangedRevision: 248 $
*/

#ifndef SGAMEBOARD_H_
#define SGAMEBOARD_H_

#include "enum.h"
#include <QObject>
#include <QPixmap>
#include <QFile>

class SBoardTunnel;
class SLayer;
class SBall;
class SXMLParse;
class QMessageBox;

class SGameBoard : public QObject
{
	Q_OBJECT
	
public:
	static void StartLevel(QFile * f_file, enum BOARD_MODE);
		
	// Es dürfen keine Objekte von Typ SGameBoard erstellt werden
	// Stattdessen sollte die Funktion "StartLevel" aufgerufen werden
private:
	SGameBoard(QFile * f_file, enum BOARD_MODE);
	virtual ~SGameBoard();
	
signals:
	void EveryWindowClosed();
	
private slots:
	void BallsCrashed(SBall * f_firstBall, SBall * f_secondBall); // f_secondBall kann NULL sein!
	void LayerFinished();
	void CloseEveryWindow();
	
	//Slots für Events, die von SLayer hochgetragen werden
	void TestLevel();
	void SaveLevel();
	void SaveLevel(QFile * f_file);
	void SaveLevelAs();
	void EditLevel();       
	void AddLayer();
	void DeleteThisLayer(SLayer * f_layer);
	void RestartLevel();
	void ShowLevel();        
		
protected:
	bool ShouldRestart() { return m_shouldRestart; }

private:
	void Init();
	void SaveLevelToFile(QFile * f_file);
	void InitLayer(SLayer * f_layer, SXMLParse f_layerXML);
	QPixmap GetCrashIcon(QColor f_firstColor, QColor f_secondColor = QColor() );

    void timerEvent(QTimerEvent *event);
    
	SBall * m_crashedBall1;
	SBall * m_crashedBall2;
	
	SLayer * * m_layers;

	int m_timestep;
	int m_layersToFinish;
	int m_amountLayers;
	
	QFile * m_file;
	enum BOARD_MODE m_boardMode;
	
	QString m_levelName;
	QString m_levelComment;
	QString m_uuid;
	QString m_author;
	QString m_contact;
	int m_versionNo;
	int m_difficulty;
	
	QMessageBox * m_restartDialog;
	bool m_shouldRestart;
	
	int m_timerID;
	int m_ID;
	static int m_nextID;
};

#endif /*SGAMEBOARD_H_*/
