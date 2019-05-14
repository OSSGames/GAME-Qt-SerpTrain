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

#ifndef SGRAPHICSTUNNELMARKER_H_
#define SGRAPHICSTUNNELMARKER_H_

#include "SGraphicsTile.h"

class SGraphicsTunnelMarker : public SGraphicsTile
{
	Q_OBJECT
	
public:
	SGraphicsTunnelMarker();
	virtual ~SGraphicsTunnelMarker();
	
	void SetTunnelID( int f_id );
	static QColor GetColor( int f_id );
	
protected:
	virtual void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
	
	int m_tunnelId;
};

#endif /*SGRAPHICSTUNNELMARKER_H_*/
