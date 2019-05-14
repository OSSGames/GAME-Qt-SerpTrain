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

#ifndef SXMLDOC_H_
#define SXMLDOC_H_

#include "SXMLElement.h"
#include <QDomDocument>
#include <QFile>

class SXMLDoc
{
public:
	SXMLDoc(QString f_startContent = QString() );
	virtual ~SXMLDoc();
	
	// Generiert das erste RootElement; darf nur einmal aufgerufen werden!
	SXMLElement GenerateRootElement(QString f_tagName);
	SXMLElement CreateElement(QString f_tagName);
	
	// Das übergebene QFile-Object wird ge-"open()"t, dann wird darin geschrieben und danach wird es ge"close()"t.
	void WriteToFile(QFile * f_file);
	
private:
	QDomDocument m_document;
};

/*  
 	Die Klasse eignet sich, um ein XML-Dokument zu erstellen und abzuspeichern.
 	Beispiel:
	
	SXMLDoc xmlDoc;
    QDomElement root = xmlDoc.GenerateRootElement("root");
    QDomElement child1 = xmlDoc.CreateElement("child1");
    QDomElement child2 = xmlDoc.CreateElement("child2");
    QDomElement child3 = xmlDoc.CreateElement("child3");
    
    child2.setAttribute("attributeOfChild2", "value");
    
    root.appendChild(child1);
    child1.appendChild(child2);
    root.appendChild(child3);
    
    xmlDoc.WriteToFile(QFile("testXML"));
*/

#endif /*SXMLDOC_H_*/
