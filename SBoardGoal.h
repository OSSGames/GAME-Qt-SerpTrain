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

#ifndef SBOARDGOAL_H_
#define SBOARDGOAL_H_

#include "SBoardTile.h"
#include "SArray.h"

class SBall;

struct SGoalField {
	SGoalField() : ballColor(RAIL_COLOR), ball(NULL) {}; //(RAIL_COLOR, heißt soviel wie "empty")
	SColor ballColor;
	SBall * ball;
};

class SBoardGoal : public SBoardTile
{
	Q_OBJECT

public:
	SBoardGoal(enum BOARD_MODE, int f_layerID, int f_gameboardID);
	virtual void SpecialInitByXML(SXMLParse f_xmlparse);
	virtual ~SBoardGoal();
	
	virtual bool IsColorOk(SColor, DIRECTION_RELATIVE );
	
	virtual QWidget * CreateCustumSettingsWidget();
			
	static const int AmountFields = 4;

public slots:
	void SetActive();
	void SetPassive();
	
signals:
	void GoalFull();
	
protected slots:
	void SetGoalColors(SArray<SColor,SBoardGoal::AmountFields> f_goalColors);
	void SetShowAllFlag(bool);
	void SetContinueFlag(bool);

protected:
	virtual void AdjustXmlInfo(SXMLElement f_element, SXMLDoc f_xmlDoc);
	virtual void UpdateColorsInfo(); // Muss die Farben m_colors[RIGHT]/Left... grafisch und spiel-logisch umsetzten
	
	virtual void SendBall( SBall * f_ball, enum DIRECTION_RELATIVE f_dirRel );
	
	virtual void ReceiveBall(SBall *, enum DIRECTION_RELATIVE);
	
	virtual bool IsRelDirectionCon ( enum DIRECTION_RELATIVE ) const;
	virtual const SPath * GetBallPath( SBall * ) const;
	
	virtual void InitGraphic( );
	virtual SBoardTile * DoCopy(enum BOARD_MODE) const;
	void UpdateGraphics();
	QColor GetGoalColor(SColor f_color, bool f_force);

	bool m_isConnected; // Gibt an, ob das Ziel nach rechts weitergeht
	int m_freeLots; // Gibt an, wieviele Bälle das Ziel noch aufnehmen kann!
	bool m_showAll; // Gibt an, ob alle Zielfarben gezeigt werden sollen!
	bool m_active;  // Gibt an, ob das Ziel schon aktiv ist! (Das Ziel kann nur dann noch nicht active sein, wenn vor diesem Ziel ein anderes Ziel geschaltet ist!)
	
	SArray<SGoalField, AmountFields> m_goalFields;
};

#endif /*SBOARDGOAL_H_*/
