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

#include "SBoardTunnel.h"
#include "SGraphicsTunnel.h"
#include "SGraphicsStraight.h"
#include "SGraphicsTunnelMarker.h"
#include "SException.h"
#include "SBall.h"
#include "SConfig.h"
#include "SXMLConv.h"
#include "SBoardTunnelSettingsWidget.h"

/*static*/ QHash<int, QMultiHash<int, SBoardTunnel *> > SBoardTunnel::m_tunnelSystem;
/*static*/ QHash<int, int> SBoardTunnel::m_tunnelIDtoConnect;

SBoardTunnel::SBoardTunnel(enum BOARD_MODE f_boardmode, int f_layerID, int f_gameboardID)
	: SBoardTile(f_boardmode, f_layerID, f_gameboardID), m_tunnelID ( -1 ), m_curCstWidget ( NULL )
{
}

void SBoardTunnel::SpecialInitByXML(SXMLParse f_xmlparse) 
{
	SXMLParse tunnel = f_xmlparse.GetSubElement();
	
	if ( tunnel.HasNextElement("tunnelID") ) {
		tunnel.ToNextElement("tunnelID");
		int tunnelID = tunnel.GetIntAttribute("value", -1);
		if ( m_tunnelSystem[m_gameboardID].count(tunnelID) >= 2 ) {
			tunnel.AddError("More than two tunnels with ID " + QString::number(tunnelID));
			SetTunnelID(-1);
		}			
		else
			SetTunnelID(tunnelID);
	}
	else
		SetTunnelID(-1);
	
	if ( tunnel.HasNextElement("ballqueue") ) {
		tunnel.ToNextElement("ballqueue");
		
		for ( SXMLParse queue = tunnel.GetSubElement(); queue.HasNextElement(); )
		{
			queue.ToNextElement();
			
			if ( queue.GetTagName() == "ball" ) {
				STunnelQueue ball;
				ball.type = STunnelQueue::BALL;
				
				ball.data.ball.ballColor =  queue.GetAttribute("color", SXMLConv::GetColorWOBlackNames(), SXMLConv::GetColorWOBlackValues(), SColor(RED) );
								
				m_queue.enqueue(ball);
			}
			else if ( queue.GetTagName() == "wait" ) {
				STunnelQueue wait;
				wait.type = STunnelQueue::WAIT;
				wait.data.wait.timesteps = queue.GetIntAttribute("time", 50, 0);
				
				m_queue.enqueue(wait);
			}
			else
				queue.AddError(queue.GetTagName() + " is an unknown member of a ballqueue" );
		}
	}
}


SBoardTunnel::~SBoardTunnel()
{
	DisconnectTunnel();
}

bool SBoardTunnel::IsRelDirectionCon ( enum DIRECTION_RELATIVE f_dirRel ) const {
	switch ( f_dirRel ) {
		case LEFT:
		case DIR_SPECIAL:
			return true;
		default:
			return false;
	}
}

void SBoardTunnel::ReceiveBall(SBall * f_ball, enum DIRECTION_RELATIVE f_dirRel )
{
	switch ( f_dirRel ) {
		case LEFT:
			f_ball->m_route = ROUTE_1;
			break;
		case DIR_SPECIAL: 
			if ( m_isGraphical )
				// Ball auf die Scene setzen! 
				f_ball->AddToScene(m_scene);
				
			f_ball->m_route = ROUTE_2;
			break;
		default:
			throwEx("Unvalid 'enum DIRECTION_RELATIVE' value: " + QString::number( f_dirRel) );
	}
}

const SPath * SBoardTunnel::GetBallPath( SBall * f_ball ) const
{
	switch ( f_ball->m_route ) { 
		case ROUTE_1:
			return & SGraphicsTunnel::Path;
		case ROUTE_2:
			return & SGraphicsTunnel::PathReversed;
		default:
			throwEx("Unvalid 'enum BALL_ROUTE' value: " + QString::number( f_ball->m_route) );
	}
}

