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
 * $LastChangedDate: 2009-03-24 16:17:09 +0100 (Di, 24. Mär 2009) $
 * $LastChangedRevision: 249 $
*/

#ifndef SXMLPARSE_H_
#define SXMLPARSE_H_

#include <QDomDocument>
#include <QDomElement>
#include <QFile>

#include "SException.h"
#include "limits.h"

class SXMLParse
{
public:
	SXMLParse(QFile * f_file );
	SXMLParse(const QByteArray & f_text );
	SXMLParse(const SXMLParse & f_toCopy);
	virtual ~SXMLParse();
	
	// Geht eine Ebene nach unten
	// Achtung! Das Kind darf nicht mehr benutzt werden, wenn Elternteil tot ist!
	SXMLParse GetSubElement();
	
	// Navigiert zum nächsten Element mit dem Namen 'f_assumedName'. 
	void ToNextElement(QString f_assumedName = QString() );
	bool HasNextElement(QString f_assumedName = QString() );
	
	// Sperrt die SXMLParse-Klasse. Es kann nur noch nach unten navigiert werden
	// Das ist gedacht, um Unterfunktionen die Klasse übergeben zu können
	// Bsp:
	// 		SXMLParse	parse;
	// 		EvilFunction( parse.GetLocked() );
	SXMLParse GetLocked();
	
	QString GetTagName();
	
	QString GetAttribute( QString f_attribute, QString f_default = QString() );
	
	template <typename T>
	T GetAttribute(QString f_attribute, const QList<QString> & f_allowedAttributes, const QList<T> & f_replacedValues, T f_defaultValue)
	{
		throwIfFalse( m_state != SHOULD_GET_DOWN );
		throwIfFalse( f_allowedAttributes.size() == f_replacedValues.size() );
		
		if ( m_rootElement.hasAttribute(f_attribute) )
		{
			QString attribute = m_rootElement.attribute(f_attribute);
			
			for (int i = 0; i < f_allowedAttributes.size(); ++i) {
				if ( f_allowedAttributes.at(i) == attribute )
					return f_replacedValues.at(i);
			}
			
			AddError("The attribute " + f_attribute + " has a silly value: " + attribute );
			return f_defaultValue;
		}
		else
			return f_defaultValue;
	};
	
	int GetIntAttribute(QString f_attribute, int f_defaultValue, int f_lowerRange = INT_MIN, int f_upperRange = INT_MAX );
	bool GetBoolAttribute(QString f_attribute, bool f_defaultValue );
	
	QString GetLog();
	int GetErrors();
	void AddError(QString f_error) { AddError(f_error, m_ID); }
	void AddLog(QString f_error) { AddLog(f_error, m_ID); }

private:
	SXMLParse(SXMLParse * m_parent);
	
	static QString GetIDTag( int f_ID );
	static QString GetIDTag( const QString &  f_ID );
	
	void AddErrorCount();
	
	void AddError(QString f_error, int f_id );
	void AddLog(QString f_log, int f_id );
	QString GetLogName(QString f_name);
	
	SXMLParse * 		m_parent;
	QDomDocument 		m_xmlDoc;
	QDomElement			m_rootElement;
	
	enum STATE { NORMAL, LOCKED, SHOULD_GET_DOWN };
	int m_state;
	int m_ID;
	
	QString	m_log;
	int m_amountErrors;
	
	static int nextID;
};

#endif /*SXMLPARSE_H_*/
