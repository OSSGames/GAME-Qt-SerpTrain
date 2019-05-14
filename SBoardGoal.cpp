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

#include "SBoardGoal.h"
#include "SGraphicsStraightPart.h"
#include "SBoardGoalSettingsWidget.h"
#include "SGraphicsGoalTurn.h"
#include "SException.h"
#include "SBall.h"
#include "SConfig.h"
#include "SXMLConv.h"

SBoardGoal::SBoardGoal(enum BOARD_MODE f_boardmode, int f_layerID, int f_gameboardID)
	: SBoardTile(f_boardmode, f_layerID, f_gameboardID), m_isConnected ( false ),
	m_freeLots ( 0 ), m_showAll ( false ), m_active ( true )
{
}

void SBoardGoal::SpecialInitByXML(SXMLParse f_xmlparse) 
{
	SXMLParse goal = f_xmlparse.GetSubElement();
	
	if ( goal.HasNextElement("continue") ) {
		goal.ToNextElement("continue");
		m_isConnected = goal.GetBoolAttribute("value", false);
	}
	else
		m_isConnected = false;
	
	if ( goal.HasNextElement("ballqueue") ) {
		goal.ToNextElement("ballqueue");
		
		m_showAll = goal.GetBoolAttribute("showAll", false);
		
		for ( SXMLParse sumOverQueue = goal.GetSubElement(); sumOverQueue.HasNextElement("ball"); )
		{
			sumOverQueue.ToNextElement();
			m_freeLots++;
		}
		
		if ( m_freeLots > AmountFields ) {
			goal.AddError("There are more than " + QString::number(AmountFields) + " balls in these Goal!");
			m_freeLots = AmountFields;
		}
		
		int index = AmountFields - m_freeLots;
		for ( SXMLParse queue = goal.GetSubElement();
			  index < AmountFields && queue.HasNextElement("ball");
			  index++ )
		{
			queue.ToNextElement();
					
			m_goalFields[index].ballColor =  queue.GetAttribute("color", SXMLConv::GetColorWOBlackNames(), SXMLConv::GetColorWOBlackValues(), SColor(RED) );
		}
	}
}


SBoardGoal::~SBoardGoal()
{
	// Aufräumen
	for ( int goalIndex = 0; goalIndex < AmountFields; goalIndex++ )
		if ( m_goalFields[goalIndex].ball != NULL )
			m_goalFields[goalIndex].ball->deleteLater();
}

bool SBoardGoal::IsRelDirectionCon ( enum DIRECTION_RELATIVE f_dirRel ) const {
	switch ( f_dirRel ) {
		case LEFT:
			return true;
		case RIGHT:
			return m_isConnected;
		default:
			return false;
	}
}

void SBoardGoal::ReceiveBall(SBall * f_ball, enum DIRECTION_RELATIVE f_dirRel )
{
	switch ( f_dirRel ) {
		case RIGHT:
			f_ball->m_route = ROUTE_6;
			break;
		case LEFT:
			switch ( m_freeLots ) {
				case 4: 
					f_ball->m_route = ROUTE_1;
					break;
				case 3: 
					f_ball->m_route = ROUTE_2;
					break;
				case 2: 
					f_ball->m_route = ROUTE_3;
					break;
				case 1: 
					f_ball->m_route = ROUTE_4;
					break;
				case 0:
					if ( m_isConnected )
						f_ball->m_route = ROUTE_5;
					else
						f_ball->m_route = ROUTE_4;
					break;
				default:
					throwEx("Unvalid m_freeLots-value: " + QString::number(m_freeLots));
			}
			break;
		default:
			throwEx("Unvalid 'enum DIRECTION_RELATIVE' value: " + QString::number( f_dirRel) );
	}
}

