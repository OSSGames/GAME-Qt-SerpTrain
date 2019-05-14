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

#include "SPathTunnel.h"
#include "SConfig.h"
#include "SException.h"

SPathTunnel::SPathTunnel(bool f_reverse) : SPathStraight ( f_reverse )
{
}

SPathTunnel::~SPathTunnel()
{
}

QPointF SPathTunnel::GetGraphicalPosOnPath( int f_pos ) const {
	QPointF pos = SPathStraight::GetGraphicalPosOnPath(f_pos);
	
	// Der Ball wird in der Mitte unter dem Tunnel festgehalten!
	// Achtung: Funktion ist nicht an die Möglichkeit einer Drehung angepasst
	// 		--> SPathTunnel darf nicht einfach über m_rotation gedreht werden
	throwIfFalse( m_rotation == ROTATION_NO );
	
	pos.rx() = qMin(pos.rx(), 0.4*SConfig::FIELD_SIZE());
	
	return pos;
}

DIRECTION_RELATIVE SPathTunnel::GetRelExit() const {
	DIRECTION_RELATIVE res = SPathStraight::GetRelExit();
	if ( res == RIGHT )
		return DIR_SPECIAL;
	
	return res;
}
