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

#ifndef SLAYER_H_
#define SLAYER_H_

#include <QObject>
#include <QSize>
#include "enum.h"
#include "SXMLParse.h"
#include "SXMLDoc.h"
#include "SBoardTile.h"

class SBoardTunnel;
class SLayerGraphics;

class SLayer : public QObject
{
	Q_OBJECT
	
public:
	SLayer(enum BOARD_MODE, int f_gameboardID);
	virtual ~SLayer();
	
	void InitByXML(SXMLParse f_xmlparse);
		
	void Timestep(int f_timestep);
	
	SXMLElement ExportXmlInfo(SXMLDoc f_xmlDoc);
	QString GetAdditionalInitErrors();
	void SetIsClosable(bool f_closable);
	
	void SetEnabled(bool f_enable);
	QWidget * Widget();
	
	void CloseWindow();
	
private slots:
	void GoalFinished();
	
	// Slot wird aufgerufen, wenn ein Tile im Editor gechanged werden soll
	void ReplaceTileWith(SBoardTile * f_old, SBoardTile * f_new);
		
	// Schnittstellen von SLayerGraphics
	void EditLayer();       
	void ChangeLayerSize(); 
	void DelLayer();        

signals:
	void LayerFinished();
	void BallsCrashed(SBall * f_firstBall, SBall * f_secondBall); // f_secondBall kann NULL sein!
	
	void DeleteThisLayer(SLayer * f_layer);

	// Schnittstellen zwischen SGameBoard und SLayerGraphics
	void TestLevel(); // Es soll der aktuelle Level im Test-Mode geöffnet werden
	void SaveLevel();
	void SaveLevelAs();
	void EditLevel();       
	void AddLayer();        
	void RestartLevel();
	void ShowLevel();        

	void WindowClosed();
	
private:
    int AdjCol(int f_col);
    int AdjRow(int f_row);

	void ReplaceTileWith(SBoardTile * f_new, int f_row, int f_col);

	// Überprüft, ob die Punkte von f_tile nach rechts mit f_right oder nach unten mit f_bottom kollidieren!
	void CheckCrashs(const QList<SCrashDetection> & f_tile, const QList<SCrashDetection> & f_right, const QList<SCrashDetection> & f_bottom );

	bool GetTilePos(SBoardTile * f_tile, int * f_row, int * f_col);
	
	SBoardTile * * * m_boardTiles; // m_boardTiles[col][row]
	QSize m_layerSize;
	SLayerGraphics * m_mainWindow;

	int m_goalToFinish;
	bool m_isGraphical;
	bool m_invisible; // Gibt an, ob es sich um ein "Tunnel"-Layer handelt
	
	bool m_hasBeenInit;

	enum BOARD_MODE m_boardMode;
	
	int m_ID;
	int m_gameboardID;
	
	static int m_nextID;
};

#endif /*SLAYER_H_*/
