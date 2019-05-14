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

#ifndef SHTTPGET_H
#define SHTTPGET_H

#include <QHttp>
#include <QUrl>
#include <QBuffer>

class SHttpGet : public QObject
{
	Q_OBJECT

public:
	// Die Datei f_url wird heruntergeladen;
	// Wenn f_to == NULL ist, dann kann das Ergebnis über GetResult() abgefragt werden (aber nur dann)!
	SHttpGet(QUrl f_url, QIODevice * f_to = NULL);
	~SHttpGet();
	// Diese Funktion startet den Übertragungsvorgang, wartet aber nicht auf das Ende
	void Start();
	
	QHttpResponseHeader GetResponseHeader() { return m_responseHeader; };
	QString GetResult();
	bool IsFinished() { return m_hasFinished; };
	
signals:
	// Nur eines dieser beiden Signale wird ausgelöst 
	void TransferFinished();
	void Error(QString f_error);
   
private slots:
	void FinishRequest(int f_requestId, bool f_error);
	void ReceiveResponseHeader(const QHttpResponseHeader & f_responseHeader);
	void SslErrors ( const QList<QSslError> & f_errors ); 

private:
	QIODevice *			m_to;
	QBuffer				m_result;
	QHttp				m_http;	
	QUrl				m_url;
	bool 				m_alreadySend;
	bool				m_hasFinished;
	int					m_httpRequestId;
	QHttpResponseHeader m_responseHeader;
};

#endif // SHTTPGET_H 
