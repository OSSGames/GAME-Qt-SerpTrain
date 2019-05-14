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

#include "SBoardStraight.h"
#include "SException.h"
#include "SBall.h"
#include "SGraphicsStraight.h"

SBoardStraight::SBoardStraight(enum BOARD_MODE f_boardmode, int f_layerID, int f_gameboardID)
	: SBoardTile(f_boardmode, f_layerID, f_gameboardID)
{
}

SBoardStraight::~SBoardStraight()
{
}

bool SBoardStraight::IsRelDirectionCon ( enum DIRECTION_RELATIVE f_dirRel ) const {
	switch ( f_dirRel ) {
		case LEFT:
		case RIGHT:
			return true;
		default:
			return false;
	}
}

void SBoardStraight::ReceiveBall(SBall * f_ball, enum DIRECTION_RELATIVE f_dirRel )
{
	switch ( f_dirRel ) {
		case LEFT:
			f_ball->m_route = ROUTE_1;
			break;
		case RIGHT:
			f_ball->m_route = ROUTE_2;
			break;
		default:
			throwEx("Unvalid 'enum DIRECTION_RELATIVE' value: " + QString::number( f_dirRel) );
	}
}

const SPath * SBoardStraight::GetBallPath( SBall * f_ball ) const
{
	switch ( f_ball->m_route ) { 
		case ROUTE_1:
			return & SGraphicsStraight::Path;
		case ROUTE_2:
			return & SGraphicsStraight::PathReversed;
		default:
			throwEx("Unvalid 'enum BALL_ROUTE' value: " + QString::number( f_ball->m_route) );
	}
}

void SBoardStraight::InitGraphic( ) {
	throwIfFalse( m_tiles.size() == 0 );
	
	m_tiles[TILE_STRAIGHT] = new SGraphicsStraight;
}

SBoardTile * SBoardStraight::DoCopy(enum BOARD_MODE f_boardMode) const
{
	return new SBoardStraight(f_boardMode, m_layerID, m_gameboardID);
}

bool SBoardStraight::IsColorOk(SColor f_color, DIRECTION_RELATIVE f_dirRel) {
	if ( f_dirRel == RIGHT )
		return true;
		
	return ( f_color == BLACK );
}

void SBoardStraight::UpdateColorsInfo() {
	if ( m_isGraphical ) {
			qensure_cast(SGraphicsRail *, m_tiles[TILE_STRAIGHT])->SetRailColor(m_colors[RIGHT]);	
	}
}

void SBoardStraight::SpecialInitByXML(SXMLParse /*f_xmlparse*/) 
{
	// Nothing to do!
}	

void SBoardStraight::AdjustXmlInfo(SXMLElement f_element, SXMLDoc /*f_xmlDoc*/) {
	f_element.setAttribute("type", "straight");
}
