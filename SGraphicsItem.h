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

#ifndef SGRAPHICSITEM_H_
#define SGRAPHICSITEM_H_

#include <QGraphicsItem>
#include <QGraphicsScene>

#include "enum.h"

class SGraphicsItem : public QObject, protected QGraphicsItem
{
	Q_OBJECT
	
public:
	SGraphicsItem( enum Z_MAJOR );
	virtual ~SGraphicsItem();
	
	void SetZMajor( enum Z_MAJOR );
	void SetZMinor( enum Z_MINOR );
	void Update();
	void AddToScene( QGraphicsScene * );
	void RemoveFromScene();
	
	static QPointF TransformRelPosition(QPointF f_relPos, enum ROTATION f_rotation, enum MIRROR f_mirror);
		
	void ChangeVisibility();
	void SetVisibility(bool f_visible);
	
	// Der Mittelpunkt des Objekts soll auf f_Pos gesetzt werden und von da aus mit f_relPos verschoben werden
	// (Dabei soll ROTATION und MIROR beachtet werden)
	void SetRelativePosition(QPoint f_Pos, QPointF f_relPos, enum ROTATION, enum MIRROR);
	
	// Legt die Orientierung fest
	void virtual SetTransform(enum ROTATION, enum MIRROR) = 0;
	
	void SetAcceptedMouseButtons ( Qt::MouseButtons buttons );
	
protected:
	// Legt die Position fest (muss überschrieben werden, wenn der Ursprung nicht
	// die Mitte des Koordinatensystem betrifft 
	void virtual SetPosition(QPointF f_pos);

    virtual void mousePressEvent ( QGraphicsSceneMouseEvent * event );

private:
	enum Z_MAJOR m_major;
	
signals:
	void MousePressed( QGraphicsSceneMouseEvent * );
	void MouseLeftClick();
};

#endif /*SGRAPHICSITEM_H_*/
