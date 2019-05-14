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

#include "SXMLParse.h"
#include "SException.h"
#include <QFile>
#include <QRegExp>

int SXMLParse::nextID = 1;

SXMLParse::SXMLParse(QFile * f_file) : m_parent ( NULL ), m_state( LOCKED ), m_amountErrors ( 0 )
{
	throwIfFalse(f_file != NULL);
	
	m_ID = nextID++;
	
 	if (!f_file->open(QIODevice::ReadOnly))
		throwOwnEx(SXMLParseException, "Cannot open file: " + f_file->fileName());
			 
 	QString error; int error_line; int error_col;
 	if (!m_xmlDoc.setContent(f_file,&error, &error_line, &error_col)) {
    	throwOwnEx(SXMLParseException, "In " + f_file->fileName() + " was an error (line " + QString::number(error_line) + " and column " + QString::number(error_col) + "):\n" + error);
 	}
 	
 	m_rootElement = m_xmlDoc.documentElement();
 	
 	f_file->close();
}

SXMLParse::SXMLParse(const QByteArray & f_text ) : m_parent ( NULL ), m_state( LOCKED ), m_amountErrors ( 0 )
{
	m_ID = nextID++;

 	QString error; int error_line; int error_col;
 	if (!m_xmlDoc.setContent(f_text,&error, &error_line, &error_col)) {
    	throwOwnEx(SXMLParseException, "There was an error (line " + QString::number(error_line) + " and column " + QString::number(error_col) + "):\n" + error);
 	}
 	
 	m_rootElement = m_xmlDoc.documentElement();
}


SXMLParse::SXMLParse(const SXMLParse & f_toCopy) : m_amountErrors ( 0 ) {
	throwIfFalse( f_toCopy.m_state != SHOULD_GET_DOWN );
	
	m_ID = f_toCopy.m_ID;
	m_rootElement = f_toCopy.m_rootElement;
	m_parent = f_toCopy.m_parent;
	m_state = f_toCopy.m_state;
}

SXMLParse::~SXMLParse()
{
}

SXMLParse SXMLParse::GetSubElement() {
	if ( m_rootElement.isNull() )
		AddError( "Current element is not valid. Cannot get SubElement!" );
		
	SXMLParse result = SXMLParse( this );
	
	AddLog(GetIDTag(result.m_ID));
	
	return result;
}

SXMLParse::SXMLParse(SXMLParse * f_parent) : m_state( SHOULD_GET_DOWN ), m_amountErrors ( 0 ){
	m_parent = f_parent;
	m_ID = nextID++;
}


void SXMLParse::ToNextElement(QString f_assumedName /*= QString()*/)
{
	switch( m_state )
	{
		case LOCKED:
			throwEx( "SXMLParse is locked!" );
			break;
		case SHOULD_GET_DOWN:
			m_rootElement = m_parent->m_rootElement.firstChildElement( f_assumedName );
			m_state = NORMAL;
			break;
		case NORMAL:
			m_rootElement = m_rootElement.nextSiblingElement( f_assumedName );
			break;
		default:
			throwEx("Unvalid m_state value: " + QString::number(m_state));
	}
		
	if ( m_rootElement.isNull() )
		AddError( "There's no next next element named " + GetLogName( f_assumedName ));
	else
		AddLog( "<" + m_rootElement.tagName() + ">" );
}

bool SXMLParse::HasNextElement(QString f_assumedName /*= QString()*/ ){
	switch( m_state )
	{
		case LOCKED:
			throwEx( "SXMLParse is locked!" );
			break;
		case SHOULD_GET_DOWN:
			return ( ! m_parent->m_rootElement.firstChildElement( f_assumedName ).isNull() );
		case NORMAL:
			return ( ! m_rootElement.nextSiblingElement( f_assumedName ).isNull() );
		default:
			throwEx("Unvalid m_state value: " + QString::number(m_state));
	}
}

void SXMLParse::AddError(QString f_error, int f_ID ) {
	AddErrorCount();

	AddLog("ERROR: " + f_error, f_ID );
}	

void SXMLParse::AddErrorCount() {
	if ( m_parent != NULL )
		m_parent->AddErrorCount();
		
	m_amountErrors++; 
}

QString SXMLParse::GetIDTag( const QString &  f_ID ) {
	return "-ID:" + f_ID + "-";
}

QString SXMLParse::GetIDTag( int f_ID ) {
	return GetIDTag(QString::number(f_ID));
}

void SXMLParse::AddLog(QString f_log, int f_ID  ) {
	if ( f_ID == m_ID ) {
		if ( ! m_log.isEmpty() )
			m_log += "\n";
			
		m_log += f_log;
	}
	else {
		m_log.replace(GetIDTag(f_ID), "\n" + f_log + GetIDTag(f_ID));
	}
	
	if ( m_parent != NULL )
		m_parent->AddLog("  " + f_log, f_ID); 
}	

QString SXMLParse::GetLogName(QString f_name) {
	return f_name.isEmpty() ? "?not specified?" : f_name;
}

SXMLParse SXMLParse::GetLocked() {
	throwIfFalse( m_state != SHOULD_GET_DOWN );
	
	SXMLParse result(*this);
	
	result.m_state = LOCKED;
	
	return result;
}

QString SXMLParse::GetTagName() {
	throwIfFalse( m_state != SHOULD_GET_DOWN );

	return m_rootElement.tagName();	
}

bool SXMLParse::GetBoolAttribute(QString f_attribute, bool f_defaultValue ) {
	throwIfFalse( m_state != SHOULD_GET_DOWN );

	if ( m_rootElement.hasAttribute(f_attribute) )
	{
		QString attribute = m_rootElement.attribute(f_attribute);
		
		if ( "yes" == attribute )
			return true;
		else if ( "no" == attribute )
			return false;
		else {
			AddError("The attribute " + f_attribute + "(" + attribute + ") cannot be connverted to an Boolean!" );
			
			return f_defaultValue;
		}
	}
	else
		return f_defaultValue;
}

int SXMLParse::GetIntAttribute(QString f_attribute, int f_defaultValue, int f_lowerRange, int f_upperRange ) {
	throwIfFalse( m_state != SHOULD_GET_DOWN );

	if ( m_rootElement.hasAttribute(f_attribute) )
	{
		QString attribute = m_rootElement.attribute(f_attribute);
		
		bool success;
		int result = attribute.toInt ( & success );
		if ( success ) {
			if ( result == qBound ( f_lowerRange, result, f_upperRange ) )
				return result;
			else {
				AddError("The attribute " + f_attribute + "(" + attribute + ") is in the wronge range. ("
					+ QString::number(f_lowerRange) + "-" + QString::number(f_upperRange) + ")" );
					
				return f_defaultValue;
			}
		}
		else {
			AddError("The attribute " + f_attribute + "(" + attribute + ") cannot be connverted to an Integer!" );
			return f_defaultValue;
		}
	}
	else
		return f_defaultValue;
}

int SXMLParse::GetErrors() {
	return m_amountErrors;
}


QString SXMLParse::GetLog() {
	QString result = m_log;
	result.replace(QRegExp("\n *\n"), "\n");
	result.replace(QRegExp(GetIDTag("\\d*")), "");
	
	return result;	
}


QString SXMLParse::GetAttribute( QString f_attribute, QString f_default ) {
		throwIfFalse( m_state != SHOULD_GET_DOWN );
		return m_rootElement.attribute(f_attribute, f_default);
}
