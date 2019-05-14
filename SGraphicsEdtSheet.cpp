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

#include "SGraphicsEdtSheet.h"
#include "SConfig.h"
#include "SException.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
 
SGraphicsEdtSheet::SGraphicsEdtSheet() : SGraphicsTile(Z_MAJOR_OVERLAY, QPoint(0, 0)), m_hasToBePaint ( false )
{
	SetAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
	setAcceptsHoverEvents(true);
	
	m_leftTop		= QRectF(QPointF(-SConfig::FIELD_SIZE()/2.0,							-SConfig::FIELD_SIZE()/2.0),							QSizeF(SConfig::FIELD_SIZE()/3.0, SConfig::FIELD_SIZE()/3.0 ));
	m_rightTop		= QRectF(QPointF( SConfig::FIELD_SIZE()/2.0-SConfig::FIELD_SIZE()/3.0,	-SConfig::FIELD_SIZE()/2.0), 							QSizeF(SConfig::FIELD_SIZE()/3.0, SConfig::FIELD_SIZE()/3.0 ));
	m_leftBottom	= QRectF(QPointF(-SConfig::FIELD_SIZE()/2.0,							 SConfig::FIELD_SIZE()/2.0-SConfig::FIELD_SIZE()/3.0),	QSizeF(SConfig::FIELD_SIZE()/3.0, SConfig::FIELD_SIZE()/3.0 ));
	m_rightBottom	= QRectF(QPointF( SConfig::FIELD_SIZE()/2.0-SConfig::FIELD_SIZE()/3.0,	 SConfig::FIELD_SIZE()/2.0-SConfig::FIELD_SIZE()/3.0),	QSizeF(SConfig::FIELD_SIZE()/3.0, SConfig::FIELD_SIZE()/3.0 ));
}

SGraphicsEdtSheet::~SGraphicsEdtSheet()
{
}

