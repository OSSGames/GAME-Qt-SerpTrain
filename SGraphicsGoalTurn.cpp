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

#include "SGraphicsGoalTurn.h"
#include "SConfig.h"
#include <QPainter>

const SPathGoalTurn SGraphicsGoalTurn::Path1	= SPathGoalTurn(0);
const SPathGoalTurn SGraphicsGoalTurn::Path2	= SPathGoalTurn(1);
const SPathGoalTurn SGraphicsGoalTurn::Path3	= SPathGoalTurn(2);
const SPathGoalTurn SGraphicsGoalTurn::Path4	= SPathGoalTurn(3);

SGraphicsGoalTurn::SGraphicsGoalTurn(int f_goalIndex ): SGraphicsRail(QPoint(SConfig::FIELD_SIZE()/2,SConfig::FIELD_SIZE()/2)), m_goalIndex( f_goalIndex)
{
	// Die ungeraden Ziele drehen
	if ( m_goalIndex % 2 == 1 )
		m_addRotation = ROTATION_90;
}

SGraphicsGoalTurn::~SGraphicsGoalTurn()
{
}

void SGraphicsGoalTurn::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
	//painter->drawRect(boundingRect());    
   
	painter->setBrush(m_color);
   
	switch ( m_goalIndex )
	{
		case 0:
   			DrawTurn( painter, 0.8 * SConfig::FIELD_SIZE() / 2, QPointF(0,0.1*SConfig::FIELD_SIZE()));
   			break;
		case 1:
   			DrawTurn( painter, 0.8 * SConfig::FIELD_SIZE() / 2, QPointF(0.1*SConfig::FIELD_SIZE(),0));
   			break;
		case 2:
   			DrawTurn( painter, 0.8 * SConfig::FIELD_SIZE() / 2, QPointF(0.4*SConfig::FIELD_SIZE(),0.1*SConfig::FIELD_SIZE()));
   			break;
		case 3:
   			DrawTurn( painter, 0.8 * SConfig::FIELD_SIZE() / 2, QPointF(0.1*SConfig::FIELD_SIZE(),0.4*SConfig::FIELD_SIZE()));
   			break;
	}
}
