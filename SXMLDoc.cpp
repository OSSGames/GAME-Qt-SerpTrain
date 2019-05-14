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

#include "SXMLDoc.h"
#include "SException.h"
#include <QDomElement>
#include <QFile>
#include <QTextStream>

SXMLDoc::SXMLDoc(QString)
{
}

SXMLDoc::~SXMLDoc()
{
}

SXMLElement SXMLDoc::CreateElement(QString f_tagName) {
	return m_document.createElement(f_tagName);
}

SXMLElement SXMLDoc::GenerateRootElement(QString f_tagName) {
	throwIfFalse(m_document.hasChildNodes() == false);
	
	m_document.setContent(QString("<?xml version=\"1.0\"?>"), false);

	QDomElement root = m_document.createElement(f_tagName);
	throwIfFalse(m_document.appendChild(root).isNull() == false );
	
	return root;
}

// Das übergebene QFile-Object wird ge-"open()"t, dann wird darin geschrieben und danach wird es ge"close()"t.
void SXMLDoc::WriteToFile(QFile * f_file) {
	throwIfFalse(f_file != NULL);
	
	QString xml = m_document.toString();
	
 	if ( ! f_file->open(QIODevice::WriteOnly))
		throwOwnEx(SXMLParseException, "Cannot open file: " + f_file->fileName());
	
	{
		QTextStream out(f_file);
		out << xml;
	}
	
	f_file->close();
}

	

