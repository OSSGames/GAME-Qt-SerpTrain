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

#include "SPath.h"
#include "SConfig.h"
#include "SBoardTile.h"
#include "SException.h"

SPath::SPath(bool f_reverse, QPoint f_middlePoint, enum ROTATION f_rotation) :
	m_reverse ( f_reverse ), m_middlePoint( f_middlePoint ), m_rotation ( f_rotation )
{
}

SPath::~SPath()
{
}

QPointF SPath::GetGraphicalPosOnPath( int f_pos ) const {
	return SConfig::FIELD_SIZE() * GetPosOnPath( f_pos ) / SConfig::FIELD_SIZE_PATH();
}

QPointF SPath::GetPosOnPath( int f_pos ) const {
	if ( f_pos < 0 || f_pos > GetPathLen() )
		throwEx("Unvalid 'f_pos' value: " + QString::number(f_pos));
	
	QPointF tmpResult; 
	
	if ( m_reverse )
		tmpResult = GetPathPos( GetPathLen() - f_pos ) - m_middlePoint;
	else
		tmpResult = GetPathPos( f_pos ) - m_middlePoint;
		
	QPointF result; 
	// Ergebnis drehen
	switch ( m_rotation ) {
		case ROTATION_NO:
			result = tmpResult;
			break;
		case ROTATION_90:
			result.rx() = +tmpResult.ry();
			result.ry() = -tmpResult.rx();
			break;
		case ROTATION_180:
			result.rx() = -tmpResult.rx();
			result.ry() = -tmpResult.ry();
			break;
		case ROTATION_270:
			result.rx() = -tmpResult.ry();
			result.ry() = +tmpResult.rx();
			break;
		default: 
			throwEx("Unvalid 'enum ROTATION' value: " + QString::number( m_rotation) );
	}
	
	return result;		
}

DIRECTION_RELATIVE SPath::GetExit() const {
	// Drehung durchführen
	DIRECTION_ABSOLUTE absDir = SBoardTile::GetAbsDirection( GetRelExit(), m_rotation, MIRROR_NO );
	
	// Ergebnis wieder in Relativ-richtung umrechnen
	return SBoardTile::GetRelDirection( absDir, ROTATION_NO, MIRROR_NO );
}
