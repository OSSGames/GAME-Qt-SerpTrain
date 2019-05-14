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

#include "SXMLConv.h"
#include "SException.h"

// Einziges Interes xmlConv-Objekt. Es zum Zugreifen auf die Listen
/*static */ SXMLConv * SXMLConv::m_single = new SXMLConv();

SXMLConv::SXMLConv()
{
	throwIfFalse( m_single == NULL);
	
	// Initialisierungen
	m_RotationNames  << "0" << "90" << "180" << "270";
	m_RotationValues << ROTATION_NO  << ROTATION_90 << ROTATION_180 << ROTATION_270;
	
	m_ColorNames  << "black" << "red" << "green" << "yellow" << "orange" << "white" << "blue";
	m_ColorValues <<  BLACK  <<  RED  <<  GREEN  <<  YELLOW  <<  ORANGE  <<  WHITE  <<  BLUE ;

	m_ColorWOBlackNames  << "red" << "green" << "yellow" << "orange" << "white" << "blue";
	m_ColorWOBlackValues <<  RED  <<  GREEN  <<  YELLOW  <<  ORANGE  <<  WHITE  <<  BLUE ;

	m_MirrorNames  << "yes" << "no";
	m_MirrorValues << MIRROR_YES << MIRROR_NO;
	
	m_SwitchStateNames  << "turn" << "straight";
	m_SwitchStateValues <<  true  << false;
	
	m_SwitchTurnStateNames  << "turn1" << "turn2";
	m_SwitchTurnStateValues <<  false  <<  true;

	m_BoardtilesNames  << "straight" << "turn" << "doubleTurn" << "switch" << "switchTurn" << "tunnel" << "none" << "goal";
	m_BoardtilesValues <<  STRAIGHT  <<  TURN  << DOUBLE_TURN  <<  SWITCH  <<  SWITCH_TURN <<  TUNNEL  <<  NONE  <<  GOAL;
}

SXMLConv::~SXMLConv()
{
}
