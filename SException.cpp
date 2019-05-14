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
 * $LastChangedDate: 2009-03-25 14:11:18 +0100 (Mi, 25. Mär 2009) $
 * $LastChangedRevision: 250 $
*/

#include "SException.h"
#include "SConfig.h"
#include "VersionInformation.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

SException::SException(QString f_msg) : m_msg ( f_msg )
{
}

SException::~SException() throw()
{
}

/*static*/ void SException::Report(std::exception & e) {
	QString msg = e.what();
	QMessageBox::critical ( NULL, "Exception", msg );
	
	if ( SConfig::LOG_EXCEPTIONS() ) {
		QFile logfile(SConfig::LOG_EXCEPTIONS_FILE());
		bool existsAlready = logfile.exists();
		if ( logfile.open(QIODevice::Append | QIODevice::Text ) ) {
			QTextStream out( &logfile );
			
			if ( ! existsAlready ) 
				out << "VersionNumber;BuildNumber;Exception;";
			
			msg.replace('"', "'");
			QString log = "\n" + VersionInformation::VersionNumber + ";" + QString::number(VersionInformation::BuildNumber) + ";\"" + msg + "\";";
			
			out << log;
		}
	}
}


const char * SException::what() const throw() {
	return qPrintable(m_msg);
}

#define ImplementNewException(exception) 								\
	exception::exception(QString f_msg) : SException ( f_msg ) {}		\
	exception::~exception() throw() {}
	
ImplementNewException(SCriticalException)
ImplementNewException(SDirNotConException)
ImplementNewException(SXMLParseException)
