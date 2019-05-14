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

#ifndef SBOARDSWITCHTURN_H_
#define SBOARDSWITCHTURN_H_

#include "SBoardTile.h"

class SBoardSwitchTurn : public SBoardTile
{
	Q_OBJECT
	
public:
	SBoardSwitchTurn(enum BOARD_MODE, int f_layerID, int f_gameboardID);
	virtual void SpecialInitByXML(SXMLParse f_xmlparse);
	virtual ~SBoardSwitchTurn();
	
protected slots:
	void ChangeSwitch();
			
protected:
	virtual void AdjustXmlInfo(SXMLElement f_element, SXMLDoc f_xmlDoc);

	virtual bool IsColorOk(SColor, DIRECTION_RELATIVE );
	virtual void UpdateColorsInfo();

	virtual void ReceiveBall(SBall *, enum DIRECTION_RELATIVE);
	
	virtual bool IsRelDirectionCon ( enum DIRECTION_RELATIVE ) const;
	virtual const SPath * GetBallPath( SBall * ) const;
	
	virtual void InitGraphic( );
	virtual SBoardTile * DoCopy(enum BOARD_MODE) const;
	
	void UpdateSwitch();
	
	bool m_switchState; // true: Weiche geht von links nach oben; false: Weiche geht von links nach unten (ungespiegelt, ungedreht)
	bool m_switchable;
};

#endif /*SBOARDSWITCH_H_*/