const SPath * SBoardGoal::GetBallPath( SBall * f_ball ) const
{
	switch ( f_ball->m_route ) { 
		case ROUTE_1:
			return & SGraphicsGoalTurn::Path1;
		case ROUTE_2:
			return & SGraphicsGoalTurn::Path2;
		case ROUTE_3:
			return & SGraphicsGoalTurn::Path3;
		case ROUTE_4:
			return & SGraphicsGoalTurn::Path4;
		case ROUTE_5:
			return & SGraphicsStraight::Path;
		case ROUTE_6:
			return & SGraphicsStraight::PathReversed;
		default:
			throwEx("Unvalid 'enum BALL_ROUTE' value: " + QString::number( f_ball->m_route) );
	}
}

void SBoardGoal::InitGraphic( ) {
	throwIfFalse( m_tiles.size() == 0 );
	
	m_tiles[TILE_GOAL_STRAIGHT]	= new SGraphicsStraightPart(0, 0.4*SConfig::FIELD_SIZE());
	
	m_tiles[TILE_GOAL_CONTINUE]	= new SGraphicsStraightPart(0.4*SConfig::FIELD_SIZE(), SConfig::FIELD_SIZE());
	 	
	m_tiles[TILE_GOAL0] 		= new SGraphicsGoalTurn(0);
	m_tiles[TILE_GOAL1] 		= new SGraphicsGoalTurn(1);
	m_tiles[TILE_GOAL2] 		= new SGraphicsGoalTurn(2);
	m_tiles[TILE_GOAL3] 		= new SGraphicsGoalTurn(3);
	
	UpdateGraphics();
}

SBoardTile * SBoardGoal::DoCopy(enum BOARD_MODE f_boardMode) const
{
	SBoardGoal * copy = new SBoardGoal(f_boardMode, m_layerID, m_gameboardID);
	
	// Kopiere die Goal-relevaten Daten
	copy->m_isConnected = m_isConnected;
	copy->m_freeLots = m_freeLots;
	copy->m_showAll = m_showAll;
	// Verzichte auf bool m_active, da es sowieso nur MODE_PLAY-relevante Wichtigkeit besitzt
	
	// Nur die Farben kopieren, nicht die Bälle
	for ( int i = 0; i < AmountFields; i++ )
		copy->m_goalFields[i].ballColor = m_goalFields[i].ballColor;
			
	return copy;
}

void SBoardGoal::SendBall( SBall * f_ball, enum DIRECTION_RELATIVE f_dirRel ) {
	if ( f_dirRel == DIR_SPECIAL ) {
		int goalIndex;
		switch ( f_ball->m_route ) {
			case ROUTE_1:
				goalIndex = 0;
				break;
			case ROUTE_2:
				goalIndex = 1;
				break;
			case ROUTE_3:
				goalIndex = 2;
				break;
			case ROUTE_4:
				goalIndex = 3;
				break;
			default:
				throwEx("Unvalid 'enum BALL_ROUTE' value: " + QString::number( f_ball->m_route) );
		}
		
		if ( m_goalFields[goalIndex].ball != NULL ){
			emit BallsCrashed(f_ball, m_goalFields[goalIndex].ball);
		}
		else {
			m_goalFields[goalIndex].ball = f_ball;
	
			// Ball entfernen	
			throwIfFalse(m_balls.removeAll(f_ball) == 1);
			
			
			if ( m_goalFields[goalIndex].ballColor != f_ball->GetColor() ) {
				// Falsche Kugel im Ziel gelandet!
				emit BallsCrashed(f_ball);
			}
			else {
				// Richtig Kugel im Ziel gelandet!
			
				// Einen Parkplatz weniger
				throwIfFalse(AmountFields - goalIndex == m_freeLots);
				m_freeLots--;
				UpdateGraphics();
				
				// Ziel voll!
				if ( m_freeLots == 0 )
					emit GoalFull();
				
				if ( f_ball->m_route == ROUTE_3 || ( m_isConnected && f_ball->m_route == ROUTE_4) ) {
					// Schauen, ob es Bälle gibt, die noch nicht abgebogen sind
					foreach (SBall * ball, m_balls ) {
						if ( ball->m_route == ROUTE_3 || ( m_isConnected && ball->m_route == ROUTE_4 ) ) { // Nur diese Routen betrifft das Problem
							if ( SGraphicsStraight::Path.GetPosOnPath(ball->m_steps) == GetBallPath(ball)->GetPosOnPath(ball->m_steps) ) {
								// Wenn ja, dann diese Bälle auf die neuen Routen setzen!
								if ( ball->m_route == ROUTE_3 )
									ball->m_route = ROUTE_4;
								else if ( m_isConnected && ball->m_route == ROUTE_4 )
									ball->m_route = ROUTE_5;
								else
									throwEx("Logical Error! Please report this error!");
							}
						}
					}
				}
			}
		}
	}
	else
		SBoardTile::SendBall(f_ball, f_dirRel);	
}

