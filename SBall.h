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

#ifndef SBALL_H
#define SBALL_H

#include "SGraphicsItem.h"
#include "SColor.h"

class SBall : public SGraphicsItem
{
	Q_OBJECT

public:
	SBall(SColor);
	virtual ~SBall();
	
	bool HasColor (SColor);
	
	// Braucht nichts zu tun, da Bälle Rotations- und Spiegelsymmetrisch sind und damit Erhaltungsgrößen darstellen
	void virtual SetTransform(enum ROTATION, enum MIRROR) {};
	
	SColor GetColor();
	
	qreal DistanceToBall(SBall * f_otherBall);
	
protected:
	virtual QRectF boundingRect() const;
	virtual void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
	
	SColor m_color;

public: //Diese Variablen werden benötigt, damit SBoardTile die Kugeln verwalten kann
		//(Korrekt wäre es, die 3 Variablen mit einer SBall-Variable in ein struct zu stecken, aber das ist
		// mir zu aufwändig!!!)
	enum BALL_ROUTE m_route;
	int m_steps;
	int m_lastTimestep;
};

#endif /*SBALL_H_*/
