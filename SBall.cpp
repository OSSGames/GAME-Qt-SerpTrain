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

#include "SBall.h"
#include "SConfig.h"
#include "SException.h"
#include <math.h>
#include <QPainter>

SBall::SBall(SColor f_color ) : SGraphicsItem( Z_MAJOR_BALL ), m_color ( f_color ), 
m_route ( ROUTE_NO ), m_steps ( 0 ), m_lastTimestep ( 0 )
{ }

SBall::~SBall()
{
}

QRectF SBall::boundingRect() const {
    return QRectF(-SConfig::RAIL_WIDTH()/2.0 , -SConfig::RAIL_WIDTH()/2.0,
                  SConfig::RAIL_WIDTH(), SConfig::RAIL_WIDTH());
}

void SBall::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    //painter->drawRect(boundingRect());

    painter->setBrush(m_color.GetQColor());
    painter->drawEllipse(boundingRect());
}

bool SBall::HasColor ( SColor f_color ) {
	return ( m_color == WHITE || f_color == m_color );
}

SColor SBall::GetColor() {
	return m_color;
}

qreal SBall::DistanceToBall(SBall * f_otherBall) {
	throwIfFalse(f_otherBall != NULL);
	
	QPointF abs = f_otherBall->pos() - this->pos();
	return sqrt(pow(abs.x(),2) + pow(abs.y(),2));	
}
