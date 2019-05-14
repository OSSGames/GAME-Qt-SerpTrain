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

#ifndef SGRAPHICSEDTCOLORS_H_
#define SGRAPHICSEDTCOLORS_H_

#include "SGraphicsTile.h"
#include "enum.h"
#include "SColor.h"

#include <QHash>

// Klasse wird benutzt, um festzulegen, welche Farben gerade erlaubt bzw. aktiv sind
class SAllowedColors
{
	public:
		SAllowedColors() {
			for ( int i = 0; i < AMOUNT_COLORS; i++ )
				m_allowed[i] = false;
				
			m_amountAllowed = 0;
		}
		
		int GetAmountAllowed() {
			return m_amountAllowed;
		}
		
		void SetAllowedColor(SColor f_color, bool f_allowed ) {
			if ( m_allowed[f_color - FIRST_COLOR] )
				m_amountAllowed--;
			
			if ( f_allowed )
				m_amountAllowed++;
				
			m_allowed[f_color - FIRST_COLOR] = f_allowed;
		}
		
		bool IsColorAllowed(SColor f_color) {
			return m_allowed[f_color - FIRST_COLOR];
		} 
	
		void SetActiveColor(SColor f_color) {
			m_activeColor = f_color;
		}
		
		bool IsColorActive(SColor f_color) {
			return (m_activeColor == f_color);
		}

		SColor GetActiveColor() {
			return m_activeColor;
		}

	private:
		bool m_allowed[AMOUNT_COLORS]; // Die internen Indizes werden immer umgerechnet
		int m_amountAllowed;
		SColor m_activeColor; 
};

class SGraphicsEdtColors : public SGraphicsTile {
	Q_OBJECT
	
public:
	SGraphicsEdtColors();
	virtual ~SGraphicsEdtColors();
	
	void SetCurrentColor( SColor f_color, DIRECTION_ABSOLUTE );
	
	// Das Sheet darf nicht rotiert/gespiegelt werden;
	void virtual SetTransform(enum ROTATION, enum MIRROR) {};
	
	void SetAllowedColors(DIRECTION_ABSOLUTE, SAllowedColors);
		
private slots:
	
signals:
	void ColorChanged(SColor, DIRECTION_ABSOLUTE);

protected:
	virtual void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
    virtual void mousePressEvent ( QGraphicsSceneMouseEvent * f_event );

	virtual QRectF boundingRect() const;
	void ChangeColor( SColor f_color, DIRECTION_ABSOLUTE );
	
private:
	QList<SColor> m_colors;
	
	QHash<DIRECTION_ABSOLUTE, SAllowedColors> m_allowedColors;
	
	qreal m_size; // Gibt den Durchmesser der Farbkreise an
};

#endif /*SGRAPHICSEDTCOLORS_H_*/
