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

#include "SPathTurn.h"
#include "SConfig.h"
#include <math.h>

SPathTurn::SPathTurn(bool f_reverse, enum ROTATION f_rotation) : SPath( f_reverse, QPoint(SConfig::FIELD_SIZE_PATH()/2,SConfig::FIELD_SIZE_PATH()/2), f_rotation)  
{
}

SPathTurn::~SPathTurn()
{
}

int SPathTurn::GetPathLen() const {
	return (int) (SConfig::FIELD_SIZE_PATH()*Pi/4);
}

QPointF SPathTurn::GetPathPos ( int f_pos ) const {
	
	qreal rad = f_pos * Pi/2 / GetPathLen();
	
	return QPointF(SConfig::FIELD_SIZE_PATH()/2*sin(rad),SConfig::FIELD_SIZE_PATH()/2*cos(rad) );
}

DIRECTION_RELATIVE SPathTurn::GetRelExit() const {
	if ( m_reverse )
		return LEFT;
	else
		return TOP; 
}
