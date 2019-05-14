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

#include "SGraphicsTunnel.h"
#include "SConfig.h"
#include <QPainter>

const SPathTunnel SGraphicsTunnel::Path			= SPathTunnel(false);
const SPathTunnel SGraphicsTunnel::PathReversed	= SPathTunnel(true);

SGraphicsTunnel::SGraphicsTunnel() : SGraphicsTile(Z_MAJOR_HILLS, QPoint(SConfig::FIELD_SIZE()/2,SConfig::FIELD_SIZE()/2)), m_numberToTell(0)
{
}

SGraphicsTunnel::~SGraphicsTunnel()
{
}

void SGraphicsTunnel::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
   //painter->drawRect(boundingRect());
   
	QPainterPath tunnel;
	tunnel.moveTo(MirrorPoint(QPointF(SConfig::FIELD_SIZE()/2.0, SConfig::FIELD_SIZE()/4.0)));
	tunnel.arcTo(MirrorRect(QRectF(SConfig::FIELD_SIZE()/5.0, SConfig::FIELD_SIZE()/4.0, 3*SConfig::FIELD_SIZE()/5.0, SConfig::FIELD_SIZE()/2.0)), 90, -180);
	tunnel.lineTo(MirrorPoint(QPointF(4*SConfig::FIELD_SIZE()/5.0, 3*SConfig::FIELD_SIZE()/4.0)));
	tunnel.arcTo(MirrorRect(QRectF(3*SConfig::FIELD_SIZE()/5.0, 3*SConfig::FIELD_SIZE()/4.0, 2*SConfig::FIELD_SIZE()/5.0, -2*(SConfig::CON_POINT_1() - SConfig::FIELD_SIZE()/4.0))), 90, -90);
	tunnel.lineTo(MirrorPoint(QPointF(SConfig::FIELD_SIZE(), SConfig::CON_POINT_1())));
	tunnel.arcTo(MirrorRect(QRectF(3*SConfig::FIELD_SIZE()/5.0, SConfig::FIELD_SIZE()/4.0, 2*SConfig::FIELD_SIZE()/5.0, 2*(SConfig::CON_POINT_1() - SConfig::FIELD_SIZE()/4.0))), 0, 90);
	tunnel.closeSubpath();
		
	QPen pen = painter->pen();
	pen.setWidth(SConfig::RAIL_WIDTH_GRAPHICAL());
	pen.setColor(SConfig::HILL_COLOR()); 
	painter->setPen(pen);
	
	painter->setBrush(SConfig::HILL_COLOR());
	
	painter->drawPath(tunnel);
   
	if ( m_numberToTell > 0 ) {
		painter->setPen(SConfig::TUNNEL_TEXT_COLOR());
		painter->setBrush(Qt::NoBrush);
   
		QRectF textRect = MirrorRect(QRectF(SConfig::FIELD_SIZE()*3.0/4.0, SConfig::FIELD_SIZE()/4.0, SConfig::FIELD_SIZE()/4.0, SConfig::FIELD_SIZE()/2.0));
		QString text = QString::number(m_numberToTell);
		if ( m_numberToTell > SConfig::TUNNEL_NUMBER_OVERFLOW() )
			text = SConfig::TUNNEL_TEXT_OVERFLOW();
		painter->drawText(textRect, Qt::AlignCenter, text);
	}
}
