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

#ifndef SGRAPHICSTILE_H_
#define SGRAPHICSTILE_H_

#include "SGraphicsItem.h"

#include <QPainter> 

class SGraphicsTile : public SGraphicsItem
{
	Q_OBJECT
	
public:
	SGraphicsTile( enum Z_MAJOR, QPoint f_middlePoint);
	virtual ~SGraphicsTile();
	void virtual SetTransform(enum ROTATION, enum MIRROR);
	void SetPosition(QPoint f_pos, enum ROTATION, enum MIRROR);
		
protected:
 	void virtual SetMirror(enum MIRROR);
 	
 	template <typename T>
	T MirrorXCoord(T f_x) const { return ( f_x - m_middlePoint.x() ) * m_mirror + m_middlePoint.x(); };

 	template <typename T>
	T MirrorPoint(T f_point) const { return T(MirrorXCoord(f_point.x()), f_point.y()); };
	
	template <typename T>
	T MirrorRect(T f_rect) const { return T(MirrorPoint(f_rect.topLeft()), MirrorPoint(f_rect.bottomRight())); }
	
	void DrawMirrorArc(QPainter * f_painter, QRectF f_rect, int f_startAngle, int f_spanAngle);

	void DrawTurn( QPainter * f_painter, qreal f_radius, QPointF f_center);
	void DrawStraight( QPainter * f_painter, qreal f_start, qreal f_end, QPointF f_leftCenter);

	QPoint m_middlePoint; // Gibt den Mittelpunkt des Spielplanstückchens an
	enum MIRROR m_mirror;
	
	enum MIRROR m_addMirror;	// Geben an, um wieviel das Spielstück intern gedreht bzw. gespiegelt werden muss!
	enum ROTATION m_addRotation;
	
	virtual void SetPosition(QPointF f_pos);
	
	virtual QRectF boundingRect() const;
};

#endif /*SGRAPHICSTILE_H_*/
