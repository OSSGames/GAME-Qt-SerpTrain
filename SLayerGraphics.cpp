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

#include "SLayerGraphics.h"
#include "SConfig.h"
#include "VersionInformation.h"
#include "SBoardTile.h"
#include "SException.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QMenuBar>
#include <QMenu>
#include <math.h>

SLayerGraphics::SLayerGraphics(bool f_tunnelLayer, enum BOARD_MODE f_boardMode) : m_view ( &m_scene, this ), m_licenseInfo (tr("&License"), this),
	m_versionInfo (tr("&Version"), this), m_qtInfo (tr("About &Qt"), this),  m_testLevel (tr("&Test Level"), this), m_saveLevel ( tr("&Save Level"), this),
	m_saveLevelAs ( tr("Save Level &as ..."), this ), m_edtLayer ( tr("&Edit Layer Properties"), this), m_chgLayerSize ( tr("&Change Layer Size"), this),
	m_edtLevel ( tr("Edit &Level Properties"), this), m_addLayer(tr("Add &New Layer"), this), m_delLayer(tr("&Delete Current Layer"), this), 
	m_restartLevel ( tr("&Restart Level"), this), m_showLevel ( tr("Show Properties"), this), 
	m_boardMode ( f_boardMode ), m_isClosable ( true )
{
	AddMenu();
	
	SetIsTunnelLayer(f_tunnelLayer);

    m_view.setRenderHint(QPainter::Antialiasing);
}

void SLayerGraphics::closeEvent(QCloseEvent *f_event) {
	if ( m_isClosable ) {
		emit WindowClosed();
		f_event->accept();
	} else
		f_event->ignore();
}

void SLayerGraphics::AddMenu() {
	QMenu * infoMenu = this->menuBar()->addMenu(tr("&Information"));

	connect(&m_licenseInfo, SIGNAL(triggered()), this, SLOT(menu_licenseInfo()));
	infoMenu->addAction(&m_licenseInfo);

	connect(&m_versionInfo, SIGNAL(triggered()), this, SLOT(menu_versionInfo()));
	infoMenu->addAction(&m_versionInfo);

//	m_qtInfo.setShortcut(tr("Ctrl+Q"));
//	m_qtInfo.setStatusTip(tr("Show Qt-Info"));
	connect(&m_qtInfo, SIGNAL(triggered()), this, SLOT(menu_qtInfo()));
	infoMenu->addAction(&m_qtInfo);
	
	if ( m_boardMode == MODE_PLAY || m_boardMode == MODE_TEST ) {
		QMenu * levelMenu = this->menuBar()->addMenu(tr("&Level"));
	
		m_restartLevel.setShortcut(tr("Ctrl+R"));
		connect(&m_restartLevel, SIGNAL(triggered()), this, SLOT(menu_restartLevel()));
		levelMenu->addAction(&m_restartLevel);
		
		connect(&m_showLevel, SIGNAL(triggered()), this, SLOT(menu_showLevel()));
		levelMenu->addAction(&m_showLevel);
	} 
	
	if ( m_boardMode == MODE_EDITOR ) {
		QMenu * editorMenu = this->menuBar()->addMenu(tr("&Editor"));
	
		m_testLevel.setShortcut(tr("Ctrl+T"));
		connect(&m_testLevel, SIGNAL(triggered()), this, SLOT(menu_testLevel()));
		editorMenu->addAction(&m_testLevel);

		m_saveLevel.setShortcut(tr("Ctrl+S"));
		connect(&m_saveLevel, SIGNAL(triggered()), this, SLOT(menu_saveLevel()));
		editorMenu->addAction(&m_saveLevel);

		m_saveLevelAs.setShortcut(tr("Ctrl+Shift+S"));
		connect(&m_saveLevelAs, SIGNAL(triggered()), this, SLOT(menu_saveLevelAs()));
		editorMenu->addAction(&m_saveLevelAs);
		
		editorMenu->addSeparator();
		
		connect(&m_edtLayer, SIGNAL(triggered()), this, SLOT(menu_edtLayer()));
		editorMenu->addAction(&m_edtLayer);

		connect(&m_edtLevel, SIGNAL(triggered()), this, SLOT(menu_edtLevel()));
		editorMenu->addAction(&m_edtLevel);

		editorMenu->addSeparator();

		connect(&m_chgLayerSize, SIGNAL(triggered()), this, SLOT(menu_chgLayerSize()));
		editorMenu->addAction(&m_chgLayerSize);

		connect(&m_addLayer, SIGNAL(triggered()), this, SLOT(menu_addLayer()));
		editorMenu->addAction(&m_addLayer);

		connect(&m_delLayer, SIGNAL(triggered()), this, SLOT(menu_delLayer()));
		editorMenu->addAction(&m_delLayer);
	}
}

