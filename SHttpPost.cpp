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

#include "SHttpPost.h"
#include "SException.h"

#include <QFileInfo>

#ifndef Q_WS_WIN // Auf Windows ist SSL nicht ohne zusätzlchen Aufwand verfügbar
	#include <QSslError>
#endif

SHttpPost::SHttpPost(QUrl f_url, QString f_dataField ) :
 m_dataField( f_dataField ), m_url( f_url ), m_buffer ( & m_bufferArray ), m_alreadySend ( false ), m_hasFinished ( false ), m_httpRequestId ( -1 )
{
   throwIfFalse(QObject::connect(&m_http, SIGNAL(requestFinished(int, bool)), this, SLOT(FinishRequest(int, bool))));
   throwIfFalse(QObject::connect(&m_http, SIGNAL(responseHeaderReceived(const QHttpResponseHeader &)), this, SLOT(ReceiveResponseHeader(const QHttpResponseHeader &))));

#ifndef Q_WS_WIN // Auf Windows ist SSL nicht ohne zusätzlchen Aufwand verfügbar
   throwIfFalse(QObject::connect(&m_http, SIGNAL(sslErrors(const QList<QSslError> &)), this, SLOT(SslErrors( const QList<QSslError> &))));
#endif
}
 
SHttpPost::~SHttpPost()
{
}

void SHttpPost::QueueFile(QString f_file) {
	m_filesToSend << f_file;
}

void SHttpPost::Start() {
	throwIfFalse(m_alreadySend == false); m_alreadySend = true;
	throwIfFalse(m_filesToSend.count() != 0 );
	
	QHttp::ConnectionMode mode = m_url.scheme().toLower() == "https" ? QHttp::ConnectionModeHttps : QHttp::ConnectionModeHttp;
	m_http.setHost(m_url.host(), mode, m_url.port() == -1 ? 0 : m_url.port());

	if ( !m_url.userName().isEmpty() )
		m_http.setUser(m_url.userName(), m_url.password());
		
	QHttpRequestHeader requestHeader("POST", m_url.path() );
	// Header setzen (vgl. Firefox mit LiveHTTPHeaders-Extension
	requestHeader.setValue("Host", m_url.host());
	requestHeader.setValue("Accept", "text/xml,application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,image/png,*/*;q=0.5");
	requestHeader.setValue("Accept-Encoding", "gzip,deflate");
	requestHeader.setValue("Accept-Charset", "ISO-8859-1,utf-8;q=0.7,*;q=0.7");
	requestHeader.setValue("Keep-Alive", "300");
	requestHeader.setValue("Connection", "keep-alive");
	QString multipartID = "95076057115111081805850575";
	requestHeader.setValue("Content-Type", "multipart/form-data; boundary=---------------------------" + multipartID);
	
	// Die einzelnen Dateien
	for (int i = 0; i < m_filesToSend.count(); i++ ) {
		QFileInfo fileInfo(m_filesToSend[i]);
		QFile file(m_filesToSend[i]);
		if ( ! file.exists() ) {
			emit Error("The file " + fileInfo.fileName() + " doest not exists." ); 
			return;
		}
		
		if ( ! file.open(QIODevice::ReadOnly)) {
			emit Error("The file " + fileInfo.fileName() + " could not be opened readable." ); 
			return;
		}
		
		m_bufferArray.append("-----------------------------" + multipartID + "\n");
		if ( m_filesToSend.count() == 1 )
			m_bufferArray.append("Content-Disposition: form-data; name=\"" + m_dataField + "\"; ");
		else
			m_bufferArray.append("Content-Disposition: form-data; name=\"" + m_dataField + "[" + QString::number(i) + "]\"; ");
	
		m_bufferArray.append("filename=\"" + fileInfo.fileName() + "\"\n");
		m_bufferArray.append("Content-length: " + QString::number(fileInfo.size()));
		m_bufferArray.append("\nContent-Type: application/octet-stream\n\n");
		m_bufferArray.append(file.readAll());
		m_bufferArray.append("\n");
	}
   
	m_bufferArray.append("-----------------------------" + multipartID + "--\n");
	
	m_httpRequestId = m_http.request(requestHeader, &m_buffer, &m_result );
}

void SHttpPost::FinishRequest(int f_requestId, bool f_error)
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

QString SHttpPost::GetResult() {
	return m_result.data().data();	
}


void SHttpPost::ReceiveResponseHeader(const QHttpResponseHeader & f_responseHeader)
{
	m_responseHeader = f_responseHeader;
} 

#ifndef Q_WS_WIN // Auf Windows ist SSL nicht ohne zusätzlchen Aufwand verfügbar
	void SHttpPost::SslErrors( const QList<QSslError> & f_errors ) {
		if ( ! m_hasFinished ) { 
			m_hasFinished = true;
			QString error;
			for ( int i = 0; i < f_errors.count(); i++ )
				error += "\n" + f_errors[i].errorString();
			emit Error("An SSL-related error occured:" + error);
		}
	}
#else 
	void SHttpPost::SslErrors( const QList<QSslError> & )
	{}
#endif