void SBoardGoal::UpdateGraphics() {
	if ( m_isGraphical ) {
		m_tiles[TILE_GOAL_CONTINUE]->SetVisibility(m_isConnected);
		
		switch ( m_freeLots ) {
			case 4:
				m_tiles[TILE_GOAL0]->SetZMinor(Z_MINOR_TOP);
				m_tiles[TILE_GOAL1]->SetZMinor(Z_MINOR_HIGHEST);
				m_tiles[TILE_GOAL2]->SetZMinor(Z_MINOR_HIGHER);
				m_tiles[TILE_GOAL3]->SetZMinor(Z_MINOR_HIGH);
				
				qensure_cast(SGraphicsRail *, m_tiles[TILE_GOAL0])->SetRailColor(GetGoalColor(m_goalFields[0].ballColor, true));
				qensure_cast(SGraphicsRail *, m_tiles[TILE_GOAL1])->SetRailColor(GetGoalColor(m_goalFields[1].ballColor, false));
				qensure_cast(SGraphicsRail *, m_tiles[TILE_GOAL2])->SetRailColor(GetGoalColor(m_goalFields[2].ballColor, false));
				qensure_cast(SGraphicsRail *, m_tiles[TILE_GOAL3])->SetRailColor(GetGoalColor(m_goalFields[3].ballColor, false));
				
				qensure_cast(SGraphicsRail *, m_tiles[TILE_GOAL_STRAIGHT])->SetRailColor(GetGoalColor(m_goalFields[2].ballColor, false));
				break;
			case 3:
				m_tiles[TILE_GOAL0]->SetZMinor(Z_MINOR_LOW);
				m_tiles[TILE_GOAL1]->SetZMinor(Z_MINOR_HIGHEST);
				m_tiles[TILE_GOAL2]->SetZMinor(Z_MINOR_HIGHER);
				m_tiles[TILE_GOAL3]->SetZMinor(Z_MINOR_HIGH);
				
				qensure_cast(SGraphicsRail *, m_tiles[TILE_GOAL0])->SetRailColor(GetGoalColor(m_goalFields[0].ballColor, true));
				qensure_cast(SGraphicsRail *, m_tiles[TILE_GOAL1])->SetRailColor(GetGoalColor(m_goalFields[1].ballColor, true));
				qensure_cast(SGraphicsRail *, m_tiles[TILE_GOAL2])->SetRailColor(GetGoalColor(m_goalFields[2].ballColor, false));
				qensure_cast(SGraphicsRail *, m_tiles[TILE_GOAL3])->SetRailColor(GetGoalColor(m_goalFields[3].ballColor, false));
				
				qensure_cast(SGraphicsRail *, m_tiles[TILE_GOAL_STRAIGHT])->SetRailColor(GetGoalColor(m_goalFields[2].ballColor, false));
				break;
			case 2:
				m_tiles[TILE_GOAL0]->SetZMinor(Z_MINOR_LOW);
				m_tiles[TILE_GOAL1]->SetZMinor(Z_MINOR_LOWER);
				m_tiles[TILE_GOAL2]->SetZMinor(Z_MINOR_HIGHER);
				m_tiles[TILE_GOAL3]->SetZMinor(Z_MINOR_HIGH);
				
				qensure_cast(SGraphicsRail *, m_tiles[TILE_GOAL0])->SetRailColor(GetGoalColor(m_goalFields[0].ballColor, true));
				qensure_cast(SGraphicsRail *, m_tiles[TILE_GOAL1])->SetRailColor(GetGoalColor(m_goalFields[1].ballColor, true));
				qensure_cast(SGraphicsRail *, m_tiles[TILE_GOAL2])->SetRailColor(GetGoalColor(m_goalFields[2].ballColor, true));
				qensure_cast(SGraphicsRail *, m_tiles[TILE_GOAL3])->SetRailColor(GetGoalColor(m_goalFields[3].ballColor, false));
				
				qensure_cast(SGraphicsRail *, m_tiles[TILE_GOAL_STRAIGHT])->SetRailColor(GetGoalColor(m_goalFields[2].ballColor, true));								
				break;
			case 0:
				if ( m_isConnected ) {
					m_tiles[TILE_GOAL0]->SetZMinor(Z_MINOR_LOW);
					m_tiles[TILE_GOAL1]->SetZMinor(Z_MINOR_LOWER);
					m_tiles[TILE_GOAL2]->SetZMinor(Z_MINOR_LOWEST);
					m_tiles[TILE_GOAL3]->SetZMinor(Z_MINOR_BOTTOM);
					
					qensure_cast(SGraphicsRail *, m_tiles[TILE_GOAL0])->SetRailColor(GetGoalColor(m_goalFields[0].ballColor, true));
					qensure_cast(SGraphicsRail *, m_tiles[TILE_GOAL1])->SetRailColor(GetGoalColor(m_goalFields[1].ballColor, true));
					qensure_cast(SGraphicsRail *, m_tiles[TILE_GOAL2])->SetRailColor(GetGoalColor(m_goalFields[2].ballColor, true));
					qensure_cast(SGraphicsRail *, m_tiles[TILE_GOAL3])->SetRailColor(GetGoalColor(m_goalFields[3].ballColor, true));
	
					qensure_cast(SGraphicsRail *, m_tiles[TILE_GOAL_STRAIGHT])->SetRailColor(m_colors[RIGHT]);								
					break;
				}
				// Falls das Schienenstück nicht fortgesetzt wird, muss es bei 0 Kugel, so gezeichnet werden,
				// als wären noch für 1 Kugel Platz.
			case 1:
				m_tiles[TILE_GOAL0]->SetZMinor(Z_MINOR_LOW);
				m_tiles[TILE_GOAL1]->SetZMinor(Z_MINOR_LOWER);
				m_tiles[TILE_GOAL2]->SetZMinor(Z_MINOR_LOWEST);
				m_tiles[TILE_GOAL3]->SetZMinor(Z_MINOR_HIGH);
				
				qensure_cast(SGraphicsRail *, m_tiles[TILE_GOAL0])->SetRailColor(GetGoalColor(m_goalFields[0].ballColor, true));
				qensure_cast(SGraphicsRail *, m_tiles[TILE_GOAL1])->SetRailColor(GetGoalColor(m_goalFields[1].ballColor, true));
				qensure_cast(SGraphicsRail *, m_tiles[TILE_GOAL2])->SetRailColor(GetGoalColor(m_goalFields[2].ballColor, true));
				qensure_cast(SGraphicsRail *, m_tiles[TILE_GOAL3])->SetRailColor(GetGoalColor(m_goalFields[3].ballColor, true));

				qensure_cast(SGraphicsRail *, m_tiles[TILE_GOAL_STRAIGHT])->SetRailColor(GetGoalColor(m_goalFields[3].ballColor, true));								
				break;
		}
		
		// Neu zeichen! (einmal reicht, da die Gerade das ganze Feld überdeckt!
		m_tiles[TILE_GOAL_STRAIGHT]->Update();
	}
}

