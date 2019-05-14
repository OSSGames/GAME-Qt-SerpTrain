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

#include "SPathStraight.h"
#include "SConfig.h"

SPathStraight::SPathStraight(bool f_reverse) : SPath( f_reverse, QPoint(SConfig::FIELD_SIZE_PATH()/2,0))  
{
}

SPathStraight::~SPathStraight()
{
}

int SPathStraight::GetPathLen() const {
	return SConfig::FIELD_SIZE_PATH();
}

QPointF SPathStraight::GetPathPos ( int f_pos ) const {
	return QPointF(f_pos * SConfig::FIELD_SIZE_PATH() / GetPathLen(), 0 );
}

DIRECTION_RELATIVE SPathStraight::GetRelExit() const {
	if ( m_reverse )
		return LEFT;
	else
		return RIGHT; 
}
