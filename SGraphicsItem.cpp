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

#include "SGraphicsItem.h"
#include "SException.h"
#include <QGraphicsSceneMouseEvent>

SGraphicsItem::SGraphicsItem( enum Z_MAJOR f_major )
{
	SetZMajor(f_major);
	SetAcceptedMouseButtons(Qt::NoButton);
}

SGraphicsItem::~SGraphicsItem()
{
}

void SGraphicsItem::SetZMajor( enum Z_MAJOR f_major) {
	m_major = f_major;
	setZValue(m_major);
}

void SGraphicsItem::SetZMinor( enum Z_MINOR f_minor) {
	setZValue(m_major + f_minor);
}

void SGraphicsItem::Update(){
	QGraphicsItem::update(boundingRect());
}

void SGraphicsItem::AddToScene( QGraphicsScene * f_scene) {
	f_scene->addItem ( this );	
}

void SGraphicsItem::RemoveFromScene() {
	QGraphicsScene * scene = QGraphicsItem::scene();
	throwIfFalse( scene != NULL );
	scene->removeItem ( this );
}

void SGraphicsItem::SetRelativePosition(QPoint f_Pos, QPointF f_relPos, enum ROTATION f_rotation, enum MIRROR f_mirror) {
	f_relPos = TransformRelPosition(f_relPos, f_rotation, f_mirror);
	
	SetPosition(f_Pos + f_relPos);
	
	SetTransform(f_rotation, f_mirror);
}

void SGraphicsItem::SetPosition(QPointF f_pos) {
	setPos(f_pos);
}


void SGraphicsItem::SetAcceptedMouseButtons ( Qt::MouseButtons buttons ) {
	setAcceptedMouseButtons(buttons);
}

void SGraphicsItem::mousePressEvent ( QGraphicsSceneMouseEvent * event ) {
	if ( event->button() == Qt::LeftButton )
		emit MouseLeftClick();
	
	emit MousePressed( event );
}

void SGraphicsItem::ChangeVisibility() {
	QGraphicsItem::setVisible(! QGraphicsItem::isVisible());
}

void SGraphicsItem::SetVisibility(bool f_visible) {
	QGraphicsItem::setVisible(f_visible);
}

QPointF SGraphicsItem::TransformRelPosition(QPointF f_relPos, enum ROTATION f_rotation, enum MIRROR f_mirror) {
	// Zuerst spiegeln, danach drehen!!! 
	// Spiegelung von f_relPos durchführen
	f_relPos.rx() *= f_mirror;

	for ( int i = 0; i < f_rotation; i++ ) {
		// Mache die Drehung nach links
		qreal new_x = + f_relPos.y();
		qreal new_y = - f_relPos.x();
		
		f_relPos.setX(new_x);
		f_relPos.setY(new_y);
	}
	
	return f_relPos;
}