void SBoardTunnel::InitGraphic( ) {
	throwIfFalse( m_tiles.size() == 0 );
	
	m_tiles[TILE_STRAIGHT] 	= new SGraphicsStraight;
	m_tiles[TILE_TUNNEL] 	= new SGraphicsTunnel;
	
	// Das Tunnel-Marker-Tile wird einfach drunter gelegt
	if ( m_boardMode == MODE_TEST || m_boardMode == MODE_EDITOR || m_boardMode == MODE_EDT_SETTING ) {
			SGraphicsTunnelMarker * marker = new SGraphicsTunnelMarker();
			m_tiles[TILE_TUNNEL_MARKER] = marker;
			
			marker->SetTunnelID(m_tunnelID);
	}
	

	UpdateBallNumber();
}

SBoardTile * SBoardTunnel::DoCopy(enum BOARD_MODE f_boardMode) const
{
	SBoardTunnel * copy = new SBoardTunnel(f_boardMode, m_layerID, m_gameboardID);
	
	// Kopiere die Tunnel-relevaten Daten
	copy->m_queue = m_queue;
	copy->SetTunnelID(m_tunnelID);
	
	return copy;
}

void SBoardTunnel::Timestep ( int f_timestepNr ) {
	SBoardTile::Timestep(f_timestepNr);
	
	if ( ! m_queue.isEmpty() ) {
		switch( m_queue.head().type ) {
			case STunnelQueue::WAIT:
				if ( m_queue.head().data.wait.timesteps-- <= 0 )
					m_queue.dequeue();
				break;	
			case STunnelQueue::BALL:
			{
				SBall * newBall = new SBall(m_queue.head().data.ball.ballColor);
				
				newBall->m_lastTimestep = f_timestepNr - 1; // Der Timestep wird gleich in DoTimestep hochgesetzt
				
				ReceiveBallFrom(newBall, GetAbsDirection(DIR_SPECIAL));
				
				m_queue.dequeue();
				
				UpdateBallNumber();
				break;
			}
			default:
			throwEx("NotImplemented: " + QString::number(m_queue.head().type));
		}
	}
}

bool SBoardTunnel::CanCrashOutsideHappen(SBall * f_ball, QPointF f_point) {
	int outerLimit = SConfig::FIELD_SIZE_PATH() / 2 - SConfig::RAIL_WIDTH_PATH();
	if ( m_tunnelID < 0 ) {
		// Crash mit der Tunneleinfahrt
		int tunnelLimit = outerLimit - SConfig::RAIL_WIDTH_PATH() / 2;
		if ( f_point.x() >= tunnelLimit )
			if ( f_ball->m_route == ROUTE_1 ) // Nur Crash erzeugen, wenn der Ball Richtung Tunnel fährt
				emit BallsCrashed(f_ball);
	}
	else {
		if ( f_point.x() >= outerLimit )
			GetConnnectedTunnel()->TestCrashThisBall(f_ball, f_point);
	}
	
	// Rechts am Tunneleingang soll nicht geprüft werden!
	return ( f_point.x() <= -outerLimit || qAbs(f_point.y()) >= outerLimit );
}

void SBoardTunnel::SendBallAbs( SBall * f_ball, enum DIRECTION_ABSOLUTE f_dirAbs ){
	if ( f_dirAbs == DIR_ABS_SPECIAL ) {
		if ( m_tunnelID < 0 ) {
			// Ball zurückschicken!
			f_ball->m_steps = 0;
			ReceiveBallFrom(f_ball, DIR_ABS_SPECIAL);
			
			emit BallsCrashed(f_ball);
		}
		else {
			if ( m_isGraphical )
				// Ball von der Zeichenoberfläche verschwinden lassen
				f_ball->RemoveFromScene();
				
			GetConnnectedTunnel()->ReceiveBallFrom(f_ball, DIR_ABS_SPECIAL);
		}
	}
	else
		SBoardTile::SendBallAbs(f_ball, f_dirAbs );
}

void SBoardTunnel::TestCrashThisBall(SBall * f_ball, QPointF f_point) {
	qreal absToTunnelEnd = SConfig::FIELD_SIZE_PATH() / 2 - f_point.x();
	for (int i = 0; i < m_balls.size(); ++i) {
		SBall * ball = m_balls.at(i); 
		QPointF point = GetBallPath( ball )->GetPosOnPath(ball->m_steps);
		qreal absToTunnelEnd2 = SConfig::FIELD_SIZE_PATH() / 2 - point.x();
		if ( SCrashDetection::IsLenTooSmall(QPointF(absToTunnelEnd + absToTunnelEnd2, 0) ) )
			emit BallsCrashed(f_ball, ball);
	}
}

