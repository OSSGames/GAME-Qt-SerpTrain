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

#ifndef SXMLCONV_H_
#define SXMLCONV_H_

#include "enum.h"
#include "SColor.h"

#define CONV(TYPE, NAME)																					\
	public: static const QList<QString> & Get##NAME##Names()  { return m_single->m_##NAME##Names;  }		\
	public: static const QList<TYPE>    & Get##NAME##Values() { return m_single->m_##NAME##Values; }		\
	private: QList<QString> m_##NAME##Names;																\
	private: QList<TYPE>    m_##NAME##Values;

class SXMLConv
{
public:
	SXMLConv(); // Darf von außen nicht aufgerufen werden!
	virtual ~SXMLConv();
	
	CONV(ROTATION, Rotation)
	CONV(SColor, Color)
	CONV(SColor, ColorWOBlack) 	//Colors without black
	CONV(MIRROR, Mirror)
	CONV(bool, SwitchState)
	CONV(bool, SwitchTurnState)
	CONV(BOARDTILES, Boardtiles)
	
private:
	static SXMLConv * m_single;
};

#endif /*SXMLCONV_H_*/