void SGraphicsEdtSheet::paint(QPainter *f_painter, const QStyleOptionGraphicsItem *, QWidget *) {
	f_painter->drawRect(boundingRect());
	   
	if ( m_hasToBePaint ) { // Nur zeichnen, wenn der Mousezeiger in der Nähe ist
		f_painter->setBrush(SConfig::EDIT_SHINY_WHITE());
		f_painter->setPen(Qt::NoPen);

		f_painter->drawEllipse(m_rightTop);
		f_painter->drawEllipse(m_leftTop);
		f_painter->drawEllipse(m_leftBottom);
		f_painter->drawEllipse(m_rightBottom);
		
		//Draw rotate symbol
		{
			QPointF origin = m_rightTop.topLeft();
			float rectSize = m_rightTop.width();
			throwIfFalse(m_rightTop.width() == m_rightTop.height());
			
			QPainterPath rotateSymbol;			
			rotateSymbol.moveTo(origin + QPointF(0, rectSize/4.0));
			rotateSymbol.lineTo(origin + QPointF(rectSize/4.0, rectSize/2.0));
			rotateSymbol.lineTo(origin + QPointF(rectSize/4.0, rectSize*2.0/6.0));
			rotateSymbol.arcTo(QRectF(origin + QPointF(-rectSize/6.0, rectSize*2.0/6.0),
							   		  origin + QPointF(rectSize/2.0+rectSize/6.0, rectSize*7.0/6.0)),
							   90, -90 );
			rotateSymbol.lineTo(origin + QPointF(rectSize/2.0, rectSize*3.0/4.0));		
			rotateSymbol.lineTo(origin + QPointF(rectSize*3.0/4.0, rectSize));		
			rotateSymbol.lineTo(origin + QPointF(rectSize, rectSize*3.0/4.0));		
			rotateSymbol.lineTo(origin + QPointF(rectSize*5.0/6.0, rectSize*3.0/4.0));		
			rotateSymbol.arcTo(QRectF(origin + QPointF(-rectSize/4.0-rectSize/12.0, rectSize*1.0/6.0),
							   		  origin + QPointF(rectSize*5.0/6.0, rectSize*5.0/4.0+rectSize/12.0)),
							   0, 90 );
			rotateSymbol.lineTo(origin + QPointF(rectSize*1.0/4.0, 0));		
			rotateSymbol.closeSubpath();
			
			f_painter->setBrush(SConfig::EDIT_ROTATE_COLOR());
			f_painter->setPen(Qt::SolidLine);
			
			f_painter->drawPath(rotateSymbol);
		}

		// Draw change symbol
		{
			QPointF origin = m_leftBottom.topLeft();
			float rectSize = m_leftBottom.width();
			throwIfFalse(m_leftBottom.width() == m_leftBottom.height());
			
			//Schwarzer Teil
			QPainterPath blackWand;
			blackWand.moveTo(origin + QPointF(rectSize/10.0, rectSize*8.0/10.0));
			blackWand.lineTo(origin + QPointF(rectSize*2.0/10.0, rectSize*9.0/10.0));
			blackWand.lineTo(origin + QPointF(rectSize*2.0/3.0, rectSize*1.0/3.0+rectSize/10.0));
			blackWand.lineTo(origin + QPointF(rectSize*2.0/3.0-rectSize/10.0, rectSize*1.0/3.0));
			blackWand.closeSubpath();
			f_painter->setBrush(Qt::black);
			f_painter->drawPath(blackWand);
			
			//Weißer Teil
			QPainterPath whiteWand;
			whiteWand.moveTo(origin + QPointF(rectSize*2.0/3.0, rectSize*1.0/3.0+rectSize/10.0));
			whiteWand.lineTo(origin + QPointF(rectSize*2.0/3.0-rectSize/10.0, rectSize*1.0/3.0));
			whiteWand.lineTo(origin + QPointF(rectSize*9.0/10.0-rectSize/10.0, rectSize*1.0/10.0));
			whiteWand.lineTo(origin + QPointF(rectSize*9.0/10.0, rectSize*1.0/10.0+rectSize/10.0));
			whiteWand.closeSubpath();
			f_painter->setBrush(Qt::white);
			f_painter->drawPath(whiteWand);
		}

		// Draw settings symbol
		{
			QPointF origin = m_rightBottom.topLeft();
			float rectSize = m_rightBottom.width();
			throwIfFalse(m_rightBottom.width() == m_rightBottom.height());
			
			f_painter->setBrush(Qt::black);
			QPen pen = f_painter->pen();
			pen.setWidthF(0);
			f_painter->setPen(pen);

			QPainterPath wrench1, wrench2, wrench3;
			
			//Stiel
			wrench1.moveTo(origin + QPointF(rectSize/5.0+rectSize/10.0, rectSize*4.0/5.0));
			wrench1.lineTo(origin + QPointF(rectSize/5.0, rectSize*4.0/5.0-rectSize/10.0));
			wrench1.lineTo(origin + QPointF(rectSize*4.0/5.0-rectSize/10.0, rectSize/5.0));
			wrench1.lineTo(origin + QPointF(rectSize*4.0/5.0, rectSize/5.0+rectSize/10.0));
			wrench1.closeSubpath();
			f_painter->drawPath(wrench1);
			
			//Oberer Schlüssel
			wrench2.moveTo(origin + QPointF(rectSize*4.0/5.0+rectSize/12.0, rectSize/4.0));
			wrench2.arcTo(QRectF(origin + QPointF(rectSize*6.5/10.0, rectSize*0.5/10.0), QSizeF(rectSize*3.0/10.0, rectSize*3.0/10.0)), 60, -300); 
			wrench2.lineTo(origin + QPointF(rectSize*4.0/5.0-rectSize/12.0, rectSize/4.0));
			wrench2.closeSubpath();
			f_painter->drawPath(wrench2);
			
			//Unterer Schlüssel
			wrench3.moveTo(origin + QPointF(rectSize*1.0/5.0+rectSize/12.0, rectSize-rectSize/4.0));
			wrench3.arcTo(QRectF(origin + QPointF(rectSize*0.5/10.0, rectSize*6.5/10.0), QSizeF(rectSize*3.0/10.0, rectSize*3.0/10.0)), -60, 300); 
			wrench3.lineTo(origin + QPointF(rectSize*1.0/5.0-rectSize/12.0, rectSize-rectSize/4.0));
			wrench3.closeSubpath();
			f_painter->drawPath(wrench3);

		}

		// Draw mirror symbol
		{
			QPointF origin = m_leftTop.topLeft();
			float rectSize = m_leftTop.width();
			throwIfFalse(m_leftTop.width() == m_leftTop.height());
		
			QPainterPath mirrorSymbol;
			//Spiegellinie
			mirrorSymbol.moveTo(origin + QPointF(rectSize/2.0, 0));
			mirrorSymbol.lineTo(origin + QPointF(rectSize/2.0, rectSize));
			
			//Linkes R
			mirrorSymbol.moveTo(origin + QPointF(rectSize*3.0/8.0, rectSize/8.0));
			mirrorSymbol.lineTo(origin + QPointF(rectSize*3.0/8.0, rectSize*7.0/8.0));
			mirrorSymbol.lineTo(origin + QPointF(rectSize*3.0/8.0, rectSize/2.0));
			mirrorSymbol.lineTo(origin + QPointF(rectSize*1.0/8.0, rectSize*7.0/8.0));
			mirrorSymbol.lineTo(origin + QPointF(rectSize*3.0/8.0, rectSize/2.0));
			mirrorSymbol.arcTo(QRectF(origin + QPointF(rectSize/8.0, rectSize/8.0),
							   		  origin + QPointF(rectSize*5.0/8.0, rectSize/2.0)),
							   270, -180 );
						
			//Rechtes R
			mirrorSymbol.moveTo(origin + QPointF(rectSize*5.0/8.0, rectSize/8.0));
			mirrorSymbol.lineTo(origin + QPointF(rectSize*5.0/8.0, rectSize*7.0/8.0));
			mirrorSymbol.lineTo(origin + QPointF(rectSize*5.0/8.0, rectSize/2.0));
			mirrorSymbol.lineTo(origin + QPointF(rectSize*7.0/8.0, rectSize*7.0/8.0));
			mirrorSymbol.lineTo(origin + QPointF(rectSize*5.0/8.0, rectSize/2.0));
			mirrorSymbol.arcTo(QRectF(origin + QPointF(rectSize*3.0/8.0, rectSize/8.0),
							   		  origin + QPointF(rectSize*7.0/8.0, rectSize/2.0)),
							   270, 180 );
			
			f_painter->setBrush(Qt::NoBrush);
			
			f_painter->setPen(Qt::SolidLine);
			QPen pen = f_painter->pen();
			pen.setWidthF(rectSize/10);
			f_painter->setPen(pen);
			
			f_painter->drawPath(mirrorSymbol);
		}
	}
}