void SBoardTunnel::UpdateBallNumber() {
	if ( m_isGraphical ) {
		// Bälle zählen
		int number = 0;
		QListIterator<STunnelQueue> i(m_queue);
		while (i.hasNext()) {
			if ( i.next().type == STunnelQueue::BALL )
				number++;
		}
				
		// Anzahl anzeigen
		qensure_cast(SGraphicsTunnel * , m_tiles[TILE_TUNNEL])->SetNumberToTell(number);
		m_tiles[TILE_TUNNEL]->Update();
	}		
}

bool SBoardTunnel::IsColorOk(SColor f_color, DIRECTION_RELATIVE f_dirRel) {
	if ( f_dirRel == RIGHT )
		return true;
	else
		return ( f_color == BLACK );
}

void SBoardTunnel::UpdateColorsInfo() {
	if ( m_isGraphical ) {
			qensure_cast(SGraphicsRail *, m_tiles[TILE_STRAIGHT])->SetRailColor(m_colors[RIGHT]);	
	}
}

void SBoardTunnel::AdjustXmlInfo(SXMLElement f_element, SXMLDoc f_xmlDoc) {
	f_element.setAttribute("type", "tunnel");
	
	SXMLElement tunnelID = f_xmlDoc.CreateElement("tunnelID");
	// Wenn unsere ID die ID in m_tunnelIDtoConnect ist, dann darf die tunnelID nicht abgespeichert werden
	if ( m_tunnelIDtoConnect.contains(m_gameboardID) && m_tunnelIDtoConnect.value(m_gameboardID) == m_tunnelID )
		tunnelID.setAttribute("value", "-1");
	else
		tunnelID.setAttribute("value", QString::number(m_tunnelID));
	f_element.appendChild(tunnelID);
	
	SXMLElement ballqueue = f_xmlDoc.CreateElement("ballqueue");
	f_element.appendChild(ballqueue);
	
	for (int queueNr = 0; queueNr < m_queue.size(); queueNr++) {
		STunnelQueue curQueue = m_queue.at(queueNr);
		switch ( curQueue.type ) {
			case STunnelQueue::BALL:
			{
				SXMLElement ball = f_xmlDoc.CreateElement("ball");
				ball.SetAttribute("color", curQueue.data.ball.ballColor, SXMLConv::GetColorWOBlackNames(), SXMLConv::GetColorWOBlackValues());
				ballqueue.appendChild(ball);
				break;
			}
			case STunnelQueue::WAIT: 
			{
				SXMLElement wait = f_xmlDoc.CreateElement("wait");
				wait.setAttribute("time", QString::number(curQueue.data.wait.timesteps));
				ballqueue.appendChild(wait);
				break;
			}
			default:
				throwEx("The value of 'curQueue.type' is invalid: " + QString::number(curQueue.type));
		}
	}
}

void SBoardTunnel::SetTunnelID(int f_id ) {
	if ( m_tunnelID == f_id )
		return;
		
	if ( m_tunnelID >= 0 ) {
		throwIfFalse(m_tunnelSystem[m_gameboardID].remove(m_tunnelID, this) == 1);
		
		// Wenn unsere ID die ID in m_tunnelIDtoConnect ist, und es keinen tunnel mehr mit dieser ID gibt,
		// dann lösche diese ID.
		if ( m_tunnelIDtoConnect.contains(m_gameboardID) && m_tunnelIDtoConnect.value(m_gameboardID) == m_tunnelID ) {
			if ( m_tunnelSystem[m_gameboardID].count(m_tunnelID) == 0 ) {
				throwIfFalse(m_tunnelIDtoConnect.remove(m_gameboardID) == 1);
			}
		} else {
			// Wenn wir nicht die ID zum Verbinden tragen, dann sind wir schon verbunden
			
			// Wenn nur noch ein weitere Tunnel dieser ID vorhanden ist, diesen auch entfernen
			QList<SBoardTunnel *> otherTunnels = m_tunnelSystem[m_gameboardID].values(m_tunnelID);
			if ( otherTunnels.size() == 1 )
				otherTunnels[0]->DisconnectTunnel();
		}
	}
	
	m_tunnelID = f_id;
	if ( m_tiles.contains(TILE_TUNNEL_MARKER) )
		qensure_cast(SGraphicsTunnelMarker *, m_tiles[TILE_TUNNEL_MARKER])->SetTunnelID(f_id);
		
	if ( m_tunnelID >= 0 )
		m_tunnelSystem[m_gameboardID].insert(m_tunnelID, this);
		
}