QColor SBoardGoal::GetGoalColor(SColor f_color, bool f_force) {
	if ( (f_force && m_active) || m_showAll ||
		m_boardMode == MODE_EDITOR || m_boardMode == MODE_TEST || m_boardMode == MODE_EDT_SETTING )
	{
		if ( f_color == RAIL_COLOR )
			return SConfig::PASSIVE_RAIL_COLOR();
		else
			return f_color; 
	}
	else
		return SConfig::PASSIVE_RAIL_COLOR();
}

void SBoardGoal::SetActive() {
	m_active = true;

	UpdateGraphics();
}

void SBoardGoal::SetPassive() {
	m_active = false;
	
	UpdateGraphics();
}

void SBoardGoal::AdjustXmlInfo(SXMLElement f_element, SXMLDoc f_xmlDoc) {
	f_element.setAttribute("type", "goal");
	
	SXMLElement toContinue = f_xmlDoc.CreateElement("continue");
	toContinue.SetBoolAttribute("value", m_isConnected);
	f_element.appendChild(toContinue);
	
	SXMLElement ballqueue = f_xmlDoc.CreateElement("ballqueue");
	ballqueue.SetBoolAttribute("showAll", m_showAll);
	f_element.appendChild(ballqueue);
	
	for ( int goalNr = 0; goalNr < AmountFields; goalNr++ ) {
		if ( m_goalFields[goalNr].ballColor != RAIL_COLOR ) {
			SXMLElement ball = f_xmlDoc.CreateElement("ball");
			ball.SetAttribute("color", m_goalFields[goalNr].ballColor, SXMLConv::GetColorWOBlackNames(), SXMLConv::GetColorWOBlackValues());
			ballqueue.appendChild(ball);
		}
	}
}

