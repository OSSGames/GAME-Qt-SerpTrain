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

#include "SBoardNone.h"
#include "SException.h"

SBoardNone::SBoardNone(enum BOARD_MODE f_boardmode, int f_layerID, int f_gameboardID)
	: SBoardTile(f_boardmode, f_layerID, f_gameboardID)
{
}

SBoardNone::~SBoardNone()
{
}

void SBoardNone::ReceiveBall(SBall *, enum DIRECTION_RELATIVE) {
	throwEx("SBoardNone should not receive a Ball!" );
}
	
bool SBoardNone::IsRelDirectionCon ( enum DIRECTION_RELATIVE ) const {
	return false;
}

const SPath * SBoardNone::GetBallPath( SBall * ) const
{
	throwEx("SBoardNone has no routes!" );
}

void SBoardNone::InitGraphic( ) {
	// Nothing to do!
}

SBoardTile * SBoardNone::DoCopy(enum BOARD_MODE f_boardMode) const
{
	return new SBoardNone(f_boardMode, m_layerID, m_gameboardID);
}

void SBoardNone::SpecialInitByXML(SXMLParse /*f_xmlparse*/) 
{
	// Nothing to do!
}	

void SBoardNone::AdjustXmlInfo(SXMLElement f_element, SXMLDoc /*f_xmlDoc*/) {
	f_element.setAttribute("type", "none");
}

bool SBoardNone::IsColorOk(SColor f_color, DIRECTION_RELATIVE ) {
	return ( f_color == BLACK );
}
