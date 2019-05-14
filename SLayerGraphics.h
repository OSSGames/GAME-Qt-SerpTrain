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

#ifndef SLAYERGRAPHICS_H_
#define SLAYERGRAPHICS_H_

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QAction>

#include "enum.h"

class SBoardTile;

class SLayerGraphics : public QMainWindow
{
	Q_OBJECT
	
public:
	SLayerGraphics(bool f_tunnelLayer, enum BOARD_MODE f_boardMode);
	void Show();	
	void AddToScene(SBoardTile * f_boardtile);
	void SetIsTunnelLayer(bool f_tunnelLayer);
	void AdjustLayerSize();
	void SetIsClosable(bool f_closable);
		
signals:
	void WindowClosed();
	void TestLevel();       // Es soll der aktuelle Level im Test-Mode geöffnet werden
	void SaveLevel();	    // Es soll der aktuelle Level gespeichert werden
	void SaveLevelAs();     // Es soll der aktuelle Level unter neuem Namen gespeichert werden
	void EditLayer();       
	void ChangeLayerSize(); 
	void EditLevel();       
	void AddLayer();        
	void DelLayer();
	void RestartLevel();
	void ShowLevel();        
		
private slots:
	void menu_qtInfo();
	void menu_licenseInfo();
	void menu_versionInfo();
	void menu_testLevel();
	void menu_saveLevel();
	void menu_saveLevelAs();
	void menu_edtLayer();
	void menu_chgLayerSize();
	void menu_edtLevel();
	void menu_addLayer();
	void menu_delLayer();
	void menu_restartLevel();
	void menu_showLevel();

protected:
	void closeEvent(QCloseEvent *event);
	void AddMenu();
	
	QGraphicsScene m_scene;
	QRadialGradient m_gradient;
	QGraphicsView m_view;

	QAction m_licenseInfo;
	QAction m_versionInfo;
	QAction m_qtInfo;
	QAction m_testLevel;
	QAction m_saveLevel;
	QAction m_saveLevelAs;
	QAction m_edtLayer;
	QAction m_chgLayerSize;
	QAction m_edtLevel;
	QAction m_addLayer;
	QAction m_delLayer;
	QAction m_restartLevel;
	QAction m_showLevel;

	enum BOARD_MODE m_boardMode;
	bool m_isClosable;
};

#endif /*SLAYERGRAPHICS_H_*/
