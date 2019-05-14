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

#ifndef SXMLELEMENT_H_
#define SXMLELEMENT_H_

#include "SException.h"
#include <QDomElement>

class SXMLElement : public QDomElement
{
public:
	SXMLElement(QDomElement f_domElement );
	virtual ~SXMLElement();
	
	void SetBoolAttribute(QString f_attribute, bool f_value);
	
	template <typename T>
	void SetAttribute(QString f_attribute, T f_curValue, const QList<QString> & f_replacedNames, const QList<T> & f_allowedValues)
	{
		throwIfFalse( f_allowedValues.size() == f_replacedNames.size() );
		
		for (int i = 0; i < f_allowedValues.size(); ++i) {
			if ( f_allowedValues.at(i) == f_curValue ) {
				setAttribute(f_attribute, f_replacedNames.at(i));
				return;
			}
		}
			
		throwEx("'" + f_curValue +"' is no valid value of '" + f_attribute + "'!" );
	};
		
private:
};

#endif /*SXMLELEMENT_H_*/
