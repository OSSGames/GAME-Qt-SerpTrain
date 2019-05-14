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

#include "SBoardTurn.h"
#include "SException.h"
#include "SBall.h"
#include "SGraphicsTurn.h"

SBoardTurn::SBoardTurn(enum BOARD_MODE f_boardmode, int f_layerID, int f_gameboardID)
	: SBoardTile(f_boardmode, f_layerID, f_gameboardID)
{
}

SBoardTurn::~SBoardTurn()
{
}

bool SBoardTurn::IsRelDirectionCon ( enum DIRECTION_RELATIVE f_dirRel ) const {
	switch ( f_dirRel ) {
		case LEFT:
		case TOP:
			return true;
		default:
			return false;
	}
}

void SBoardTurn::ReceiveBall(SBall * f_ball, enum DIRECTION_RELATIVE f_dirRel )
{
	switch ( f_dirRel ) {
		case LEFT:
			f_ball->m_route = ROUTE_1;
			break;
		case TOP:
			f_ball->m_route = ROUTE_2;
			break;
		default:
			throwEx("Unvalid 'enum DIRECTION_RELATIVE' value: " + QString::number( f_dirRel) );
	}
}

const SPath * SBoardTurn::GetBallPath( SBall * f_ball ) const
{
	switch ( f_ball->m_route ) { 
		case ROUTE_1:
			return & SGraphicsTurn::Path;
		case ROUTE_2:
			return & SGraphicsTurn::PathReversed;
		default:
			throwEx("Unvalid 'enum BALL_ROUTE' value: " + QString::number( f_ball->m_route) );
	}
}

void SBoardTurn::InitGraphic( ) {
	throwIfFalse( m_tiles.size() == 0 );
	
	m_tiles[TILE_TURN]	 		= new SGraphicsTurn;
}

SBoardTile * SBoardTurn::DoCopy(enum BOARD_MODE f_boardMode) const
{
	return new SBoardTurn(f_boardMode, m_layerID, m_gameboardID);
}

bool SBoardTurn::IsColorOk(SColor f_color, DIRECTION_RELATIVE f_dirRel) {
	if ( f_dirRel == TOP )
		return true;
	
	return ( f_color == BLACK );
}

void SBoardTurn::UpdateColorsInfo() {
	if ( m_isGraphical ) {
			qensure_cast(SGraphicsRail *, m_tiles[TILE_TURN])->SetRailColor(m_colors[TOP]);	
	}
}

void SBoardTurn::SpecialInitByXML(SXMLParse /*f_xmlparse*/) 
{
	// Nothing to do!
}	

void SBoardTurn::AdjustXmlInfo(SXMLElement f_element, SXMLDoc /*f_xmlDoc*/) {
	f_element.setAttribute("type", "turn");
}
