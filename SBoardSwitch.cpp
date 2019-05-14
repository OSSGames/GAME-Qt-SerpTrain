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

#include "SBoardSwitch.h"
#include "SException.h"
#include "SBall.h"
#include "SConfig.h"
#include "SGraphicsStraight.h"
#include "SGraphicsTurn.h"
#include "SGraphicsSwitchControl.h"
#include "SXMLConv.h"

SBoardSwitch::SBoardSwitch(enum BOARD_MODE f_boardmode, int f_layerID, int f_gameboardID)
	: SBoardTile(f_boardmode, f_layerID, f_gameboardID), m_switchState ( false ), m_switchable ( true )
{
	UpdateSwitch();
}

void SBoardSwitch::SpecialInitByXML(SXMLParse f_xmlparse) 
{
	SXMLParse boardSwitch = f_xmlparse.GetSubElement();
	
	if ( boardSwitch.HasNextElement("switchState") ) {
		boardSwitch.ToNextElement("switchState");
		
		m_switchState = boardSwitch.GetAttribute("value", SXMLConv::GetSwitchStateNames(), SXMLConv::GetSwitchStateValues(), false );
	}
	else
		m_switchState = false;

	UpdateSwitch();
}



SBoardSwitch::~SBoardSwitch()
{
}

bool SBoardSwitch::IsRelDirectionCon ( enum DIRECTION_RELATIVE f_dirRel ) const {
	switch ( f_dirRel ) {
		case LEFT:
		case RIGHT:
		case TOP:
			return true;
		default:
			return false;
	}
}

// ROUTE_1: Gerade von links nach rechts (ROUTE_2 umgekehrt)
// ROUTE_3: Kurbe von links nach oben (ROUTE_4 umgekehrt)
void SBoardSwitch::ReceiveBall(SBall * f_ball, enum DIRECTION_RELATIVE f_dirRel )
{
	switch ( f_dirRel ) {
		case LEFT:
			if ( m_switchState )
				f_ball->m_route = ROUTE_3;
			else
				f_ball->m_route = ROUTE_1;
				
			if ( !m_switchable ) {
				// Weiche ist nicht mehr schaltbar
				if ( m_switchState && f_ball->HasColor(m_colors[RIGHT]) )
					f_ball->m_route = ROUTE_1;
				else if ( f_ball->HasColor(m_colors[TOP]) )
					f_ball->m_route = ROUTE_3;
			}
			break;
		case RIGHT:
			f_ball->m_route = ROUTE_2;
			break;
		case TOP:
			f_ball->m_route = ROUTE_4;
			break;
		default:
			throwEx("Unvalid 'enum DIRECTION_RELATIVE' value: " + QString::number( f_dirRel) );
	}
}

const SPath * SBoardSwitch::GetBallPath( SBall * f_ball ) const
{
	switch ( f_ball->m_route ) { 
		case ROUTE_1:
			return & SGraphicsStraight::Path;
		case ROUTE_2:
			return & SGraphicsStraight::PathReversed;
		case ROUTE_3:
			return & SGraphicsTurn::Path;
		case ROUTE_4:
			return & SGraphicsTurn::PathReversed;
		default:
			throwEx("Unvalid 'enum BALL_ROUTE' value: " + QString::number( f_ball->m_route) );
	}
}

void SBoardSwitch::InitGraphic( ) {
	throwIfFalse( m_tiles.size() == 0 );
	
	m_tiles[TILE_STRAIGHT] 		= new SGraphicsStraight;
	m_tiles[TILE_TURN]	 		= new SGraphicsTurn;
	m_tiles[TILE_SWITCH_CONTROL]= new SGraphicsSwitchControl;

	m_tiles[TILE_STRAIGHT]->SetAcceptedMouseButtons(Qt::LeftButton);
	throwIfFalse(connect(m_tiles[TILE_STRAIGHT], SIGNAL(MouseLeftClick()), this, SLOT(ChangeSwitch())));
	
	UpdateSwitch();
}

