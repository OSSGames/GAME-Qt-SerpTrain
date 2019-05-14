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

#include "SPathGoalTurn.h"
#include "SConfig.h"
#include "SGraphicsStraight.h"
#include "SException.h"
#include <math.h>

SPathGoalTurn::SPathGoalTurn(int f_goalIndex) : SPath( false, QPoint(SConfig::FIELD_SIZE_PATH()/2,SConfig::FIELD_SIZE_PATH()/2)), m_goalIndex(f_goalIndex)  
{
	throwIfFalse(m_goalIndex <= 3 && m_goalIndex >= 0);
}

SPathGoalTurn::~SPathGoalTurn()
{
}

int SPathGoalTurn::GetPathLen() const {
	qreal turn = (SConfig::FIELD_SIZE_PATH()*0.8)*Pi/4;
	qreal straight = (SConfig::FIELD_SIZE_PATH()*0.4);
	if ( m_goalIndex >= 2 )
		return (int) ( turn + straight);
	else
		return (int) turn;
}

QPointF SPathGoalTurn::GetPathPos ( int f_pos ) const {
	
	QPointF path; qreal rad; qreal straightLen;
	
	switch( m_goalIndex ){
		case 0:
		case 1:
			rad = f_pos * Pi/2 / GetPathLen();
			path = QPointF(SConfig::FIELD_SIZE_PATH()*0.8/2*sin(rad),SConfig::FIELD_SIZE_PATH()*0.8/2*cos(rad) )
				+ QPointF(0,SConfig::FIELD_SIZE_PATH()*0.1); // In die Mitte, da wo die Kugel auch herkommen!
			break;
		case 2:
		case 3:
			straightLen = 0.4 * SConfig::FIELD_SIZE_PATH(); 
			if ( f_pos <= straightLen ) {
				path = QPointF(f_pos, SConfig::FIELD_SIZE_PATH()/2);
			}
			else {
				rad = (f_pos-straightLen) * Pi/2 / (GetPathLen()-straightLen);
				path = QPointF(SConfig::FIELD_SIZE_PATH()*0.8/2*sin(rad),SConfig::FIELD_SIZE_PATH()*0.8/2*cos(rad) )
					+ QPointF(0,SConfig::FIELD_SIZE_PATH()*0.1) // In die Mitte, da wo die Kugel auch herkommen!
					+ QPointF(straightLen, 0); // Die Gerade dazu addieren
			}
			break;
		default:
			throwEx("Unvalid m_goalIndex: " + QString::number(m_goalIndex));			
	} 

	switch( m_goalIndex ){
		case 0:
		case 2:
			return path;
		case 1:
		case 3:
			return QPointF(path.x(), SConfig::FIELD_SIZE_PATH() - path.y());
		default:
			throwEx("Unvalid m_goalIndex: " + QString::number(m_goalIndex));			
	} 
}

DIRECTION_RELATIVE SPathGoalTurn::GetRelExit() const {
	return DIR_SPECIAL; 
}