void SGraphicsEdtSheet::mousePressEvent( QGraphicsSceneMouseEvent * f_event ) {
	if ( m_leftTop.contains( f_event->pos() ) || m_rightTop.contains( f_event->pos() ) || 
		 m_leftBottom.contains( f_event->pos() ) || m_rightBottom.contains( f_event->pos() ) )
	{
		// Eltern-Methode aufrufen
		SGraphicsItem::mousePressEvent( f_event );
		if ( f_event->button() == Qt::LeftButton ) {
			if ( m_leftTop.contains( f_event->pos() ) )
				emit EdtMirror();
			else if ( m_rightTop.contains( f_event->pos() ) ) 
				emit EdtRotate(ROTATION_RIGHT);
			else if ( m_leftBottom.contains( f_event->pos() ) ) 
				emit EdtChange();
			else if ( m_rightBottom.contains( f_event->pos() ) ) 
				emit EdtSettings();
		} else if ( f_event->button() == Qt::RightButton ) {
			if ( m_rightTop.contains( f_event->pos() ) ) 
				emit EdtRotate(ROTATION_LEFT);
		}
	} else {
		// Das Event ist nicht für uns bestimmt
		// --> weiterleiten
		f_event->ignore();
	}
}

void SGraphicsEdtSheet::hoverEnterEvent ( QGraphicsSceneHoverEvent * /*f_event */) {
	if ( ! m_hasToBePaint ) {
		m_hasToBePaint = true;
		Update();
	}
}
void SGraphicsEdtSheet::hoverLeaveEvent ( QGraphicsSceneHoverEvent * /*f_event */) {
	if ( m_hasToBePaint ) {
		m_hasToBePaint = false;
		Update();
	}
}
