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

#include "SGraphicsTunnelMarker.h"
#include "SConfig.h"
#include "SException.h"
#include <QPainter>


SGraphicsTunnelMarker::SGraphicsTunnelMarker() : SGraphicsTile(Z_MAJOR_BACKGROUND, QPoint(SConfig::FIELD_SIZE()/2,SConfig::FIELD_SIZE()/2))
{
}

SGraphicsTunnelMarker::~SGraphicsTunnelMarker()
{
}

// Spiegelung entfällt komplett, da die Markierung symetrisch zur Spiegelachse ist
void SGraphicsTunnelMarker::paint(QPainter *f_painter, const QStyleOptionGraphicsItem *, QWidget *) {
	QPen pen = f_painter->pen();
	pen.setWidthF(SConfig::TUNNEL_MARKER_WIDTH());
	pen.setColor(GetColor(m_tunnelId));
	f_painter->setPen(pen);
	
	QPainterPath marker;
	marker.moveTo(QPointF(                    0 + SConfig::TUNNEL_MARKER_WIDTH()/2.0,                     0 + SConfig::TUNNEL_MARKER_WIDTH()/2.0));
	marker.lineTo(QPointF(SConfig::FIELD_SIZE() - SConfig::TUNNEL_MARKER_WIDTH()/2.0,                     0 + SConfig::TUNNEL_MARKER_WIDTH()/2.0));
	marker.lineTo(QPointF(SConfig::FIELD_SIZE() - SConfig::TUNNEL_MARKER_WIDTH()/2.0, SConfig::FIELD_SIZE() - SConfig::TUNNEL_MARKER_WIDTH()/2.0));
	marker.lineTo(QPointF(                    0 + SConfig::TUNNEL_MARKER_WIDTH()/2.0, SConfig::FIELD_SIZE() - SConfig::TUNNEL_MARKER_WIDTH()/2.0));
	marker.closeSubpath();
	
	f_painter->drawPath(marker);
}

void SGraphicsTunnelMarker::SetTunnelID( int f_id ) {
	m_tunnelId = f_id;
	
	Update();
}

/*static*/ QColor SGraphicsTunnelMarker::GetColor( int f_id ) {
	if ( f_id < 0 ) 
		return QColor(0,0,0,0); // Transparente Farbe! 

	static const QColor colors[] = {QColor(255,0,0), //rot
									QColor(0,0,255), //blau
									QColor(255, 255, 0), // gelb
									QColor(255, 0, 255), // pink						
									QColor(0,255, 255), // türkis
									QColor(255, 128, 0), // orange
									QColor(128, 0, 255), // lila
									QColor(255, 255, 255), // weiß
									QColor(128, 255, 0) // hellgrün
								};
	
	static const int len = sizeof(colors)/sizeof(colors[0]);
	
	return colors[f_id % len].darker( (f_id / len) * 80 + 100); // Für jedes Mal, welches das array wiederholt wird, wird die Farbe dunkler machen 
}
