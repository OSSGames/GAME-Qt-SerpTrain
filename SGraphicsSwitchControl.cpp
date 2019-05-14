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

#include "SGraphicsSwitchControl.h"
#include "SConfig.h"
#include <QPainter>

SGraphicsSwitchControl::SGraphicsSwitchControl(bool f_straightSwitch) : SGraphicsTile( Z_MAJOR_RAIL, QPoint(SConfig::FIELD_SIZE()/2,SConfig::FIELD_SIZE()/2)), m_straightSwitch ( f_straightSwitch )
{
}

SGraphicsSwitchControl::~SGraphicsSwitchControl()
{
}

void SGraphicsSwitchControl::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
	//painter->drawRect(boundingRect());    
   
	QPen pen = painter->pen();
	pen.setWidth(SConfig::SWITCH_CONTROL_WIDTH_GRAPHICAL()); 
	painter->setPen(pen);

	painter->setBrush(SConfig::SWITCH_CONTROL_COLOR());
	if ( m_straightSwitch )
   		painter->drawEllipse(MirrorRect(QRectF(SConfig::FIELD_SIZE()-1.5*SConfig::RAIL_WIDTH(),0.5*SConfig::RAIL_WIDTH(),SConfig::RAIL_WIDTH(),SConfig::RAIL_WIDTH())));
   	else
   		// Zwischen die beiden Kurven setzen
   		painter->drawEllipse(MirrorRect(QRectF(SConfig::FIELD_SIZE()-2.0*SConfig::RAIL_WIDTH(),SConfig::FIELD_SIZE()/2.0-0.5*SConfig::RAIL_WIDTH(),SConfig::RAIL_WIDTH(),SConfig::RAIL_WIDTH())));
}
