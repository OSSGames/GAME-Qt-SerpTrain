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

#ifndef SCOLOR_H_
#define SCOLOR_H_

#include <QColor>

// BLACK ist keine Ballfarbe!
// Die erste Farbe muss bei 0 anfangen und es muss AMOUNT_COLORS geben
// Die Reihenfolge muss die Reihenfolge sein, die im EDITOR angezeigt werden soll
// Dabei ist die erste Farbe SCHWARZ und damit keine BALLFARBE
enum COLOR { BLACK = 0, BLUE, GREEN, YELLOW, ORANGE, RED, WHITE, RAIL_COLOR = BLACK };
// ?_COLOR gibt an, von wo bis wo die Farben gehen; ?_BALL_COLOR bezeichnet wirkliche BALLFARBEN
enum COLOR_MARKERS { FIRST_COLOR = BLACK, LAST_COLOR = WHITE, FIRST_BALL_COLOR = BLUE, LAST_BALL_COLOR = WHITE, AMOUNT_COLORS = LAST_COLOR - FIRST_COLOR + 1, AMOUNT_BALL_COLORS = LAST_BALL_COLOR - FIRST_BALL_COLOR + 1 }; 

class SColor
{
public:
	SColor(enum COLOR f_color = RAIL_COLOR) : m_color ( f_color ) {};
	SColor(enum COLOR_MARKERS f_color);
	virtual ~SColor() {};
	
	QColor GetQColor() const; 
	
	operator const QColor () const {return GetQColor(); }; 
	operator enum COLOR () const {return m_color; };
    SColor & operator++();    // Präfix
	
private:
	enum COLOR m_color;
};

#endif /*SCOLOR_H_*/
