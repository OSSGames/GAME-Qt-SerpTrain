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

#include "SGraphicsTile.h"
#include "SConfig.h"
#include "SException.h"
#include <math.h>

SGraphicsTile::SGraphicsTile(enum Z_MAJOR f_major, QPoint f_middlePoint) : SGraphicsItem(f_major),
	m_middlePoint ( f_middlePoint ), m_mirror ( MIRROR_NO ), m_addMirror ( MIRROR_NO ), m_addRotation ( ROTATION_NO )
{
}

SGraphicsTile::~SGraphicsTile()
{
}

void SGraphicsTile::SetTransform(enum ROTATION f_rotation, enum MIRROR f_mirror) {
	resetTransform();
	
	// Rotate this item "-90*f_rotation" degrees around m_middlePoint.
	// Achtung: rotate würde von sich aus rechtsherum drehen!
	int x = m_middlePoint.x(); int y = m_middlePoint.y();
 	setTransform(QTransform().translate(x,y).rotate(-90*(f_rotation+f_mirror*m_addRotation)).translate(-x,-y));
 	
 	SetMirror(f_mirror);
}

void SGraphicsTile::SetMirror(enum MIRROR f_mirror) {
	m_mirror = (enum MIRROR) (f_mirror * m_addMirror);
	
	Update();
}
 
void SGraphicsTile::SetPosition(QPoint f_pos, enum ROTATION f_rotation, enum MIRROR f_mirror) {
	SetRelativePosition(f_pos, QPoint(0,0), f_rotation, f_mirror );	
}
 
void SGraphicsTile::SetPosition(QPointF f_pos) {
	SGraphicsItem::SetPosition(f_pos - m_middlePoint);
}

QRectF SGraphicsTile::boundingRect() const {
	return QRectF(m_middlePoint.x() - SConfig::FIELD_SIZE()/2.0, m_middlePoint.y() - SConfig::FIELD_SIZE()/2.0, SConfig::FIELD_SIZE(), SConfig::FIELD_SIZE());
}


void SGraphicsTile::DrawMirrorArc(QPainter * f_painter, QRectF f_rect, int f_startAngle, int f_spanAngle) {
	// Beachte, dass die Winkel als 16tel eines Grades eingegeben werden!
	f_painter->drawArc(MirrorRect(f_rect), m_mirror * (f_startAngle - 90*16) + 90*16, m_mirror * f_spanAngle);
}


void SGraphicsTile::DrawTurn( QPainter * f_painter, qreal f_radius, QPointF f_center) {
	int amountLogs = (int) (SConfig::LOGS_ON_TURN() * (2*f_radius) / SConfig::FIELD_SIZE()); 
	qreal conPoint1 = f_radius - SConfig::RAIL_WIDTH() / 2.0;
	qreal conPoint2 = f_radius + SConfig::RAIL_WIDTH() / 2.0;

	qreal conPoint1Logs = conPoint1 - SConfig::LOGS_OVERHEAD();
	qreal conPoint2Logs = conPoint2 + SConfig::LOGS_OVERHEAD();

	for ( int i = 0; i < amountLogs; i++ ) {
		QPolygonF polygon;
		 
		qreal radPerLog = (Pi/4)/amountLogs;
		 
		polygon << MirrorPoint(QPointF(conPoint1Logs*sin((4*i+1)*radPerLog/2), conPoint1Logs*cos((4*i+1)*radPerLog/2))+f_center);
		polygon << MirrorPoint(QPointF(conPoint1Logs*sin((4*i+3)*radPerLog/2), conPoint1Logs*cos((4*i+3)*radPerLog/2))+f_center);
		polygon << MirrorPoint(QPointF(conPoint2Logs*sin((4*i+3)*radPerLog/2), conPoint2Logs*cos((4*i+3)*radPerLog/2))+f_center);
		polygon << MirrorPoint(QPointF(conPoint2Logs*sin((4*i+1)*radPerLog/2), conPoint2Logs*cos((4*i+1)*radPerLog/2))+f_center);
		 
		f_painter->drawPolygon(polygon);
	}
   
	QPen pen = f_painter->pen();
	pen.setWidth(SConfig::RAIL_WIDTH_GRAPHICAL());
	f_painter->setPen(pen);
	
	// Hinten heißt es "* 16", da der Kreis für diese Funktion in 360*16 Teile geteilt wurde
	DrawMirrorArc(f_painter, QRectF(QPointF(-conPoint1,-conPoint1)+f_center, QSizeF(2*conPoint1, 2*conPoint1)), 0, -90*16);       	
	DrawMirrorArc(f_painter, QRectF(QPointF(-conPoint2,-conPoint2)+f_center, QSizeF(2*conPoint2, 2*conPoint2)), 0, -90*16);
}

void SGraphicsTile::DrawStraight( QPainter * f_painter, qreal f_start, qreal f_end, QPointF f_leftCenter) {
	throwIfFalse( f_start < f_end );
	
	if ( m_mirror == MIRROR_YES ) {
		qreal start = f_start;
		qreal end	= f_end;
		
		f_start = SConfig::FIELD_SIZE() - end;
		f_end	= SConfig::FIELD_SIZE() - start;
	}
	
	QPointF modFromNormal = f_leftCenter - QPointF(0, SConfig::FIELD_SIZE()/2);
      
	for ( int i = 0; i < SConfig::LOGS_ON_STRAIGHT(); i++ ) {
		QRectF log = QRectF(
						QPointF((4*i+1)*SConfig::FIELD_SIZE()/(4.0*SConfig::LOGS_ON_STRAIGHT()),
							SConfig::CON_POINT_1_LOGS()),
						QSizeF(SConfig::FIELD_SIZE()/(2.0*SConfig::LOGS_ON_STRAIGHT()),
							SConfig::RAIL_WIDTH()+2*SConfig::LOGS_OVERHEAD()));
		if ( log.x() >= f_start && log.x() < f_end ) {
			log.translate(modFromNormal);
			f_painter->drawRect(log);
		}	
   	} 
      
   	QPen pen = f_painter->pen();
   	pen.setWidth(SConfig::RAIL_WIDTH_GRAPHICAL()); 
   	f_painter->setPen(pen);
   
   	f_painter->drawLine(QLineF(QPointF(f_start, SConfig::CON_POINT_1()) + modFromNormal, QPointF(f_end, SConfig::CON_POINT_1()) + modFromNormal));
   	f_painter->drawLine(QLineF(QPointF(f_start, SConfig::CON_POINT_2()) + modFromNormal, QPointF(f_end, SConfig::CON_POINT_2()) + modFromNormal));
}
