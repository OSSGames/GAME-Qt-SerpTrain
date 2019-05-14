/* SerpTrain (a game with balls on rails)
 * Copyright (C) 2008, 2009  Michael \"Serpedon\" Walz (http://serpedon.de)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even  the implied warranty of
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

#include "SEdtSettingsDlg.h"
#include "SException.h"
#include "SConfig.h"
#include "SBoardTile.h"
#include <QMessageBox>
#include <QtGui>

SEdtSettingsDlg::SEdtSettingsDlg(SBoardTile * f_boardTile) : 
  m_boardTile ( f_boardTile )
{ 
	throwIfFalse(m_boardTile != NULL);
	
	// Fenster aufbauen
	m_view.setScene(&m_scene);
	
	m_scene.setSceneRect(QRectF()); // If set to a null QRectF, sceneRect() will return the largest bounding rect of all items on the scene since the scene was created.  
	
	m_scene.setBackgroundBrush(SConfig::LAYER_ACTIVE_1());
	m_view.setRenderHint(QPainter::Antialiasing);
	
	// Boardtile in die Mitte setzen
	m_boardTile->SetPosition(QPoint(0,0));
	m_boardTile->AddToScene(&m_scene);
	
	// Buttons erstellen
	QPushButton *saveButton = new QPushButton("&Save settings", this);
	QPushButton *cancelButton = new QPushButton("&Cancel", this);
	
	throwIfFalse(QObject::connect(cancelButton, SIGNAL(pressed()), this, SLOT(reject()))); 
	throwIfFalse(QObject::connect(saveButton,   SIGNAL(pressed()), this, SLOT(accept()))); 
	
	// Layout festlegen
	QHBoxLayout *mainLayout;
	QVBoxLayout *vboxLayout;
    QSpacerItem *spacerItem;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem1;
    QSpacerItem *spacerItem2;
    QSpacerItem *spacerItem3;

	// Hauptlayout legt links die Farben und rechts das CustumSettingsWidget hin
	mainLayout = new QHBoxLayout(this);

    vboxLayout = new QVBoxLayout();
    mainLayout->addLayout(vboxLayout);
    
    QWidget * customWidget = m_boardTile->CreateCustumSettingsWidget();
    if ( customWidget != NULL )
    	mainLayout->addWidget( customWidget );	
       
    vboxLayout->addWidget(&m_view);
    
    // Die Abstandshalter richtig setzen
    spacerItem = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    //vboxLayout->addItem(spacerItem);

    hboxLayout = new QHBoxLayout();
    vboxLayout->addLayout(hboxLayout);
    
    spacerItem1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    hboxLayout->addItem(spacerItem1);

	// Auf Windowssystemen, soll der Save-Button links sein!
#ifdef Q_WS_WIN
	hboxLayout->addWidget(saveButton);
#else
    hboxLayout->addWidget(cancelButton);
#endif

    spacerItem2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    hboxLayout->addItem(spacerItem2);

#ifdef Q_WS_WIN
    hboxLayout->addWidget(cancelButton);
#else
	hboxLayout->addWidget(saveButton);
#endif

    spacerItem3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    hboxLayout->addItem(spacerItem3);
    
    // Sicherstellen, dass die gesamte view sichtbar ist
    // +5 zu Höhe und Breite, damit keine Scrollbalken entstehen, mit denen die Größe dann nicht mehr passt.
    m_view.setMinimumSize(m_view.sceneRect().toAlignedRect().size()+QSize(5,5));

    // Den Dialog auf die minimale Größe schrumpfen
    resize(10,10);
}

SEdtSettingsDlg::~SEdtSettingsDlg()
{
	delete m_boardTile;
	m_boardTile = NULL;
}
