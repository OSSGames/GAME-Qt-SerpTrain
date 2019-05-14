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

#ifndef SBOARDTUNNEL_H_
#define SBOARDTUNNEL_H_

#include "SBoardTile.h"
#include <QQueue>

class SBall;

struct STunnelQueue {
	enum TYPE { WAIT, BALL };
	enum TYPE type;
	
	struct s_data {
		struct s_wait {
			s_wait() : timesteps ( 0 ) { }; 
			int timesteps;
		} wait;
		struct s_ball {
			s_ball() : ballColor ( BLACK ) { }; 
			SColor ballColor;
		} ball;
	} data;
};

class SBoardTunnel : public SBoardTile
{
	Q_OBJECT

public:
	SBoardTunnel(enum BOARD_MODE, int f_layerID, int f_gameboardID);
	virtual void SpecialInitByXML(SXMLParse f_xmlparse);	
	
	virtual QString GetAdditionalInitErrors();

	virtual ~SBoardTunnel();
	virtual void Timestep ( int f_timestepNr );
	
	virtual QWidget * CreateCustumSettingsWidget();
	
public slots: 
	
protected slots:
	void ChangeTunnelQueue( const QQueue<STunnelQueue> & );
	void ConnectButtonPressed(); // In SBoardTunnelSettingsWidget wurde der connectButton gedrückt (impliziert, dass m_cstWidget noch gültig ist)
	
protected:
	void DisconnectTunnel();
	void SetTunnelID(int f_id );
	SBoardTunnel * GetConnnectedTunnel();
	virtual void AdjustXmlInfo(SXMLElement f_element, SXMLDoc f_xmlDoc);

	virtual bool IsColorOk(SColor, DIRECTION_RELATIVE );
	virtual void UpdateColorsInfo();

	virtual void ReceiveBall(SBall *, enum DIRECTION_RELATIVE);
	virtual void SendBallAbs( SBall *, enum DIRECTION_ABSOLUTE );
	
	virtual bool IsRelDirectionCon ( enum DIRECTION_RELATIVE ) const;
	virtual const SPath * GetBallPath( SBall * ) const;
	virtual bool CanCrashOutsideHappen(SBall * ball, QPointF f_point); 
	
	virtual void InitGraphic( );
	virtual SBoardTile * DoCopy(enum BOARD_MODE) const;
	
	void SetCstWidgetConTunnelMode();

	QQueue<STunnelQueue> m_queue;
	int m_tunnelID;
	
	void TestCrashThisBall(SBall * f_ball, QPointF f_point);
	
	void UpdateBallNumber();
	
	QWidget * m_curCstWidget; // Dient nur zum kurzzeiten Speichern des custonWidgets; darf nicht gelöscht werden
	
	// 1. int = m_gameboardID
	// 2. int = tunnelID
	static QHash<int, QMultiHash<int, SBoardTunnel *> > m_tunnelSystem;
	
	// 1. int = m_gameboardID
	// 2. int = tunnelID
	static QHash<int, int> m_tunnelIDtoConnect; // Wird benutzt, um sich zu merken, welche ID als nächstes verbunden werden soll!
};

#endif /*SBOARDTUNNEL_H_*/
