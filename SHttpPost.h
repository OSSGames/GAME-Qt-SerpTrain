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

#ifndef SHTTPPOST_H
#define SHTTPPOST_H

#include <QHttp>
#include <QUrl>
#include <QBuffer>

class SHttpPost : public QObject
{
	Q_OBJECT

public:
	// Die Dateien werden an f_url geschickt; die Antwort wird (falls gewünscht) in f_resultFile geschrieben
	SHttpPost(QUrl f_url, QString f_dataField = "file" );
	~SHttpPost();
	// Alle Dateien werden auf einmal gesendet; wenn nur eine Datei gesendet wird,
	// wird das 'dataField' als Feldnamen benutzt. Bei mehreren Dateien wird ein 
	// null-basierter array-Index (wie z.B.[0]) angefügt.
	void QueueFile(QString f_file);
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
	QBuffer				m_result;
	QHttp				m_http;	
	QString				m_dataField;
	QUrl				m_url;
	QList<QString>		m_filesToSend;
	QBuffer				m_buffer;
	QByteArray			m_bufferArray;
	bool 				m_alreadySend;
	bool				m_hasFinished;
	int					m_httpRequestId;
	QHttpResponseHeader m_responseHeader;
};

#endif // SHTTPPOST_H 
