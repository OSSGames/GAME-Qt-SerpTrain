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

#include "SHttpGet.h"
#include "SException.h"

#include <QFileInfo>

#ifndef Q_WS_WIN // Auf Windows ist SSL nicht ohne zusätzlchen Aufwand verfügbar
	#include <QSslError>
#endif

SHttpGet::SHttpGet(QUrl f_url, QIODevice * f_to ) :
 m_to ( f_to ), m_url( f_url ), m_alreadySend ( false ), m_hasFinished ( false ), m_httpRequestId ( -1 )
{
   throwIfFalse(QObject::connect(&m_http, SIGNAL(requestFinished(int, bool)), this, SLOT(FinishRequest(int, bool))));
   throwIfFalse(QObject::connect(&m_http, SIGNAL(responseHeaderReceived(const QHttpResponseHeader &)), this, SLOT(ReceiveResponseHeader(const QHttpResponseHeader &))));

#ifndef Q_WS_WIN // Auf Windows ist SSL nicht ohne zusätzlchen Aufwand verfügbar
   throwIfFalse(QObject::connect(&m_http, SIGNAL(sslErrors(const QList<QSslError> &)), this, SLOT(SslErrors( const QList<QSslError> &))));
#endif
}
 
SHttpGet::~SHttpGet()
{
}

void SHttpGet::Start() {
	throwIfFalse(m_alreadySend == false); m_alreadySend = true;
		
	QHttp::ConnectionMode mode = m_url.scheme().toLower() == "https" ? QHttp::ConnectionModeHttps : QHttp::ConnectionModeHttp;
	m_http.setHost(m_url.host(), mode, m_url.port() == -1 ? 0 : m_url.port());

	if ( !m_url.userName().isEmpty() )
		m_http.setUser(m_url.userName(), m_url.password());
		
	if ( m_to != NULL )
		m_httpRequestId = m_http.get(m_url.path(), m_to );
	else
		m_httpRequestId = m_http.get(m_url.path(), &m_result );
}

void SHttpGet::FinishRequest(int f_requestId, bool f_error)
{
	if ( f_error ) {
		if ( ! m_hasFinished ) { 
			m_hasFinished = true;
			emit Error("An unexpected error occured: " + m_http.errorString() );
		}
	}
	
	if (f_requestId == m_httpRequestId) {
		if ( ! m_hasFinished ) { 
			m_hasFinished = true;
   			emit TransferFinished();
		}
	}
}

QString SHttpGet::GetResult() {
	return m_result.data().data();	
}


void SHttpGet::ReceiveResponseHeader(const QHttpResponseHeader & f_responseHeader)
{
	m_responseHeader = f_responseHeader;
} 

#ifndef Q_WS_WIN // Auf Windows ist SSL nicht ohne zusätzlchen Aufwand verfügbar
	void SHttpGet::SslErrors( const QList<QSslError> & f_errors ) {
		if ( ! m_hasFinished ) { 
			m_hasFinished = true;
			QString error;
			for ( int i = 0; i < f_errors.count(); i++ )
				error += "\n" + f_errors[i].errorString();
			emit Error("An SSL-related error occured:" + error);
		}
	}
#else 
	void SHttpGet::SslErrors( const QList<QSslError> & )
	{}
#endif
