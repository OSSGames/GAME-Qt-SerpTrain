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

#include "SGraphicsEdtColors.h"
#include "SConfig.h"
#include "SException.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <math.h>
 
SGraphicsEdtColors::SGraphicsEdtColors() : SGraphicsTile(Z_MAJOR_OVERLAY, QPoint(0, 0))
{
	SetAcceptedMouseButtons(Qt::LeftButton);
	
	// Farbenarray aufbauen
	for ( SColor color = FIRST_COLOR; color <= LAST_COLOR; ++color ) 
		m_colors << color;

	m_size = (qreal) SConfig::FIELD_SIZE() / m_colors.size();
}

SGraphicsEdtColors::~SGraphicsEdtColors()
{
}

void SGraphicsEdtColors::paint(QPainter *f_painter, const QStyleOptionGraphicsItem *, QWidget *) {
	throwIfFalse( m_allowedColors.contains(NORTH));
	throwIfFalse( m_allowedColors.contains(EAST) );
	throwIfFalse( m_allowedColors.contains(SOUTH));
	throwIfFalse( m_allowedColors.contains(WEST) );
	
	f_painter->setPen(Qt::NoPen);
	QRectF rect;
	for ( int i = 0; i < m_colors.size(); i++ ) {
		f_painter->setBrush(m_colors[i].GetQColor());
		
		// North
		if ( m_allowedColors[NORTH].GetAmountAllowed() > 1 ) {
			if ( m_allowedColors[NORTH].IsColorAllowed(m_colors[i]) ) {
				rect = QRectF(-SConfig::FIELD_SIZE()/2.0 + m_size * i,-SConfig::FIELD_SIZE()/2.0 - 2* m_size,m_size,m_size);
				if ( m_allowedColors[NORTH].IsColorActive(m_colors[i]) )
					rect.adjust(0,m_size/2, 0, m_size/2);
				f_painter->drawEllipse(rect);
			}
		}
		
		// East
		if ( m_allowedColors[EAST].GetAmountAllowed() > 1 ) {
			if ( m_allowedColors[EAST].IsColorAllowed(m_colors[i]) ) {
				rect = QRectF(+SConfig::FIELD_SIZE()/2.0 + m_size,-SConfig::FIELD_SIZE()/2.0 + i* m_size,m_size,m_size);
				if ( m_allowedColors[EAST].IsColorActive(m_colors[i]) )
					rect.adjust(-m_size/2, 0, -m_size/2, 0);
				f_painter->drawEllipse(rect);
			}
		}

		// South
		if ( m_allowedColors[SOUTH].GetAmountAllowed() > 1 ) {
			if ( m_allowedColors[SOUTH].IsColorAllowed(m_colors[i]) ) {
				rect = QRectF(-SConfig::FIELD_SIZE()/2.0 + m_size * i,+SConfig::FIELD_SIZE()/2.0 + 1* m_size,m_size,m_size);
				if ( m_allowedColors[SOUTH].IsColorActive(m_colors[i]) )
					rect.adjust(0,-m_size/2, 0, -m_size/2);
				f_painter->drawEllipse(rect);
			}
		}
		
		// West
		if ( m_allowedColors[WEST].GetAmountAllowed() > 1 ) {
			if ( m_allowedColors[WEST].IsColorAllowed(m_colors[i]) ) {
				rect = QRectF(-SConfig::FIELD_SIZE()/2.0 - 2* m_size,-SConfig::FIELD_SIZE()/2.0 + i* m_size,m_size,m_size);
				if ( m_allowedColors[WEST].IsColorActive(m_colors[i]) )
					rect.adjust(m_size/2, 0, m_size/2, 0);
				f_painter->drawEllipse(rect);
			}
		}
	}
}

void SGraphicsEdtColors::mousePressEvent( QGraphicsSceneMouseEvent * f_event ) {
	qreal x = f_event->pos().x();
	qreal y = f_event->pos().y();
	qreal x_rel = x + SConfig::FIELD_SIZE()/2.0; // ?_rel hat den Ursprung links oben
	qreal y_rel = y + SConfig::FIELD_SIZE()/2.0;
	if ( -SConfig::FIELD_SIZE()/2.0 - 2 * m_size <= y && y <= -SConfig::FIELD_SIZE()/2.0 - 1 * m_size ) {
		// North
		int index = (int) floor(x_rel / m_size);
		if ( index >= 0 && index < m_colors.size() )
			ChangeColor(m_colors[index], NORTH);
	} else if ( SConfig::FIELD_SIZE()/2.0 + 1 * m_size <= x && x <= SConfig::FIELD_SIZE()/2.0 + 2 * m_size ) {
		// East
		int index = (int) floor(y_rel / m_size);
		if ( index >= 0 && index < m_colors.size() )
			ChangeColor(m_colors[index], EAST);
	} else if ( SConfig::FIELD_SIZE()/2.0 + 1 * m_size <= y && y <= SConfig::FIELD_SIZE()/2.0 + 2 * m_size ) {
		// South
		int index = (int) floor(x_rel/ m_size);
		if ( index >= 0 && index < m_colors.size() )
			ChangeColor(m_colors[index], SOUTH);
	} else if ( -SConfig::FIELD_SIZE()/2.0 - 2 * m_size <= x && x <= -SConfig::FIELD_SIZE()/2.0 - 1 * m_size ) {
		// West
		int index = (int) floor(y_rel / m_size);
		if ( index >= 0 && index < m_colors.size() )
			ChangeColor(m_colors[index], WEST);
	} else {
		// Das Event ist nicht für uns bestimmt
		// --> weiterleiten
		f_event->ignore();
	}
}

QRectF SGraphicsEdtColors::boundingRect() const {
	// Das Rect muss modifiziert werden, da die Farben aushalb liegen
	QRectF rect = SGraphicsTile::boundingRect();
	rect.adjust(-3*m_size, -3*m_size, +3*m_size, +3*m_size);
	
	return rect;
}

void SGraphicsEdtColors::ChangeColor( SColor f_color, DIRECTION_ABSOLUTE f_absDir ) {
	if ( m_allowedColors[f_absDir].IsColorAllowed(f_color) ) {
		emit ColorChanged(f_color, f_absDir);
		Update();
	}
}

void SGraphicsEdtColors::SetAllowedColors(DIRECTION_ABSOLUTE f_absDir, SAllowedColors f_allowedColors) {
	m_allowedColors[f_absDir] = f_allowedColors;	
}
