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

#include "SColor.h"
#include "SConfig.h"
#include "SException.h"

QColor SColor::GetQColor() const {
	switch ( m_color ) {
		case RED:
			return SConfig::BALL_COLOR_RED();
		case GREEN:
			return SConfig::BALL_COLOR_GREEN();
		case YELLOW:
			return SConfig::BALL_COLOR_YELLOW();
		case WHITE:
			return SConfig::BALL_COLOR_WHITE();
		case BLUE:
			return SConfig::BALL_COLOR_BLUE();
		case ORANGE:
			return SConfig::BALL_COLOR_ORANGE();
		case RAIL_COLOR:
			return SConfig::STD_RAIL_COLOR();
		default:
			throwEx("Unvalid 'enum SBallColor' value: " + QString::number(m_color));
	}
}

SColor::SColor(enum COLOR_MARKERS f_color) : m_color ( (COLOR) f_color )
{
	throwIfFalse( (int) f_color >= (int) FIRST_COLOR && (int) m_color <= (int) LAST_COLOR );
}

// Praefix-Inkrement
SColor & SColor::operator++()
{
	m_color = (COLOR) (m_color + 1);
	return (*this);
}