bool SBoardGoal::IsColorOk(SColor f_color, DIRECTION_RELATIVE f_relDir ) {
	if ( f_relDir == RIGHT && m_isConnected )
		return true;
	
	return ( f_color == BLACK );
}

void SBoardGoal::UpdateColorsInfo() {
	if ( m_isGraphical )
		qensure_cast(SGraphicsRail *, m_tiles[TILE_GOAL_CONTINUE])->SetRailColor(m_colors[RIGHT]);

	UpdateGraphics();
}

QWidget * SBoardGoal::CreateCustumSettingsWidget() {
	SBoardGoalSettingsWidget * curCstWidget = new SBoardGoalSettingsWidget(m_isConnected, m_showAll);
	
	SArray<SColor, AmountFields> goalColors;
	for ( int fieldNr = 0; fieldNr < AmountFields; fieldNr++ )
		goalColors[fieldNr] = m_goalFields[fieldNr].ballColor;
		 
	curCstWidget->SetGoalFields(goalColors);
	
	throwIfFalse(QObject::connect(curCstWidget, SIGNAL(GoalColorsChanged(SArray<SColor,SBoardGoal::AmountFields>)), this, SLOT(SetGoalColors(SArray<SColor,SBoardGoal::AmountFields>))));
	throwIfFalse(QObject::connect(curCstWidget, SIGNAL(ShowAllFlagChanged(bool)), this, SLOT(SetShowAllFlag(bool))));
	throwIfFalse(QObject::connect(curCstWidget, SIGNAL(ContinueFlagChanged(bool)), this, SLOT(SetContinueFlag(bool))));
	
	return curCstWidget;
}

void SBoardGoal::SetGoalColors(SArray<SColor, AmountFields> f_goalColors) {
	for ( int fieldNr = 0; fieldNr < AmountFields; fieldNr++ )
		m_goalFields[fieldNr].ballColor = f_goalColors[fieldNr];
	
	m_freeLots = 0;
	for ( int fieldNr = 0; fieldNr < AmountFields; fieldNr++ ) {
		if ( m_goalFields[fieldNr].ballColor != RAIL_COLOR )
			m_freeLots++;
	}

	UpdateGraphics();
}

void SBoardGoal::SetShowAllFlag(bool f_showAll) {
	m_showAll = f_showAll;
}

void SBoardGoal::SetContinueFlag(bool f_isConnected) {
	m_isConnected = f_isConnected;
	
	UpdateGraphics();	
}