QWidget * SBoardTunnel::CreateCustumSettingsWidget() {
	m_curCstWidget = new SBoardTunnelSettingsWidget(m_queue);
	
	throwIfFalse(QObject::connect(m_curCstWidget, SIGNAL(TunnelQueueChanged( const QQueue<STunnelQueue> & )), this, SLOT(ChangeTunnelQueue( const QQueue<STunnelQueue> & ))));
	throwIfFalse(QObject::connect(m_curCstWidget, SIGNAL(ConnectButtonPressed()), this, SLOT(ConnectButtonPressed())));
	
	SetCstWidgetConTunnelMode();
	
	return m_curCstWidget;
}

void SBoardTunnel::ChangeTunnelQueue( const QQueue<STunnelQueue> & f_queue ) {
	m_queue = f_queue;
	UpdateBallNumber();
}

SBoardTunnel * SBoardTunnel::GetConnnectedTunnel() {
	throwIfFalse(m_tunnelSystem.contains(m_gameboardID));
	
	QList<SBoardTunnel *> tunnels = m_tunnelSystem[m_gameboardID].values(m_tunnelID);
	throwIfFalse(tunnels.size() >= 2);
	
	if ( tunnels[0] == this )
		return tunnels[1];
	else
		return tunnels[0];
}

void SBoardTunnel::DisconnectTunnel() {
	SetTunnelID(-1);
}

QString SBoardTunnel::GetAdditionalInitErrors() {
	QString result;
	if ( m_tunnelID >= 0 && m_tunnelSystem[m_gameboardID].count(m_tunnelID) <= 1 ) {
		result = "There is only one tunnel with ID " + QString::number(m_tunnelID); 
		DisconnectTunnel();
	}
	
	return result;
}

void SBoardTunnel::ConnectButtonPressed() {
	if ( m_tunnelID >= 0 )
		// Tunnel ist verbunden ==> Verbindung wird gelöscht
		DisconnectTunnel();
	else if ( m_tunnelIDtoConnect.contains(m_gameboardID) ) {
		// Wenn schon eine ID in m_tunnelIDtoConnect drin steht, dann verbinde mit dieser
		SetTunnelID(m_tunnelIDtoConnect[m_gameboardID]);
		throwIfFalse(m_tunnelIDtoConnect.remove(m_gameboardID) == 1);
	} else {
		// Neue ID generieren und als Platzhalten hinstellen
		int newID;
		for ( newID = 0; m_tunnelSystem[m_gameboardID].contains(newID); newID++ )
			;// Erste nicht benutzte ID ist toll
			
		SetTunnelID(newID);
		m_tunnelIDtoConnect[m_gameboardID] = newID;
	}
	
	SetCstWidgetConTunnelMode();
}

void SBoardTunnel::SetCstWidgetConTunnelMode() {
	// Zustand des ConTunnelButtons festlegen
	if ( m_tunnelID >= 0 )
		// Tunnel ist verbunden ==> Verbindung wird gelöscht
		qensure_cast(SBoardTunnelSettingsWidget *, m_curCstWidget)->SetConnectButtonMode(SBoardTunnelSettingsWidget::DISCONNECT_TUNNEL);
	else if ( m_tunnelIDtoConnect.contains(m_gameboardID) ) {
		// Wenn schon eine ID in m_tunnelIDtoConnect drin steht, dann verbinde mit dieser
		qensure_cast(SBoardTunnelSettingsWidget *, m_curCstWidget)->SetConnectButtonMode(SBoardTunnelSettingsWidget::CONNECT_WITH_THIS_TUNNEL, SGraphicsTunnelMarker::GetColor(m_tunnelIDtoConnect[m_gameboardID]));
	} else {
		// Ansonsten sei dies der neue Mit-zu-verbinden-Tunnel
		qensure_cast(SBoardTunnelSettingsWidget *, m_curCstWidget)->SetConnectButtonMode(SBoardTunnelSettingsWidget::CONNECT_TUNNEL_WITH);
	}
}