SBoardTile * SBoardSwitch::DoCopy(enum BOARD_MODE f_boardMode) const
{
	SBoardSwitch * copy = new SBoardSwitch(f_boardMode, m_layerID, m_gameboardID);
	// Kopiere die Switch-relevaten Daten
	copy->m_switchState = m_switchState;
	copy->m_switchable = m_switchable;	
	
	return copy;
}

void SBoardSwitch::UpdateSwitch() {
	if ( m_isGraphical ) {
		if ( m_switchable ) {					
			qensure_cast(SGraphicsRail *, m_tiles[TILE_STRAIGHT])->SetRailColor( !m_switchState ? m_colors[LEFT] : SConfig::PASSIVE_RAIL_COLOR());
			qensure_cast(SGraphicsRail *, m_tiles[TILE_TURN])	 ->SetRailColor(  m_switchState ? m_colors[LEFT] : SConfig::PASSIVE_RAIL_COLOR());
		}
		else {
			if ( m_switchState ) {
				qensure_cast(SGraphicsRail *, m_tiles[TILE_STRAIGHT])->SetRailColor(m_colors[RIGHT]);
				qensure_cast(SGraphicsRail *, m_tiles[TILE_TURN]    )->SetRailColor(m_colors[LEFT]);
			} else {
				qensure_cast(SGraphicsRail *, m_tiles[TILE_STRAIGHT])->SetRailColor(m_colors[LEFT]);
				qensure_cast(SGraphicsRail *, m_tiles[TILE_TURN]    )->SetRailColor(m_colors[TOP]);
			}
		}
		
		m_tiles[TILE_STRAIGHT]	->SetZMinor( !m_switchState ? Z_MINOR_DEFAULT : Z_MINOR_LOW );
		m_tiles[TILE_TURN]		->SetZMinor(  m_switchState ? Z_MINOR_DEFAULT : Z_MINOR_LOW );
	}	
}

void SBoardSwitch::ChangeSwitch() {
	if ( m_switchable ) {
		m_switchState = !m_switchState;
		UpdateSwitch();
	}
}

bool SBoardSwitch::IsColorOk(SColor f_color, DIRECTION_RELATIVE f_dirRel) {
	if ( f_dirRel == LEFT )
		return true;
		
	if ( f_dirRel == BOTTOM ) // Die Bottom-Farbe darf nicht geändert werden
		return ( f_color == BLACK );
		
	if ( f_dirRel == TOP )
		return ( f_color == BLACK || m_colors[RIGHT] == BLACK ); // Entweder oben oder rechts muss schwarz sein
	
	
	throwIfFalse(f_dirRel == RIGHT);
		return ( f_color == BLACK || m_colors[TOP] == BLACK ); // Entweder oben oder rechts muss schwarz sein
}
 	  	 
void SBoardSwitch::UpdateColorsInfo() {
	if ( m_colors[RIGHT] != BLACK ) {
		m_switchable = false;
		m_switchState = true;
	} else if ( m_colors[TOP] != BLACK ) {
		m_switchable = false;
		m_switchState = false;
	}
	else
		m_switchable = true;
		
	if ( m_isGraphical ) {
		qensure_cast(SGraphicsItem *, m_tiles[TILE_SWITCH_CONTROL])->SetVisibility(m_switchable);
	}
	
	UpdateSwitch();
}

void SBoardSwitch::AdjustXmlInfo(SXMLElement f_element, SXMLDoc f_xmlDoc) {
	f_element.setAttribute("type", "switch");
	
	SXMLElement switchState = f_xmlDoc.CreateElement("switchState");
	switchState.SetAttribute("value", m_switchState, SXMLConv::GetSwitchStateNames(), SXMLConv::GetSwitchStateValues());
	f_element.appendChild(switchState);
}
