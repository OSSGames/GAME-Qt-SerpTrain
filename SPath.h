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

#ifndef SPATH_H_
#define SPATH_H_

#include <QPoint>
#include "enum.h"

class SPath
{
public:
	SPath(bool f_reverse, QPoint f_middlePoint, enum ROTATION f_rotation = ROTATION_NO );
	virtual ~SPath();
	
	virtual int GetPathLen() const = 0;
	QPointF GetPosOnPath( int f_pos ) const; // Gibt die wirkliche Position zurück (damit werden auch Kollisionen berechnet)
	virtual QPointF GetGraphicalPosOnPath( int f_pos ) const;
	DIRECTION_RELATIVE GetExit() const;

protected:
	virtual DIRECTION_RELATIVE GetRelExit() const = 0;
	virtual QPointF GetPathPos ( int f_pos ) const = 0;
	
	bool m_reverse;
	QPoint m_middlePoint;
	enum ROTATION m_rotation;
};

#endif /*SPATH_H_*/