void SLayerGraphics::menu_qtInfo() {
	QMessageBox::aboutQt(this);
}

void SLayerGraphics::menu_versionInfo() {
	QString title = "Version Information";
	QString text = "SerpTrain Version Information:"
    "\n"
	"\nVersion:    \t" + VersionInformation::VersionNumber;
	
	if ( VersionInformation::VersionNumber.endsWith( "1" ) || VersionInformation::VersionNumber.endsWith( "3" ) || VersionInformation::VersionNumber.endsWith( "5" ) ||
		 VersionInformation::VersionNumber.endsWith( "7" ) || VersionInformation::VersionNumber.endsWith( "9" ) )
	{
		text += " (Developer Version)";
	}  
	
	text += "\nBuild:      \t" + QString::number(VersionInformation::BuildNumber) + 
			"\nCompiled at:\t" + VersionInformation::CompileTime;
    
	QMessageBox::information(this, title, text);
}

void SLayerGraphics::menu_licenseInfo() {
	QString title = "License Information";
	QString text = "SerpTrain (a game with balls on rails)"
    "\nCopyright (C) 2008, 2009  Michael \"Serpedon\" Walz (http://serpedon.de)"
	"\n"
    "\nThis program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License version 2 or version 3 as published by the Free Software Foundation."
	"\n"
    "\nThis program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details."
	"\n"
 	"\nYou should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110, USA";
    
	QMessageBox::information(this, title, text);
}

void SLayerGraphics::menu_testLevel() {
	emit TestLevel();
}

void SLayerGraphics::menu_saveLevel() {
	emit SaveLevel();
}

void SLayerGraphics::menu_saveLevelAs() {
	emit SaveLevelAs();
}

void SLayerGraphics::menu_edtLayer() {
	emit EditLayer();       
}

void SLayerGraphics::menu_chgLayerSize() {
	emit ChangeLayerSize(); 
}

void SLayerGraphics::menu_edtLevel() {
	emit EditLevel();       
}

void SLayerGraphics::menu_addLayer() {
	emit AddLayer();        
}

void SLayerGraphics::menu_delLayer() {
	emit DelLayer();
}

void SLayerGraphics::menu_restartLevel() {
	emit RestartLevel();
}

void SLayerGraphics::menu_showLevel() {
	emit ShowLevel();
}

void SLayerGraphics::Show() {
	this->setCentralWidget(&m_view);
	AdjustLayerSize();

	this->show();
	this->move(100,100); //ToDo: Überlegen, wie das Spiel genau in die Mitte plaziert werden kann! Oder vllt noch besser, bei verschiedenen Layer irgendwie nebeneinander!
	m_view.show();
}

void SLayerGraphics::AdjustLayerSize() {
	m_scene.setSceneRect(m_scene.itemsBoundingRect());
	QRectF sceneRect = m_scene.sceneRect();
	QRect geometry = QRect((int) sceneRect.x()-SConfig::FIELD_SIZE(), (int) sceneRect.y()-SConfig::FIELD_SIZE(), (int) sceneRect.width()+2*SConfig::FIELD_SIZE(), (int) sceneRect.height()+2*SConfig::FIELD_SIZE());

	qreal radius = (sqrt(pow(geometry.width()/2.0,2)+pow(geometry.height()/2.0,2)));
	m_gradient.setCenter(geometry.center());
	m_gradient.setFocalPoint(geometry.center());
	// +0.5 addieren, damit es am Rand nicht abrupt endet
	m_gradient.setRadius(radius+0.5);
	m_scene.setBackgroundBrush(m_gradient);
	
	this->resize(geometry.size());
	this->setMinimumSize(geometry.size());
	this->setMaximumSize(geometry.size());
}

void SLayerGraphics::AddToScene(SBoardTile * f_boardtile) {
	throwIfFalse(f_boardtile != NULL);
	
	f_boardtile->AddToScene(&m_scene);
}

void SLayerGraphics::SetIsTunnelLayer(bool f_tunnelLayer) {
	if ( f_tunnelLayer ) {
    	m_gradient.setColorAt(0, SConfig::LAYER_INACTIVE_1());
    	m_gradient.setColorAt(1, SConfig::LAYER_INACTIVE_2());
	} else {
    	m_gradient.setColorAt(0, SConfig::LAYER_ACTIVE_1());
    	m_gradient.setColorAt(1, SConfig::LAYER_ACTIVE_2());
    }
	m_scene.setBackgroundBrush(m_gradient);
}

void SLayerGraphics::SetIsClosable(bool f_closable) {
	m_isClosable = f_closable;
}