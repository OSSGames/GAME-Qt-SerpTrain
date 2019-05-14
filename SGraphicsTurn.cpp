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

#include "SGraphicsTurn.h"
#include "SConfig.h"
#include <QPainter>

const SPathTurn SGraphicsTurn::Path					= SPathTurn(false);
const SPathTurn SGraphicsTurn::PathReversed			= SPathTurn(true);
const SPathTurn SGraphicsTurn::PathRot90			= SPathTurn(false, ROTATION_90);
const SPathTurn SGraphicsTurn::PathRot90Reversed	= SPathTurn(true,  ROTATION_90);
const SPathTurn SGraphicsTurn::PathRot180			= SPathTurn(false, ROTATION_180);
const SPathTurn SGraphicsTurn::PathRot180Reversed	= SPathTurn(true,  ROTATION_180);
const SPathTurn SGraphicsTurn::PathRot270			= SPathTurn(false, ROTATION_270);
const SPathTurn SGraphicsTurn::PathRot270Reversed	= SPathTurn(true,  ROTATION_270);

SGraphicsTurn::SGraphicsTurn(enum ROTATION f_addRotation) : SGraphicsRail(QPoint(SConfig::FIELD_SIZE()/2,SConfig::FIELD_SIZE()/2))
{
	m_addRotation = f_addRotation;
}

SGraphicsTurn::~SGraphicsTurn()
{
}

void SGraphicsTurn::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
   //painter->drawRect(boundingRect());    
   
   painter->setBrush(m_color);
   
   DrawTurn( painter, SConfig::FIELD_SIZE() / 2, QPointF(0,0));
}

